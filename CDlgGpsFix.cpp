// CDlgGpsFix.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgGpsFix.h"
#include "afxdialogex.h"


// CDlgGpsFix dialog

IMPLEMENT_DYNAMIC(CDlgGpsFix, CDlgPopBase)

CDlgGpsFix::CDlgGpsFix(CWnd* pParent /*=nullptr*/)
	: CDlgPopBase(IDD_DIALOG_GPS_FIX, pParent)
{

}

CDlgGpsFix::~CDlgGpsFix()
{
}

void CDlgGpsFix::DoDataExchange(CDataExchange* pDX)
{
   CDlgPopBase::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE1, m_StaticTextLin1);
}


BEGIN_MESSAGE_MAP(CDlgGpsFix, CDlgPopBase)
   ON_BN_CLICKED(IDC_BUTTON_PROCEED, &CDlgGpsFix::OnBnClickedButtonProceed)
   ON_BN_CLICKED(IDC_BUTTON_WAIT, &CDlgGpsFix::OnBnClickedButtonWait)
END_MESSAGE_MAP()


// CDlgGpsFix message handlers


void CDlgGpsFix::OnBnClickedButtonProceed()
{
   OnOK();
}


void CDlgGpsFix::OnBnClickedButtonWait()
{
   OnCancel();
}


BOOL CDlgGpsFix::OnInitDialog()
{
   CDlgPopBase::OnInitDialog();

   if (m_nMode == CDlgPopBase::GPS_PROCEED)
   {
      GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_BUTTON_PROCEED)->ShowWindow(SW_NORMAL);
      GetDlgItem(IDC_BUTTON_WAIT)->ShowWindow(SW_NORMAL);
   }

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
