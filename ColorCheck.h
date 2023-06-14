#if !defined(AFX_COLORCHECK_H__A897D857_09A3_11D6_8CAD_00500439B545__INCLUDED_)
#define AFX_COLORCHECK_H__A897D857_09A3_11D6_8CAD_00500439B545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorCheck.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorCheck window

class CColorCheck : public CButton
{
DECLARE_DYNAMIC(CColorCheck)

// Construction
public:

	CColorCheck();
	virtual ~CColorCheck();
	
//	BOOL Attach(const UINT nID, CWnd* pParent);	
	virtual void SetCheck(int nCheck);
	virtual int GetCheck();
	COLORREF SetBkColor(COLORREF color);
	COLORREF SetArrowColor(COLORREF newColor);
   void SetArrowColorBk(COLORREF newColor) { newArrowColorBk = newColor; };
   void SetArrowColorBkOffFlash(COLORREF newColor) { m_ArrowColorBkOffFlash = newColor; };
   COLORREF GetArrowColorBk(void) { return newArrowColorBk; };
   COLORREF GetArrowColorBkOffFlash(void) { return m_ArrowColorBkOffFlash; };
	COLORREF SetMyTextColor(COLORREF txtColor);
// Implementation
public:
	BOOL checkFlag;
	UINT oldAction;
	UINT oldState;
	BOOL drawFocus;
	COLORREF newColor, newArrowColor, newTextColor, newArrowColorBk = GREEN_CHECK_BOX;
   COLORREF m_ArrowColorBkOffFlash = RED_CHECK_BOX;
	// Generated message map functions
protected:
	void DrawCheckCaption(CDC *pDC, CRect R, const TCHAR *Buf, COLORREF TextColor);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItem);	//override the standard function (no WM_DRAWITEM !!!)
	COLORREF GetDisabledColor() { return disabled; }
	//{{AFX_MSG(CColorCheck)
	//}}AFX_MSG
private:
	COLORREF disabled;

	DECLARE_MESSAGE_MAP()
public:
   afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORCHECK_H__A897D857_09A3_11D6_8CAD_00500439B545__INCLUDED_)
