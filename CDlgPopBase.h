#pragma once
#include "ColorStaticST.h"

#define TIMER_CLOSE_WINDOW 1000
class CDlgPopBase : public CDialogEx
{
   DECLARE_DYNAMIC(CDlgPopBase)
protected:
   CColorStaticST m_StaticTextLin1;
   CColorStaticST m_StaticTextLin2;
   CColorStaticST m_StaticTextLin3;
   CString m_sMsg1;
   CString m_sMsg2;
   CString m_sMsg3;
   CString m_sMsg4;
   COLORREF m_Line2Color;
   int m_nMode;
   bool m_bResultSucess;
   HACCEL m_hAccel; // accelerator table
   virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
   void SetLine2Color(COLORREF color) { m_Line2Color = color; };
   CDlgPopBase(UINT nIDTemplate, CWnd* pParentWnd = NULL);   // standard constructor
   enum { CAL_OK = 1, CAL_OK_CANCEL, CALIBRATE, CAL_RESULT, STABILIZE, GPS_OK, GPS_PROCEED};

   CDlgPopBase() {};
   ~CDlgPopBase();
   virtual BOOL OnInitDialog();
   void SetOptions(int nMode, CString sMsg1 = _T(""), CString sMsg2 = _T(""), CString sMsg3 = _T(""), CString sMsg4 = _T(""));
   void SetResultSucess(bool bVal) { m_bResultSucess = bVal; };
   DECLARE_MESSAGE_MAP()
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);

};

