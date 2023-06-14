// TransparentCheckBox.cpp : implementation file
//

#include "stdafx.h"
#include "TransparentCheckBox.h"


// CTransparentCheckBox

IMPLEMENT_DYNAMIC(CTransparentCheckBox, CButton)
CTransparentCheckBox::CTransparentCheckBox()
{
   m_Brush.CreateSolidBrush(WHITE_COLOR);
}

CTransparentCheckBox::~CTransparentCheckBox()
{
}


BEGIN_MESSAGE_MAP(CTransparentCheckBox, CButton)
      ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



HBRUSH CTransparentCheckBox::CtlColor(CDC* pDC, UINT/* nCtlColor*/) 
{
   pDC->SetBkMode(TRANSPARENT);
   //We could return GetStockObject(HOLLOW_BRUSH) to make it transparent however the ::SetWindowTheme L"wstr would need to be used
   return (HBRUSH)m_Brush;// GetStockObject(HOLLOW_BRUSH);
}

// CTransparentCheckBox message handlers


