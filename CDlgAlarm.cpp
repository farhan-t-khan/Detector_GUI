// CDlgAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgAlarm.h"
#include "afxdialogex.h"
#include "Detector_guiDlg.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
// CDlgAlarm dialog

IMPLEMENT_DYNAMIC(CDlgAlarm, CDialogEx)

CDlgAlarm::CDlgAlarm(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ALARM, pParent)
   , m_Settings(*CSettings::GetInstance())
   , m_bAlarmLevelChange(false)
{

}

CDlgAlarm::~CDlgAlarm()
{
}

void CDlgAlarm::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_BUTTON_2, m_Btn2);
   DDX_Control(pDX, IDC_BUTTON_3, m_Btn3);
   DDX_Control(pDX, IDC_EDIT_ALARM_LEVEL, m_EditCtrlAlarmLevel);
   DDX_Control(pDX, IDC_STATIC_ALARM_LEVEL, m_AlarmLevelStatic);
   DDX_Control(pDX, IDC_STATIC_WAVE_FILE, m_StaticWavFile);
   DDX_Control(pDX, IDC_EDIT_WAVE_FILE, m_EditCtrlWavFile);
   DDX_Control(pDX, IDC_BUTTON_WAV_FILE, m_BtnWavFile);
   DDX_Control(pDX, IDC_SLIDER_VOLUME, m_SliderCtrlVolume);
   DDX_Control(pDX, IDC_BUTTON_TEST_VOLUME, m_BtnTestVolume);
}


BEGIN_MESSAGE_MAP(CDlgAlarm, CDialogEx)
   ON_WM_ERASEBKGND()
   ON_BN_CLICKED(IDC_BUTTON_1, &CDlgAlarm::OnBnClickedButton1)
   ON_BN_CLICKED(IDC_BUTTON_2, &CDlgAlarm::OnBnClickedButton2)
   ON_STN_CLICKED(IDC_BUTTON_3, &CDlgAlarm::OnStnClickedButton3)
   ON_STN_CLICKED(IDC_BUTTON_4, &CDlgAlarm::OnStnClickedButton4)
   ON_STN_CLICKED(IDC_BUTTON_5, &CDlgAlarm::OnStnClickedButton5)
   ON_STN_CLICKED(IDC_BUTTON_6, &CDlgAlarm::OnStnClickedButton6)
   ON_STN_CLICKED(IDC_BUTTON_7, &CDlgAlarm::OnStnClickedButton7)
   ON_STN_CLICKED(IDC_BUTTON_8, &CDlgAlarm::OnStnClickedButton8)
   ON_STN_CLICKED(IDC_BUTTON_9, &CDlgAlarm::OnStnClickedButton9)
   ON_STN_CLICKED(IDC_BUTTON_0, &CDlgAlarm::OnStnClickedButton0)
   ON_STN_CLICKED(IDC_BUTTON_X, &CDlgAlarm::OnStnClickedButtonX)
   ON_STN_CLICKED(IDC_BUTTON_CHECK, &CDlgAlarm::OnStnClickedButtonCheck)
   ON_BN_CLICKED(IDC_BUTTON_WAV_FILE, &CDlgAlarm::OnBnClickedButtonWavFile)
   ON_EN_SETFOCUS(IDC_EDIT_ALARM_LEVEL, &CDlgAlarm::OnEnSetfocusEditAlarmLevel)
   ON_EN_CHANGE(IDC_EDIT_ALARM_LEVEL, &CDlgAlarm::OnEnChangeEditAlarmLevel)
   ON_EN_KILLFOCUS(IDC_EDIT_ALARM_LEVEL, &CDlgAlarm::OnEnKillfocusEditAlarmLevel)
   ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlgAlarm message handlers
BOOL CDlgAlarm::OnEraseBkgnd(CDC* pDC)
{
   CRect r;
   pDC->GetClipBox(&r);
   pDC->FillSolidRect(r, WHITE_COLOR);
   return TRUE;// CDialogEx::OnEraseBkgnd(pDC);
}

BOOL CDlgAlarm::OnInitDialog()
{
   CDialogEx::OnInitDialog();
   HBITMAP hAndle = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_1));
   ((CButton*)GetDlgItem(IDC_BUTTON_1))->SetBitmap(hAndle);
   m_BtnWavFile.DrawTransparent();
   m_BtnWavFile.SetIcon(IDI_ICON_FOLDER_OPEN);
   CString sTemp;
   m_EditCtrlWavFile.SetWindowText(m_Settings.m_sWavFile);
   sTemp.Format(_T("%d"), m_Settings.m_nAlarmLevel);
   m_EditCtrlAlarmLevel.SetWindowText(sTemp);
   m_BtnTestVolume.DrawTransparent();
   m_BtnTestVolume.SetBitmaps(IDB_BITMAP_BUTTON_TEST, WHITE_COLOR);
   m_BtnTestVolume.SetSound(m_Settings.m_sWavFile, 0, TRUE, TRUE);

   m_SliderCtrlVolume.SetRange(0, 100);
   m_SliderCtrlVolume.SetPageSize(2);
   m_SliderCtrlVolume.SetPos(int(GetMasterVolumeScalar() * 100 + .5));
   //m_Btn3.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_3)));
   //m_Btn2.SetSound(_T("c:\\BascomTurnerMLDS\\Data\\Alarm07.wav"), 0, TRUE, FALSE);
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

float CDlgAlarm::GetMasterVolumeScalar(void)
{
   HRESULT hr = NULL;

   CoInitialize(NULL);
   IMMDeviceEnumerator *deviceEnumerator = NULL;
   hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
      __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
   IMMDevice *defaultDevice = NULL;

   hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
   deviceEnumerator->Release();
   deviceEnumerator = NULL;

   IAudioEndpointVolume *endpointVolume = NULL;
   hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
      CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
   defaultDevice->Release();
   defaultDevice = NULL;

   // -------------------------
   float currentVolume = 0;
   hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);

   if (FAILED(hr))
   {
      AfxMessageBox(_T("Failed to get Master volume."));
   }
   endpointVolume->Release();
   CoUninitialize();
   return currentVolume;
}

bool CDlgAlarm::ChangeVolume(double nVolume, bool bScalar)
{
   HRESULT hr = NULL;
   bool decibels = false;
   bool scalar = false;
   double newVolume = nVolume;

   CoInitialize(NULL);
   IMMDeviceEnumerator *deviceEnumerator = NULL;
   hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
      __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
   IMMDevice *defaultDevice = NULL;

   hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
   deviceEnumerator->Release();
   deviceEnumerator = NULL;

   IAudioEndpointVolume *endpointVolume = NULL;
   hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
      CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
   defaultDevice->Release();
   defaultDevice = NULL;

   // -------------------------
   float currentVolume = 0;
   endpointVolume->GetMasterVolumeLevel(&currentVolume);
   //printf("Current volume in dB is: %f\n", currentVolume);

   hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
   //CString strCur=L"";
   //strCur.Format(_T("%f"),currentVolume);
   //AfxMessageBox(strCur);

   // printf("Current volume as a scalar is: %f\n", currentVolume);
   if (bScalar == false)
   {
      hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
   }
   else if (bScalar == true)
   {
      hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
   }
   endpointVolume->Release();

   CoUninitialize();

   return FALSE;
}

void CDlgAlarm::OnBnClickedButton1()
{
   UpdateAlarmLevel(_T("1"));
}

void CDlgAlarm::Clearppm(CString& sString)
{
   m_EditCtrlAlarmLevel.GetWindowText(sString);
   if (sString.Find(_T("ppm")) >= 0)
   {
      sString.Empty();
   }
   CWnd*pWnd = m_EditCtrlAlarmLevel.GetFocus();
   if (!pWnd || pWnd->m_hWnd != m_EditCtrlAlarmLevel.m_hWnd)
      m_EditCtrlAlarmLevel.SetFocus();
}

void CDlgAlarm::UpdateAlarmLevel(TCHAR *pzVal)
{
   CString sText;
   Clearppm(sText);
   sText += pzVal;

   m_EditCtrlAlarmLevel.SetWindowText(sText);
   m_EditCtrlAlarmLevel.SetSel(-1);
}


void CDlgAlarm::OnBnClickedButton2()
{
   UpdateAlarmLevel(_T("2"));
}


void CDlgAlarm::OnStnClickedButton3()
{
   UpdateAlarmLevel(_T("3"));
}

void CDlgAlarm::OnStnClickedButton4()
{
   UpdateAlarmLevel(_T("4"));
}

void CDlgAlarm::OnStnClickedButton5()
{
   UpdateAlarmLevel(_T("5"));
}

void CDlgAlarm::OnStnClickedButton6()
{
   UpdateAlarmLevel(_T("6"));
}

void CDlgAlarm::OnStnClickedButton7()
{
   UpdateAlarmLevel(_T("7"));
}

void CDlgAlarm::OnStnClickedButton8()
{
   UpdateAlarmLevel(_T("8"));
}

void CDlgAlarm::OnStnClickedButton9()
{
   UpdateAlarmLevel(_T("9"));
}

void CDlgAlarm::OnStnClickedButton0()
{
   UpdateAlarmLevel(_T("0"));
}

void CDlgAlarm::OnStnClickedButtonX()
{
   CString sText;
   //sText.Format(_T("%d ppm"), m_Settings.m_nAlarmLevel);
   sText.Format(_T(""), m_Settings.m_nAlarmLevel);
   m_EditCtrlAlarmLevel.SetWindowText(sText);
   m_EditCtrlAlarmLevel.EnableWindow(FALSE);
   m_EditCtrlAlarmLevel.EnableWindow(TRUE);
}

void CDlgAlarm::OnStnClickedButtonCheck()
{
   CString sString;
   m_EditCtrlAlarmLevel.GetWindowText(sString);
   if (sString.Find(_T("ppm")) >= 0)
   {
      sString.Replace(_T("ppm"), 0);
   }
   int nLevel = _ttoi(sString);
   sString.Format(_T("%d"), nLevel);
   m_EditCtrlAlarmLevel.SetWindowText(sString);
   m_EditCtrlAlarmLevel.EnableWindow(FALSE);
   m_EditCtrlAlarmLevel.EnableWindow(TRUE);

   CDetectorguiDlg* pParent = (CDetectorguiDlg*)GetParent();
   pParent->m_SetupDlg.m_EditCtrlAlarmLevel.SetWindowText(sString);
   m_Settings.SaveAlarmLevel(nLevel);
   pParent->m_plotGraph.AddAlarmLine();
}

void CDlgAlarm::OnBnClickedButtonWavFile()
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
      pParent->m_SetupDlg.m_EditCtrlWavFile.SetWindowText(sFileName);
      m_Settings.SaveAlarmWavFile(sFileName);
      m_BtnTestVolume.SetSound(sFileName, 0, TRUE, TRUE);
   }
}


void CDlgAlarm::OnEnSetfocusEditAlarmLevel()
{
   m_EditCtrlAlarmLevel.SendMessage(WM_KILLFOCUS, 0, 0);
   return;
   CString sString;
   m_EditCtrlAlarmLevel.GetWindowText(sString);
   if (sString.Find(_T("ppm")) >= 0)
   {
      sString.Delete(sString.GetLength() - 4, 4);
      m_EditCtrlAlarmLevel.SetWindowText(sString);
   }
}


void CDlgAlarm::OnEnChangeEditAlarmLevel()
{
   m_bAlarmLevelChange = true;
}


void CDlgAlarm::OnEnKillfocusEditAlarmLevel()
{
   if (m_bAlarmLevelChange)
   {
      m_bAlarmLevelChange = false;
      CString sString;
      m_EditCtrlAlarmLevel.GetWindowText(sString);
      /*if (sString.Find(_T("ppm")) >= 0)
      {
         sString.Replace(_T("ppm"), 0);
      }*/
      int nLevel = _ttoi(sString);
      if (nLevel > 200) nLevel = 200;
      if (nLevel < 0) nLevel = 0;
      sString.Format(_T("%d"), nLevel);
      m_EditCtrlAlarmLevel.SetWindowText(sString);
   }
}


void CDlgAlarm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
   if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_VOLUME)
   {
      int   nPos = ((CSliderCtrl*)pScrollBar)->GetPos();
      double dNewVolume = nPos / 100.0;
      ChangeVolume(dNewVolume, true);
   }
   CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
