#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgPopBase.h"

IMPLEMENT_DYNAMIC(CDlgPopBase, CDialogEx)
CDlgPopBase::CDlgPopBase(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
   : CDialogEx(nIDTemplate, pParent)
, m_nMode(0)
, m_Line2Color(0)
{
}


CDlgPopBase::~CDlgPopBase()
{
}


BOOL CDlgPopBase::OnInitDialog()
{
   CDialogEx::OnInitDialog();

   m_StaticTextLin1.SetWindowText(m_sMsg1);
   m_StaticTextLin1.m_bTransparent = true;
   if (m_StaticTextLin2.m_hWnd)
   {
      m_StaticTextLin2.SetWindowText(m_sMsg2);
      m_StaticTextLin2.m_bTransparent = true;
      if (m_Line2Color != 0)
      {
         m_StaticTextLin2.SetTextColor(m_Line2Color);
      }
   }
   if (m_StaticTextLin3.m_hWnd)
   {
      m_StaticTextLin3.SetWindowText(m_sMsg3);
      m_StaticTextLin3.m_bTransparent = true;
   }
   
   if (m_nMode == CAL_OK || m_nMode == CAL_OK_CANCEL)
   {
      CWnd* pWnd = GetDlgItem(IDC_BUTTON_CALIBRATE);
      if (pWnd)
      {      
         pWnd->ShowWindow(SW_HIDE);
      }
      pWnd = GetDlgItem(IDCANCEL);
      if (pWnd && m_nMode == CAL_OK_CANCEL)
      {
         pWnd->ShowWindow(SW_NORMAL);
      }
      GetDlgItem(IDOK)->ShowWindow(SW_NORMAL);
      SendMessage(DM_SETDEFID, IDOK, 0);
      GetDlgItem(IDOK)->SetFocus();

   }
   else if (m_nMode == CALIBRATE)
   {
      GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_BUTTON_CALIBRATE)->ShowWindow(SW_NORMAL);
      SendMessage(DM_SETDEFID, IDC_BUTTON_CALIBRATE, 0);
      GetDlgItem(IDC_BUTTON_CALIBRATE)->SetFocus();
   }
   else if (m_nMode == CAL_RESULT)
   {
      if (m_bResultSucess)
      {
         m_StaticTextLin2.SetTextColor(DARK_GREEN_COLOR);
      }
      else
      {
         m_StaticTextLin2.SetTextColor(RED_COLOR);
      }

      SetTimer(TIMER_CLOSE_WINDOW, WAIT_TIME_MS, 0);
   }

   m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR_IGNORE_KEYS)); // use same resource name as dialog


   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPopBase::SetOptions(int nMode, CString sMsg1, CString sMsg2, CString sMsg3, CString sMsg4)
{
   m_nMode = nMode;
   m_sMsg1 = sMsg1;
   m_sMsg2 = sMsg2;
   m_sMsg3 = sMsg3;
   m_sMsg4 = sMsg4;
}

BEGIN_MESSAGE_MAP(CDlgPopBase, CDialogEx)
   ON_WM_ERASEBKGND()
   ON_WM_TIMER()
END_MESSAGE_MAP()


void CDlgPopBase::OnTimer(UINT_PTR nIDEvent)
{
   if (TIMER_CLOSE_WINDOW == nIDEvent)
   {
      KillTimer(nIDEvent);
      OnOK();
      return;
   }
   CDialogEx::OnTimer(nIDEvent);
}

//////////////////
// Pre-translate message: translate keystrokes using acclerator table. 
//
BOOL CDlgPopBase::PreTranslateMessage(MSG* pMsg)
{
   if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST) {
      HACCEL hAccel = m_hAccel;
      if (hAccel && ::TranslateAccelerator(m_hWnd, hAccel, pMsg))
         return TRUE;
   }
   return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgPopBase::OnEraseBkgnd(CDC* pDC)
{
   CRect r;
   pDC->GetClipBox(&r);

   pDC->FillSolidRect(r, GRAY_CHECK_BOX);
   return TRUE;
}
