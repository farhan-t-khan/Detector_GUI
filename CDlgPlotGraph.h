#pragma once
#include "ChartCtrl/ChartCtrl.h"
#include "ColorStaticST.h"
#include "Settings.h"
// CDlgPlotGraph dialog

class CDlgPlotGraph : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPlotGraph)
public:
	CDlgPlotGraph(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgPlotGraph();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PLOT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   CChartCtrl m_ChartCtrl;
   CSettings& m_Settings;
   time_t m_LastppmUpdate = clock();
   bool m_bDisablePlotGuiUpdate = false;
   double m_dPeakLat = 0.0;
   double m_dPeakLon = 0.0;
   //int m_nPPMCounter = 0;
	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   void AddAlarmLine(void);
   unsigned int m_nAlarmSeriesId;
   void DummyUpdate(int nLogging);
   time_t GetLastppmUpdate()
   {
      return m_LastppmUpdate;
   }
   CColorStaticST m_Numppm;
   CFont m_Fontppm;
   CColorStaticST m_Staticppm;
   CColorStaticST m_StaticLat;
   CColorStaticST m_StaticLong;
   float m_fLat = 0.0f;
   float m_fLong = 0.0f;
   CFont m_FontLatLong;
   void SetXRange(int nVal);
   void SetYRange(int nVal);
   unsigned int AddSeries(int nPenThickness, int nPenType, double* pdX, double* pdY, int nPoints, COLORREF color = BLACK_COLOR, bool bSmooth = false);
   void AddPointToSeries(unsigned int& nSeriesId, double dX, double dY);
   void AddPointsToSeries(unsigned int& nSeriesId, double* pdX, double* pdY, int nPoints);
   unsigned int UpdateSeries(unsigned int&, int nPenThickness, int nPenType, double* pdX, double* pdY, int nPoints, COLORREF color = BLACK_COLOR, bool bSmooth = false);
   void DeleteSeries(unsigned int nSerieId)
   {
      m_ChartCtrl.RemoveSerie(nSerieId);
   }
   CColorStaticST m_StaticLastPeak;
   int m_nLastPeak;
   int m_nLastPeakMax = 0;
   int m_nLastPeakCount = 0;
   int maxnVal = 0;
   const double R_earth = 6371000.0;
   double dDeltaLat = 0.0;
   double dDeltaLon = 0.0;
   double dMeanLat = 0.0;
   double dDistance = 0.0;
   CColorStaticST m_StaticDistance;
   void Updateppm(int nVal, double dLon, double dLat);
   void UpdateLat(LPCTSTR  szLat);
   void UpdateLon(LPCTSTR  szLon);
   afx_msg void OnStnClickedStaticLongText();
   CColorStaticST m_StaticLatText;
   CColorStaticST m_StaticLonText;
   afx_msg void OnStnClickedStaticLon();
   afx_msg void OnBnClickedButtonPause();
   afx_msg void OnBnClickedButtonClear();
   CProgressCtrl m_ProgressBarStatus;
   COLORREF m_ProgressBarNormalColor = RGB(0, 255, 50);
};
