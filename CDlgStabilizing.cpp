// CDlgStabilizing.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "Detector_guiDlg.h"
#include "CDlgStabilizing.h"
#include "afxdialogex.h"


// CDlgStabilizing dialog

IMPLEMENT_DYNAMIC(CDlgStabilizing, CDlgPopBase);

CDlgStabilizing::CDlgStabilizing(CWnd* pParent /*=nullptr*/)
	: CDlgPopBase(IDD_DIALOG_STABILIZING, pParent)
{

}

CDlgStabilizing::~CDlgStabilizing()
{
}

void CDlgStabilizing::DoDataExchange(CDataExchange* pDX)
{
   CDlgPopBase::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE1, m_StaticTextLin1);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE2, m_StaticTextLin2);
}


BEGIN_MESSAGE_MAP(CDlgStabilizing, CDlgPopBase)
   ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgStabilizing message handlers


void CDlgStabilizing::OnTimer(UINT_PTR nIDEvent)
{

   
   if (TIMER_COUNTDOWN)
   {
      CString sVal;
      int nVal = 0;
      m_StaticTextLin2.GetWindowText(sVal);
      nVal = _ttoi(sVal);
      nVal--;
      if (nVal < 0 ) // StartBumpTest
      {

            KillTimer(nIDEvent);
            CDetectorguiDlg *pParent = (CDetectorguiDlg*)GetParent();
            pParent->PostMessage(UWM_BUMP_TEST, 0, 0);

            OnOK();
            return;


      }
      sVal.Format(_T("%d"), nVal);
      m_StaticTextLin2.SetWindowText(sVal);
      Invalidate();
   }

   CDlgPopBase::OnTimer(nIDEvent);
}


BOOL CDlgStabilizing::OnInitDialog()
{
   CDlgPopBase::OnInitDialog();

   SetTimer(TIMER_COUNTDOWN, 1000, 0);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgStabilizing::SetOptionsGui(CString sMsg1, CString sMsg2, int nRestartTimer)
{
   m_StaticTextLin1.SetWindowText(sMsg1);
   m_StaticTextLin2.SetWindowText(sMsg2);
   if (nRestartTimer)
   {
      SetTimer(TIMER_COUNTDOWN, 1000, 0);
   }
}
void CDlgStabilizing::StopGui(void)
{
   KillTimer(TIMER_COUNTDOWN);
   OnOK();
}