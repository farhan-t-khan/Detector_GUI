// CDlgBluetoothData.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgBluetoothData.h"
#include "afxdialogex.h"
#include "CBlueTooth.h"
#include "Detector_guiDlg.h"

// CDlgBluetoothData dialog

IMPLEMENT_DYNAMIC(CDlgBluetoothData, CDialog)

CDlgBluetoothData::CDlgBluetoothData(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_BLUETOOTH_DATA, pParent)
{

}

CDlgBluetoothData::~CDlgBluetoothData()
{
}

void CDlgBluetoothData::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_LIST_DATA, m_CtrlListData);
}


BEGIN_MESSAGE_MAP(CDlgBluetoothData, CDialog)
   ON_BN_CLICKED(IDOK, &CDlgBluetoothData::OnBnClickedOk)
   ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgBluetoothData message handlers


void CDlgBluetoothData::OnBnClickedOk()
{
   // TODO: Add your control notification handler code here
   CDialog::OnOK();
}


BOOL CDlgBluetoothData::OnInitDialog()
{
   CDialog::OnInitDialog();

   CDetectorguiDlg* pMainGui = (CDetectorguiDlg*)GetParent();
   
   m_CtrlListData.InsertColumn(0, _T("NAME"), LVCFMT_LEFT, 200);
   m_CtrlListData.InsertColumn(1, _T("VALUE"), LVCFMT_LEFT, 200);

   for (UINT i = 0; i < DATA_SIZE; i++)
   {
      m_CtrlListData.InsertItem(i, g_szLabels[i]);
      CString sData;
      sData.Format(_T("%d"), pMainGui->m_MyComms.m_aData[i]);
      m_CtrlListData.SetItemText(i, 1, sData);
   }
   SetTimer(UPDATE_TIMER, 1000, 0);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgBluetoothData::OnTimer(UINT_PTR nIDEvent)
{
   if (nIDEvent == UPDATE_TIMER)
   {
      CDetectorguiDlg* pMainGui = (CDetectorguiDlg*)GetParent();
      KillTimer(UPDATE_TIMER);
      for (UINT i = 0; i < DATA_SIZE; i++)
      {
         CString sData;
         sData.Format(_T("%d"), pMainGui->m_MyComms.m_aData[i]);
         m_CtrlListData.SetItemText(i, 1, sData);
      }
      SetTimer(UPDATE_TIMER, 1000, 0);
   }
   CDialog::OnTimer(nIDEvent);
}
