
// Detector_guiDlg.h : header file
//

#pragma once
#include "BtnST.h"
#include "ColorStaticST.h"
#include "TransparentCheckBox.h"
#include "ColorCheck.h"
#include "CDlgPlotGraph.h"
#include "CDlgSetup.h"
#include "Settings.h"
#include "CDlgStabilizing.h"
#include "CDlgSaveFileMsg.h"
#include "CDlgAlarm.h"
#include "CDlgExportKml.h"
#include "CBlueTooth.h"
#include "NMEAParser.h"
#include "CDlgBluetoothData.h"
#include "CPleaseWaitDlg.h"

#define FLASH_FINISH_TIMER 100
#define REFRESH_CONNECTIONS 101
#define WAIT_FOR_GPS 102
#define PROGRESS_BAR_STATUS 103

struct filter
{
   enum {
      UNDEF = -1,
      THRESHOLD = 10,
      THRESHOLD_M = 10
   };
   short aMedmin1floc = UNDEF;
   short aMedmax1floc = UNDEF;
   short aMedmin2floc = UNDEF;
};

extern filter g_MedFilter;
extern float fBgppm;
extern float g_fFitO;
extern float g_fFitG;
extern const int UWM_GPS_BTN_UPDATE;


struct structUpdateGraph
{
   ~structUpdateGraph()
   {
      clear();
   }
   void clear()
   {
      if (pdX) delete[] pdX; pdX = 0;
      if (pdY) delete[] pdY; pdY = 0;
   }
   void resize(UINT nNewSize);
   int nMaxPoints = 0;
   int nCurrentPoints = 0;
   int nPenThickness = 0;
   int nPenType = 0;
   bool bSmooth = false;
   double* pdX = 0;
   double* pdY = 0;
   unsigned int nSeriesId = 0;
   clock_t lastUpdate = 0;
};
// CDetectorguiDlg dialog
struct dataReadings
{
   float fSample;
   short aData[8];
};
class CDetectorguiDlg : public CDialogEx
{
   structUpdateGraph m_CurrentGraphInfo;
   bool m_bStartingLaser = false;
   bool m_bSurveyInit = false;
   bool m_bShowCalibrationMsg = true;
   FILE* m_pSurveyFileG = 0;
   FILE* m_pSurveyFileR = 0;
   CCriticalSection m_ccSyncThreads;
   double m_dBumpTest;
   std::vector<dataReadings> m_vCalibrationReadings;

   bool m_bDisableAlarm = false;
   bool CalibrationCheck(CString& sFile);
   void WriteHeader(std::ofstream& outKml);
   bool CreateKMLFile(CString& sFileSurvey, CString& sFileKML);
   bool m_bPauseBlueToothThread = false;
   time_t m_LastGraphUpdate = clock();
   bool m_bStoppingBluetooth = false;
   HACCEL m_hAccel = 0;
   CDlgBluetoothData m_ShowDataDlg;
   static CCriticalSection m_st_LogCS;
// Construction
public:
	CDetectorguiDlg(CWnd* pParent = nullptr);	// standard constructor
   ~CDetectorguiDlg();
   void SetPauseBlueToothThread(bool bPause) { m_bPauseBlueToothThread = bPause; };
   bool PauseBlueToothThread(void) { return m_bPauseBlueToothThread; };
   void BluetoothStop(void) { m_bStoppingBluetooth = false; };
   static void ProcessMessages(void);
   bool IsAlarmDisabled() { return m_bDisableAlarm; }
   void UpdateGraphForThread(void);
   void Plot_ppm(float fnppm, short navg_min_2F, short navg_max_2F,
      short navg_min_1F, short navg_max_1F, short navg_minlocation1,
      short navg_maxlocation1, short navg_minlocation2, short navg_maxlocation2);
   double GetBumpTest() { return m_dBumpTest; }
   static unsigned m_st_nSeriesIdForthread;
   static unsigned m_st_nTerminaThread;
   static UINT UpdateGraphThread(LPVOID pParam);
   CWinThread* m_pApp_Thread = 0;
   bool m_bCalibrationInProgress = false;
   static UINT App_Thread(LPVOID pParam);
   void WaitForThreads(void);
   void ClearPlot(void);
   CWinThread* m_pWinThread;
   void WriteToSurveyFile(CString sData);
   void WriteToSurveyFileR(CString sData);
   CPleaseWaitDlg* m_pCurrentProgressDlg = 0;
   int msgboxID = IDOK;
   bool bEnableBlkMsg = true;
 //  int pointcounter = 1;
   
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DETECTOR_GUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
   void ChangeStatusMsg(CString sMessage);
   //??? was not static m_Settings CSettings is a Singleton
   //static 
	CSettings& m_Settings;
   bool m_bInitComplete;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
   HBITMAP PaintNewImage(CDC* srcCdc, BITMAP& bm, CRect& rect);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   CButtonST m_BtnSetup;
   CButtonST m_BtnStartStop;
   CButtonST m_BtnLogData;
   CButtonST m_BtnCalibrate;
   CButtonST m_BtnAlarmSet;
   CButtonST m_BtnExportKML;
   HBITMAP m_hBitmapLogo;
   ULONG_PTR m_gdiplusToken;
   CStatic m_LogoBitmap;
   CColorStaticST m_StaticStatus;
   CColorStaticST m_StaticStatusMsg;
   CColorCheck m_LaserCheck;
   CColorCheck m_BluetoothCheck;
   CColorCheck m_BtnGps;
   CColorCheck m_BtnBattery;
   CDlgPlotGraph m_plotGraph;
   CDlgSetup m_SetupDlg;
   CDlgAlarm m_AlarmDlg;
   CDlgExportKml m_ExportKmlDlg;
   CDlgSaveFileMsg m_SaveFileMsgDlg;
   bool m_bStandby;
   CBlueTooth& m_MyComms = CBlueTooth::GetInstance();
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   void Initialize(void);
   int wPrintf(TCHAR* szFormat, ...);
   enum PageSelection {
      NO_PAGE = -1, READY_PAGE = 0, SETUP_PAGE, CAL_PAGE, SAVE_FILE_PAGE, ALARM_PAGE, EXPORT_KML_PAGE
   };
   PageSelection m_ActivePage;
   void HidePage(PageSelection currPage);
   void HideActivePage(void);
   afx_msg void OnBnClickedButtonSetup();
   afx_msg void OnBnClickedButtonLogData();
   CFont m_StatusFont;
   afx_msg void OnBnClickedButtonCalibrate();

   int m_nTec_Temperature = 0;
   int m_nBattery_Voltage = 0;
   bool IsGpsOK(void);
   bool IsBatteryOK(void);
   bool IsBatteryGreen(void);
   bool IsBatteryYellow(void);
   int GetBatteryPercentage(void);
   bool IsLaserOK(void);
   bool IsLaserBlocked(void);
   void Updateconnections();
   afx_msg void OnBnClickedButtonStartStop();
   CDlgStabilizing m_DlgStabilizing;
   afx_msg LRESULT OnBumpTest(WPARAM, LPARAM);
   afx_msg LRESULT OnUpdateGraph(WPARAM, LPARAM);

   afx_msg LRESULT OnResetGui(WPARAM, LPARAM);
   afx_msg LRESULT OnGPSBtnUpdate(WPARAM, LPARAM);
   
   afx_msg void OnDestroy();
   afx_msg void OnBnClickedButtonAlarmSet();
   afx_msg void OnBnClickedButtonExportKlm();
   CListBox DW;
   CFont font;
   


   int ResetGui();
   afx_msg void OnClose();
   virtual void OnCancel();
   virtual BOOL PreTranslateMessage(MSG* pMsg);
   afx_msg void OnAcceleratorHidden();
   afx_msg void OnAcceleratorFw();
   afx_msg void OnAcceleratorReset();
   afx_msg void OnBnClickedCheckLaser();
   afx_msg void OnBnClickedCheckBattery();
   afx_msg void OnStnClickedStaticStatus();
};
