#pragma once
#include "CDlgPopBase.h"

// CDlgMessageBox dialog

class CDlgMessageBox : public CDlgPopBase
{
	DECLARE_DYNAMIC(CDlgMessageBox)

public:
	CDlgMessageBox(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgMessageBox();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MESSAGE_BOX_OK_CANCEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedButtonCancel();
};
