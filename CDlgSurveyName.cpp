// CDlgSurveyName.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgSurveyName.h"
#include "afxdialogex.h"


// CDlgSurveyName dialog

IMPLEMENT_DYNAMIC(CDlgSurveyName, CDlgPopBase)

CDlgSurveyName::CDlgSurveyName(CWnd* pParent /*=nullptr*/)
	: CDlgPopBase(IDD_DIALOG_SURVEY_NAME_MSG, pParent)
{

}

CDlgSurveyName::~CDlgSurveyName()
{
}

void CDlgSurveyName::DoDataExchange(CDataExchange* pDX)
{
   CDlgPopBase::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE1, m_StaticTextLin1);
}


BEGIN_MESSAGE_MAP(CDlgSurveyName, CDlgPopBase)
   ON_BN_CLICKED(IDC_BUTTON_SETUP, &CDlgSurveyName::OnBnClickedButtonSetup)
END_MESSAGE_MAP()


// CDlgSurveyName message handlers


void CDlgSurveyName::OnBnClickedButtonSetup()
{
   OnCancel();
}


BOOL CDlgSurveyName::OnInitDialog()
{
   CDlgPopBase::OnInitDialog();
   GetDlgItem(IDC_EDIT_FILE_NAME)->SetWindowText(m_sMsg2);
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
