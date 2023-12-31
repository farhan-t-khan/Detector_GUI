// CDlgExportKml.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgExportKml.h"
#include "afxdialogex.h"
#include <vector>
#include <algorithm>
#include "Detector_guiDlg.h"

// CDlgExportKml dialog

IMPLEMENT_DYNAMIC(CDlgExportKml, CDialogEx)

CDlgExportKml::CDlgExportKml(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_EXPORT_KML, pParent)
   , m_Settings(*CSettings::GetInstance())
{

}

CDlgExportKml::~CDlgExportKml()
{
}

void CDlgExportKml::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC_START_DATE, m_StaticStartDate);
   DDX_Control(pDX, IDC_STATIC_END_DATE, m_StaticEndDate);
   DDX_Control(pDX, IDC_STATIC_LOCATION, m_StaticLocation);
   DDX_Control(pDX, IDC_BUTTON_OK, m_BtnOk);
   DDX_Control(pDX, IDC_LIST_KML, m_KmlListCtrl);
   DDX_Control(pDX, IDC_DATETIMEPICKER_STARTTIME, m_StartDateCtrl);
   DDX_Control(pDX, IDC_DATETIMEPICKER_ENDTIME, m_EndDateCtrl);
   DDX_Control(pDX, IDC_EDIT_LOCATION, m_EditLocationCtrl);
}


BEGIN_MESSAGE_MAP(CDlgExportKml, CDialogEx)
   ON_WM_ERASEBKGND()
   ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgExportKml::OnBnClickedButtonOk)
   ON_NOTIFY(HDN_ITEMCHANGED, 0, &CDlgExportKml::OnHdnItemchangedListKml)
   ON_NOTIFY(NM_CLICK, IDC_LIST_KML, &CDlgExportKml::OnNMClickListKml)
   ON_BN_CLICKED(IDC_BUTTON_DISPLAY, &CDlgExportKml::OnBnClickedButtonDisplay)
END_MESSAGE_MAP()


// CDlgExportKml message handlers
BOOL CDlgExportKml::OnEraseBkgnd(CDC* pDC)
{
   CRect r;
   pDC->GetClipBox(&r);
   pDC->FillSolidRect(r, WHITE_COLOR);
   return TRUE;// CDialogEx::OnEraseBkgnd(pDC);
}

void CDlgExportKml::OnCancel()
{
   // TODO: Add your specialized code here and/or call the base class


}



BOOL CDlgExportKml::OnInitDialog()
{
   CDialogEx::OnInitDialog();
   m_BtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, WHITE_COLOR);
   m_BtnOk.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WHITE_COLOR);
   m_BtnOk.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WHITE_COLOR);
   m_BtnOk.SetFlatBorderAlways();


   LVCOLUMN lvColumn;
   ::ZeroMemory(&lvColumn, sizeof(lvColumn));
   lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
   lvColumn.fmt = LVCFMT_CENTER;
   lvColumn.cx = 125;
   lvColumn.pszText = _T("Record");
   m_KmlListCtrl.InsertColumn(0, &lvColumn);

   lvColumn.pszText = _T("Date");
   m_KmlListCtrl.InsertColumn(1, &lvColumn);

   lvColumn.pszText = _T("Time");
   m_KmlListCtrl.InsertColumn(2, &lvColumn);

   lvColumn.pszText = _T("Location");
   m_KmlListCtrl.InsertColumn(3, &lvColumn);

   lvColumn.pszText = _T("Peak");
   m_KmlListCtrl.InsertColumn(4, &lvColumn);

   m_KmlListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

   lvColumn.mask = LVCF_FMT;
   lvColumn.fmt = LVCFMT_CENTER;
   m_KmlListCtrl.SetColumn(0, &lvColumn);




   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}


// Ascending date sorting function
struct KmlInfo
{
   CTime time;
   CString sLocation;
   CString sKmlFileName;
   int nPeak;
};
struct CompareKmlInfo
{
   bool operator()(const KmlInfo& rpStart, const KmlInfo& rpEnd)
   {
      if (rpStart.time < rpEnd.time)
      {
         return true;
      }
      else if (rpStart.time == rpEnd.time)
      {
         return rpStart.nPeak < rpEnd.nPeak;
      }
      return false;
   }
};


void CDlgExportKml::OnBnClickedButtonOk()
{
   CTime tStartTime, tEndTime;
   m_StartDateCtrl.GetTime(tStartTime);
   tStartTime = CTime(tStartTime.GetYear(), tStartTime.GetMonth(), tStartTime.GetDay(), 0, 0, 0);
   m_EndDateCtrl.GetTime(tEndTime);
   tEndTime = CTime(tEndTime.GetYear(), tEndTime.GetMonth(), tEndTime.GetDay(), 0, 0, 0);
   CTimeSpan oneDay(1, 0, 0, 0);
   tEndTime += oneDay;
   CString sTestLocation;
   GetDlgItem(IDC_EDIT_LOCATION)->GetWindowText(sTestLocation);

   std::vector<KmlInfo> vKml;
   CString sSearch = m_Settings.m_sSurveyDataPath + _T("\\Sur*.kml");
   CFileFind finder;
   BOOL bFileFound = finder.FindFile(sSearch);
   m_vKmlFiles.clear();
   while (bFileFound)
   {
      CString sFileName;
      bFileFound = finder.FindNextFile();
      sFileName = finder.GetFileName();
      if (sFileName == _T(".") || sFileName == _T("..") || finder.IsDirectory())
      {
         continue;
      }
      CString sFileNameR = sFileName;
      sFileNameR.Replace(_T(".kml"), _T("R.csv"));
      CString sKmlFileName = m_Settings.m_sSurveyDataPath + _T("\\") + sFileName;
      //m_vKmlFiles.push_back(m_Settings.m_sSurveyDataPath + _T("\\") + sFileName);
      FILE* pFile = _tfopen(m_Settings.m_sSurveyDataPath + _T("\\") + sFileNameR, _T("r"));
      if (!pFile)
      {
         AfxMessageBox(_T("Failed to open file"));
         continue;
      }
      TCHAR szLine[64];
      float fPeakMax = 0.0f;
      float fPeak = 0.0f;
      while (_fgetts(szLine, ARRAYSIZE(szLine), pFile) != NULL)
      {

         //11:02:17.517,42.210581,-71.178653,69.0
         double dTemp = 0.0;
         TCHAR szData[64] = { 0 };
         if (_stscanf(szLine, _T("%[^,],%lf,%lf,%f"), szData, &dTemp, &dTemp, &fPeak) == 4)
         {
            if (fPeak > fPeakMax)
            {
               fPeakMax = fPeak;
            }
         }
      }
      
      
      fclose(pFile);
      

      sFileName.Delete(0, 3);
      sFileName.Delete(sFileName.GetLength() - 4, 4);
      int nMonth = 0, nDay = 0, nYear = 0, nHour = 0, nMinute = 0;
      TRACE(_T("%s\n"), sFileName);
      TCHAR szLocation[MAX_PATH] = { 0 };
      if (_stscanf(sFileName.GetString(), _T("%2d%2d%4d_%2d%2d_%[^\n\r]"), &nMonth, &nDay, &nYear, &nHour, &nMinute, szLocation) == 6)
      {
         CTime time(nYear, nMonth, nDay, nHour, nMinute, 0);
         if (time >= tStartTime && time <= tEndTime && (sTestLocation.IsEmpty() || sTestLocation == szLocation))
         {
            KmlInfo temp;
            temp.time = time;
            temp.sKmlFileName = sKmlFileName;
            temp.sLocation = szLocation;
            temp.nPeak = static_cast<int>(std::round(fPeakMax));
            vKml.push_back(temp);
            
         }
      }
   }
   std::sort(vKml.begin(), vKml.end(), CompareKmlInfo());
   m_KmlListCtrl.DeleteAllItems();
   LVITEM lvItem = { 0 };
   TCHAR szRecord[64];
   CString sTemp;
   for(size_t nRow = 0; nRow < vKml.size(); nRow++)
   {
      LVITEM lvItem;
      lvItem.mask = LVIF_TEXT;
      lvItem.iSubItem = 0;
      lvItem.iItem = (int)nRow;
      _stprintf(szRecord, _T("%d"), int(nRow + 1));
      lvItem.pszText = szRecord;
      int nItem = m_KmlListCtrl.InsertItem(&lvItem);

      sTemp.Format(_T("%2d/%2d/%d"), vKml[nRow].time.GetMonth(), vKml[nRow].time.GetDay(), vKml[nRow].time.GetYear());

      m_KmlListCtrl.SetItemText(nItem, 1, sTemp);
      sTemp.Format(_T("%2d:%02d"), vKml[nRow].time.GetHour(), vKml[nRow].time.GetMinute());
      m_KmlListCtrl.SetItemText(nItem, 2, sTemp);

      m_KmlListCtrl.SetItemText(nItem, 3, vKml[nRow].sLocation);
      sTemp.Format(_T("%d"), vKml[nRow].nPeak);
      m_KmlListCtrl.SetItemText(nItem, 4, sTemp);
      m_vKmlFiles.push_back(vKml[nRow].sKmlFileName);
   }
}




void CDlgExportKml::OnHdnItemchangedListKml(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
   // TODO: Add your control notification handler code here
   int nSelection = m_KmlListCtrl.GetSelectionMark();
   if (nSelection >= 0)
   {
      GetDlgItem(IDC_BUTTON_DISPLAY)->EnableWindow(TRUE);
   }

   *pResult = 0;
}


void CDlgExportKml::OnNMClickListKml(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
   int nSelection = m_KmlListCtrl.GetSelectionMark();
   if (nSelection >= 0)
   {
      GetDlgItem(IDC_BUTTON_DISPLAY)->EnableWindow(TRUE);
   }
   *pResult = 0;
}


void CDlgExportKml::OnBnClickedButtonDisplay()
{
   int nSelection = m_KmlListCtrl.GetSelectionMark();
   if (nSelection >= 0)
   {
      ShellExecute(0, 0, m_vKmlFiles[nSelection], 0, 0, SW_SHOW);
   }
   else
   {
      AfxMessageBox(_T("Please select a kml record"));
   }
}
