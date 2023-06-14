#pragma once


// CTransparentCheckBox

class CTransparentCheckBox : public CButton
{
	DECLARE_DYNAMIC(CTransparentCheckBox)

public:
	CTransparentCheckBox();
	virtual ~CTransparentCheckBox();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
   CBrush m_Brush;
public:

};


