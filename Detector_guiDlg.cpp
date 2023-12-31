
// Detector_guiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "Detector_guiDlg.h"
#include "afxdialogex.h"
#include "CGdiPlusBitmap.h"
#include "CDlgPopup.h"
#include "CDlgCalCellPopup.h"
#include "CDlgCalPassFail.h"
#include "CDlgSurveyName.h"
#include "CDlgStabilizing.h"
#include "CDlgGpsFix.h"
#include "CDlgSurveyKmlSave.h"
#include "CDlgMessageBox.h"
#include "CDlgSetup.h"
#include "CBlueTooth.h"
#include "math.h"
#include "mmsystem.h"
#include "windowsx.h"
#include "SerialGPS.h"
#include "Profiler.h"
#include "Settings.h"
#include "SaveCal.h"
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma comment(lib, "IPHLPAPI.lib")

extern "C" {
#include "setupapi.h"
#include "hidsdi.h"
#include "process.h"
}

const int UWM_BUMP_TEST = WM_APP_BASE + 1;
const int UWM_UPDATE_GRAPH = WM_APP_BASE + 2;
const int UWM_RESET_UI = WM_APP_BASE + 3;
const int UWM_GPS_BTN_UPDATE = WM_APP_BASE + 4;

// CDetectorguiDlg dialog

// NMEA GPS Setup strings
const unsigned char GPGSA_OFF[16] = { 0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x32 };
const unsigned char GPGGA_OFF[16] = { 0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24 };
const unsigned char GPGLL_OFF[16] = { 0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x2B };
const unsigned char GPGSV_OFF[16] = { 0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x39 };
const unsigned char GPRMC_OFF[16] = { 0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x40 };
const unsigned char GPVTG_OFF[16] = { 0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x47 };

const unsigned char NAVPVT_ON[16] = { 0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x18,0xE1 };
const unsigned char NAVPOSLLH_ON[16] = { 0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x02,0x00,0x01,0x00,0x00,0x00,0x00,0x13,0xBE };
const unsigned char NAVSTATUS_ON[16] = { 0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x03,0x00,0x01,0x00,0x00,0x00,0x00,0x14,0xC5 };

const unsigned char GPS_10Hz[16] = { 0xB5,0x62,0x06,0x08,0x06,0x00,0x64,0x00,0x01,0x00,0x01,0x00,0x7A,0x12 };
const unsigned char GPS_5Hz[16] = { 0xB5,0x62,0x06,0x08,0x06,0x00,0xC8,0x00,0x01,0x00,0x01,0x00,0xDE,0x6A };
const unsigned char GPS_1Hz[16] = { 0xB5,0x62,0x06,0x08,0x06,0x00,0xE8,0x03,0x01,0x00,0x01,0x00,0x01,0x39 };

float fBgppm = 1.9f;
float g_fFitO = 0.0f;
float g_fFitG = 1.0f;
vector<short> v_min1floc, v_max1floc, v_min2floc;

filter g_MedFilter;



void structUpdateGraph::resize(UINT nNewSize)
{
   if (nNewSize == nMaxPoints) return;
   clear();
   nMaxPoints = nNewSize;
   pdX = new double[nNewSize];
   pdY = new double[nNewSize];
}

CDetectorguiDlg::CDetectorguiDlg(CWnd* pParent /*=nullptr*/)
   : CDialogEx(IDD_DETECTOR_GUI_DIALOG, pParent)
   , m_Settings(*CSettings::GetInstance())
   , m_bInitComplete(false)
   , m_pWinThread(0)
   , m_bStandby(false)
{
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDetectorguiDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_BUTTON_SETUP, m_BtnSetup);
   DDX_Control(pDX, IDC_BUTTON_START_STOP, m_BtnStartStop);
   DDX_Control(pDX, IDC_BUTTON_LOG_DATA, m_BtnLogData);
   DDX_Control(pDX, IDC_BUTTON_CALIBRATE, m_BtnCalibrate);
   DDX_Control(pDX, IDC_BUTTON_ALARM_SET, m_BtnAlarmSet);
   DDX_Control(pDX, IDC_BUTTON_EXPORT_KLM, m_BtnExportKML);
   DDX_Control(pDX, IDC_STATIC_LOGO, m_LogoBitmap);
   DDX_Control(pDX, IDC_STATIC_STATUS, m_StaticStatus);
   DDX_Control(pDX, IDC_STATIC_STATUS_MSG, m_StaticStatusMsg);
   DDX_Control(pDX, IDC_CHECK_LASER, m_LaserCheck);
   DDX_Control(pDX, IDC_CHECK_BLUETHOOTH, m_BluetoothCheck);
   DDX_Control(pDX, IDC_CHECK_GPS, m_BtnGps);
   DDX_Control(pDX, IDC_CHECK_BATTERY, m_BtnBattery);
   DDX_Control(pDX, IDC_DebugWindow, DW);
}

BEGIN_MESSAGE_MAP(CDetectorguiDlg, CDialogEx)
   ON_WM_PAINT()
   ON_WM_QUERYDRAGICON()
   ON_WM_ERASEBKGND()
   ON_MESSAGE(UWM_BUMP_TEST, OnBumpTest)
   ON_MESSAGE(UWM_UPDATE_GRAPH, OnUpdateGraph)
   ON_MESSAGE(UWM_RESET_UI, OnResetGui)
   ON_MESSAGE(UWM_GPS_BTN_UPDATE, OnGPSBtnUpdate)
   

   ON_WM_TIMER()
   ON_BN_CLICKED(IDC_BUTTON_SETUP, &CDetectorguiDlg::OnBnClickedButtonSetup)
   ON_BN_CLICKED(IDC_BUTTON_LOG_DATA, &CDetectorguiDlg::OnBnClickedButtonLogData)
   ON_BN_CLICKED(IDC_BUTTON_CALIBRATE, &CDetectorguiDlg::OnBnClickedButtonCalibrate)
   ON_BN_CLICKED(IDC_BUTTON_START_STOP, &CDetectorguiDlg::OnBnClickedButtonStartStop)
   ON_WM_DESTROY()
   ON_BN_CLICKED(IDC_BUTTON_ALARM_SET, &CDetectorguiDlg::OnBnClickedButtonAlarmSet)
   ON_BN_CLICKED(IDC_BUTTON_EXPORT_KLM, &CDetectorguiDlg::OnBnClickedButtonExportKlm)
   ON_WM_CLOSE()
   ON_COMMAND(ID_ACCELERATOR_HIDDEN, &CDetectorguiDlg::OnAcceleratorHidden)
   ON_COMMAND(ID_ACCELERATOR_FW, &CDetectorguiDlg::OnAcceleratorFw)
   ON_BN_CLICKED(IDC_CHECK_LASER, &CDetectorguiDlg::OnBnClickedCheckLaser)
    ON_BN_CLICKED(IDC_CHECK_BATTERY, &CDetectorguiDlg::OnBnClickedCheckBattery)
END_MESSAGE_MAP()


// CDetectorguiDlg message handlers

BOOL CDetectorguiDlg::OnInitDialog()
{
   //CString sSurvey = _T("c:\\BascomTurnerMLDS\\Data\\Survey\\Sur11152018_1101_ClemtestG.csv");
   //CString sKlm = _T("c:\\BascomTurnerMLDS\\Data\\Survey\\Sur11152018_1101_Clemtest.klm");
   //CreateKMLFile(sSurvey, sKlm);

   CDialogEx::OnInitDialog();
   EnableWindow(FALSE);
   // Set the icon for this dialog.  The framework does this automatically
   //  when the application's main window is not a dialog
   SetIcon(m_hIcon, TRUE);			// Set big icon
   SetIcon(m_hIcon, FALSE);		// Set small icon

   // TODO: Add extra initialization here

   CButtonST* buttons[] = { &m_BtnSetup,  &m_BtnStartStop, &m_BtnLogData, &m_BtnCalibrate, &m_BtnAlarmSet, &m_BtnExportKML };

   for (int i = 0; i < ARRAYSIZE(buttons); i++)
   {
      buttons[i]->DrawTransparent();
      buttons[i]->SetColor(CButtonST::BTNST_COLOR_FG_IN, WHITE_COLOR, FALSE);
      buttons[i]->SetColor(CButtonST::BTNST_COLOR_FG_OUT, WHITE_COLOR, FALSE);
      buttons[i]->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, WHITE_COLOR, FALSE);
   }

   Gdiplus::GdiplusStartupInput gdiplusStartupInput;
   Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

   CGdiPlusBitmapResource plusImage;
   plusImage.Load(IDB_FULL_BT_LOGO_PNG, _T("PNG"));
   Gdiplus::Color backColor(0xff, 0xff, 0xff);
   //backColor = RGB(244, 244, 240);//GetSysColor(COLOR_WINDOW);
   HBITMAP hBitmapOriginal;
   plusImage.m_pBitmap->GetHBITMAP(backColor, &hBitmapOriginal);

   CRect rect;
   m_LogoBitmap.GetClientRect(rect);
   BITMAP bm;
   GetObject(hBitmapOriginal, sizeof(bm), &bm);
   CDC dc;
   dc.CreateCompatibleDC(m_LogoBitmap.GetDC());
   HBITMAP hbmOld = (HBITMAP)SelectObject(dc.m_hDC, hBitmapOriginal);
   m_hBitmapLogo = PaintNewImage(&dc, bm, rect);
   m_LogoBitmap.SetBitmap(m_hBitmapLogo);
   ::SelectObject(dc.m_hDC, hbmOld);
   ::DeleteObject(hBitmapOriginal);
   m_StaticStatus.m_bTransparent = true;
   m_StaticStatus.SetTextColor(WHITE_COLOR);
   m_StaticStatusMsg.m_bTransparent = true;
   m_StaticStatusMsg.SetTextColor(GREEN_COLOR);
   ::SetWindowTheme(m_LaserCheck.m_hWnd, _T("wstr"), _T("wstr"));

   m_BluetoothCheck.SetMyTextColor(WHITE_COLOR);
   m_LaserCheck.SetMyTextColor(WHITE_COLOR);
   m_BtnGps.SetMyTextColor(WHITE_COLOR);
   m_BtnBattery.SetMyTextColor(WHITE_COLOR);

   SetTimer(FLASH_FINISH_TIMER, 3000, 0);

   LOGFONT lf;
   memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
   m_StaticStatusMsg.GetFont()->GetLogFont(&lf);
   lf.lfHeight -= 4;
   //lf.lfQuality = CLEARTYPE_QUALITY;
   lf.lfWeight = FW_BOLD;
   m_StatusFont.CreateFontIndirect(&lf);
   //m_StatusFont.CreatePointFont(180, _T("Cambria"));

   m_StaticStatusMsg.SetFont(&m_StatusFont);

   // DW = static_cast<CListBox*>(GetDlgItem(IDC_DebugWindow));
    //m_StaticLat = static_cast<CEdit*>(GetDlgItem(IDC_STATIC_LAT_TEXT));
    //CString file1 = _T("c:\\BascomTurnerMLDS\\Data\\Survey\\Sur11152018_1101_Clemtest");
    //CString file2 = _T("c:\\BascomTurnerMLDS\\Data\\Survey\\Sur11152018_1101_Clemtest.kml");
    //CreateKMLFile(file1, file2);
   m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR_SECRET)); // use same resource name as dialog

   return TRUE;  // return TRUE  unless you set the focus to a control
}

HBITMAP CDetectorguiDlg::PaintNewImage(CDC* srcCdc, BITMAP& bm, CRect& rect)
{
   HBITMAP hBitMap = ::CreateCompatibleBitmap(srcCdc->m_hDC, rect.right, rect.bottom);
   CBitmap*  pOldBitmap = NULL;
   CDC dcMem;
   dcMem.CreateCompatibleDC(srcCdc);
   ::SelectObject(dcMem.m_hDC, hBitMap);

   int nHeight = bm.bmHeight;
   int nWidth = bm.bmWidth;

   int nCol = 0;
   int nRow = 0;

   dcMem.SetStretchBltMode(HALFTONE);
   if (rect.right >= nWidth && rect.bottom >= nHeight)
   {
      long left = (rect.right - nWidth) / 2;
      long top = (rect.bottom - nHeight) / 2;

      dcMem.StretchBlt(left, top, nWidth, nHeight, srcCdc, nCol, nRow,
         nWidth, nHeight, SRCCOPY);
   }
   else
   {
      float x_ratio = (float)rect.right / (float)nWidth;
      float y_ratio = (float)rect.bottom / (float)nHeight;

      if (x_ratio > y_ratio)
      {
         x_ratio = y_ratio;
      }
      int nImageHeight = (int)(nHeight*x_ratio);
      int nImageWidth = (int)(nWidth*x_ratio);
      long left = (long)((rect.right - nWidth * x_ratio) / 2);
      long top = (long)((rect.bottom - nHeight * x_ratio) / 2);
      dcMem.StretchBlt(left, top, nImageWidth,
         nImageHeight, srcCdc, nCol, nRow,
         nWidth, nHeight, SRCCOPY);

      CBrush brushWhite(WHITE_COLOR);
      HGDIOBJ hBrushOld = dcMem.SelectObject(brushWhite);
      CPen penWhite;
      penWhite.CreatePen(PS_SOLID, 1, WHITE_COLOR);
      CPen* pOldPen = dcMem.SelectObject(&penWhite);

      CRect fillRect = rect;
      fillRect.bottom = top;
      dcMem.Rectangle(fillRect);
      fillRect.top = fillRect.bottom + nImageHeight;
      fillRect.bottom = rect.bottom;
      dcMem.Rectangle(fillRect);



      dcMem.SelectObject(hBrushOld);
      dcMem.SelectObject(pOldPen);
   }
   return hBitMap;
}

CDetectorguiDlg::~CDetectorguiDlg()
{
   ::DeleteObject(m_hBitmapLogo);
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDetectorguiDlg::OnPaint()
{
   if (IsIconic())
   {
      CPaintDC dc(this); // device context for painting

      SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

      // Center icon in client rectangle
      int cxIcon = GetSystemMetrics(SM_CXICON);
      int cyIcon = GetSystemMetrics(SM_CYICON);
      CRect rect;
      GetClientRect(&rect);
      int x = (rect.Width() - cxIcon + 1) / 2;
      int y = (rect.Height() - cyIcon + 1) / 2;

      // Draw the icon
      dc.DrawIcon(x, y, m_hIcon);
   }
   else
   {
      CPaintDC dc(this); // device context for painting
      CRect rect;
      CWnd* pWnd = GetDlgItem(IDC_STATIC_ALIGN);

      pWnd->GetWindowRect(rect);
      ScreenToClient(rect);
      dc.FillSolidRect(rect, WHITE_COLOR);
      int nWindowWith = rect.Width();


      HGDIOBJ hBrushOld = dc.SelectObject(GetStockObject(NULL_BRUSH));

      // create and select a thick, black pen
      CPen penBlack;
      penBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
      CPen* pOldPen = dc.SelectObject(&penBlack);


      // shrink our rect 20 pixels in each direction
      //rect.DeflateRect(20, 20);

      CButtonST* buttons[] = { &m_BtnSetup,  &m_BtnStartStop, &m_BtnLogData, &m_BtnCalibrate, &m_BtnAlarmSet, &m_BtnExportKML };
      CRect rectPrev;
      buttons[0]->GetWindowRect(rect);
      ScreenToClient(rect);
      rect.bottom = rect.top;
      rect.top = 0;
      dc.Rectangle(rect);
      int nHeight = rect.Height();
      int nNewHeight = int(nHeight *0.4 + .5);
      rect.left = rect.right;
      rect.bottom = nNewHeight;
      rect.right = nWindowWith + rect.right;
      dc.Rectangle(rect);
      rect.top = rect.bottom;
      rect.bottom = rect.top + nHeight - nNewHeight;
      dc.Rectangle(rect);
      for (int i = 0; i < ARRAYSIZE(buttons); i++)
      {
         buttons[i]->GetWindowRect(rect);
         ScreenToClient(rect);
         if (i > 0)
         {
            rect.top = rectPrev.top + rectPrev.Height();
         }
         dc.Rectangle(rect);
         rectPrev = rect;
      }

      // put back the old objects
      dc.SelectObject(hBrushOld);
      dc.SelectObject(pOldPen);

      CDialogEx::OnPaint();
   }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDetectorguiDlg::OnQueryDragIcon()
{
   return static_cast<HCURSOR>(m_hIcon);
}



BOOL CDetectorguiDlg::OnEraseBkgnd(CDC* pDC)
{
   CRect r;
   pDC->GetClipBox(&r);

   pDC->FillSolidRect(r, BLUE_COLOR);
   return TRUE;// CDialogEx::OnEraseBkgnd(pDC);
}

void CDetectorguiDlg::Initialize(void)
{
   if (m_bInitComplete)
   {
      return;
   }
   //for (;;) {
     // wPrintf(_T("Looking for BT Device"));
     // int x = MyComms.Look_For_Bluetooth_Devices();
     // if (x == 1){
       //  wPrintf(_T("Found BT Device"));
       //  m_Settings.m_ConnectionStatus.bBluetooth = true;
       //  break;
     // }
   //}
   m_Settings.ReadSettings();
   CWnd* pWnd = GetDlgItem(IDC_STATIC_ALIGN);
   CRect rect;
   pWnd->GetWindowRect(rect);
   ScreenToClient(rect);

   m_plotGraph.Create(IDD_DIALOG_PLOT, this);
   m_plotGraph.ShowWindow(SW_HIDE);
   m_plotGraph.MoveWindow(&rect);

   m_SetupDlg.Create(IDD_DIALOG_SETUP, this);
   m_SetupDlg.ShowWindow(SW_HIDE);
   m_SetupDlg.MoveWindow(&rect);


   m_AlarmDlg.Create(IDD_DIALOG_ALARM, this);
   m_AlarmDlg.ShowWindow(SW_HIDE);
   m_AlarmDlg.MoveWindow(&rect);


   m_ExportKmlDlg.Create(IDD_DIALOG_EXPORT_KML, this);
   m_ExportKmlDlg.ShowWindow(SW_HIDE);
   m_ExportKmlDlg.MoveWindow(&rect);

   m_SaveFileMsgDlg.Create(IDD_DIALOG_FILE_SAVE_MSG, this);
   CRect rectNew;
   m_SaveFileMsgDlg.GetWindowRect(rectNew);
   ScreenToClient(rectNew);
   m_SaveFileMsgDlg.ShowWindow(SW_HIDE);
   m_SaveFileMsgDlg.MoveWindow(rect.left + (rect.Width() - rectNew.Width()) / 2, rect.bottom - rectNew.Height(), rectNew.Width(), rectNew.Height());
   font.CreatePointFont(200, _T("Times New Roman"), 0);
   m_plotGraph.GetDlgItem(IDC_STATIC_LASER_BLOCKED)->SetFont(&font);


   Updateconnections();
   SetTimer(REFRESH_CONNECTIONS, m_Settings.m_nRefreshConnectionMS, 0);
   m_bInitComplete = true;
   m_MyComms.m_pMainGui = this;
   m_pApp_Thread = AfxBeginThread(App_Thread, this);
}

bool CDetectorguiDlg::IsGpsOK(void)
{
   return m_Settings.m_ConnectionStatus.bGPS || (m_plotGraph.m_fLat != 0.0f && m_plotGraph.m_fLong != 0.0f);
}

bool CDetectorguiDlg::IsBatteryOK(void)
{
   return m_Settings.m_ConnectionStatus.bBattery || (m_nBattery_Voltage > 10000);
}

bool CDetectorguiDlg::IsBatteryGreen(void)
{
   return m_Settings.m_ConnectionStatus.bBattery || (m_nBattery_Voltage > 12000);
}

bool CDetectorguiDlg::IsBatteryYellow(void)
{
   return m_nBattery_Voltage > 10000 && m_nBattery_Voltage <= 12000;
   //return m_nBattery_Voltage > 10000 && m_nBattery_Voltage <= 12000;
}

int CDetectorguiDlg::GetBatteryPercentage(void)
{
   return 100 * (0.1 + (0.9 / 6500 * (m_nBattery_Voltage - 10000.0)));
}

bool CDetectorguiDlg::IsLaserOK(void)
{
   ASSERT(_tcscmp(g_szLabels[Laser_Current], _T("Laser_current")) == 0);


   return m_Settings.m_ConnectionStatus.bLaser || m_MyComms.m_aData[Laser_Current] > 320;/* (m_nTec_Temperature > 1);*/
}

bool CDetectorguiDlg::IsLaserBlocked(void)
{
   bool bBlocked = false;
   if (IsLaserOK())
   {
      
      int nAverage = (m_MyComms.m_aData[avg_min_1F] + m_MyComms.m_aData[avg_max_1F]) / 2;
      if (nAverage < 100)
      {
         bBlocked = true;
      }
   }
   return bBlocked;
}

void CDetectorguiDlg::Updateconnections()
{

#ifdef SIMULATE_HW

   bool aOldValues[] = { m_Settings.m_ConnectionStatus.bBluetooth, m_Settings.m_ConnectionStatus.bLaser, false, m_Settings.m_ConnectionStatus.bBattery };
   m_Settings.ReadConnectionStatus();
   bool bGPS = IsGpsOK();
   bool bBattery = IsBatteryOK();
   bool bLaser = IsLaserOK();


   if (!m_bStandby)
   {
      CString sButton;
      m_BtnStartStop.GetWindowText(sButton);
      if (m_Settings.m_ConnectionStatus.bBluetooth)
      {
         if (!m_BtnStartStop.IsWindowEnabled())
         {
            m_BtnStartStop.EnableWindow(TRUE);
            if (!m_plotGraph.IsWindowVisible())
            {
               m_LogoBitmap.ShowWindow(SW_HIDE);
               HideActivePage();
               m_plotGraph.ShowWindow(SW_NORMAL);
               ChangeStatusMsg(_T("READY"));
               m_ActivePage = READY_PAGE;
            }
         }
      }
      else if (sButton == _T("Start Laser"))
      {
         m_BtnStartStop.EnableWindow(FALSE);
         m_BtnCalibrate.EnableWindow(FALSE);
      }
   }

   CColorCheck* aChecks[] = { &m_BluetoothCheck, &m_LaserCheck, &m_BtnGps, &m_BtnBattery };
   bool aNewValues[] = { m_Settings.m_ConnectionStatus.bBluetooth, bLaser, bGPS, bBattery };
   for (int i = 0; i < ARRAYSIZE(aNewValues); i++)
   {
      aOldValues[i] = aChecks[i]->GetCheck();
      bool bCheckBoxBk = false;
      COLORREF checkBox = 0;
      if (aChecks[i] == &m_BtnBattery)
      {

         if (aNewValues[i])
         {
            if (IsBatteryYellow())
            {
               checkBox = YELLOW_CHECK_BOX;
            }
            else
            {
               checkBox = GREEN_CHECK_BOX;
            }
            if (m_BtnBattery.GetArrowColorBk() != checkBox)
            {
               m_BtnBattery.SetArrowColorBk(checkBox);
               bCheckBoxBk = true;
            }
         }
         CString sBatteryLabelOld;
         m_BtnBattery.GetWindowText(sBatteryLabelOld);
         CString sBatteryLabelNew(_T("MLDS Battery"));
         if (m_Settings.m_ConnectionStatus.bBluetooth)
         {
            sBatteryLabelNew.Format(_T("MLDS Battery %d%%"), GetBatteryPercentage());
         }
         if (sBatteryLabelNew != sBatteryLabelOld)
         {
            m_BtnBattery.SetWindowText(sBatteryLabelNew);
            bCheckBoxBk = true;
         }

      }



      if (aOldValues[i] != aNewValues[i] || bCheckBoxBk)
      {
         aChecks[i]->SetCheck(aNewValues[i]);
         CRect rect;
         aChecks[i]->GetWindowRect(&rect);
         ScreenToClient(rect);
         InvalidateRect(rect);
      }
   }

   if (m_Settings.m_ConnectionStatus.bBluetooth && bLaser /*&&
      bGPS*/ && bBattery)
   {

      CString sButton;
      m_BtnStartStop.GetWindowText(sButton);
      if (sButton != _T("Start Laser") && !m_bStandby && !m_bStartingLaser)
      {
         m_BtnCalibrate.EnableWindow(TRUE);
      }


      if (IsLaserBlocked() && !m_bStartingLaser)
      {
         m_plotGraph.GetDlgItem(IDC_STATIC_LASER_BLOCKED)->ShowWindow(SW_NORMAL);
         Sleep(100);
         PlaySound(m_Settings.m_sWavFile2, NULL, SND_ASYNC);
      }
      else
      {
         m_plotGraph.GetDlgItem(IDC_STATIC_LASER_BLOCKED)->ShowWindow(SW_HIDE);
      }
   }
}



#endif


LRESULT CDetectorguiDlg::OnGPSBtnUpdate(WPARAM wParam, LPARAM lParam)
{
   int nMode = wParam;
   COLORREF checkBoxBk = RED_CHECK_BOX;
   if (nMode == 1)
   {
      if (lParam)
      {
         checkBoxBk = YELLOW_CHECK_BOX;
      }
      else
      {
         checkBoxBk = BLUE_COLOR;
      }
   }
   if (m_BtnGps.GetArrowColorBkOffFlash() == checkBoxBk)
   {
      return 0;
   }
   m_BtnGps.SetArrowColorBkOffFlash(checkBoxBk);
   CRect rect;
   m_BtnGps.GetWindowRect(&rect);
   ScreenToClient(rect);
   InvalidateRect(rect);

   return 0;
}

void CDetectorguiDlg::OnTimer(UINT_PTR nIDEvent)
{
   if (nIDEvent == FLASH_FINISH_TIMER)
   {
      KillTimer(FLASH_FINISH_TIMER);
      Initialize();
      EnableWindow(TRUE);

   }
   else if (REFRESH_CONNECTIONS == nIDEvent)
   {
      KillTimer(nIDEvent);
      Updateconnections();
      SetTimer(REFRESH_CONNECTIONS, m_Settings.m_nRefreshConnectionMS, 0);
   }
   else if (WAIT_FOR_GPS == nIDEvent)
   {
      KillTimer(nIDEvent);
      if (IsGpsOK())
      {
         CDlgGpsFix dlg;
         dlg.SetOptions(0, _T("GSPS fix found"));
         dlg.DoModal();
         if (m_ActivePage != READY_PAGE)
         {
            HidePage(m_ActivePage);
            m_plotGraph.ShowWindow(SW_NORMAL);
            m_ActivePage = READY_PAGE;
         }
         ChangeStatusMsg(_T("SURVEY"));
         UpdateGraphForThread();
         //m_pWinThread = AfxBeginThread(UpdateGraphThread, this, THREAD_PRIORITY_NORMAL, 0);
      }
      else
      {
         SetTimer(nIDEvent, 2000, 0);
      }

   }
   else if (PROGRESS_BAR_STATUS == nIDEvent)
   {
      COLORREF redColor = RGB(255, 0, 0);
      COLORREF greenColor = RGB(0, 255, 50);
      int nLower = 0, nUpper = 100;
      m_plotGraph.m_ProgressBarStatus.GetRange(nLower, nUpper);
      int nCurrentVal = m_plotGraph.m_ProgressBarStatus.GetPos();
      int nStep = 33;
      if (nCurrentVal + nStep > nUpper)
      {
         nCurrentVal = nLower;
      }
      else
      {
         nCurrentVal += nStep;
      }
      double dElapseTime = ((double)clock() - m_plotGraph.GetLastppmUpdate()) / CLOCKS_PER_SEC;
      if (dElapseTime > 1)
      {
         m_plotGraph.m_ProgressBarNormalColor = m_plotGraph.m_ProgressBarStatus.SetBarColor(redColor);
      }
      else
      {
         m_plotGraph.m_ProgressBarNormalColor = m_plotGraph.m_ProgressBarStatus.SetBarColor(greenColor);
        // m_plotGraph.m_ProgressBarStatus.SetBarColor(m_plotGraph.m_ProgressBarNormalColor);
      }
      m_plotGraph.m_ProgressBarStatus.SetPos(nCurrentVal);
   }
   CDialogEx::OnTimer(nIDEvent);
}

void CDetectorguiDlg::HideActivePage(void)
{
   HidePage(m_ActivePage);
   m_ActivePage = NO_PAGE;
}

void CDetectorguiDlg::HidePage(PageSelection currPage)
{

   switch (currPage)
   {
   case READY_PAGE:
      m_plotGraph.ShowWindow(SW_HIDE);
      break;
   case SETUP_PAGE:
      m_SetupDlg.ShowWindow(SW_HIDE);
      break;
   case SAVE_FILE_PAGE:
      m_SaveFileMsgDlg.ShowWindow(SW_HIDE);
      break;
   case ALARM_PAGE:
      m_AlarmDlg.ShowWindow(SW_HIDE);
      break;
   case EXPORT_KML_PAGE:
      m_ExportKmlDlg.ShowWindow(SW_HIDE);
      break;
   }
}


void CDetectorguiDlg::OnBnClickedButtonSetup()
{
   if (m_ActivePage == SETUP_PAGE)
      return;
   HidePage(m_ActivePage);
   m_SetupDlg.ShowWindow(SW_NORMAL);
   ChangeStatusMsg(_T("SETUP"));
   m_ActivePage = SETUP_PAGE;

}


unsigned CDetectorguiDlg::CDetectorguiDlg::m_st_nSeriesIdForthread = 0;
unsigned CDetectorguiDlg::CDetectorguiDlg::m_st_nTerminaThread = 0;

void CDetectorguiDlg::UpdateGraphForThread(void)
{

   m_CurrentGraphInfo.resize((m_Settings.m_nXAxis + 1) * 60);
   m_CurrentGraphInfo.nCurrentPoints = 0;
   m_CurrentGraphInfo.nPenThickness = 3;
   m_CurrentGraphInfo.nPenType = PS_SOLID;
   m_CurrentGraphInfo.bSmooth = false;
   int nPoints = 1;
   double dX = 0.0;
   double dY = 0.0;// m_Settings.ppm_Measurement();
   m_CurrentGraphInfo.lastUpdate = clock();
   m_CurrentGraphInfo.nSeriesId = m_st_nSeriesIdForthread = m_plotGraph.AddSeries(m_CurrentGraphInfo.nPenThickness, m_CurrentGraphInfo.nPenType, &dX, &dY, nPoints);
}

UINT CDetectorguiDlg::UpdateGraphThread(LPVOID pParam)
{
   m_st_nTerminaThread = 0;
   CDetectorguiDlg* pMainDlg = (CDetectorguiDlg*)pParam;
   structUpdateGraph graphInfo;

   int nMaxPoints = graphInfo.nMaxPoints = int((pMainDlg->m_Settings.m_nXAxis + 1) * 60);
   int nCurrentPoints = 0;
   graphInfo.nSeriesId = m_st_nSeriesIdForthread;
   double dDelta = (double)pMainDlg->m_Settings.m_nXAxis / graphInfo.nMaxPoints;
   graphInfo.pdX = new double[graphInfo.nMaxPoints];
   graphInfo.pdY = new double[graphInfo.nMaxPoints];
   double dAmplitude = pMainDlg->m_Settings.m_nYAxis / 2.0;
   double dPeriod = pMainDlg->m_Settings.m_nXAxis / 60.0;
   graphInfo.nPenThickness = 3;
   graphInfo.nPenType = PS_SOLID;
   graphInfo.bSmooth = false;

   double dOldDelta = dDelta;
   while (!m_st_nTerminaThread)
   {
      if (nCurrentPoints >= nMaxPoints)
      {
         for (int i = 0; i < nCurrentPoints - 1; i++)
         {
            graphInfo.pdX[i] = -pMainDlg->m_Settings.m_nXAxis + i * .5;
            graphInfo.pdY[i] = graphInfo.pdY[i + 1];
         }
         graphInfo.pdX[nCurrentPoints - 1] = 0.0;
         graphInfo.pdY[nCurrentPoints - 1] = CSettings::GetInstance()->ppm_Measurement();
      }
      else
      {
         double dMinX = -(nCurrentPoints) * .5;
         for (int i = 0; i < nCurrentPoints; i++)
         {
            graphInfo.pdX[i] = dMinX + i * .5;
         }
         graphInfo.pdX[nCurrentPoints] = 0.0;
         graphInfo.pdY[nCurrentPoints] = CSettings::GetInstance()->ppm_Measurement();
         nCurrentPoints++;
         graphInfo.nCurrentPoints = nCurrentPoints;
      }
      //Update to graphs must come from same thread
      pMainDlg->SendMessage(UWM_UPDATE_GRAPH, 0, (LPARAM)&graphInfo);
      Sleep(500);
   }

   delete[] graphInfo.pdX;
   delete[] graphInfo.pdY;
   return 0;
}

void CDetectorguiDlg::WriteToSurveyFileR(CString sData)
{
#ifdef USE_PROFILER
   CProfiler func(__FUNCDNAME__);
#endif
   m_ccSyncThreads.Lock();

   if (m_pSurveyFileR)
   {
      _ftprintf(m_pSurveyFileR, _T("%s\n"), sData.GetString());
   }

   m_ccSyncThreads.Unlock();
}

void CDetectorguiDlg::WriteToSurveyFile(CString sData)
{
#ifdef USE_PROFILER
   CProfiler func(__FUNCDNAME__);
#endif
   m_ccSyncThreads.Lock();

   if (m_pSurveyFileG)
   {
      _ftprintf(m_pSurveyFileG, _T("%s\n"), sData.GetString());
   }

   m_ccSyncThreads.Unlock();
}

void CDetectorguiDlg::Plot_ppm(float fnppm, short navg_min_2F, short navg_max_2F,
   short navg_min_1F, short navg_max_1F, short navg_minlocation1, short navg_maxlocation1,
   short navg_minlocation2, short navg_maxlocation2)
{
#ifdef USE_PROFILER
   CProfiler func(__FUNCDNAME__);
#endif
   if (m_bCalibrationInProgress)
   {
      dataReadings temp;
      temp.fSample = fnppm;
      temp.aData[0] = navg_min_2F;
      temp.aData[1] = navg_max_2F;
      temp.aData[2] = navg_min_1F;
      temp.aData[3] = navg_max_1F;
      temp.aData[4] = navg_minlocation1;
      temp.aData[5] = navg_maxlocation1;
      temp.aData[6] = navg_minlocation2;
      temp.aData[7] = navg_maxlocation2;
      m_ccSyncThreads.Lock();
      m_vCalibrationReadings.push_back(temp);
      m_ccSyncThreads.Unlock();
   }
   // update median here
   //if (g_MedFilter.aMedmin1floc != filter::UNDEF)
   if (g_MedFilter.aMedmin1floc != filter::UNDEF && !IsLaserBlocked()&&
      (abs(navg_minlocation1 - g_MedFilter.aMedmin1floc) < filter::THRESHOLD_M &&
       abs(navg_maxlocation1 - g_MedFilter.aMedmax1floc) < filter::THRESHOLD_M &&
       abs(navg_minlocation2 - g_MedFilter.aMedmin2floc) < filter::THRESHOLD_M))
   {
      // min1floc
      auto it = std::upper_bound(v_min1floc.begin(), v_min1floc.end(), navg_minlocation1);
      v_min1floc.insert(it, navg_minlocation1);
      if (navg_minlocation1 > g_MedFilter.aMedmin1floc)
      {
         v_min1floc.erase(v_min1floc.begin());
      }
      else
      {
         v_min1floc.pop_back();
      }
      g_MedFilter.aMedmin1floc = v_min1floc[v_min1floc.size() / 2];

      //max1floc
      it = std::upper_bound(v_max1floc.begin(), v_max1floc.end(), navg_maxlocation1);
      v_max1floc.insert(it, navg_maxlocation1);
      if (navg_maxlocation1 > g_MedFilter.aMedmax1floc)
      {
         v_max1floc.erase(v_max1floc.begin());
      }
      else
      {
         v_max1floc.pop_back();
      }
      g_MedFilter.aMedmax1floc = v_max1floc[v_max1floc.size() / 2];

      // min2floc
      it = std::upper_bound(v_min2floc.begin(), v_min2floc.end(), navg_minlocation2);
      v_min2floc.insert(it, navg_minlocation2);
      if (navg_minlocation2 > g_MedFilter.aMedmin2floc)
      {
         v_min2floc.erase(v_min2floc.begin());
      }
      else
      {
         v_min2floc.pop_back();
      }
      g_MedFilter.aMedmin2floc = v_min2floc[v_min2floc.size() / 2];
   }
   else 
   {
      //FILE* pFile = fopen("C:\\BascomturnerMLDS\\filtered_data.txt", "a+");

      //if (pFile)
      //{
      //   fprintf(pFile, "value=%.1f min1=%d(%d) min2=%d(%d) max1=%d(%d)\n",
      //      fnppm, navg_minlocation1, g_MedFilter.aMedmin1floc,
      //      navg_minlocation2, g_MedFilter.aMedmin2floc,
      //      navg_maxlocation1, g_MedFilter.aMedmax1floc);
      //   fclose(pFile);
      //}
      return;
   }
   //g_MedFilter.aMedmax1floc = v_max1floc[m_vCalibrationReadings.size() / 2];
   //g_MedFilter.aMedmin2floc = v_min2floc[m_vCalibrationReadings.size() / 2];



/*   if (g_MedFilter.aMedmin1floc != filter::UNDEF &&
      !(abs(navg_minlocation1 - g_MedFilter.aMedmin1floc) < filter::THRESHOLD &&
         abs(navg_maxlocation1 - g_MedFilter.aMedmax1floc) < filter::THRESHOLD &&
         abs(navg_minlocation2 - g_MedFilter.aMedmin2floc) < filter::THRESHOLD)
      && IsLaserBlocked())
   {
      FILE* pFile = fopen("C:\\BascomturnerMLDS\\filtered_data.txt", "a+");

      if (pFile)
      {
         fprintf(pFile, "value=%.1f min1=%d(%d) min2=%d(%d) max1=%d(%d)\n",
            fnppm, navg_minlocation1, g_MedFilter.aMedmin1floc,
            navg_minlocation2, g_MedFilter.aMedmin2floc,
            navg_maxlocation1, g_MedFilter.aMedmax1floc);
         fclose(pFile);
      }
      return;
   }
   */
   if (m_CurrentGraphInfo.nSeriesId <= 0)
      return;
   clock_t tCurrentTime = clock();
   double dElapseTime = ((double)tCurrentTime - m_CurrentGraphInfo.lastUpdate) / CLOCKS_PER_SEC;
   m_CurrentGraphInfo.lastUpdate = tCurrentTime;
   if (m_CurrentGraphInfo.nCurrentPoints >= m_CurrentGraphInfo.nMaxPoints)
   {
      for (int i = 0; i < m_CurrentGraphInfo.nCurrentPoints - 1; i++)
      {
         m_CurrentGraphInfo.pdX[i] = m_CurrentGraphInfo.pdX[i + 1] - dElapseTime;
         m_CurrentGraphInfo.pdY[i] = m_CurrentGraphInfo.pdY[i + 1];
      }
      m_CurrentGraphInfo.pdX[m_CurrentGraphInfo.nCurrentPoints - 1] = 0.0;
     /* if (pointcounter % (m_CurrentGraphInfo.nMaxPoints / 10) == 0 && fnppm < 1.0)
      {
         m_CurrentGraphInfo.pdY[m_CurrentGraphInfo.nCurrentPoints - 1] = 2.0;
      }
      else
      {*/
         m_CurrentGraphInfo.pdY[m_CurrentGraphInfo.nCurrentPoints - 1] = fnppm;
      /*}
      }
      pointcounter++;
      if (pointcounter % m_CurrentGraphInfo.nMaxPoints == 0)
      {
         pointcounter = 0;
      }*/
      
   }
   else
   {
      double dMinX = -(m_CurrentGraphInfo.nCurrentPoints) * .5;
      for (int i = 0; i < m_CurrentGraphInfo.nCurrentPoints; i++)
      {
         m_CurrentGraphInfo.pdX[i] -= dElapseTime;
      }
      m_CurrentGraphInfo.pdX[m_CurrentGraphInfo.nCurrentPoints] = 0.0;
      //if (pointcounter % (m_CurrentGraphInfo.nMaxPoints / 10) == 0 && fnppm < 1.0)
      //{
      //   m_CurrentGraphInfo.pdY[m_CurrentGraphInfo.nCurrentPoints - 1] = 2.0;
      //}
      //else
      //{
         m_CurrentGraphInfo.pdY[m_CurrentGraphInfo.nCurrentPoints] = fnppm;
      //}
      m_CurrentGraphInfo.nCurrentPoints++;
   //   pointcounter++;
   }
   //Update to graphs must come from same thread
   if (((double)clock() - m_LastGraphUpdate) / CLOCKS_PER_SEC > 0.20)
   {
      PostMessage(UWM_UPDATE_GRAPH, 0, (LPARAM)&m_CurrentGraphInfo);
      m_LastGraphUpdate = clock();
   }

}

LRESULT CDetectorguiDlg::OnUpdateGraph(WPARAM wparam, LPARAM lParam)
{
#ifdef USE_PROFILER
   CProfiler func(__FUNCDNAME__);
#endif
   structUpdateGraph* pParam = (structUpdateGraph*)lParam;
   if (pParam->nSeriesId <= 0)
      return 0;
   //if (pParam->nCurrentPoints > 0)
   //   m_plotGraph.UpdateppmUnsafe((int)round(pParam->pdY[pParam->nCurrentPoints - 1]));
   m_st_nSeriesIdForthread = pParam->nSeriesId = m_plotGraph.UpdateSeries(pParam->nSeriesId, pParam->nPenThickness, pParam->nPenType, pParam->pdX, pParam->pdY, pParam->nCurrentPoints);

   return 0;
}

void CDetectorguiDlg::OnBnClickedButtonLogData()
{
   CButtonST* buttons[] = { &m_BtnSetup,  &m_BtnStartStop, &m_BtnCalibrate, &m_BtnAlarmSet, &m_BtnExportKML };
   CString sButton;
   m_BtnLogData.GetWindowText(sButton);

   if (sButton == _T("Start Survey"))
   {
      m_bSurveyInit = true;
      CString sFileName = m_SetupDlg.SetCsvFileName();
      CDlgSurveyName surveyNameDlg;
      CString sSurveyFilePath = m_Settings.m_sSurveyDataPath + _T("\\Sur") + sFileName;
      surveyNameDlg.SetOptions(0, _T("Survey Name?"), sSurveyFilePath);
      if (surveyNameDlg.DoModal() == IDOK)
      {
         if (m_ActivePage != READY_PAGE)
         {
            HidePage(m_ActivePage);
            m_plotGraph.ShowWindow(SW_NORMAL);
            m_ActivePage = READY_PAGE;
         }
         m_plotGraph.DummyUpdate(0);
         bool bProceed = true;
         if (!IsGpsOK())
         {
            CDlgGpsFix dlg;
            dlg.SetOptions(CDlgPopBase::GPS_PROCEED, _T("No GSPS Fix"));
            if (dlg.DoModal() == IDOK)
            {
               bProceed = true;
            }
            else
            {
               bProceed = false;
            }
         }
         m_BtnLogData.SetWindowText(_T("Stop Survey"));
         m_plotGraph.GetDlgItem(IDC_BUTTON_PAUSE)->ShowWindow(SW_NORMAL);
         m_bStandby = true;
         for (int i = 0; i < ARRAYSIZE(buttons); i++)
         {
            buttons[i]->EnableWindow(FALSE);
         }

         CString sSurveyPath = m_Settings.m_sSurveyDataPath + _T("\\Sur") + m_SetupDlg.GetLastFileName();
         //sSurveyPath += _T("G.csv");
         m_ccSyncThreads.Lock();

         m_pSurveyFileG = _tfopen(sSurveyPath + _T("G.csv"), _T("w"));
         m_pSurveyFileR = _tfopen(sSurveyPath + _T("R.csv"), _T("w"));
         m_ccSyncThreads.Unlock();

         if (bProceed)
         {
            ChangeStatusMsg(_T("SURVEY"));
            UpdateGraphForThread();
         }
         else
         {
            ChangeStatusMsg(_T("READY"));
            SetTimer(WAIT_FOR_GPS, 1000, 0);
         }
      }
      else
      {
         PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_SETUP, BN_CLICKED), (LPARAM)m_BtnSetup.m_hWnd);
      }
      m_bSurveyInit = false;
      SetTimer(PROGRESS_BAR_STATUS, 1000, nullptr);
      m_plotGraph.m_ProgressBarStatus.ShowWindow(SW_NORMAL);
   }
   else
   {
      KillTimer(WAIT_FOR_GPS);
      KillTimer(PROGRESS_BAR_STATUS);
      m_plotGraph.m_ProgressBarStatus.ShowWindow(SW_HIDE);
      ClearPlot();
      //WaitForThreads();
      m_plotGraph.DummyUpdate(0);
      ChangeStatusMsg(_T("STANDBY"));
      m_ccSyncThreads.Lock();
      if (m_pSurveyFileG)
      {
         fclose(m_pSurveyFileG);
         m_pSurveyFileG = 0;
      }
      if (m_pSurveyFileR)
      {
         fclose(m_pSurveyFileR);
         m_pSurveyFileR = 0;
      }
      m_ccSyncThreads.Unlock();
      CDlgSurveyKmlSave dlg;
      CString sSurveyPath = m_Settings.m_sSurveyDataPath + _T("\\Sur") + m_SetupDlg.GetLastFileName();
      CString sKmlPath = sSurveyPath;

      sKmlPath += _T(".kml");
      if (CreateKMLFile(sSurveyPath, sKmlPath))
      {
         dlg.SetOptions(0, _T("Survey Saved as"), _T("KML Saved as"), sSurveyPath, sKmlPath);
         dlg.DoModal();
      }
      m_BtnLogData.SetWindowText(_T("Start Survey"));
      m_plotGraph.GetDlgItem(IDC_BUTTON_PAUSE)->ShowWindow(SW_HIDE);
      for (int i = 0; i < ARRAYSIZE(buttons); i++)
      {
         buttons[i]->EnableWindow(TRUE);
      }
      m_bStandby = false;
   }
}

void CDetectorguiDlg::WriteHeader(std::ofstream& outKml)
{
   CString sPath = m_Settings.m_sSurveyDataPath;
   sPath.Replace(_T("\\"), _T("\\\\"));
   CT2A strPath(sPath);
   //'Header Information
   outKml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
   outKml << "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\"> " << std::endl;
   outKml << "<Document>" << std::endl;
   outKml << "<Style id=\"RedCross\">" << std::endl;
   outKml << "<IconStyle>" << std::endl;
   outKml << "<Icon><href> " << strPath << "\\\\GPSMAP\\\\icon63.png</href> </Icon>" << std::endl;
   outKml << "<scale>0.3</scale>" << std::endl;
   outKml << "</IconStyle>" << std::endl;
   outKml << "<LabelStyle>" << std::endl;
   outKml << "<color>ff00ffff</color>" << std::endl;
   outKml << "<colorMode>normal</colorMode>" << std::endl;
   outKml << "<scale>1</scale>" << std::endl;
   outKml << "</LabelStyle>" << std::endl;
   outKml << "</Style>" << std::endl;
   outKml << std::endl;
   outKml << "    <Style id=\"BlackDot\">" << std::endl;
   outKml << "<IconStyle>" << std::endl;
   outKml << "<Icon><href> " << strPath << "\\\\GPSMAP\\\\icon57.png</href> </Icon>" << std::endl;
   outKml << "<scale>0.3</scale>" << std::endl;
   outKml << "</IconStyle>" << std::endl;
   outKml << "<LabelStyle>" << std::endl;
   outKml << "<color>ff00ffff</color>" << std::endl;
   outKml << "<colorMode>normal</colorMode>" << std::endl;
   outKml << "<scale>0.6</scale>" << std::endl;
   outKml << "</LabelStyle>" << std::endl;
   outKml << "</Style>" << std::endl;
   outKml << "<Style id=\"BlueGreenPath\">" << std::endl;
   outKml << "<LineStyle>" << std::endl;
   outKml << "<color>ffffff00</color>" << std::endl;
   outKml << "<width>2</width>" << std::endl;
   outKml << "</LineStyle>" << std::endl;
   outKml << "</Style>" << std::endl;
   outKml << "<Style id=\"OrangeNeedle\">" << std::endl;
   outKml << "<LineStyle>" << std::endl;
   outKml << "<color>ff00ffff</color>" << std::endl;
   outKml << "<width>4</width>" << std::endl;
   outKml << "</LineStyle>" << std::endl;
   outKml << "</Style>" << std::endl;

}

bool CDetectorguiDlg::CreateKMLFile(CString& sFileSurvey, CString& sFileKML)
{
   CString sFileSurveyG = sFileSurvey + _T("G.csv");
   CString sFileSurveyR = sFileSurvey + _T("R.csv");
   CT2A sInFileG(sFileSurveyG);
   CT2A sInFileR(sFileSurveyR);
   CT2A sOutFile(sFileKML);
   std::ifstream inSurveyG(sInFileG, std::ios::in);
   std::ifstream inSurveyR(sInFileR, std::ios::in);
   if (!inSurveyG.is_open())
   {
      CString sError;
      sError.Format(_T("Failed to open file %s"), sFileSurveyG.GetString());
      AfxMessageBox(sError);
      return false;
   }
   if (!inSurveyR.is_open())
   {
      CString sError;
      sError.Format(_T("Failed to open file %s"), sFileSurveyR.GetString());
      AfxMessageBox(sError);
      return false;
   }
   std::ofstream outKml(sOutFile, std::ios::out);
   if (!outKml.is_open())
   {
      CString sError;
      sError.Format(_T("Failed to open file %s"), sFileKML.GetString());
      AfxMessageBox(sError);
      return false;
   }
   WriteHeader(outKml);
   char szContext[MAX_PATH] = { 0 };
   inSurveyR.getline(szContext, ARRAYSIZE(szContext));
   SYSTEMTIME systemTime = { 0 };
   GetLocalTime(&systemTime);
   double dGpsLat = 0.0, dGpsLon = 0.0, dDummy = 0.0;
   float fSample = 0.0f;
   while (!inSurveyR.eof())
   {
      if (sscanf(szContext, "%hu:%hu:%hu.%hu,%lf,%lf,%f", &systemTime.wHour, &systemTime.wMinute, &systemTime.wSecond,
         &systemTime.wMilliseconds, &dGpsLat, &dGpsLon, &fSample) != 7)
      {
         inSurveyR.getline(szContext, ARRAYSIZE(szContext));
         continue;
      }
      //'Write a placemark for each leak
      outKml << "<Placemark>" << std::endl;
      outKml << "<styleUrl>#RedCross</styleUrl>" << std::endl;
      outKml << "<name> " << std::setprecision(0) << fSample << " </name>" << std::endl;
      outKml << "<Point>" << std::endl;
      outKml << std::fixed << std::setprecision(6) << "<coordinates>" << dGpsLon << "," << dGpsLat << ",0</coordinates>" << std::endl;
      outKml << "</Point>" << std::endl;
      outKml << "</Placemark>" << std::endl;
      outKml << "<Placemark>" << std::endl;
      outKml << "<styleUrl>#OrangeNeedle</styleUrl>" << std::endl;
      outKml << "<LineString>" << std::endl;
      outKml << "<coordinates>" << std::endl;
      outKml << std::fixed << std::setprecision(6) << dGpsLon << "," << dGpsLat << "," << std::setprecision(2) << (fSample + 1.0) << std::endl;
      outKml << std::fixed << std::setprecision(6) << dGpsLon << "," << dGpsLat << ",1.0" << std::endl;
      outKml << "</coordinates>" << std::endl;
      outKml << "<altitudeMode>relativeToGround</altitudeMode>" << std::endl;
      outKml << "</LineString>" << std::endl;
      outKml << "</Placemark>" << std::endl;
      inSurveyR.getline(szContext, ARRAYSIZE(szContext));
   }
   inSurveyR.clear();
   inSurveyR.seekg(0, std::ios::beg);

   SYSTEMTIME systemTimeBegin = systemTime, oldTime = { 0 };

   inSurveyG.getline(szContext, ARRAYSIZE(szContext));
   while (!inSurveyG.eof() && sscanf(szContext, "%hu:%hu:%hu.%hu,%lf,%lf,%lf,%lf,%f", &systemTime.wHour, &systemTime.wMinute, &systemTime.wSecond,
      &systemTime.wMilliseconds, &dGpsLat, &dGpsLon, &dDummy, &dDummy, &fSample) != 9)
   {
      inSurveyG.getline(szContext, ARRAYSIZE(szContext));
   }

   int Count = 1;

   //Start the path
   outKml << "<Placemark>" << std::endl;
   outKml << "<name> Survey Path " << Count << "</name>" << std::endl;
   outKml << "<styleUrl>#BlueGreenPath</styleUrl>" << std::endl;
   outKml << "<LineString>" << std::endl;
   outKml << "<coordinates>" << std::endl;
   outKml << std::fixed << std::setprecision(6) << dGpsLon << "," << dGpsLat << std::endl;
   oldTime = systemTime;
   while (!inSurveyG.eof())
   {
      //'If current point > 6 seconds + BeginTime, set new begin time
      //'Else Write point and set Begin information again
      if (sscanf(szContext, "%hu:%hu:%hu.%hu,%lf,%lf,%lf,%lf,%f", &systemTime.wHour, &systemTime.wMinute, &systemTime.wSecond,
         &systemTime.wMilliseconds, &dGpsLat, &dGpsLon, &dDummy, &dDummy, &fSample) != 9)
      {
         inSurveyG.getline(szContext, ARRAYSIZE(szContext));
         continue;
      }


      if (Timediff(systemTime, oldTime) > 600000000)
      {
         //dGpsLonBegin = dGpsLon;
         //dGpsLatBegin = dGpsLat;
         //systemTimeBegin = systemTime;
         Count++;
         outKml << "</coordinates>" << std::endl;
         outKml << "</LineString>" << std::endl;
         outKml << "</Placemark>" << std::endl;
         outKml << "<Placemark>" << std::endl;
         outKml << "<name> Survey Path " << Count << "</name>" << std::endl;
         outKml << "<styleUrl>#BlueGreenPath</styleUrl>" << std::endl;
         outKml << "<LineString>" << std::endl;
         outKml << "<coordinates>" << std::endl;
         outKml << std::fixed << std::setprecision(6) << dGpsLon << "," << dGpsLat << std::endl;
      }

      //'New Line
      inSurveyG.getline(szContext, ARRAYSIZE(szContext));


      //'Write the coordinates
      outKml << std::fixed << std::setprecision(6) << dGpsLon << "," << dGpsLat << std::endl;
      oldTime = systemTime;
   }
   outKml << "</coordinates>" << std::endl;
   outKml << "</LineString>" << std::endl;
   outKml << "</Placemark>" << std::endl;
   //'End the document
   outKml << "</Document>" << std::endl;
   outKml << "</kml>" << std::endl;
   return true;
}

bool CDetectorguiDlg::CalibrationCheck(CString& sFile)
{
   FILE* pFile = _tfopen(sFile, _T("w"));
   double dAverage = 0.0;
   double dBefore = 0.0;
   double dAfter = 0.0;

   for (UINT i = 0; i < m_vCalibrationReadings.size(); i++)
   {
      dAverage += m_vCalibrationReadings[i].fSample;
   }
   if (m_vCalibrationReadings.size() != 0)
   {
      dAverage /= m_vCalibrationReadings.size();
   }
   else
   {
      ASSERT(FALSE);
      dAverage = 100.0;
   }
   //fitG = (40-fitO)/fsample
   dBefore = g_fFitG * dAverage + g_fFitO - fBgppm;
   if (fabs(dBefore - m_Settings.m_nCalibrationCell) >= 15)
   {
      return false;
   }
   //fitG * fsample + fitO - subtractBG
   g_fFitG = (m_Settings.m_nCalibrationCell - g_fFitO + fBgppm) / dAverage;

   dAfter = g_fFitG * dAverage + g_fFitO - fBgppm;

   char szRecord[64] = { 0 };
   //char szDate[64] = { 0 };
   sprintf(szRecord, "%s,%d,%.1lf,%f,%.1lf", m_SetupDlg.GetLastDateFile(),
      m_Settings.m_nCalibrationCell, dBefore, g_fFitG, dAfter);

   m_MyComms.m_CalRecords.Insert(szRecord);
   Sleep(200);

   m_MyComms.Mode = CBlueTooth::Write_Flashc_Block;
   //m_MyComms.Mode = CBlueTooth::Start_Sampling;

   if (pFile)
   {
      _ftprintf(pFile, _T("%.01f,%.01f,%.01f\n"), dAverage, dBefore, dAfter);

      fclose(pFile);
      pFile = 0;
   }

   return fabs(dAfter - m_Settings.m_nCalibrationCell) <= 1;
}

void CDetectorguiDlg::OnBnClickedButtonCalibrate()
{
   if (m_MyComms.m_CalRecords.m_WriteflashInProgress)
   {
      return;
   }
   HidePage(m_ActivePage);
   m_ActivePage = CAL_PAGE;
   ChangeStatusMsg(_T("CALIBRATE"));
   m_bDisableAlarm = true;

   CDlgCalCellPopup dlgCalCell;
   dlgCalCell.SetOptions(CDlgPopBase::CAL_OK_CANCEL, _T("Please place Calibration"), _T("Cell in Light Path."), _T("Press OK when done"));

   if (dlgCalCell.DoModal() != IDOK)
   {
      HidePage(m_ActivePage);
      ChangeStatusMsg(_T("READY"));
      m_plotGraph.ShowWindow(SW_NORMAL);
      m_ActivePage = READY_PAGE;
      m_bDisableAlarm = false;
      return;
   }





   bool bCalibrationSucess = true;
   m_vCalibrationReadings.clear();
   m_bCalibrationInProgress = true;

   EnableWindow(FALSE);
   for (int i = 0; i < 20; i++)
   {
      ProcessMessages();
      Sleep(50);
   }
   EnableWindow(TRUE);
   m_bCalibrationInProgress = false;
   m_ccSyncThreads.Lock(); m_ccSyncThreads.Unlock();
   CString sFileName;
   sFileName = m_Settings.m_sCalDataPath + _T("\\Cal") + m_SetupDlg.ConstructFileName() + _T(".csv");
   bCalibrationSucess = CalibrationCheck(sFileName);
   //SaveCal SaveCalDlg;
   CDlgCalPassFail passFailDlg;
   passFailDlg.SetResultSucess(bCalibrationSucess);
   if (bCalibrationSucess)
   {
      CTime time = CTime::GetCurrentTime();
      m_SaveFileMsgDlg.UpdateValues(_T("Calibration Saved as"), sFileName);
      m_SaveFileMsgDlg.ShowWindow(SW_NORMAL);
      m_ActivePage = SAVE_FILE_PAGE;
      CString sMessage;
      sMessage.Format(_T("Measured %d ppm Cell"), m_Settings.m_nCalibrationCell);
      passFailDlg.SetOptions(CDlgPopBase::CAL_RESULT, sMessage, _T("PASS"));
      passFailDlg.DoModal();
      //SaveCalDlg.DoModal();
      while (m_MyComms.m_CalRecords.m_WriteflashInProgress)
      {
         ProcessMessages();
         Sleep(50);
      }
      //SaveCalDlg.ShowWindow(SW_HIDE);
      dlgCalCell.SetOptions(CDlgPopBase::CAL_OK, _T("Please remove"), _T("Calibration Cell"), _T("Press OK when done"));
      dlgCalCell.DoModal();
      m_Settings.SaveCurrentCalTime(time);
      HidePage(m_ActivePage);
      ChangeStatusMsg(_T("READY"));

      m_plotGraph.ShowWindow(SW_NORMAL);
      m_ActivePage = READY_PAGE;
      //PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_START_STOP, BN_CLICKED), (LPARAM)m_BtnStartStop.m_hWnd);
   }
   else
   {
      passFailDlg.SetOptions(CDlgPopBase::CAL_RESULT, _T("Calibration "), _T("FAIL"));
      passFailDlg.DoModal();
      dlgCalCell.SetOptions(CDlgPopBase::CAL_OK, _T("Please Consult"), _T("Troubleshooting Guide"), _T("Press OK when done"));
      dlgCalCell.DoModal();

   }
   m_bDisableAlarm = false;
}

void CDetectorguiDlg::ChangeStatusMsg(CString sMessage)
{
   m_StaticStatusMsg.SetWindowText(sMessage);
   CRect rect;
   m_StaticStatusMsg.GetWindowRect(&rect);
   ScreenToClient(rect);
   InvalidateRect(rect);
}

void CDetectorguiDlg::OnBnClickedButtonStartStop()
{
   if (m_bStartingLaser)
   {
      return;
   }
   CString sButton;
   //unsigned char wb[10];
   //int sent;
   m_BtnStartStop.GetWindowText(sButton);
   /*if (m_ActivePage == SETUP_PAGE)
   {
      HWND hWnd;
      GetDlgItem(IDC_EDIT_LOCATION, &hWnd);
      ::PostMessage(hWnd, WM_SETFOCUS, 0, 0);
      SetFocus();
   }*/
   if (sButton == _T("Start Laser"))
   {

      HidePage(m_ActivePage);
      wPrintf(_T("Start pressed"));
      m_bStartingLaser = true;
      //m_BtnStartStop.EnableWindow(FALSE);
      m_BtnCalibrate.EnableWindow(FALSE);
      //if (MyComms.Mode == 1)
      //{
        // wPrintf(_T("sending start"));

        // MyComms.flush();

        // wb[0] = 'S';
        // wb[1] = 't';
        // wb[2] = 'a';
        // wb[3] = 'r';
        // wb[4] = 't';
        // sent = send(MyComms.BtSocket, (char *)wb, 5, 0);
      //}

      m_MyComms.Mode = CBlueTooth::Start_Sampling;




      ChangeStatusMsg(_T("STARTUP"));
      CString sMsg1 = _T("Stabilizing, please wait");
      CString sMsg2 = m_Settings.m_sStabilizingTimer;
      if (!m_DlgStabilizing.m_hWnd)
      {
         m_DlgStabilizing.SetOptions(CDlgPopBase::STABILIZE, sMsg1, sMsg2);
         m_DlgStabilizing.Create(IDD_DIALOG_STABILIZING, this);
      }
      else
      {
         m_DlgStabilizing.SetOptionsGui(sMsg1, sMsg2, 1);
      }
      m_DlgStabilizing.ShowWindow(SW_NORMAL);
      m_BtnStartStop.SetWindowText(_T("Stop Laser"));
   }
   else
   {
      // m_DlgStabilizing.StopGui();
      CDlgMessageBox dlg;
      dlg.SetOptions(0, _T("This will shut down the LASER!"), _T("Do you really want to stop?"));
      INT_PTR nResult = dlg.DoModal();
      if (nResult == IDOK)
      {
         m_bStoppingBluetooth = true;
         m_MyComms.Mode = CBlueTooth::Stop_Sampling;
         wPrintf(_T("Stop pressed"));
         m_BtnStartStop.SetWindowText(_T("Start Laser"));
         while (m_bStoppingBluetooth && m_MyComms.Mode != CBlueTooth::Idle)
         {
            this->ProcessMessages();
            Sleep(0);
         }
         m_BtnLogData.EnableWindow(FALSE);
         m_BtnCalibrate.EnableWindow(FALSE);
         m_nTec_Temperature = 0;
         //Sleep(100);
         //OnOK();
      }
   }

}

LRESULT CDetectorguiDlg::OnBumpTest(WPARAM, LPARAM)
{
   
   bool bBumpTestSucess = true; 
   HideActivePage();
   ChangeStatusMsg(_T("STARTUP"));
   //m_BtnStartStop.SetWindowText(_T("Start"));
   CDlgPopup dlg(this);
   CString sMsg1 = _T("Bump Test");
   CString sMsg2 = _T("PASS");
   COLORREF color = DARK_GREEN_COLOR;
   int nMode = CDlgPopup::CAL_OK;
   
   if (!IsLaserOK()) {
      CString sMsg1 = _T("Laser Start FAILURE");
      CString sMsg2 = _T("Power Cycle Laser");
      COLORREF color = RED_COLOR;
      bBumpTestSucess = false;
      dlg.SetOptions(nMode, sMsg1, sMsg2);
      dlg.SetLine2Color(color);
      dlg.DoModal();
      HideActivePage();

      ChangeStatusMsg(_T("READY"));
      m_plotGraph.ShowWindow(SW_NORMAL);
      m_ActivePage = READY_PAGE;
      EnableWindow(FALSE);
      OnClose();
      return 0;

   }
   m_dBumpTest = 0.0;
   m_vCalibrationReadings.clear();
   m_bCalibrationInProgress = true;
   EnableWindow(FALSE);
   for (int i = 0; i < 20; i++)
   {
      ProcessMessages();
      Sleep(50);
   }
   EnableWindow(TRUE);
   m_bCalibrationInProgress = false;
   m_ccSyncThreads.Lock(); m_ccSyncThreads.Unlock();
   double dBumpTest = 0.0;
   double adData[8] = { 0 };
   int iter = 0;

   for (UINT i = 0; i < m_vCalibrationReadings.size(); i++)
   {
      v_min1floc.push_back(m_vCalibrationReadings[i].aData[4]);
      v_max1floc.push_back(m_vCalibrationReadings[i].aData[5]);
      v_min2floc.push_back(m_vCalibrationReadings[i].aData[6]);
   }
   if (m_vCalibrationReadings.size() > 0)
   {
      sort(v_min1floc.begin(), v_min1floc.end());
      sort(v_max1floc.begin(), v_max1floc.end());
      sort(v_min2floc.begin(), v_min2floc.end());

      g_MedFilter.aMedmin1floc = v_min1floc[m_vCalibrationReadings.size() / 2];
      g_MedFilter.aMedmax1floc = v_max1floc[m_vCalibrationReadings.size() / 2];
      g_MedFilter.aMedmin2floc = v_min2floc[m_vCalibrationReadings.size() / 2];
   }


   for (UINT i = 0; i < m_vCalibrationReadings.size(); i++)
   {

      if (abs(m_vCalibrationReadings[i].aData[4] - g_MedFilter.aMedmin1floc) < g_MedFilter.THRESHOLD &&
         abs(m_vCalibrationReadings[i].aData[5] - g_MedFilter.aMedmax1floc) < g_MedFilter.THRESHOLD &&
         abs(m_vCalibrationReadings[i].aData[6] - g_MedFilter.aMedmin2floc) < g_MedFilter.THRESHOLD) // its a valid point
      {
         iter++;
         dBumpTest += m_vCalibrationReadings[i].fSample;
         for (UINT j = 0; j < ARRAYSIZE(adData); j++)
         {
            adData[j] += m_vCalibrationReadings[i].aData[j];
         }
      }
   }

   //if (m_vCalibrationReadings.size() > 0)
   if (iter > 0)

   {


      dBumpTest /= iter;
      for (UINT j = 0; j < ARRAYSIZE(adData); j++)
      {
         adData[j] /= iter;
      }

      m_dBumpTest = dBumpTest;
      //g_fFitO = fBgppm - dBumpTest;
      //g_fFitO = fBgppm - dBumpTest;
      //fBgppm = m_dBumpTest;
      fBgppm = 1.9;
   }

   if (m_dBumpTest < 1.0000)
   {
      sMsg2 = _T("Please clean lenses");
      bBumpTestSucess = false;
      fBgppm = 1.9;
   }
   else if (m_dBumpTest > 6.0)
   {
      sMsg2 = _T("Please ensure clean air");
      bBumpTestSucess = false;
      fBgppm = 1.9;
   }
   if (!bBumpTestSucess && m_dBumpTest > 6.0)
   {

      if (AfxMessageBox(_T("Measured higher than normal Background! \n Proceed?"), MB_YESNO) == IDNO)
      {
         sMsg2 = _T("FAIL/Find Clean Air");
         color = RED_COLOR;
         fBgppm = 1.9;
         nMode = CDlgPopup::CALIBRATE;
      }
      else
      {
         sMsg2 = _T("HIGH GAS / Override");
         color = BLUE_COLOR;
         bBumpTestSucess = true;
         fBgppm = 1.9;
         m_BtnLogData.EnableWindow(TRUE);

      }

   }
   else
   {
      m_ActivePage = SAVE_FILE_PAGE;
      m_BtnLogData.EnableWindow(TRUE);
   }
   CString sFileName;
   sFileName = m_Settings.m_sCalDataPath + _T("\\BT") + m_SetupDlg.ConstructFileName() + _T(".csv");
   m_SaveFileMsgDlg.UpdateValues(_T("Bump Test Saved as"), sFileName);
   FILE* pFile = _tfopen(sFileName, _T("w"));
   if (pFile)
   {
      _ftprintf(pFile, _T("%0.2lf"), m_dBumpTest);
      for (UINT j = 0; j < ARRAYSIZE(adData); j++)
      {
         _ftprintf(pFile, _T(",%6.2lf"), adData[j]);
      }
      _ftprintf(pFile, _T("\n"));
      fclose(pFile); pFile = 0;
   }
   else
   {
      m_SaveFileMsgDlg.UpdateValues(_T("Failed to open file:"), sFileName);
   }
   m_SaveFileMsgDlg.ShowWindow(SW_NORMAL);



   dlg.SetOptions(nMode, sMsg1, sMsg2);
   dlg.SetLine2Color(color);
   dlg.DoModal();
   m_bStartingLaser = false;
   CString sButton;
   m_BtnStartStop.GetWindowText(sButton);
   if (sButton != _T("Start Laser") && m_bShowCalibrationMsg)
   {
      CDlgPopup dlg(this);
      CTime time = CTime::GetCurrentTime();
      CString sTime;
      sTime.Format(_T("%d/%d/%d"), m_Settings.m_TimeNextCal.GetMonth(), m_Settings.m_TimeNextCal.GetDay(), m_Settings.m_TimeNextCal.GetYear());
      bool bCalibrate = false;
      if (time < m_Settings.m_TimeNextCal)
      {
         dlg.SetOptions(CDlgPopup::CAL_OK, _T("Calibration Due on:"), sTime);
      }
      else
      {
         dlg.SetOptions(CDlgPopup::CALIBRATE, _T("Calibration is Due"), sTime);
         bCalibrate = true;
      }
      dlg.DoModal();
      if (bCalibrate)
      {
         OnBnClickedButtonCalibrate();
      }
      m_bShowCalibrationMsg = false;
   }
   m_SaveFileMsgDlg.CloseWindow();
   if (color == RED_COLOR)
   {
      PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_CALIBRATE, BN_CLICKED), (LPARAM)m_BtnCalibrate.m_hWnd);
   }
   else
   {
      HideActivePage();
      ChangeStatusMsg(_T("READY"));
      m_plotGraph.ShowWindow(SW_NORMAL);
      m_ActivePage = READY_PAGE;
   }

   return 0;
}

void CDetectorguiDlg::ProcessMessages(void)
{
   MSG msg;
   while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
   {
      if (!AfxGetApp()->PumpMessage())
      {
         ::PostQuitMessage(0);
         break;
      }
   }
}

void CDetectorguiDlg::WaitForThreads(void)
{
   while (!TryEnterCriticalSection(CBlueTooth::m_st_CS))
   {
      ProcessMessages();
   }
   m_st_nTerminaThread = 1;
   m_MyComms.Mode = CBlueTooth::Exit_Software;
   LeaveCriticalSection(CBlueTooth::m_st_CS);
   int nCount = 1;
   HANDLE aHandles[2] = { 0 };
   aHandles[0] = m_pApp_Thread->m_hThread;

   if (m_pWinThread)
   {
      nCount++;
      aHandles[1] = m_pWinThread->m_hThread;
   }
   DWORD dwMilliseconds = 30000;//INFINITE is recommended
   WaitForMultipleObjects(nCount, aHandles, TRUE, dwMilliseconds);
   m_plotGraph.DeleteSeries(m_st_nSeriesIdForthread);
}

void CDetectorguiDlg::ClearPlot(void)
{
   if (m_CurrentGraphInfo.nSeriesId)
   {
      m_plotGraph.DeleteSeries(m_CurrentGraphInfo.nSeriesId);
      m_CurrentGraphInfo.nSeriesId = 0;
   }
}

void CDetectorguiDlg::OnDestroy()
{
   CButtonST* buttons[] = { &m_BtnSetup,  &m_BtnStartStop, &m_BtnCalibrate, &m_BtnAlarmSet, &m_BtnExportKML };

   CString sButton;
   m_BtnLogData.GetWindowText(sButton);

   if (sButton == _T("Stop Survey")) 
   {
      KillTimer(WAIT_FOR_GPS);
      ClearPlot();
      //WaitForThreads();
      m_plotGraph.DummyUpdate(0);
      ChangeStatusMsg(_T("STANDBY"));
      m_ccSyncThreads.Lock();
      if (m_pSurveyFileG)
      {
         fclose(m_pSurveyFileG);
         m_pSurveyFileG = 0;
      }
      if (m_pSurveyFileR)
      {
         fclose(m_pSurveyFileR);
         m_pSurveyFileR = 0;
      }
      m_ccSyncThreads.Unlock();
      CDlgSurveyKmlSave dlg;
      CString sSurveyPath = m_Settings.m_sSurveyDataPath + _T("\\Sur") + m_SetupDlg.GetLastFileName();
      CString sKmlPath = sSurveyPath;

      sKmlPath += _T(".kml");
      if (CreateKMLFile(sSurveyPath, sKmlPath))
      {
         dlg.SetOptions(0, _T("Survey Saved as"), _T("KML Saved as"), sSurveyPath, sKmlPath);
         dlg.DoModal();
      }
      m_BtnLogData.SetWindowText(_T("Start Survey"));
      m_plotGraph.GetDlgItem(IDC_BUTTON_PAUSE)->ShowWindow(SW_HIDE);
      for (int i = 0; i < ARRAYSIZE(buttons); i++)
      {
         buttons[i]->EnableWindow(TRUE);
      }
      m_bStandby = false;
   }
   
   
   
   AfxMessageBox(_T("Please remember to turn LASER off."));

   WaitForThreads();

   //CProfiler::PrintProfileInfo();

   CDialogEx::OnDestroy();

}


void CDetectorguiDlg::OnBnClickedButtonAlarmSet()
{
   if (m_ActivePage != ALARM_PAGE)
   {
      HideActivePage();
      m_AlarmDlg.ShowWindow(SW_SHOW);
      m_ActivePage = ALARM_PAGE;
      ChangeStatusMsg(_T("SET ALARM"));
   }
}


void CDetectorguiDlg::OnBnClickedButtonExportKlm()
{
   if (m_ActivePage != EXPORT_KML_PAGE)
   {
      HideActivePage();
      m_ExportKmlDlg.ShowWindow(SW_SHOW);
      m_ActivePage = EXPORT_KML_PAGE;
      ChangeStatusMsg(_T("DISPLAY KML FILE"));
   }
}

CCriticalSection CDetectorguiDlg::m_st_LogCS;
int CDetectorguiDlg::wPrintf(TCHAR* szFormat, ...)
{
   CSingleLock lock(&m_st_LogCS, TRUE);
   if (m_st_nTerminaThread) return 0;
   //TCHAR debug_str[1000];
   TCHAR szBuffer[1024];
   //	int x;
   va_list pArgList;

   va_start(pArgList, szFormat);
   _vsntprintf(szBuffer, sizeof(szBuffer), szFormat, pArgList);
   va_end(pArgList);

   //for (x = 0; x < 256; ++x)
   //	debug_str[x] = szBuffer[x];
   TRACE(_T("wPrintf %s\n"), szBuffer);
   DW.InsertString(0, szBuffer);
   TRACE(_T("wPrintf exit"));
   return(1);
}

UINT CDetectorguiDlg::App_Thread(LPVOID pParam)
{
   int x, sent;
   unsigned char wb[10];
   unsigned char temp[50];
   unsigned char *ptr;
   //CFont Font1;
   //CFont Font2;
   //CFont Font3;
   //CFont Font4;
   //CFont Font5;
//   unsigned char buff1[40];
//   unsigned short test[3];
   COLORREF cr = RGB(255, 0, 0);
   CBlueTooth& MyComms = CBlueTooth::GetInstance();
   CDetectorguiDlg& mainGui = *static_cast<CDetectorguiDlg*>(pParam);
   CSettings& settings = *CSettings::GetInstance();
   CSerialGPS& MySerial = CSerialGPS::GetInstance();
   HANDLE hCom = 0;
   CString sPort;
   BOOL bReadRC, bWriteRC;
   DCB dcb;
   COMMTIMEOUTS CommTimeouts;
   DWORD dwWritten;
ScanForBTandGPSLoop:
   int result = MyComms.ScanForBTandGPS();
   if (result != 0)
   {
      AfxMessageBox(_T("Please connect Telemetry Module \nClick OK when done"));
      goto ScanForBTandGPSLoop;
   }
WaitAgain:
   settings.m_ConnectionStatus.bBluetooth = false;

   for (; !m_st_nTerminaThread;)
   {
      mainGui.wPrintf(_T("Searching Bluetooth"));
      //x = MyComms.Look_For_Bluetooth_Devices();
      x = MyComms.scan_bluetooth_devices();
      if (x == 1)
         mainGui.wPrintf(_T("Found Bluetooth"));
      break;
   }
   if (MyComms.m_dlgSelectBT.m_sBTName.IsEmpty())
   {
      goto WaitAgain;
   }
   char temp1[100];
   _tcscpy((WCHAR*)&temp1, MyComms.m_dlgSelectBT.m_sBTName.GetString());
   temp[0] = temp1[32];
   temp[1] = temp1[34];
   temp[2] = 0;
   sscanf((const char *)&temp[0], "%02x", (const char *)&MyComms.bt_address[0]);

   temp[0] = temp1[36];
   temp[1] = temp1[38];
   temp[2] = 0;
   sscanf((const char *)&temp[0], "%02x", (const char *)&MyComms.bt_address[1]);

   temp[0] = temp1[40];
   temp[1] = temp1[42];
   temp[2] = 0;
   sscanf((const char *)&temp[0], "%02x", (const char *)&MyComms.bt_address[2]);

   temp[0] = temp1[44];
   temp[1] = temp1[46];
   temp[2] = 0;
   sscanf((const char *)&temp[0], "%02x", (const char *)&MyComms.bt_address[3]);

   temp[0] = temp1[48];
   temp[1] = temp1[50];
   temp[2] = 0;
   sscanf((const char *)&temp[0], "%02x", (const char *)&MyComms.bt_address[4]);

   temp[0] = temp1[52];
   temp[1] = temp1[54];
   temp[2] = 0;
   sscanf((const char *)&temp[0], "%02x", (const char *)&MyComms.bt_address[5]);

   for (;;)
   {
      MyComms.Reopen_Bluetooth_port();
      x = MyComms.Connect_To_Bluetooth_Device();
      if (x == 1)
         break;
   }

   if (m_st_nTerminaThread) return 0;

   if (MyComms.Running_Mode == 'I')
   {
      Sleep(200);
      MyComms.read_flashf_block();
      Sleep(200);
      MyComms.read_flashc_block();

      mainGui.wPrintf(_T("Send App Ready cmd"));
      wb[0] = 'R';
      wb[1] = 'e';
      wb[2] = 'a';
      wb[3] = 'd';
      wb[4] = 'y';
      sent = MyComms.WriteBluetooth(&wb[0], 5, 0);
   }
   //
   //	Lets see if we get a KeepAlive message. The Bluetooth chip may be on and 
   //	awake but the micro not running
   //
   ptr = &temp[0];
   x = MyComms.ReadBluetooth((unsigned char *)temp);


   //{
   //   mainGui.wPrintf(_T("sending stop 1"));
   //   MyComms.flush();
   //   wb[0] = 'S';
   //   wb[1] = 't';
   //   wb[2] = 'o';
   //   wb[3] = 'p';
   //   wb[4] = ' ';

   //   sent = MyComms.WriteBluetooth(&wb[0], 5, 0);
   //   MyComms.flush();
   //}


   if (x == -1)
   {
      Sleep(1000);
      for (int nRetries = 0; nRetries < 3 && x == -1; nRetries++)
      {
         x = MyComms.ReadBluetooth((unsigned char *)temp);
      }
   }

   if (x == -1)
   {
      //
      //		If -1 then we timed out so close the socket, clean up and
      //		go back to waiting for connection
      //
      MyComms.Close_Bluetooth_port();
      goto WaitAgain;
   }


   //test[0] = 254;
   //test[1] = 0;
   //test[2] = 0;
   //btconn->SetSel(0, -1);
   //btconn->Clear();
   //btconn->ReplaceSel((LPCTSTR)&test[0], FALSE);


   PlaySound(TEXT("c:\\BascomTurnerMLDS\\tada.wav"), NULL, SND_ASYNC | SND_LOOP);
   Sleep(2000);
   PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP);
   //
   //
   //	This is our main command loop. We wait here for the user to 
   //	press a button to tell us what to do. That button press will
   //	be reflected in the Mode variable.
   //
   //
   //	Stop_Sampling		Mode = 0
   //	Start_Sampling		Mode = 1
   //	Idle			Mode = 2
   //	Calibrate		Mode = 3
   //	Dump_Data		Mode = 4;
   //	Download Code		Mode = 5;
   //	Read Flash Block	Mode = 6;
   //	Write Flash Block	Mode = 7;
   //
   MyComms.Mode = CBlueTooth::Idle;		// Start off with mode 2. Idle, do nothing.

   sPort = "\\\\.\\Com12";
   sPort.Format(_T("\\\\.\\Com%d"), MyComms.m_gps);
   //sPort = "Com3";
   if (hCom)
   {
      CloseHandle(hCom);
   }

   hCom = MyComms.gps_port = MySerial.OpenSerialGPS(sPort);
   if (hCom != INVALID_HANDLE_VALUE) {

      bReadRC = MySerial.SetupSerialGPS(hCom, dcb);
      bReadRC = MySerial.SetupTimeouts(hCom, CommTimeouts);
      bWriteRC = WriteFile(hCom, &GPGSA_OFF[0], 16, &dwWritten, NULL);
      Sleep(200);
      bWriteRC = WriteFile(hCom, &GPGLL_OFF[0], 16, &dwWritten, NULL);
      Sleep(200);
      bWriteRC = WriteFile(hCom, &GPGSV_OFF[0], 16, &dwWritten, NULL);
      Sleep(200);
      bWriteRC = WriteFile(hCom, &GPRMC_OFF[0], 16, &dwWritten, NULL);
      Sleep(200);
      bWriteRC = WriteFile(hCom, &GPVTG_OFF[0], 16, &dwWritten, NULL);
      Sleep(200);
      //MyComms.InitGPS();
      mainGui.wPrintf(_T("GPSSetup Ok!"));
   }
   MyComms.init_flashc_block();
   Sleep(200);
   MyComms.init_flashf_block();


   /*


   for (int i = 0; i < 1536; i++)
   {

      MyComms.Test_Flash[i]= MyComms.Flash_Block[i];
   }
   */
   settings.m_ConnectionStatus.bBluetooth = true;
   for (; !m_st_nTerminaThread;)
   {
      //
      //		Idle
      //
      if (MyComms.Mode == CBlueTooth::Idle)
      {
         //
         //	Do a read for the keep alive msg to make sure micro is still up
         //
         ptr = &temp[0];
         //        x = MyComms.ReadBluetooth((unsigned char *)temp);
         while (MyComms.m_CalRecords.m_WriteflashInProgress)
         {
            Sleep(100);
         }
         x = MyComms.Sample_Data_And_Display(hCom);
         if (x == -1)
         {
            //
            //			If -1 then we timed out so close the socket, clean up and
            //			go back to waiting for connection
            //
            MyComms.Close_Bluetooth_port();
            //
            //		Uncheck connected box
            //
            settings.m_ConnectionStatus.bBluetooth = false;
            //btconn->SetSel(0, -1);
            //btconn->Clear();

            PlaySound(TEXT("c:\\BascomTurnerMLDS\\tada.wav"), NULL, SND_ASYNC | SND_LOOP);
            Sleep(2000);
            PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP);

            goto WaitAgain;
         }
         Sleep(100);
         continue;
      }
      //
      //		Stop
      //
      if (MyComms.Mode == CBlueTooth::Stop_Sampling)
      {
         mainGui.wPrintf(_T("Stopping"));
         MyComms.Mode = CBlueTooth::Idle;	// Idle
         MyComms.flush();
         wb[0] = 'S';
         wb[1] = 't';
         wb[2] = 'o';
         wb[3] = 'p';
         wb[4] = ' ';

         sent = MyComms.WriteBluetooth(&wb[0], 5, 0);
         MyComms.flush();

         continue;
      }
      //
      //		Start
      //
      if (MyComms.Mode == CBlueTooth::Start_Sampling)
      {
         mainGui.wPrintf(_T("Set Laser Temp"));
         MyComms.Send_CONFIG_Parameters();

         MyComms.flush();

         wb[0] = 'W';
         wb[1] = 'a';
         wb[2] = 'r';
         wb[3] = 'm';
         wb[4] = ' ';
         sent = MyComms.WriteBluetooth(&wb[0], 5, 0);

         Sleep(1000);

         MyComms.flush();

         mainGui.wPrintf(_T("Start Laser"));
         wb[0] = 'S';
         wb[1] = 't';
         wb[2] = 'a';
         wb[3] = 'r';
         wb[4] = 't';
         sent = MyComms.WriteBluetooth(&wb[0], 5, 0);

         x = MyComms.Sample_Data_And_Display(hCom);
         if (x == -1)
         {
            MyComms.Mode = CBlueTooth::Idle;	//idle
                        //
                        //			If -1 then we timed out so close the socket, clean up and
                        //			go back to waiting for connection
                        //
            MyComms.Close_Bluetooth_port();
            //
            //		Uncheck connected box
            //
            settings.m_ConnectionStatus.bBluetooth = false;
            //btconn->SetSel(0, -1);
            //btconn->Clear();

            PlaySound(TEXT("c:\\BascomTurnerMLDS\\program\\ding.wav"), NULL, SND_ASYNC | SND_LOOP);
            Sleep(2000);
            PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP);

            goto WaitAgain;
         }
      }
      //
      //		Calibrate
      //
      if (MyComms.Mode == CBlueTooth::Calibrate)
      {
         mainGui.wPrintf(_T("Calibrate"));
         wb[0] = 'Z';
         wb[1] = 'e';
         wb[2] = 'r';
         wb[3] = 'o';
         wb[4] = ' ';
         sent = MyComms.WriteBluetooth(&wb[0], 5, 0);
         MyComms.flush();
         MyComms.Mode = CBlueTooth::Idle;	// back to start
      }
      //
      //		Dump Data
      //
      if (MyComms.Mode == CBlueTooth::Dump_Data)
      {
         mainGui.wPrintf(_T("Dump Raw Data"));
         wb[0] = 'D';
         wb[1] = 'u';
         wb[2] = 'm';
         wb[3] = 'p';
         wb[4] = ' ';
         sent = MyComms.WriteBluetooth(&wb[0], 5, 0);
         MyComms.flush();
         MyComms.Mode = MyComms.saved_mode;
      }
      //
      //		Download new program
      //
      if (MyComms.Mode == CBlueTooth::Download_Code)
         MyComms.download_new_program();

      //
      //		Read Flash Block
      //
      if (MyComms.Mode == CBlueTooth::Read_Flashc_Block)
      {
         MyComms.read_flashc_block();
         MyComms.Mode = CBlueTooth::Idle;		// back to idle
      }

      if (MyComms.Mode == CBlueTooth::Read_Flashf_Block)
      {
         MyComms.read_flashf_block();
         MyComms.Mode = CBlueTooth::Idle;		// back to idle
      }
      //
      //		Write Flash Block
      //
      if (MyComms.Mode == CBlueTooth::Write_Flashc_Block)
      {

         MyComms.write_flashc_block();
         MyComms.Mode = CBlueTooth::Idle;		// back to idle
      }
   }
   return 0;
}


LRESULT CDetectorguiDlg::OnResetGui(WPARAM, LPARAM)
{
   if (m_bStandby)
   {
      KillTimer(WAIT_FOR_GPS);
      ClearPlot();
      m_plotGraph.DummyUpdate(0);
      m_BtnLogData.SetWindowText(_T("Log Data"));
      m_bStandby = false;
   }
   ChangeStatusMsg(_T("WELCOME"));
   m_BtnStartStop.SetWindowText(_T("Start Laser"));
   m_BtnStartStop.EnableWindow(FALSE);
   m_BtnLogData.EnableWindow(FALSE);
   m_BtnCalibrate.EnableWindow(FALSE);
   m_BtnSetup.EnableWindow(TRUE);
   m_BtnAlarmSet.EnableWindow(TRUE);
   m_BtnExportKML.EnableWindow(TRUE);
   return FALSE;
}

int CDetectorguiDlg::ResetGui()
{
   if (m_st_nTerminaThread) return FALSE;
   m_Settings.m_ConnectionStatus.bBluetooth = false;
   while (m_bSurveyInit)
   {
      Sleep(500);
   }
   //lets make it thread safe so that only gui thread handles it
   SendMessage(UWM_RESET_UI, 0, 0);
   return 0;
}



void CDetectorguiDlg::OnClose()
{
   // TODO: Add your message handler code here and/or call default
   m_MyComms.Close_Bluetooth_port();
   //m_MyComms.Mode = CBlueTooth::Stop_Sampling;
   m_MyComms.Mode = CBlueTooth::Exit_Software;

   CDialogEx::OnOK();
}


void CDetectorguiDlg::OnCancel()
{
   // TODO: Add your specialized code here and/or call the base class
   //MyComms.Mode = CBlueTooth::Stop_Sampling;
   //CDialogEx::OnCancel();
}


BOOL CDetectorguiDlg::PreTranslateMessage(MSG* pMsg)
{
   if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
   {
      HACCEL hAccel = m_hAccel;
      if (hAccel && ::TranslateAccelerator(m_hWnd, hAccel, pMsg))
      {
         return TRUE;
      }
   }
   return CDialogEx::PreTranslateMessage(pMsg);
}


void CDetectorguiDlg::OnAcceleratorHidden()
{
   if (!m_ShowDataDlg.GetSafeHwnd())
   {
      m_ShowDataDlg.Create(IDD_DIALOG_BLUETOOTH_DATA, this);
   }
   m_ShowDataDlg.ShowWindow(SW_NORMAL);
}


void CDetectorguiDlg::OnAcceleratorFw()
{
   m_SetupDlg.GetDlgItem(IDC_BUTTON_UPDATE_FW)->ShowWindow(SW_NORMAL);
}



void CDetectorguiDlg::OnBnClickedCheckLaser()
{
   // TODO: Add your control notification handler code here
}


void CDetectorguiDlg::OnBnClickedCheckBattery()
{
    // TODO: Add your control notification handler code here
}
