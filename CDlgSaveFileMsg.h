#pragma once
#include "CDlgPopBase.h"

// CDlgSaveFileMsg dialog

class CDlgSaveFileMsg : public CDlgPopBase
{
	DECLARE_DYNAMIC(CDlgSaveFileMsg)

public:
	CDlgSaveFileMsg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSaveFileMsg();
   void UpdateValues(CString sMessage, CString sFileName);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE_SAVE_MSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
