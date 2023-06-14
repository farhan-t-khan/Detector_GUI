// TransparentSlider.cpp : implementation file
//

#include "stdafx.h"
#include "TransparentSlider.h"


// CTransparentSlider

IMPLEMENT_DYNAMIC(CTransparentSlider, CSliderCtrl)

CTransparentSlider::CTransparentSlider()
{
   m_Brush.CreateSolidBrush(WHITE_COLOR);
}

CTransparentSlider::~CTransparentSlider()
{
}


BEGIN_MESSAGE_MAP(CTransparentSlider, CSliderCtrl)
   ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CTransparentSlider message handlers




HBRUSH CTransparentSlider::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
   pDC->SetBkMode(TRANSPARENT);
   //We could return GetStockObject(HOLLOW_BRUSH) to make it transparent however the ::SetWindowTheme L"wstr would need to be used
   return (HBRUSH)m_Brush;// GetStockObject(HOLLOW_BRUSH);
}
