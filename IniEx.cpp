//////////////////////////////////////////////////////////////////////
// IniEx.cpp: implementation of the CIniEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//GrowSize for Dynmiz Section Allocation
CIniEx::CIniEx()
{

   m_SectionNo       = -1;
   m_writeWhenChange = FALSE;
   m_makeBackup      = FALSE;
   m_NoCaseSensitive = TRUE;
   m_Changed         = FALSE;
   m_nEditControl = 0;
}

CIniEx::~CIniEx()
{
   if (m_writeWhenChange)
   {
      WriteFile(m_makeBackup);
   }
   ResetContent();

}

//////////////////////////////////////////////////////////////////////
BOOL CIniEx::OpenAtExeDirectory(LPCTSTR pFileName,
                     BOOL writeWhenChange,/*=TRUE*/
                     BOOL createIfNotExist/*=TRUE*/,
                     BOOL noCaseSensitive /*=TRUE*/,
                     BOOL makeBackup      /*=FALSE*/)
{
   BOOL bRetVal;


   CString filePath;
//if it's a dll argv will be NULL and it may cause memory leak 
#ifndef _USRDLL
   CString tmpFilePath = _T("");
   int nPlace          = 0;
   tmpFilePath         =__argv[0];
   nPlace              = tmpFilePath.ReverseFind('\\');
   
   
   if (nPlace!=-1)
   {
      filePath = tmpFilePath.Left(nPlace);
   }
   else
   {
      TCHAR curDir[MAX_PATH];
      GetCurrentDirectory(MAX_PATH,curDir);
      filePath = curDir;
   }
#else
   //it must be safe for dll's
   char curDir[MAX_PATH];
   GetCurrentDirectory(MAX_PATH,curDir);
   filePath = curDir;
#endif
   filePath += _T("\\");
   filePath += pFileName;
   bRetVal = Open(filePath,writeWhenChange,createIfNotExist,noCaseSensitive,makeBackup);

   return bRetVal;
}
void CIniEx::SetEditControlType(int nEditControl)
{
   m_nEditControl = nEditControl;
}
//////////////////////////////////////////////////////////////////////
BOOL CIniEx::Open(LPCTSTR pFileName,
                  BOOL writeWhenChange,/*=TRUE*/
                  BOOL createIfNotExist/*=TRUE*/,
                  BOOL noCaseSensitive /*=TRUE*/,
                  BOOL makeBackup      /*=FALSE*/)
{

   CFileException e;
   BOOL    bRet;
   CString Line;
   int     nPlace;

   UINT mode = CFile::modeReadWrite;

   //if it's second ini file for this instance
   //we have to save it and delete member variables contents
   if (!m_FileName.IsEmpty()) 
   {
      WriteFile();
      ResetContent();
   }

   m_NoCaseSensitive = noCaseSensitive;
   m_writeWhenChange = writeWhenChange;
   m_makeBackup      = makeBackup;

   //Global Section
//   m_Keys.Add(new CStringArray);
   m_Keys.push_back(CStringVector());
   m_Values.push_back(CStringVector());
   m_Sections.push_back(_T(" "));
   m_SectionNo++;    

   CString tmpKey;
   CString tmpValue;

   if (createIfNotExist)
      mode = mode | CFile::modeCreate | CFile::modeNoTruncate;

   CStdioFile file;
   bRet=file.Open( pFileName, mode, &e );
   if( !bRet )
   {
      return FALSE;
   }
   m_FileName=pFileName;

   file.SeekToBegin();
   while (TRUE)
   {
      //Read one line from given ini file
      bRet=file.ReadString(Line);
      if (!bRet) break;
      if (m_nEditControl)
      {
         int nLength = Line.GetLength();
         for(int nI = 0; nI < nLength; nI++)
         {
            Line.SetAt(nI, Line[nI] + 14);
         }
      }
      Line.TrimRight();
      
      // Removing comments

      if (Line==_T("")) continue;
      
      //if line's first character = '[' 
      //and last character = ']' it's section 
      if (Line.Left(1)==_T("[") && Line.Right(1)==_T("]"))
      {
         //m_Keys.Add(new CStringArray);
         m_Keys.push_back(CStringVector());
         m_Values.push_back(CStringVector());
         m_SectionNo++;    
          //m_Sections.SetAtGrow(m_SectionNo,);
         m_Sections.push_back(Line.Mid(1,Line.GetLength()-2));
         continue;
      }
      
      nPlace=Line.Find(_T("="));
      if (nPlace==-1)
      {
         tmpKey=Line;
         tmpValue=_T("");
      }
      else
      {
         tmpKey=Line.Left(nPlace);
         tmpKey.TrimLeft();
         tmpKey.TrimRight();
         tmpValue=Line.Mid(nPlace+1);
         tmpValue.TrimLeft();
         tmpValue.TrimRight();
      }
      
      m_Keys[m_SectionNo].push_back(tmpKey);
      m_Values[m_SectionNo].push_back(tmpValue);
   }
   file.Close();

   return TRUE;
}

//////////////////////////////////////////////////////////////////////
CString CIniEx::GetValue(CString Key)
{
   return GetValue(_T(" "),Key);
}

//////////////////////////////////////////////////////////////////////
//if Section Name="" -> looking up key for without section
CString CIniEx::GetValue(CString Section,CString Key,CString DefaultValue/*=""*/)
{

   int nIndex=LookupSection(&Section);
   if (nIndex==-1) return DefaultValue;
   int nRet;
   CString retStr;
   for (int i=(int)m_Keys[nIndex].size()-1;i>=0;i--)
   {
      nRet=CompareStrings(&m_Keys[nIndex][i],&Key);
      if (nRet==0)
      {
         retStr=m_Values[nIndex][i];
         int nPlace=retStr.ReverseFind(';');
         if (nPlace!=-1) retStr.Delete(nPlace,retStr.GetLength()-nPlace);
         return retStr;
      }
   }


   return DefaultValue;
}

//////////////////////////////////////////////////////////////////////
//returns index of key for given section
//if no result returns -1
int CIniEx::LookupKey(int nSectionIndex,CString *Key)
{

   ASSERT(nSectionIndex<=m_SectionNo);
   int nRet;
   for (int i=(UINT)m_Keys[nSectionIndex].size()-1;i>=0;i--)
   {
      nRet=CompareStrings(&m_Keys[nSectionIndex][i],Key);
      if (nRet==0) return i;
   }

   return -1;
}

//////////////////////////////////////////////////////////////////////
//return given sections index in array
int CIniEx::LookupSection(CString *Section)
{

   int nRet;
   //for (int i=0;i<m_Sections.GetSize();i++)
   for (UINT i=0;i<m_Sections.size();i++)
   {
      //nRet=CompareStrings(&m_Sections.GetAt(i),Section);
      nRet=CompareStrings(&m_Sections[i],Section);
      if (nRet==0) return i;
   }

   return -1;
}

//////////////////////////////////////////////////////////////////////
//Sets for Key=Value for without section
void CIniEx::SetValue(CString Key,CString Value)
{
   SetValue(_T(" "),Key,Value);
}

//////////////////////////////////////////////////////////////////////
//writes Key=value given section
void CIniEx::SetValue(CString Section,CString Key,CString Value)
{
   //file opened?
   ASSERT(!m_FileName.IsEmpty());

   //if given key already existing, overwrite it
   int nIndex=LookupSection(&Section);
   int nKeyIndex;
   if (nIndex==-1)
   {
      //if key not exist grow arrays (if necessary)
      m_Changed=TRUE;
      m_Keys.push_back(CStringVector());
      m_Values.push_back(CStringVector());
      m_SectionNo++;
      nIndex=m_SectionNo;
      //m_Sections.SetAtGrow(m_SectionNo,Section);
      m_Sections.push_back(Section);
   }
   
   //looking up keys for section
   nKeyIndex=LookupKey(nIndex,&Key);
   
   //if key exist -> overwrite it
   //if not add to end of array
   if (nKeyIndex!=-1)
   {
      if (CompareStrings(&m_Values[nIndex][nKeyIndex],&Value)!=0)
         m_Changed=TRUE;
      m_Values[nIndex][nKeyIndex] = Value;
   }
   else  //if not exist
   {
      m_Changed=TRUE;
      m_Keys[nIndex].push_back(Key);
      m_Values[nIndex].push_back(Value);
   }

}

//////////////////////////////////////////////////////////////////////
//returns backup file name
//if you didn't want backup (when openning file) it returns ""
CString CIniEx::WriteFile(BOOL makeBackup/*=FALSE*/)
{

   if (!m_Changed) return _T("");
   CString tmpFileName=m_FileName;

   if (makeBackup)
   {
      if (m_BackupFileName.IsEmpty())
      {
         FindBackupFile();
      }
      if ( !CopyFile(m_FileName,m_BackupFileName,FALSE) )
      {
         CString out;
         out.Format(_T("Ini file error:\n%s to %s could not be copied."),m_FileName,m_BackupFileName);
         AfxMessageBox(out);
      }
   }

   CStdioFile file;
   if (!file.Open(m_FileName,CFile::modeCreate | CFile::modeWrite)) 
   {
      #ifdef _DEBUG
         afxDump << "ERROR!!!!: The file could not open for writing\n";
      #endif
      return _T("");
   }

   CString tmpLine;
   int nSkipFirstEmptyLine = 1;
   //for (int i=0;i<m_Sections.GetSize();i++)
   for (UINT i=0;i<m_Sections.size();i++)
   {
      //if (!m_Sections.GetAt(i).IsEmpty())
      if (!m_Sections[i].IsEmpty())
      {
         //if ( m_Sections.GetAt(i) != _T(" ") )
         if ( m_Sections[i] != _T(" ") )
         {
            if (nSkipFirstEmptyLine)
            {
               //tmpLine.Format(_T("[%s]\n"),m_Sections.GetAt(i));
               tmpLine.Format(_T("[%s]\n"),m_Sections[i]);
               nSkipFirstEmptyLine = 0;
            }
            else
            {
               tmpLine.Format(_T("\n[%s]\n"),m_Sections[i]);//make it more readiable
            }
            file.WriteString(tmpLine);
         }
      }

      for (UINT j=0;j<m_Keys[i].size();j++)
      {
         //if key is empts we don't write "="
         tmpLine.Format(_T("%s%s%s\n"),m_Keys[i][j],
                               m_Keys[i][j].IsEmpty()?_T(""):_T("="),
                               m_Values[i][j]);
      
         file.WriteString(tmpLine);
      }
   }

   file.Close();

   return m_BackupFileName;
}

//////////////////////////////////////////////////////////////////////
BOOL CIniEx::GetWriteWhenChange(void)
{
   return m_writeWhenChange;
}

//////////////////////////////////////////////////////////////////////
void CIniEx::SetWriteWhenChange(BOOL WriteWhenChange)
{
   m_writeWhenChange=WriteWhenChange;
}

//////////////////////////////////////////////////////////////////////
void CIniEx::SetBackupFileName(CString &backupFile)
{

   m_BackupFileName=backupFile;

}

//////////////////////////////////////////////////////////////////////
void CIniEx::FindBackupFile(void)
{

   WIN32_FIND_DATA ffData;
   BOOL bContinue=TRUE;
   CString filePath(m_FileName);
   CString ext;
   int nPlace=filePath.ReverseFind('.');
   filePath.Delete(nPlace,filePath.GetLength()-nPlace);
   filePath+="*.*";
   int extNo=0;
   TCHAR *p;
   HANDLE handle=FindFirstFile(filePath,&ffData);
   while (bContinue)
   {
      bContinue=FindNextFile(handle,&ffData);
      p=ffData.cFileName;
      p+=_tcslen(ffData.cFileName)-3;
      if (_tstoi(p)>extNo) extNo=_tstoi(p);
   }
   m_BackupFileName.Format(_T("%s.%.3d"),m_FileName,extNo+1);
}

//////////////////////////////////////////////////////////////////////
void CIniEx::ResetContent()
{

//   int j;

   //for ( j=0; j<m_Keys.GetSize();j++ )
   //   delete m_Keys[j];
   m_Keys.clear();

   //for ( j=0; j<m_Values.GetSize();j++ )
   //   delete m_Values[j];
   m_Values.clear();

   m_Sections.clear();
   m_SectionNo=-1;
   m_FileName="";
   m_Changed=FALSE;

}

//////////////////////////////////////////////////////////////////////
//Removes key and it's value from given section
BOOL CIniEx::RemoveKey(CString Section,CString Key)
{

   int nIndex=LookupSection(&Section);
   if (nIndex==-1) return FALSE;

   int nKeyIndex=LookupKey(nIndex,&Key);
   if (nKeyIndex==-1) return FALSE;

   m_Keys[nIndex].erase(m_Keys[nIndex].begin() + nKeyIndex);
   m_Values[nIndex].erase(m_Values[nIndex].begin() + nKeyIndex);
   m_Changed=TRUE;


   return TRUE;
}

//////////////////////////////////////////////////////////////////////
//Removes key and it's value 
BOOL CIniEx::RemoveKey(CString Key)
{
   return RemoveKey(_T(" "),Key);
}

//////////////////////////////////////////////////////////////////////
//Removes given section(including all keys and values)
//return FALSE when given section not found
BOOL CIniEx::RemoveSection(CString Section)
{

   int nIndex=LookupSection(&Section);
   if (nIndex==-1) return FALSE;

//    delete m_Keys[nIndex];
//   delete m_Values[nIndex];

   m_Keys.erase(m_Keys.begin() + nIndex);
   m_Values.erase(m_Values.begin() + nIndex);

   //m_Sections.RemoveAt(nIndex);
   m_Sections.erase(m_Sections.begin() + nIndex);
   m_SectionNo--;
   m_Changed=TRUE;

   return TRUE;
}

//////////////////////////////////////////////////////////////////////
int CIniEx::CompareStrings(const CString *str1,const CString *str2)
{
   int nRetVal;

   if (m_NoCaseSensitive)
   {
      nRetVal = str1->CompareNoCase(*str2);
   }
   else
   {
      nRetVal = str1->Compare(*str2);
   }

   return nRetVal;
}

//////////////////////////////////////////////////////////////////////
//copy each string (section name) because 
//if sections parametter be a pointer it may clear content of member
void CIniEx::GetSections(CStringArray sections)
{
   for (UINT i=0;i<m_Sections.size();i++)
   {
      sections.Add(m_Sections[i]);
   }

}

//////////////////////////////////////////////////////////////////////
void CIniEx::GetKeysInSection(CString section, CStringArray &keys)
{

   int nIndex=LookupSection(&section);
   if (nIndex==-1) return;

   for (UINT i=0;i<m_Keys[nIndex].size();i++)
   {
      keys.Add(m_Keys[nIndex][i]);
   }

}

////////////////////////////////////////////////////////////
BOOL CIniEx::OpenSearchDirectory(LPCTSTR pFileName,
                                 BOOL writeWhenChange,
                                 BOOL createIfNotExist,
                                 BOOL noCaseSensitive,
                                 BOOL makeBackup)
{

   if ( OpenAtExeDirectory(pFileName,
                           writeWhenChange,
                           createIfNotExist,
                           noCaseSensitive,
                           makeBackup)
      ) return true;

   if ( OpenAtWindowsDirectory(pFileName,
                               writeWhenChange,
                               createIfNotExist,
                               noCaseSensitive,
                               makeBackup)
      ) return true;

   return false;
}

BOOL CIniEx::OpenAtWindowsDirectory(LPCTSTR pFileName,
                           BOOL writeWhenChange,
                           BOOL createIfNotExist,
                           BOOL noCaseSensitive,
                           BOOL makeBackup)
{
   BOOL bRetVal;


   CString filePath;
   TCHAR curDir[MAX_PATH];

   if ( !GetWindowsDirectory(curDir,MAX_PATH) ) return false;
   filePath=curDir;
   filePath+=_T("\\");
   filePath+=pFileName;
   bRetVal = Open(filePath,writeWhenChange,createIfNotExist,noCaseSensitive,makeBackup);

   return bRetVal;
}

 