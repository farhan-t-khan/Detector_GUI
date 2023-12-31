#pragma once


// CDlgGpsFix dialog
#include "CDlgPopBase.h"
class CDlgGpsFix : public CDlgPopBase
{
	DECLARE_DYNAMIC(CDlgGpsFix)

public:
	CDlgGpsFix(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgGpsFix();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GPS_FIX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedButtonProceed();
   afx_msg void OnBnClickedButtonWait();
   virtual BOOL OnInitDialog();
};
