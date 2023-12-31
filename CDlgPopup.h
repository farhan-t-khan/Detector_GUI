#pragma once

#include "CDlgPopBase.h"
// CDlgPopup dialog

class CDlgPopup : public CDlgPopBase
{
	DECLARE_DYNAMIC(CDlgPopup)
public:
	CDlgPopup(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgPopup();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STATUS };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
   afx_msg void OnBnClickedButtonCalibrate();
};
