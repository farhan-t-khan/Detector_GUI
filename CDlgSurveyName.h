#pragma once
#include "CDlgPopBase.h"

// CDlgSurveyName dialog

class CDlgSurveyName : public CDlgPopBase
{
	DECLARE_DYNAMIC(CDlgSurveyName)

public:
	CDlgSurveyName(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSurveyName();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SURVEY_NAME_MSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedButtonSetup();
   virtual BOOL OnInitDialog();
};
