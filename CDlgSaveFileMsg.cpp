// CDlgSaveFileMsg.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgSaveFileMsg.h"
#include "afxdialogex.h"


// CDlgSaveFileMsg dialog

IMPLEMENT_DYNAMIC(CDlgSaveFileMsg, CDlgPopBase)

CDlgSaveFileMsg::CDlgSaveFileMsg(CWnd* pParent /*=nullptr*/)
	: CDlgPopBase(IDD_DIALOG_FILE_SAVE_MSG, pParent)
{

}

CDlgSaveFileMsg::~CDlgSaveFileMsg()
{
}

void CDlgSaveFileMsg::DoDataExchange(CDataExchange* pDX)
{
   CDlgPopBase::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE1, m_StaticTextLin1);
}


BEGIN_MESSAGE_MAP(CDlgSaveFileMsg, CDlgPopBase)
END_MESSAGE_MAP()


// CDlgSaveFileMsg message handlers
void CDlgSaveFileMsg::UpdateValues(CString sMessage, CString sFileName)
{
   m_StaticTextLin1.SetWindowText(sMessage);
   GetDlgItem(IDC_EDIT_FILE_NAME)->SetWindowText(sFileName);
}