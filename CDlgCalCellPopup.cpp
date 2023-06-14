// CDlgCalCellPopup.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgCalCellPopup.h"



// CDlgCalCellPopup dialog

IMPLEMENT_DYNAMIC(CDlgCalCellPopup, CDlgPopBase)

CDlgCalCellPopup::CDlgCalCellPopup(CWnd* pParent /*=nullptr*/)
	: CDlgPopBase(IDD_DIALOG_CAL_CELL, pParent)
{

}

CDlgCalCellPopup::~CDlgCalCellPopup()
{
}

void CDlgCalCellPopup::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE1, m_StaticTextLin1);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE2, m_StaticTextLin2);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE3, m_StaticTextLin3);
}


BEGIN_MESSAGE_MAP(CDlgCalCellPopup, CDlgPopBase)
END_MESSAGE_MAP()


// CDlgCalCellPopup message handlers
//////////////////

BOOL CDlgCalCellPopup::OnInitDialog()
{
   CDlgPopBase::OnInitDialog();
   return TRUE;  
}