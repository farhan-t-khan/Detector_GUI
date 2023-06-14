//////////////////////////////////////////////////////////////////////
// IniEx.h: interface for the CIniEx class.
//
//////////////////////////////////////////////////////////////////////
// no malloc, no realloc, no free --> more fun

#if !defined(AFX_INIEX_H__36888C4C_12D3_4F65_A78B_2F3C3576B5B8__INCLUDED_)
#define AFX_INIEX_H__36888C4C_12D3_4F65_A78B_2F3C3576B5B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include <vector>
 
#define MAX_SECTION_COUNT  256

class CIniEx  
{
private:
   //Functions
   int      LookupKey      (int nSectionIndex,CString *Key);
   int      LookupSection  (CString *Section);
   int      CompareStrings (const CString *str1,const CString *str2);
   void  FindBackupFile (void);
private:
   CString  m_ErrStr;
   std::vector<CString> m_Sections;
   //CTypedPtrArray<CObArray, CStringArray*> m_Keys;
   typedef std::vector<CString> CStringVector;
   std::vector<CStringVector > m_Keys;
   //CTypedPtrArray<CObArray, CStringArray*> m_Values;
   std::vector<CStringVector > m_Values;
   int      m_SectionNo;
   BOOL  m_NoCaseSensitive;
   BOOL  m_writeWhenChange;
   CString  m_BackupFileName;
   CString m_FileName;
   BOOL  m_makeBackup;
   BOOL  m_Changed;  
   int m_nEditControl;
public:
   void GetKeysInSection(CString section,CStringArray &keys);
   void GetSections(CStringArray sections);

   CIniEx();
   virtual ~CIniEx();
   void SetEditControlType(int nEditControl);
   BOOL Open(LPCTSTR pFileName,
                        BOOL writeWhenChange=TRUE,
                        BOOL createIfNotExist=TRUE,
                        BOOL noCaseSensitive=TRUE,
                        BOOL makeBackup=FALSE);
   BOOL OpenAtExeDirectory(LPCTSTR pFileName,
                        BOOL writeWhenChange=TRUE,
                        BOOL createIfNotExist=TRUE,
                        BOOL noCaseSensitive=TRUE,
                        BOOL makeBackup=FALSE);
   BOOL OpenAtWindowsDirectory(LPCTSTR pFileName,
                        BOOL writeWhenChange=TRUE,
                        BOOL createIfNotExist=TRUE,
                        BOOL noCaseSensitive=TRUE,
                        BOOL makeBackup=FALSE);
   BOOL OpenSearchDirectory(  LPCTSTR pFileName,
                        BOOL writeWhenChange=TRUE,
                        BOOL createIfNotExist=TRUE,
                        BOOL noCaseSensitive=TRUE,
                        BOOL makeBackup=FALSE);
   void ResetContent();

   CString WriteFile(BOOL makeBackup=FALSE);

   CString GetValue(CString Section,CString Key,CString DefaultValue=_T(""));
   CString GetValue(CString Key);

   void SetValue(CString Key,CString Value);
   void SetValue(CString Section,CString Key,CString Value);

   BOOL RemoveKey(CString Key);
   BOOL RemoveKey(CString Section,CString Key);

   BOOL RemoveSection(CString Section);

   BOOL GetWriteWhenChange();
   void SetWriteWhenChange(BOOL WriteWhenChange);

   void SetBackupFileName(CString &backupFile);
};

#endif // !defined(AFX_INIEX_H__36888C4C_12D3_4F65_A78B_2F3C3576B5B8__INCLUDED_)
