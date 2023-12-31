// CDlgMessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgMessageBox.h"
#include "afxdialogex.h"


// CDlgMessageBox dialog

IMPLEMENT_DYNAMIC(CDlgMessageBox, CDlgPopBase)

CDlgMessageBox::CDlgMessageBox(CWnd* pParent /*=nullptr*/)
	: CDlgPopBase(IDD_DIALOG_MESSAGE_BOX_OK_CANCEL, pParent)
{

}

CDlgMessageBox::~CDlgMessageBox()
{
}

void CDlgMessageBox::DoDataExchange(CDataExchange* pDX)
{
   CDlgPopBase::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE1, m_StaticTextLin1);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE2, m_StaticTextLin2);
}


BEGIN_MESSAGE_MAP(CDlgMessageBox, CDlgPopBase)
   ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgMessageBox::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CDlgMessageBox message handlers


void CDlgMessageBox::OnBnClickedButtonCancel()
{
   OnCancel();
}
