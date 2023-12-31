#pragma once
#include "TransparentCheckBox.h"
#include "BtnST.h"
#include "Settings.h"
#include <vector>
// CDlgExportKml dialog

class CDlgExportKml : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgExportKml)

public:
	CDlgExportKml(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgExportKml();
   CSettings& m_Settings;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EXPORT_KML };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   virtual void OnCancel();
   CTransparentCheckBox m_StaticStartDate;
   CTransparentCheckBox m_StaticEndDate;
   CTransparentCheckBox m_StaticLocation;
   CButtonST m_BtnOk;
   afx_msg void OnBnClickedButtonOk();
   virtual BOOL OnInitDialog();
   CListCtrl m_KmlListCtrl;
   CDateTimeCtrl m_StartDateCtrl;
   CDateTimeCtrl m_EndDateCtrl;
   CEdit m_EditLocationCtrl;
   afx_msg void OnHdnItemchangedListKml(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnNMClickListKml(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnBnClickedButtonDisplay();
   std::vector<CString> m_vKmlFiles;
};
