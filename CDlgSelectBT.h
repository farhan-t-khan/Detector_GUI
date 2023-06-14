#pragma once


// CDlgSelectBT dialog
#include <vector>
class CDlgSelectBT : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectBT)

public:
	CDlgSelectBT(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSelectBT();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELECT_BT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   CComboBox m_cbBTDevices;
   afx_msg void OnBnClickedOk();
   virtual BOOL OnInitDialog();
   int m_nSelection = 0;
   CString m_sBTName;
   static CString m_st_sBTName;
   std::vector<CString> m_vBT;
   static int m_st_nShowDialog;
   bool m_bScan = false;
   afx_msg void OnBnClickedButtonScanBt();
};
