// CDlgCalPassFail.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgCalPassFail.h"



// CDlgCalPassFail dialog

IMPLEMENT_DYNAMIC(CDlgCalPassFail, CDialogEx)

CDlgCalPassFail::CDlgCalPassFail(CWnd* pParent /*=nullptr*/)
	: CDlgPopBase(IDD_DIALOG_CAL_PASS_FAIL, pParent)
{

}

CDlgCalPassFail::~CDlgCalPassFail()
{
}

void CDlgCalPassFail::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE1, m_StaticTextLin1);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE2, m_StaticTextLin2);
}


BEGIN_MESSAGE_MAP(CDlgCalPassFail, CDlgPopBase)
END_MESSAGE_MAP()


// CDlgCalPassFail message handlers
