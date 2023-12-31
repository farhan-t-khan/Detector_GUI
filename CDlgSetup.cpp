// CDlgSetup.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgSetup.h"
#include "afxdialogex.h"
#include "Detector_guiDlg.h"
#include "CPleaseWaitDlg.h"
#include "CDlgSelectBT.h"
#include "CDlgExportKml.h"



// CDlgSetup dialog

IMPLEMENT_DYNAMIC(CDlgSetup, CDialogEx)

CDlgSetup::CDlgSetup(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETUP, pParent)
   , m_Settings(*CSettings::GetInstance())
   , m_bXAxisChange(false)
   , m_bYAxisChange(false)
   , m_bCalTimeChange(false)
   , m_bAlarmLevelChange(false)
   , m_bSurveyPathChange(false)
   , m_bCalPathChange(false)
{

}

CDlgSetup::~CDlgSetup()
{
}

void CDlgSetup::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC_VERSION, m_StaticVersions);
   DDX_Control(pDX, IDC_CHECK_SUB_BACKGROUND, m_CheckSubBackground);
   DDX_Control(pDX, IDC_STATIC_SURVEY, m_SurveyStatic);
   DDX_Control(pDX, IDC_STATIC_CAL_DATA, m_CalDataStatic);
   DDX_Control(pDX, IDC_STATIC_ALARM_LEVEL, m_AlarmLevelStatic);
   DDX_Control(pDX, IDC_STATIC_XAXIS, m_XAxisStatic);
   DDX_Control(pDX, IDC_STATIC_FILENAME, m_FileNameStatic);
   DDX_Control(pDX, IDC_STATIC_CSV, m_CsvStatic);
   DDX_Control(pDX, IDC_EDIT_ALARM_LEVEL, m_EditCtrlAlarmLevel);
   DDX_Control(pDX, IDC_EDIT_SURVEY_PATH, m_EditCtrlSurveyPath);
   DDX_Control(pDX, IDC_EDIT_CAL_PATH, m_EditCtrlCalPath);
   DDX_Control(pDX, IDC_STATIC_CAL_DUE, m_StaticCalDue);
   DDX_Control(pDX, IDC_CHECK_24H_CLOCK, m_Check24hClock);
   DDX_Control(pDX, IDC_EDIT_CAL_DUE, m_EditCtrlCalDue);
   DDX_Control(pDX, IDC_BUTTON_OPEN_SURVEY_PATH, m_BtnSurveyPath);
   DDX_Control(pDX, IDC_BUTTON_OPEN_CAL_PATH, m_BtnCalDataPath);
   DDX_Control(pDX, IDC_STATIC_WAVE_FILE, m_StaticWavFile);
   DDX_Control(pDX, IDC_EDIT_WAVE_FILE, m_EditCtrlWavFile);
   DDX_Control(pDX, IDC_BUTTON_WAV_FILE, m_BtnWavFile);
   DDX_Control(pDX, IDC_CHECK_DATE, m_CheckDate);
   DDX_Control(pDX, IDC_CHECK_TIME, m_CheckTime);
   DDX_Control(pDX, IDC_STATIC_FILENAME2, m_StaticLocation);
   DDX_Control(pDX, IDC_EDIT2, m_EditCtrlLocation);
   DDX_Control(pDX, IDC_STATIC_YAXIS, m_YAxisStatic);
   DDX_Control(pDX, IDC_EDIT_XAXIS, m_EditCtrlXAxis);
   DDX_Control(pDX, IDC_EDIT_YAXIS, m_EditCtrlYAxis);
}


BEGIN_MESSAGE_MAP(CDlgSetup, CDialogEx)
   ON_WM_ERASEBKGND()
   ON_WM_PAINT()
   ON_EN_CHANGE(IDC_EDIT_ALARM_LEVEL, &CDlgSetup::OnEnChangeEditAlarmLevel)
   ON_BN_CLICKED(IDC_BUTTON_OPEN_SURVEY_PATH, &CDlgSetup::OnBnClickedButtonOpenSurveyPath)
   ON_BN_CLICKED(IDC_BUTTON_OPEN_CAL_PATH, &CDlgSetup::OnBnClickedButtonOpenCalPath)
   ON_BN_CLICKED(IDC_BUTTON_WAV_FILE, &CDlgSetup::OnBnClickedButtonWavFile)
   ON_BN_CLICKED(IDC_CHECK_DATE, &CDlgSetup::OnBnClickedCheckDate)
   ON_BN_CLICKED(IDC_CHECK_TIME, &CDlgSetup::OnBnClickedCheckTime)
   ON_BN_CLICKED(IDC_CHECK_24H_CLOCK, &CDlgSetup::OnBnClickedCheck24hClock)
   ON_EN_CHANGE(IDC_EDIT_LOCATION, &CDlgSetup::OnEnChangeEditLocation)
   ON_EN_CHANGE(IDC_EDIT_XAXIS, &CDlgSetup::OnEnChangeEditXaxis)
   ON_EN_CHANGE(IDC_EDIT_YAXIS, &CDlgSetup::OnEnChangeEditYaxis)
   ON_BN_CLICKED(IDC_CHECK_SUB_BACKGROUND, &CDlgSetup::OnBnClickedCheckSubBackground)
   ON_EN_KILLFOCUS(IDC_EDIT_XAXIS, &CDlgSetup::OnEnKillfocusEditXaxis)
   ON_EN_KILLFOCUS(IDC_EDIT_YAXIS, &CDlgSetup::OnEnKillfocusEditYaxis)
   ON_EN_CHANGE(IDC_EDIT_CAL_DUE, &CDlgSetup::OnEnChangeEditCalDue)
   ON_EN_KILLFOCUS(IDC_EDIT_CAL_DUE, &CDlgSetup::OnEnKillfocusEditCalDue)
   ON_EN_SETFOCUS(IDC_EDIT_ALARM_LEVEL, &CDlgSetup::OnEnSetfocusEditAlarmLevel)
   ON_EN_KILLFOCUS(IDC_EDIT_ALARM_LEVEL, &CDlgSetup::OnEnKillfocusEditAlarmLevel)
   ON_EN_CHANGE(IDC_EDIT_SURVEY_PATH, &CDlgSetup::OnEnChangeEditSurveyPath)
   ON_EN_CHANGE(IDC_EDIT_CAL_PATH, &CDlgSetup::OnEnChangeEditCalPath)
   ON_EN_KILLFOCUS(IDC_EDIT_SURVEY_PATH, &CDlgSetup::OnEnKillfocusEditSurveyPath)
   ON_EN_KILLFOCUS(IDC_EDIT_CAL_PATH, &CDlgSetup::OnEnKillfocusEditCalPath)
   ON_WM_SHOWWINDOW()
   ON_BN_CLICKED(IDC_BUTTON_UPDATE_FW, &CDlgSetup::OnBnClickedButtonUpdateFw)
   ON_EN_CHANGE(IDC_EDIT_WAVE_FILE, &CDlgSetup::OnEnChangeEditWaveFile)
    ON_STN_CLICKED(IDC_STATIC_YAXIS, &CDlgSetup::OnStnClickedStaticYaxis)
END_MESSAGE_MAP()


// CDlgSetup message handlers


BOOL CDlgSetup::OnEraseBkgnd(CDC* pDC)
{
   CRect r;
   pDC->GetClipBox(&r);
   pDC->FillSolidRect(r, WHITE_COLOR);
   return TRUE;// CDialogEx::OnEraseBkgnd(pDC);
}


void CDlgSetup::OnPaint()
{
   CPaintDC dc(this); // device context for painting
                      // TODO: Add your message handler code here
                      // Do not call CDialogEx::OnPaint() for painting messages
   CRect rect;
   GetWindowRect(rect);
   ScreenToClient(rect);
   int nHeight = int(rect.Height() * .12);
   rect.top = nHeight;
   rect.bottom = nHeight;
   CBrush brush;

   brush.CreateSolidBrush(BLACK_COLOR);
   CPoint point;
   point.x = 0;
   point.y = nHeight;
   dc.MoveTo(point);
   point.x = rect.Width();
   dc.LineTo(point);
   //dc.FrameRect(rect, &brush);
   
   brush.DeleteObject();
}


BOOL CDlgSetup::OnInitDialog()
{
   CDialogEx::OnInitDialog();

   m_StaticVersions.m_bTransparent = true;
   m_StaticVersions.SetTextColor(BLUE_COLOR);

   CString sTemp;
   m_EditCtrlSurveyPath.SetWindowText(m_Settings.m_sSurveyDataPath);
   m_EditCtrlCalPath.SetWindowText(m_Settings.m_sCalDataPath);
   m_EditCtrlWavFile.SetWindowText(m_Settings.m_sWavFile);
   sTemp.Format(_T("%d"), m_Settings.m_nAlarmLevel);
   m_EditCtrlAlarmLevel.SetWindowText(sTemp);
   sTemp.Format(_T("%d"), m_Settings.m_nCalDuePeriod);
   m_EditCtrlCalDue.SetWindowText(sTemp);
   m_EditCtrlLocation.SetWindowText(m_Settings.m_fileNameInfo.sLocation);
   sTemp.Format(_T("%d"), m_Settings.m_nXAxis);
   m_EditCtrlXAxis.SetWindowText(sTemp);
   sTemp.Format(_T("%d"), m_Settings.m_nYAxis);
   m_EditCtrlYAxis.SetWindowText(sTemp);



   m_BtnSurveyPath.DrawTransparent();
   m_BtnSurveyPath.SetIcon(IDI_ICON_FOLDER_OPEN);
   m_BtnCalDataPath.DrawTransparent();
   m_BtnCalDataPath.SetIcon(IDI_ICON_FOLDER_OPEN);
   m_BtnWavFile.DrawTransparent();
   m_BtnWavFile.SetIcon(IDI_ICON_FOLDER_OPEN);
   m_Check24hClock.SetCheck(m_Settings.m_b24hClock);
   m_CheckDate.SetCheck(m_Settings.m_fileNameInfo.bDate);
   m_CheckTime.SetCheck(m_Settings.m_fileNameInfo.bTime);
   m_CheckSubBackground.SetCheck(m_Settings.m_bSubtractBackground);
   SetCsvFileName();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSetup::OnEnChangeEditAlarmLevel()
{
   m_bAlarmLevelChange = true;
}

CString CDlgSetup::SelectDataFolder(CEdit * pEditCtrl, CString &sTitle)
{
   CString sFileName;
   pEditCtrl->GetWindowText(sFileName);
   CFolderPickerDialog  pickFolder(sFileName, 0, NULL, 0);

   pickFolder.m_ofn.lpstrInitialDir = sFileName;
   pickFolder.m_ofn.lpstrTitle = sTitle;
   if (pickFolder.DoModal() == IDOK)
   {
      sFileName = pickFolder.GetPathName();
      pEditCtrl->SetWindowText(sFileName);
   }
   else
   {
      sFileName.Empty();
   }
   return sFileName;
}



void CDlgSetup::OnBnClickedButtonOpenSurveyPath()
{
   CEdit* pEditCtrl = &m_EditCtrlSurveyPath;
   CString sTitle = _T("Survey Data Path");

   CString sFolder = SelectDataFolder(pEditCtrl, sTitle);
   if (!sFolder.IsEmpty())
   {
      m_Settings.SaveSurveyDataPath(sFolder);
   }
}



void CDlgSetup::OnBnClickedButtonOpenCalPath()
{
   CEdit* pEditCtrl = &m_EditCtrlCalPath;
   CString sTitle = _T("Cal Data Path");

   CString sFolder = SelectDataFolder(pEditCtrl, sTitle);
   if (!sFolder.IsEmpty())
   {
      m_Settings.SaveCalPath(sFolder);
   }
}


void CDlgSetup::OnBnClickedButtonWavFile()
{
   CEdit* pEditCtrl = &m_EditCtrlWavFile;
   CString sTitle = _T("Alarm Wav File");

   CFileDialog pickFile(TRUE, _T("*.wav"), 0, 0, 0, this, 0, TRUE);
   CString sFileName;
   pEditCtrl->GetWindowText(sFileName);
   pickFile.m_ofn.lpstrInitialDir = sFileName;
   pickFile.m_ofn.lpstrTitle = sTitle;
   if (pickFile.DoModal() == IDOK)
   {
      sFileName = pickFile.GetPathName();
      pEditCtrl->SetWindowText(sFileName);
      CDetectorguiDlg* pParent = (CDetectorguiDlg*)GetParent();
      pParent->m_AlarmDlg.m_EditCtrlWavFile.SetWindowText(sFileName);
      pParent->m_AlarmDlg.m_BtnTestVolume.SetSound(sFileName, 0, TRUE, TRUE);
      m_Settings.SaveAlarmWavFile(sFileName);
   }
}

CString CDlgSetup::ConstructFileName(void)
{
   CString sFullName;
   CString sLocation;
   m_EditCtrlLocation.GetWindowText(sLocation);
   CTime date = CTime::GetCurrentTime();
   int nHour = date.GetHour();
   bool b24hClock = !!m_Check24hClock.GetCheck();
   bool bDate = !!m_CheckDate.GetCheck();
   bool bTime = !!m_CheckTime.GetCheck();
   if (nHour > 12 && !b24hClock)
   {
      nHour -= 12;
   }
   sprintf(m_szDate, "%02d-%02d-%02d", date.GetMonth(), date.GetDay(), date.GetYear());
   if (bDate && bTime)
   {
      sFullName.Format(_T("%02d%02d%d_%02d%02d_%s"), date.GetMonth(), date.GetDay(), date.GetYear(), nHour, date.GetMinute(), sLocation.GetString());

   }
   else if (bDate)
   {
      sFullName.Format(_T("%02d%02d%d_%s"), date.GetMonth(), date.GetDay(), date.GetYear(), sLocation.GetString());

   }
   else if (bTime)
   {
      sFullName.Format(_T("%02d%02d_%s"), nHour, date.GetMinute(), sLocation.GetString());
   }
   else
   {
      sFullName = sLocation;
   }
   sFullName = CheckCsvFileName(sFullName);
   m_sLastFileName = sFullName;
   return sFullName;
}

CString CDlgSetup::SetCsvFileName()
{
   CString sFullName = ConstructFileName() + _T(".csv");
   m_CsvStatic.SetWindowText(sFullName);


   return sFullName;
}

CString CDlgSetup::CheckCsvFileName(CString sName)
{
   sName.Replace(_T("&"), _T("&&"));
   sName.Remove('\\');
   sName.Remove('/');
   sName.Remove(':');
   sName.Remove('*');
   sName.Remove('?');
   sName.Remove('"');
   sName.Remove('<');
   sName.Remove('>');
   sName.Remove('|');
   return sName;
}

void CDlgSetup::OnBnClickedCheckDate()
{
   SetCsvFileName();
   m_Settings.SaveFileNameDate(!!m_CheckDate.GetCheck());
}


void CDlgSetup::OnBnClickedCheckTime()
{
   SetCsvFileName();
   m_Settings.SaveFileNameTime(!!m_CheckTime.GetCheck());
}


void CDlgSetup::OnBnClickedCheck24hClock()
{
   SetCsvFileName();
   m_Settings.Save24hClock(!!m_Check24hClock.GetCheck());
   
}


void CDlgSetup::OnEnChangeEditLocation()
{
   SetCsvFileName();
   CString sLocation;
   m_EditCtrlLocation.GetWindowText(sLocation);
   m_Settings.SaveFileNameLocation(sLocation);
}

void CDlgSetup::OnBnClickedCheckSubBackground()
{
   m_Settings.SaveSubstractBrk(!!m_CheckSubBackground.GetCheck());
}


void CDlgSetup::OnEnChangeEditXaxis()
{
   m_bXAxisChange = true;
}


void CDlgSetup::OnEnChangeEditYaxis()
{
   m_bYAxisChange = true;

}

void CDlgSetup::OnEnKillfocusEditXaxis()
{
   if (!m_bXAxisChange)
      return;
   m_bXAxisChange = false;
   CString sVal;
   m_EditCtrlXAxis.GetWindowText(sVal);
   int nValue = _tstoi(sVal);
   CDetectorguiDlg* pGui = (CDetectorguiDlg*)GetParent();
   if (nValue < 10) nValue = 10;
   if (nValue > 90) nValue = 90;
   sVal.Format(L"%d",nValue);
   m_EditCtrlXAxis.SetWindowText(sVal);
   if (nValue > 0)
   {
      pGui->m_plotGraph.SetXRange(nValue);
      m_Settings.SaveXAxis(nValue);
      pGui->m_plotGraph.AddAlarmLine();
   }
}


void CDlgSetup::OnEnKillfocusEditYaxis()
{
   if (!m_bYAxisChange)
      return;
   m_bYAxisChange = false;
   CString sVal;
   m_EditCtrlYAxis.GetWindowText(sVal);
   int nValue = _tstoi(sVal);
   CDetectorguiDlg* pGui = (CDetectorguiDlg*)GetParent();
   if (nValue < 10) nValue = 10;
   if (nValue > 200) nValue = 200;
   sVal.Format(L"%d", nValue);
   m_EditCtrlYAxis.SetWindowText(sVal);
   
   
   if (nValue > 0)
   {
      pGui->m_plotGraph.SetYRange(nValue);
      m_Settings.SaveYAxis(nValue);
   }
}


void CDlgSetup::OnEnChangeEditCalDue()
{
   m_bCalTimeChange = true;
}


void CDlgSetup::OnEnKillfocusEditCalDue()
{
   if (m_bCalTimeChange)
   {
      m_bCalTimeChange = false;
      CString sVal;
      m_EditCtrlCalDue.GetWindowText(sVal);
      int nValue = _tstoi(sVal);
      m_Settings.SaveCalTimeDue(nValue);
   }
}


void CDlgSetup::OnCancel()
{
   //avoid closing dialog
}


void CDlgSetup::OnOK()
{
   //avoid closing dialog
}


void CDlgSetup::OnEnSetfocusEditAlarmLevel()
{
   CString sString;
   m_EditCtrlAlarmLevel.GetWindowText(sString);
   if (sString.Find(_T("ppm")) >= 0)
   {
      sString.Delete(sString.GetLength() - 4, 4);
      m_EditCtrlAlarmLevel.SetWindowText(sString);
   }
}


void CDlgSetup::OnEnKillfocusEditAlarmLevel()
{
   if (m_bAlarmLevelChange)
   {
      m_bAlarmLevelChange = false;
      CString sString;
      m_EditCtrlAlarmLevel.GetWindowText(sString);
     /* if (sString.Find(_T("ppm")) >= 0)
      {
         sString.Replace(_T("ppm"), 0);
      }*/
      int nLevel = _ttoi(sString);
      if (nLevel > 200) nLevel = 200;
      if (nLevel < 0) nLevel = 0;
      sString.Format(_T("%d"), nLevel);
      m_EditCtrlAlarmLevel.SetWindowText(sString);

      CDetectorguiDlg* pParent = (CDetectorguiDlg*)GetParent();
      pParent->m_AlarmDlg.m_EditCtrlAlarmLevel.SetWindowText(sString);
      m_Settings.SaveAlarmLevel(nLevel);
      pParent->m_plotGraph.AddAlarmLine();
   }
}


void CDlgSetup::OnEnChangeEditSurveyPath()
{
   m_bSurveyPathChange = true;
}


void CDlgSetup::OnEnChangeEditCalPath()
{
   m_bCalPathChange = true;
}


void CDlgSetup::OnEnKillfocusEditSurveyPath()
{
   if (m_bSurveyPathChange)
   {
      CString sFolder;
      m_EditCtrlSurveyPath.GetWindowText(sFolder);
      if (!sFolder.IsEmpty())
      {
         m_Settings.SaveSurveyDataPath(sFolder);
      }
      m_bSurveyPathChange = false;
   }
}


void CDlgSetup::OnEnKillfocusEditCalPath()
{
   if (m_bCalPathChange)
   {
      CString sFolder;
      m_EditCtrlCalPath.GetWindowText(sFolder);
      if (!sFolder.IsEmpty())
      {
         m_Settings.SaveCalPath(sFolder);
      }
      m_bCalPathChange = false;
   }
}


void CDlgSetup::OnShowWindow(BOOL bShow, UINT nStatus)
{
   CDialogEx::OnShowWindow(bShow, nStatus);

   if (bShow)
   {
      CString sLabel;
      m_StaticVersions.GetWindowText(sLabel);
      CDetectorguiDlg* pGui = (CDetectorguiDlg*)GetParent();
      if (sLabel.IsEmpty() && strlen(pGui->m_MyComms.m_CalRecords.InstrumentRecord) <= 128)
      {

         //"SN: BTI3df2               SW Release: V1.0            FW Release: V1.0"
         //BT00001,SWV1.0,HWV1.1
         char szBT[64] = { 0 };
         char szBTMac[64] = { 0 };
         char szFWV[32] = { 0 };
         char szHWV[32] = { 0 };
         char szDacOff[32] = { 0 };
         char szTecTemp[32] = { 0 };
         char szF1P[32] = { 0 };
         char szF2P[32] = { 0 };
         char szRamp[32] = { 0 };
         char szModul[32] = { 0 };
         char szFitA[32] = { 0 };
         char szFitB[32] = { 0 };
         char szFitD[32] = { 0 };
         char szFitE[32] = { 0 };
         
         if (sscanf(pGui->m_MyComms.m_CalRecords.InstrumentRecord, "%63[^,],%31[^,],%31[^,],%31[^,],%31[^,],%31[^,],%31[^,],%31[^,],%31[^,],%31[^,],%31[^,],%31[^,],%31[^,]", szBT, szFWV, szHWV,szDacOff,szTecTemp,szF1P,szF2P,szRamp,szModul,szFitA,szFitB,szFitD,szFitE) == 13)
         {
            char szLabel[256] = { 0 };
            CString sBTName = CDlgSelectBT::m_st_sBTName.Mid(12);
            CT2A szBTName(sBTName);
            sprintf(szLabel, "SN: %s              FW Release: %s            HW Release: %s",
               (char*)szBTName, szFWV, szHWV);
           // szBT, szFWV, szHWV);
            CA2T sRecord(szLabel);
            sLabel = sRecord;
            m_StaticVersions.SetWindowText(sLabel);
         }

      }
   }
}


void CDlgSetup::OnBnClickedButtonUpdateFw()
{
   CDetectorguiDlg* pGui = (CDetectorguiDlg*)GetParent();
   CWnd* pCheckLaser = pGui->GetDlgItem(IDC_BUTTON_START_STOP);
   CString sLaserBtnStatus;
   pCheckLaser->GetWindowText(sLaserBtnStatus);
   
   if (sLaserBtnStatus == _T("Stop Laser"))
   {
      AfxMessageBox(_T("Please stop laser first"));
      return;
   }

   CString sFileName;

   CA2T sDefault(pGui->m_MyComms.m_szFWFile);
   sFileName = sDefault;
   CFileDialog pickFile(TRUE, _T("*.hex"), 0, 0, _T("hex File (*.hex)|*.hex||"), this, 0, TRUE);

   //pickFile.m_ofn.lpstrInitialDir = sFileName;
   pickFile.m_ofn.lpstrTitle = _T("Select FW File");
   if (pickFile.DoModal() == IDOK)
   {
      sFileName = pickFile.GetPathName();
      if (sFileName.GetLength() > ARRAYSIZE(pGui->m_MyComms.m_szFWFile) - 1)
      {
         CString sError;
         sError.Format(_T("File name length = %d is greater than %d"), sFileName.GetLength(), ARRAYSIZE(pGui->m_MyComms.m_szFWFile) - 1);
         AfxMessageBox(sError);
         return;
      }
      CT2A szFile(sFileName);
      strcpy(pGui->m_MyComms.m_szFWFileProduction, szFile);
      pGui->m_MyComms.Mode = CBlueTooth::Download_Code;
      CPleaseWaitDlg dlg;
      pGui->m_pCurrentProgressDlg = &dlg;
      GetDlgItem(IDC_BUTTON_UPDATE_FW)->ShowWindow(SW_HIDE);
      dlg.DoModal();
      pGui->m_pCurrentProgressDlg = 0;
   }

}


void CDlgSetup::OnEnChangeEditWaveFile()
{
   // TODO:  If this is a RICHEDIT control, the control will not
   // send this notification unless you override the CDialogEx::OnInitDialog()
   // function and call CRichEditCtrl().SetEventMask()
   // with the ENM_CHANGE flag ORed into the mask.

   // TODO:  Add your control notification handler code here
}




void CDlgSetup::OnStnClickedStaticYaxis()
{
    // TODO: Add your control notification handler code here
}
