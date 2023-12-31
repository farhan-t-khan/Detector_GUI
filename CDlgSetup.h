#pragma once

#include "ColorStaticST.h"
#include "TransparentCheckBox.h"
#include "BtnST.h"
#include "Settings.h"
// CDlgSetup dialog

class CDlgSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetup)
   bool m_bXAxisChange;
   bool m_bYAxisChange;
   bool m_bCalTimeChange;
   CString m_sLastFileName;
public:
	CDlgSetup(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSetup();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   afx_msg void OnPaint();
   CString GetLastFileName() { return m_sLastFileName; };
   CColorStaticST m_StaticVersions;
   virtual BOOL OnInitDialog();
   CTransparentCheckBox m_CheckSubBackground;
   CTransparentCheckBox m_SurveyStatic;
   CTransparentCheckBox m_CalDataStatic;
   CTransparentCheckBox m_AlarmLevelStatic;
   CTransparentCheckBox m_XAxisStatic;
   CTransparentCheckBox m_FileNameStatic;
   CTransparentCheckBox m_CsvStatic;
   CEdit m_EditCtrlAlarmLevel;
   CEdit m_EditCtrlSurveyPath;
   CEdit m_EditCtrlCalPath;
   CTransparentCheckBox m_StaticCalDue;
   CTransparentCheckBox m_Check24hClock;
   CEdit m_EditCtrlCalDue;
   afx_msg void OnEnChangeEditAlarmLevel();
   CButtonST m_BtnSurveyPath;
   afx_msg void OnBnClickedButtonOpenSurveyPath();
   CString SelectDataFolder(CEdit * pEditCtrl, CString &sTitle);
   CButtonST m_BtnCalDataPath;
   afx_msg void OnBnClickedButtonOpenCalPath();
   CTransparentCheckBox m_StaticWavFile;
   CEdit m_EditCtrlWavFile;
   afx_msg void OnBnClickedButtonWavFile();
   CButtonST m_BtnWavFile;
   CTransparentCheckBox m_CheckDate;
   CTransparentCheckBox m_CheckTime;
   CTransparentCheckBox m_StaticLocation;
   CEdit m_EditCtrlLocation;
   CString SetCsvFileName();
   CString CheckCsvFileName(CString sName);
   CString ConstructFileName(void);
   char m_szDate[64] = { 0 };
   char* GetLastDateFile(void) { return m_szDate; };
   afx_msg void OnBnClickedCheckDate();
   afx_msg void OnBnClickedCheckTime();
   afx_msg void OnBnClickedCheck24hClock();
   afx_msg void OnEnChangeEditLocation();
   CTransparentCheckBox m_YAxisStatic;
   CEdit m_EditCtrlXAxis;
   CEdit m_EditCtrlYAxis;
   afx_msg void OnEnChangeEditXaxis();
   afx_msg void OnEnChangeEditYaxis();
   CSettings& m_Settings;
   afx_msg void OnBnClickedCheckSubBackground();
   afx_msg void OnEnKillfocusEditXaxis();
   afx_msg void OnEnKillfocusEditYaxis();
   afx_msg void OnEnChangeEditCalDue();
   afx_msg void OnEnKillfocusEditCalDue();
   virtual void OnCancel();
   virtual void OnOK();
   afx_msg void OnEnSetfocusEditAlarmLevel();
   afx_msg void OnEnKillfocusEditAlarmLevel();
   bool m_bAlarmLevelChange;

   bool m_bSurveyPathChange;
   bool m_bCalPathChange;
   afx_msg void OnEnChangeEditSurveyPath();
   afx_msg void OnEnChangeEditCalPath();
   afx_msg void OnEnKillfocusEditSurveyPath();
   afx_msg void OnEnKillfocusEditCalPath();
   afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
   afx_msg void OnBnClickedButtonUpdateFw();
   afx_msg void OnEnChangeEditWaveFile();
   CButton b_Reset_CFlash;
   afx_msg void OnStnClickedStaticYaxis();
};
