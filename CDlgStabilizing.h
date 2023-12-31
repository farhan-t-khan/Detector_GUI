#pragma once
#include "CDlgPopBase.h"

// CDlgStabilizing dialog
#define TIMER_COUNTDOWN 300

class CDlgStabilizing : public CDlgPopBase
{
	DECLARE_DYNAMIC(CDlgStabilizing)

public:
	CDlgStabilizing(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgStabilizing();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STABILIZING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   virtual BOOL OnInitDialog();
   void SetOptionsGui(CString sMsg1, CString sMsg2, int nRestartTimer);
   void StopGui(void);
};
