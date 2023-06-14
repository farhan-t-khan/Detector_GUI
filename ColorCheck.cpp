// ColorCheck.cpp : implementation file
//
// Copyright (c) 2002 Radu Naiu
//
// Distribute and use freely, except:
// a) Don't alter or remove this notice.
// b) Mark the changes you make.
// This file is provided "as is" with no expressed or implied warranty.
// Use at your own risk. 
//
// History:
//
//      v1.0  24.01.2002  Initial release
//		v1.1  28.01.2002  BUG corrected: default checked state afeter initialization handled  

#include "stdafx.h"
#include "ColorCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorCheck
IMPLEMENT_DYNAMIC(CColorCheck, CButton)

CColorCheck::CColorCheck()
{
	checkFlag = FALSE;
	oldState = 0;
	oldAction = 0;
	drawFocus = FALSE;
	newColor = RGB(230,0,0);
	newArrowColor = RGB(0,0,0);
	newTextColor = RGB(0,0,0);
}

CColorCheck::~CColorCheck()
{
}


BEGIN_MESSAGE_MAP(CColorCheck, CButton)
	//{{AFX_MSG_MAP(CColorCheck)
	//}}AFX_MSG_MAP
   ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorCheck message handlers

void CColorCheck::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
   CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

   // Retrieve the button's caption
   //
   int nBkClr = pDC->GetBkColor();
   const int bufSize = 512;
   TCHAR buffer[bufSize];
   GetWindowText(buffer, bufSize);
   //
   // Set the Text Position more to the left
   //
   CSize captionSize;
   CRect frameRect, btnRect;
   frameRect.CopyRect(&lpDrawItemStruct->rcItem);
   captionSize = pDC->GetTextExtent(buffer, (int)_tcslen(buffer));
   btnRect = frameRect;
   int nMargingX = int(captionSize.cy * .7 + .5);
   int nCenterY = (frameRect.bottom + frameRect.top) / 2;
   btnRect.left += nMargingX;
   //Start drawing the check box
   DrawCheckCaption(pDC, btnRect, buffer, newTextColor);

   CBrush blackBrush;
   blackBrush.CreateSolidBrush(BLACK_COLOR);
   pDC->FrameRect(frameRect, &blackBrush);

   // save bk color; is changed by drawing functions

   COLORREF clrBk = GREEN_CHECK_BOX;
   if (!checkFlag)
   {
      clrBk = RED_CHECK_BOX;
      if (m_ArrowColorBkOffFlash != RED_CHECK_BOX)
      {
         //maybe do something else
         clrBk = m_ArrowColorBkOffFlash;
      }
   }
   else
   {
      clrBk = newArrowColorBk; 
   }
   int nBoxSize = int(captionSize.cy * 1.4);
   int nLeft = frameRect.right - nBoxSize - nMargingX;
   int nTop = nCenterY - nBoxSize/2;
   CRect rcBox(nLeft, nTop, nLeft + nBoxSize, nTop + nBoxSize);
   // Add aditional states (inactive, pushed) as required
   pDC->FrameRect(rcBox, &blackBrush);

   CRect rcBk(rcBox);
   // Use -3 when themed and CBS_UNCHECKEDHOT
   rcBk.InflateRect(-1, -1);
   pDC->FillSolidRect(&rcBk, clrBk);

   COLORREF clrArr = BLACK_COLOR;
   CPoint cener((rcBox.right + rcBox.left) / 2, (rcBox.bottom + rcBox.top) / 2);
   CRect rcArr(cener.x - 8, // left start position
      cener.y - 0, // top start position
      cener.x - 7, // width 1 pixel
      cener.y + 4); // height 3 pixels
   if (checkFlag)
   {
      for (int i = 0; i < 18; i++)
      {
         pDC->FillSolidRect(&rcArr, clrArr);
         rcArr.OffsetRect(1, (i < 5) ? 1 : -1);
      }
   }
   else
   {
      buffer[0] = _T('X');
      buffer[1] = 0;
      int nOldTextColor = pDC->SetTextColor(BLACK_COLOR);
      pDC->DrawText(buffer, (int)_tcslen(buffer), rcBox, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
      pDC->SetTextColor(nOldTextColor);
   }
   blackBrush.DeleteObject();
   pDC->SetBkColor(nBkClr); // restore bk color
   return;
}

void CColorCheck::DrawCheckCaption(CDC *pDC, CRect R, const TCHAR *Buf, COLORREF TextColor)
{
   COLORREF prevColor = pDC->SetTextColor(TextColor);
   pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(Buf, (int)_tcslen(Buf), R, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(prevColor);
}


void CColorCheck::SetCheck(int nCheck)
{
//	nCheck = GetCheck();
   int nPrev = checkFlag;
	if (nCheck == 1)
		checkFlag = 1;
	else
		checkFlag = 0;
   if (nPrev != checkFlag)
      Invalidate(TRUE);

}

BOOL CColorCheck::GetCheck()
{
	if (checkFlag == 1)
		return 1;	
	else
		return 0;
}

COLORREF CColorCheck::SetBkColor(COLORREF boxColor)
{
	newColor = boxColor;
	return newColor;
}

COLORREF CColorCheck::SetArrowColor(COLORREF arrowColor)
{
	newArrowColor = arrowColor;
	return newArrowColor;
}

COLORREF CColorCheck::SetMyTextColor(COLORREF textColor)
{
	newTextColor = textColor;
	return newTextColor;
}

HBRUSH CColorCheck::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
   pDC->SetBkMode(TRANSPARENT);
   //pDC->SetTextColor(WHITE_COLOR);
   return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
}
