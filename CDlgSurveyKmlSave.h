#pragma once
#include "CDlgPopBase.h"

// CDlgSurveyKmlSave dialog
#define CLOSE_TIMER 100

class CDlgSurveyKmlSave : public CDlgPopBase
{
	DECLARE_DYNAMIC(CDlgSurveyKmlSave)

public:
	CDlgSurveyKmlSave(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSurveyKmlSave();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SURVEY_KML_SAVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
   afx_msg void OnTimer(UINT_PTR nIDEvent);
};
