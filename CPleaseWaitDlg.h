#pragma once

const int WMU_PLEASEDLG_COMPLETE = WM_APP + 10;
const int WMU_PLEASEDLG_UPDATE = WM_APP + 11;
// CPleaseWaitDlg dialog

class CPleaseWaitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPleaseWaitDlg)

public:
	CPleaseWaitDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPleaseWaitDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PLEASE_WAIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg LRESULT OnComplete(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);
   afx_msg void OnBnClickedCancel();
   CProgressCtrl m_ProgresssCtrl;
};
