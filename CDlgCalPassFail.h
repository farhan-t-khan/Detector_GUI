#pragma once
#include "CDlgPopBase.h"

// CDlgCalPassFail dialog

class CDlgCalPassFail : public CDlgPopBase
{
	DECLARE_DYNAMIC(CDlgCalPassFail)

public:
	CDlgCalPassFail(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgCalPassFail();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAL_PASS_FAIL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
