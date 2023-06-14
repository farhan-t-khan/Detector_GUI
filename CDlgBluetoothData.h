#pragma once


// CDlgBluetoothData dialog
#define UPDATE_TIMER 100
class CDlgBluetoothData : public CDialog
{
	DECLARE_DYNAMIC(CDlgBluetoothData)

public:
	CDlgBluetoothData(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgBluetoothData();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BLUETOOTH_DATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedOk();
   virtual BOOL OnInitDialog();
   CListCtrl m_CtrlListData;
   afx_msg void OnTimer(UINT_PTR nIDEvent);
};
