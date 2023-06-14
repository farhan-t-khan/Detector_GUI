// CDlgSelectBT.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgSelectBT.h"
#include "CBlueTooth.h"


// CDlgSelectBT dialog

IMPLEMENT_DYNAMIC(CDlgSelectBT, CDialog)

CString CDlgSelectBT::m_st_sBTName;
CDlgSelectBT::CDlgSelectBT(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SELECT_BT, pParent)
{

}

CDlgSelectBT::~CDlgSelectBT()
{
}

void CDlgSelectBT::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_COMBO_BT_DEVICE, m_cbBTDevices);
}


BEGIN_MESSAGE_MAP(CDlgSelectBT, CDialog)
   ON_BN_CLICKED(IDOK, &CDlgSelectBT::OnBnClickedOk)
   ON_BN_CLICKED(IDC_BUTTON_SCAN_BT, &CDlgSelectBT::OnBnClickedButtonScanBt)
END_MESSAGE_MAP()


// CDlgSelectBT message handlers


void CDlgSelectBT::OnBnClickedOk()
{
   m_nSelection = m_cbBTDevices.GetCurSel();
   if (m_nSelection < 0)
   {
      m_nSelection = 0;
   }
   if (m_nSelection < m_vBT.size())
   {
      m_st_sBTName = m_sBTName = m_vBT[m_nSelection];
      m_st_sBTName.Remove('\n');
      m_sBTName.Remove('\n');
   }
   CDialog::OnOK();
}

int CDlgSelectBT::m_st_nShowDialog = 0;

BOOL CDlgSelectBT::OnInitDialog()
{
   CDialog::OnInitDialog();
   m_bScan = false;
   if (m_vBT.size() > 0)
   {
      m_st_sBTName = m_sBTName = m_vBT[0];
      for (UINT i = 0; i < m_vBT.size(); i++)
      {
         m_cbBTDevices.AddString(m_vBT[i]);
      }
      m_cbBTDevices.SetCurSel(0);
   }
   else
   {
      extern BlueToothList* BT[BTArraySize];
      for (int i = 0; i < BTArraySize && BT[i]; i++)
      {
         if (BT[i]->name.IsEmpty())
         {
            break;
         }
         m_cbBTDevices.AddString(BT[i]->name);
      }
   }
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSelectBT::OnBnClickedButtonScanBt()
{
   m_bScan = true;
   CDialog::OnOK();
}
