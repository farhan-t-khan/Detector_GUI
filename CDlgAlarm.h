#pragma once
#include "BtnST.h"
#include "TransparentCheckBox.h"
#include "Settings.h"
#include "TransparentSlider.h"
// CDlgAlarm dialog

class CDlgAlarm : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAlarm)
   bool m_bAlarmLevelChange;
public:
	CDlgAlarm(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgAlarm();
   bool ChangeVolume(double nVolume, bool bScalar);
   float GetMasterVolumeScalar(void);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ALARM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   virtual BOOL OnInitDialog();
   afx_msg void OnBnClickedButton1();
   CStatic m_Btn2;
   void Clearppm(CString& sString);
   void UpdateAlarmLevel(TCHAR *pzVal);
   afx_msg void OnBnClickedButton2();
   CStatic m_Btn3;
   CStatic m_Btn4;
   CStatic m_Btn5;
   CStatic m_Btn6;
   CStatic m_Btn7;
   CStatic m_Btn8;
   CStatic m_Btn9;
   CStatic m_Btn0;
   CStatic m_BtnX;
   CStatic m_BtnCheck;

   afx_msg void OnStnClickedButton3();
   afx_msg void OnStnClickedButton4();
   afx_msg void OnStnClickedButton5();
   afx_msg void OnStnClickedButton6();
   afx_msg void OnStnClickedButton7();
   afx_msg void OnStnClickedButton8();
   afx_msg void OnStnClickedButton9();
   afx_msg void OnStnClickedButton0();
   afx_msg void OnStnClickedButtonX();
   afx_msg void OnStnClickedButtonCheck();

   CEdit m_EditCtrlAlarmLevel;
   CTransparentCheckBox m_AlarmLevelStatic;
   CTransparentCheckBox m_StaticWavFile;
   CEdit m_EditCtrlWavFile;
   CButtonST m_BtnWavFile;
   CSettings& m_Settings;
   afx_msg void OnBnClickedButtonWavFile();
   afx_msg void OnEnSetfocusEditAlarmLevel();
   afx_msg void OnEnChangeEditAlarmLevel();
   afx_msg void OnEnKillfocusEditAlarmLevel();
   CTransparentSlider m_SliderCtrlVolume;
   CButtonST m_BtnTestVolume;
   afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
