// CDlgPopup.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgPopup.h"
#include "afxdialogex.h"


// CDlgPopup dialog

IMPLEMENT_DYNAMIC(CDlgPopup, CDlgPopBase)

CDlgPopup::CDlgPopup(CWnd* pParent /*=nullptr*/)
	: CDlgPopBase(IDD_DIALOG_STATUS, pParent)
{

}

CDlgPopup::~CDlgPopup()
{
}

void CDlgPopup::DoDataExchange(CDataExchange* pDX)
{
   CDlgPopBase::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE1, m_StaticTextLin1);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE2, m_StaticTextLin2);
}


BEGIN_MESSAGE_MAP(CDlgPopup, CDlgPopBase)
   ON_BN_CLICKED(IDC_BUTTON_CALIBRATE, &CDlgPopup::OnBnClickedButtonCalibrate)
END_MESSAGE_MAP()


// CDlgPopup message handlers



BOOL CDlgPopup::OnInitDialog()
{
   CDlgPopBase::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgPopup::OnBnClickedButtonCalibrate()
{
   OnOK();
}
