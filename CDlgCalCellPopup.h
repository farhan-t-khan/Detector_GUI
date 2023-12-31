#pragma once
#include "CDlgPopBase.h"
// CDlgCalCellPopup dialog

class CDlgCalCellPopup : public CDlgPopBase
{
	DECLARE_DYNAMIC(CDlgCalCellPopup)
public:
	CDlgCalCellPopup(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgCalCellPopup();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAL_CELL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();

   afx_msg void OnStnClickedStaticTextLine3();
};
