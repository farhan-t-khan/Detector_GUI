#pragma once


// CMyListBox
struct ListBoxElement
{
   ListBoxElement();
   ~ListBoxElement();
   CString m_sName;
   BOOL m_bEnabled;
   BOOL m_bChecked;
};

#include <vector>
//using namespace std;
class CTransparentCheckList : public CListBox
{
	DECLARE_DYNAMIC(CTransparentCheckList)

public:
	CTransparentCheckList();
	virtual ~CTransparentCheckList();
   void AddString(CString sName);
   int DeleteString(UINT nIndex);
   int InsertString(int  nIndex, CString sItem);
   void ResetContent();
   virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	BOOL IsEnabled(int nIndex);
	void Enable(int nIndex, BOOL bEnable);
   void SetCheck(int nIndex, BOOL bChecked);
   BOOL GetCheck(int nIndex);
protected:
   BOOL m_bSelItemDrawn;
   std::vector<ListBoxElement> m_ListElements;
	DECLARE_MESSAGE_MAP()
protected:
   virtual void PreSubclassWindow();
public:
   //afx_msg void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
   afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


