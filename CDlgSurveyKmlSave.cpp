// CDlgSurveyKmlSave.cpp : implementation file
//

#include "stdafx.h"
#include "Detector_gui.h"
#include "CDlgSurveyKmlSave.h"
#include "afxdialogex.h"


// CDlgSurveyKmlSave dialog

IMPLEMENT_DYNAMIC(CDlgSurveyKmlSave, CDlgPopBase)

CDlgSurveyKmlSave::CDlgSurveyKmlSave(CWnd* pParent /*=nullptr*/)
	: CDlgPopBase(IDD_DIALOG_SURVEY_KML_SAVE, pParent)
{

}

CDlgSurveyKmlSave::~CDlgSurveyKmlSave()
{
}

void CDlgSurveyKmlSave::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE1, m_StaticTextLin1);
   DDX_Control(pDX, IDC_STATIC_TEXT_LINE2, m_StaticTextLin2);
}


BEGIN_MESSAGE_MAP(CDlgSurveyKmlSave, CDlgPopBase)
   ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgSurveyKmlSave message handlers


BOOL CDlgSurveyKmlSave::OnInitDialog()
{
   CDlgPopBase::OnInitDialog();

   GetDlgItem(IDC_EDIT_FILE_NAME)->SetWindowText(m_sMsg3);
   GetDlgItem(IDC_EDIT_FILE_NAME2)->SetWindowText(m_sMsg4);

   CRect rectParent;
   GetParent()->GetWindowRect(rectParent);
   CRect rectSelf;
   GetWindowRect(rectSelf);
   MoveWindow(rectParent.left+237, rectParent.top+309, rectSelf.Width(), rectSelf.Height());
   SetTimer(CLOSE_TIMER, WAIT_TIME_MS, 0);
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSurveyKmlSave::OnTimer(UINT_PTR nIDEvent)
{
   if (CLOSE_TIMER == nIDEvent)
   {
      KillTimer(CLOSE_TIMER);
      OnOK();
      return;
   }
   CDlgPopBase::OnTimer(nIDEvent);
}
