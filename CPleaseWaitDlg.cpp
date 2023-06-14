// CPleaseWaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CPleaseWaitDlg.h"
#include "afxdialogex.h"


// CPleaseWaitDlg dialog

IMPLEMENT_DYNAMIC(CPleaseWaitDlg, CDialogEx)

CPleaseWaitDlg::CPleaseWaitDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PLEASE_WAIT, pParent)
{

}

CPleaseWaitDlg::~CPleaseWaitDlg()
{
}

void CPleaseWaitDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_PROGRESS1, m_ProgresssCtrl);
}


BEGIN_MESSAGE_MAP(CPleaseWaitDlg, CDialogEx)
   ON_BN_CLICKED(IDCANCEL, &CPleaseWaitDlg::OnBnClickedCancel)
   ON_MESSAGE(WMU_PLEASEDLG_UPDATE, OnUpdate)
   ON_MESSAGE(WMU_PLEASEDLG_COMPLETE, OnComplete)
END_MESSAGE_MAP()


// CPleaseWaitDlg message handlers


void CPleaseWaitDlg::OnBnClickedCancel()
{

}

LRESULT CPleaseWaitDlg::OnComplete(WPARAM wParam, LPARAM lParam)
{
   OnOK();
   return 0;
}

LRESULT CPleaseWaitDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
   int nProgress = wParam;
   CString sText;
   sText.Format(_T("%d%%"), nProgress);
   GetDlgItem(IDC_STATIC_PERCENT)->SetWindowText(sText);
   m_ProgresssCtrl.SetPos(nProgress);
   return 0;
}
