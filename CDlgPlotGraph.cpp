// CDlgPlotGraph.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "Detector_guiDlg.h"
#include "CDlgPlotGraph.h"
#include "afxdialogex.h"
#include "Profiler.h"
#define _USE_MATH_DEFINES
#include <cmath>


// CDlgPlotGraph dialog

IMPLEMENT_DYNAMIC(CDlgPlotGraph, CDialogEx)

CDlgPlotGraph::CDlgPlotGraph(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PLOT, pParent)
   , m_Settings(*CSettings::GetInstance())
   , m_nAlarmSeriesId(-1)
{
}

CDlgPlotGraph::~CDlgPlotGraph()
{
}

void CDlgPlotGraph::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_CUSTOM_PLOT, m_ChartCtrl);

   DDX_Control(pDX, IDC_STATIC_NUM_PPM, m_Numppm);
   DDX_Control(pDX, IDC_STATIC_PPM, m_Staticppm);
   DDX_Control(pDX, IDC_STATIC_LAT_TEXT, m_StaticLat);
   DDX_Control(pDX, IDC_STATIC_LONG_TEXT, m_StaticLong);
   DDX_Control(pDX, IDC_STATIC_LAST_PEAK, m_StaticLastPeak);
   DDX_Control(pDX, IDC_STATIC_DISTANCE, m_StaticDistance);
   DDX_Control(pDX, IDC_STATIC_LAT, m_StaticLatText);
   DDX_Control(pDX, IDC_STATIC_LON, m_StaticLonText);
   DDX_Control(pDX, IDC_PROGRESS_STATUS, m_ProgressBarStatus);
}


BEGIN_MESSAGE_MAP(CDlgPlotGraph, CDialogEx)
   ON_WM_ERASEBKGND()
   ON_WM_SIZE()
   ON_STN_CLICKED(IDC_STATIC_LONG_TEXT, &CDlgPlotGraph::OnStnClickedStaticLongText)
   ON_STN_CLICKED(IDC_STATIC_LON, &CDlgPlotGraph::OnStnClickedStaticLon)
   ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CDlgPlotGraph::OnBnClickedButtonPause)
   ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDlgPlotGraph::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CDlgPlotGraph message handlers
#include "ChartCtrl/ChartXYSerie.h"
#include "ChartCtrl/ChartLineSerie.h"

BOOL CDlgPlotGraph::OnInitDialog()
{
   CDialogEx::OnInitDialog();

   CChartStandardAxis* pBottomAxis =
      m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
   pBottomAxis->SetMinMax(-m_Settings.m_nXAxis, 0);
   CChartStandardAxis* pLeftAxis =
      m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
   pLeftAxis->SetMinMax(0, m_Settings.m_nYAxis);
   

   AddAlarmLine();



   m_ChartCtrl.SetBackColor(WHITE_COLOR);
   m_ChartCtrl.RefreshCtrl();


   m_Numppm.m_bTransparent = true;
   m_Staticppm.m_bTransparent = true;
   m_StaticLastPeak.m_bTransparent = true;
   m_StaticDistance.m_bTransparent = true;
   m_StaticLatText.m_bTransparent = true;
   m_StaticLonText.m_bTransparent = true;
   m_StaticLastPeak.SetTextColor(RED_COLOR);
   m_StaticDistance.SetTextColor(RED_COLOR);
   m_Numppm.SetTextColor(RED_COLOR);

   m_Fontppm.CreatePointFont(450, _T("Times New Roman"), 0);
   m_Numppm.SetFont(&m_Fontppm);
   m_Staticppm.SetFont(&m_Fontppm);

   m_StaticLat.m_bTransparent = true;
   m_StaticLong.m_bTransparent = true;
   m_FontLatLong.CreatePointFont(200, _T("Times New Roman"), 0);
   m_StaticLat.SetFont(&m_FontLatLong);
   m_StaticLatText.SetFont(&m_FontLatLong);
   m_StaticLong.SetFont(&m_FontLatLong);
   m_StaticLonText.SetFont(&m_FontLatLong);
   m_StaticLastPeak.SetFont(&m_FontLatLong);
   m_StaticDistance.SetFont(&m_FontLatLong);
   m_ProgressBarStatus.SetRange(0, 100);
   SetWindowTheme(m_ProgressBarStatus.m_hWnd, L"", L"");
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPlotGraph::AddAlarmLine(void)
{
   if (m_nAlarmSeriesId != -1)
   {
      DeleteSeries(m_nAlarmSeriesId);
      m_nAlarmSeriesId = -1;
   }

   int nPoints = abs(m_Settings.m_nXAxis);
   if (nPoints < 2)
      return;
   double* XValues = new double[nPoints];
   double* YValues = new double[nPoints];
   double dStep = (0.0 + m_Settings.m_nXAxis) / (nPoints - 2);
   int nXStart = -m_Settings.m_nXAxis;


   for (int i = 0; i<nPoints; i++)
   {
      XValues[i] = nXStart + i * dStep;
      YValues[i] = m_Settings.m_nAlarmLevel;
   }
   m_nAlarmSeriesId = AddSeries(2, PS_DASH, XValues, YValues, nPoints, RED_COLOR, true);

   delete[] XValues;
   delete[] YValues;
}


void CDlgPlotGraph::Updateppm(int nVal, double dLon, double dLat)
{

   
   if (CDetectorguiDlg::m_st_nTerminaThread) return;
   //??? not initialized when ppm above alarmlevel COLORREF newColor;
   COLORREF newColor = BLACK_COLOR;
   bool bRefreshControl = false;
   if (nVal>maxnVal) maxnVal = nVal;
   if (nVal >= m_Settings.m_nAlarmLevel)
   {
      DummyUpdate(1);
      newColor = RED_COLOR;
      if (nVal > m_nLastPeakMax)
      {
         m_nLastPeakMax = m_nLastPeak = (int) nVal;
         m_dPeakLon = dLon;
         m_dPeakLat = dLat;
         CString sText;
         sText.Format(_T("Last Peak:        %5d"), (int) nVal);
         m_StaticLastPeak.SetWindowText(sText);
         //m_StaticLastPeak.ShowWindow(SW_SHOW);
         CRect rect;
         m_StaticLastPeak.GetWindowRect(rect);
         ScreenToClient(rect);
         InvalidateRect(rect);
      }
   }
   else
   {
      if (dDistance > 30.0)
      {
         m_nLastPeakMax = 0;
         dDistance = 0.0;
      }

   }
   if (newColor != m_Numppm.GetTextColor())
   {
      m_Numppm.SetTextColor(newColor);
      bRefreshControl = true;
   }

   //m_nPPMCounter++;
   double dElapseTime = ((double)clock() - m_LastppmUpdate) / CLOCKS_PER_SEC;
   if (dElapseTime > 0.30)
   {
      //m_nPPMCounter = 0;
      CString sText;
      sText.Format(_T("%d"), maxnVal);
      CString sPrevVal;
      m_Numppm.GetWindowText(sPrevVal);
      if (sText != sPrevVal || bRefreshControl)
      {
         m_Numppm.SetWindowText(sText);
         CRect rect;
         m_Numppm.GetWindowRect(rect);
         ScreenToClient(rect);
         InvalidateRect(rect);
      }
      m_bDisablePlotGuiUpdate = false;
      m_LastppmUpdate = clock();

      if (fabs(m_dPeakLat) > 0.0000000001)
      {
         dDeltaLat = (m_dPeakLat - dLat)*M_PI / 180.0;
         dDeltaLon = (m_dPeakLon - dLon)*M_PI / 180.0;
         dMeanLat = (m_dPeakLat + dLat) / 2.0 * M_PI / 180.0;
         dDistance = ((dDeltaLon*cos(dMeanLat)) *(dDeltaLon*cos(dMeanLat)) + dDeltaLat * dDeltaLat);
         dDistance = R_earth * sqrt(dDistance);
         dDistance /= 0.0254;
         dDistance /= 12.0;
         sText.Format(_T("Distance:        %4.0lf'"), dDistance);
         m_StaticDistance.SetWindowText(sText);
         CRect rect;
         m_StaticDistance.GetWindowRect(rect);
         ScreenToClient(rect);
         InvalidateRect(rect);
      }
      maxnVal = 0;
   }
   else
   {
      m_bDisablePlotGuiUpdate = true;
   }
}

void CDlgPlotGraph::UpdateLat(LPCTSTR szLat)
{
   if (m_bDisablePlotGuiUpdate) return;
   float fNewVal = static_cast<float>(_ttof(szLat));

   if (fNewVal != m_fLat)
   {
      m_fLat = fNewVal;
      m_StaticLat.SetWindowText(szLat);
      CRect rect;
      m_StaticLat.GetWindowRect(rect);
      ScreenToClient(rect);
      InvalidateRect(rect);
   }

}

void CDlgPlotGraph::UpdateLon(LPCTSTR szLon)
{
   if (m_bDisablePlotGuiUpdate) return;
   float fNewVal = static_cast<float>(_ttof(szLon));
   if (fNewVal != m_fLong)
   {
      m_StaticLong.SetWindowText(szLon);
      m_fLong = fNewVal;
      CRect rect;
      m_StaticLong.GetWindowRect(rect);
      ScreenToClient(rect);
      InvalidateRect(rect);
   }
}

void CDlgPlotGraph::DummyUpdate(int nLogging)
{
   int nShow = SW_HIDE;
   if (nLogging)
   {
      nShow = SW_NORMAL;
   }
   m_StaticDistance.ShowWindow(nShow);
   m_StaticLastPeak.ShowWindow(nShow);
   GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(nShow);
}

void CDlgPlotGraph::SetXRange(int nVal)
{
   CChartAxis* pBottomAxis = m_ChartCtrl.GetBottomAxis();
   if (pBottomAxis)
      pBottomAxis->SetMinMax(-nVal, 0);
}

void CDlgPlotGraph::SetYRange(int nVal)
{
   CChartAxis* pBottomAxis = m_ChartCtrl.GetLeftAxis();
   if (pBottomAxis)
      pBottomAxis->SetMinMax(0, nVal);
}


BOOL CDlgPlotGraph::OnEraseBkgnd(CDC* pDC)
{
   CRect r;
   pDC->GetClipBox(&r);
   pDC->FillSolidRect(r, WHITE_COLOR);
   return TRUE;// CDialogEx::OnEraseBkgnd(pDC);
}


void CDlgPlotGraph::OnSize(UINT nType, int cx, int cy)
{
   CDialogEx::OnSize(nType, cx, cy);
   if (!m_ChartCtrl.m_hWnd)
      return;

   CRect size;
   GetClientRect(&size);

   int nHeight = int(size.Height() * .76);


   m_ChartCtrl.SetWindowPos(NULL, 0, cy - nHeight, cx, nHeight, SWP_NOZORDER);
   
   m_StaticLastPeak.GetWindowRect(&size);
   ScreenToClient(size);
   m_StaticLastPeak.SetWindowPos(NULL, size.left-(size.right - cx + size.Height()), size.top, size.Width(), size.Height(), SWP_NOZORDER);
   m_StaticDistance.GetWindowRect(&size);
   ScreenToClient(size);
   m_StaticDistance.SetWindowPos(NULL, size.left - (size.right - cx + size.Height()), size.top, size.Width(), size.Height(), SWP_NOZORDER);

}

unsigned int CDlgPlotGraph::AddSeries(int nPenThickness, int nPenType, double* pdX, double* pdY, int nPoints, COLORREF color, bool bSmooth)
{
   CChartLineSerie* pSeries = m_ChartCtrl.CreateLineSerie();
   pSeries->SetWidth(nPenThickness);
   pSeries->SetPenStyle(nPenType);
   pSeries->SetPoints(pdX, pdY, nPoints);
   pSeries->SetColor(color);
   pSeries->SetSmooth(bSmooth);
   return pSeries->GetSerieId();
}

void CDlgPlotGraph::AddPointsToSeries(unsigned int& nSeriesId, double* pdX, double* pdY, int nPoints)
{
   CChartLineSerie* pSeries = (CChartLineSerie*)m_ChartCtrl.GetSerie(nSeriesId);
   if (pSeries)
   {
      pSeries->AddPoints(pdX, pdY, nPoints);
   }
}

void CDlgPlotGraph::AddPointToSeries(unsigned int& nSeriesId, double dX, double dY)
{
   CChartLineSerie* pSeries = (CChartLineSerie*)m_ChartCtrl.GetSerie(nSeriesId);
   if (pSeries)
   {
      pSeries->AddPoint(dX, dY);
   }
}

unsigned int CDlgPlotGraph::UpdateSeries(unsigned int& nSeriesId, int nPenThickness, int nPenType, double* pdX, double* pdY, int nPoints, COLORREF color, bool bSmooth)
{
   CChartLineSerie* pSeries = (CChartLineSerie*)m_ChartCtrl.GetSerie(nSeriesId);
   m_ChartCtrl.EnableRefresh(false);
   //removing and adding is faster than trying to modify existing values
   m_ChartCtrl.RemoveSerie(nSeriesId);
   nSeriesId = AddSeries(nPenThickness, nPenType, pdX, pdY, nPoints, color, bSmooth);
   m_ChartCtrl.EnableRefresh(true);
   return nSeriesId;
}


void CDlgPlotGraph::OnStnClickedStaticLongText()
{
   // TODO: Add your control notification handler code here
}


void CDlgPlotGraph::OnStnClickedStaticLon()
{
   // TODO: Add your control notification handler code here
}


void CDlgPlotGraph::OnBnClickedButtonPause()
{
   CDetectorguiDlg* pMainGui = static_cast<CDetectorguiDlg*>(GetParent());
   CButton* bPause = static_cast<CButton*>(GetDlgItem(IDC_BUTTON_PAUSE));
   CString sText;
   bPause->GetWindowText(sText);
   if (sText == _T("Pause"))
   {
      pMainGui->SetPauseBlueToothThread(true);
      bPause->SetWindowText(_T("Resume"));
   }
   else
   {
      pMainGui->SetPauseBlueToothThread(false);
      bPause->SetWindowText(_T("Pause"));
   }

}





void CDlgPlotGraph::OnBnClickedButtonClear()
{
   // TODO: Add your control notification handler code here
   m_StaticDistance.ShowWindow(SW_HIDE);
   m_StaticLastPeak.ShowWindow(SW_HIDE);
   GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(SW_HIDE);
   DummyUpdate(0);
}

