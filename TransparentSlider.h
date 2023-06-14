#pragma once


// CTransparentSlider

class CTransparentSlider : public CSliderCtrl
{
   DECLARE_DYNAMIC(CTransparentSlider)

public:
   CTransparentSlider();
   virtual ~CTransparentSlider();
   CBrush m_Brush;
protected:
   DECLARE_MESSAGE_MAP()
public:
   afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};


