// MyListBox.cpp : implementation file
//

#include "stdafx.h"
#include "3000.h"
#include "TransparentCheckList.h"


// CMyListBox
IMPLEMENT_DYNAMIC(CTransparentCheckList, CListBox)
CTransparentCheckList::CTransparentCheckList()
{
   m_bSelItemDrawn = TRUE;
}

CTransparentCheckList::~CTransparentCheckList()
{

}

ListBoxElement::ListBoxElement()
{
   m_sName = "";
   m_bEnabled = TRUE;
   m_bChecked = FALSE;
}

ListBoxElement::~ListBoxElement()
{

}

BEGIN_MESSAGE_MAP(CTransparentCheckList, CListBox)
   ON_WM_CTLCOLOR_REFLECT()
   ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CMyListBox message handlers
void CTransparentCheckList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
   // TODO: Add your message handler code here
   CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
   if (lpDrawItemStruct->itemID == (UINT)-1)
   {
		if (lpDrawItemStruct->itemAction & ODA_FOCUS)
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		return;
   }
   else
   {
      int selChange = lpDrawItemStruct->itemAction & ODA_SELECT;
      int focusChange = lpDrawItemStruct->itemAction & ODA_FOCUS;
      int drawEntire = lpDrawItemStruct->itemAction & ODA_DRAWENTIRE;
		BOOL bSel;
		BOOL bEnable = m_ListElements[lpDrawItemStruct->itemID].m_bEnabled;
		UINT nState = 0;

      if (selChange || drawEntire) 
      {
         BOOL sel = lpDrawItemStruct->itemState & ODS_SELECTED;

         pDC->FillSolidRect(&lpDrawItemStruct->rcItem, 
                     ::GetSysColor(COLOR_BTNFACE));

			if (sel)
			{
            if (!m_bSelItemDrawn)
            {
               if (bEnable)
               {
                  bSel = m_ListElements[lpDrawItemStruct->itemID].m_bChecked;
				      m_ListElements[lpDrawItemStruct->itemID].m_bChecked = !bSel;
               }

               m_bSelItemDrawn = TRUE;
            }
			}

         // Draw check button
         int h = 
            lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

         CRect rect(lpDrawItemStruct->rcItem.left+1, 
                     lpDrawItemStruct->rcItem.top+1, 
                     lpDrawItemStruct->rcItem.left+h-2, 
                     lpDrawItemStruct->rcItem.top+h-2);
			nState = DFCS_BUTTONCHECK | (m_ListElements[lpDrawItemStruct->itemID].m_bChecked ? DFCS_CHECKED : 0);
			if (!bEnable)
			{
				nState |= DFCS_INACTIVE;
			}

         pDC->DrawFrameControl(&rect, DFC_BUTTON, nState); 

         // Draws item text
			pDC->SetTextColor(bEnable ? COLOR_WINDOWTEXT : RGB(0x80, 0x80, 0x80));
         pDC->SetBkMode(TRANSPARENT);
         lpDrawItemStruct->rcItem.left += h+2;
         pDC->DrawText((LPCTSTR)m_ListElements[lpDrawItemStruct->itemID].m_sName
            , &lpDrawItemStruct->rcItem, DT_LEFT);
      }

      // draws focus rectangle
      if (focusChange || (drawEntire && 
               (lpDrawItemStruct->itemState & ODS_FOCUS)))
            pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
   }
}

HBRUSH CTransparentCheckList::CtlColor(CDC* /*pDC*/, UINT nCtlColor)
{
   // TODO:  Change any attributes of the DC here
	if ( (GetExStyle()&WS_EX_TRANSPARENT) && nCtlColor==CTLCOLOR_LISTBOX)
		return (HBRUSH)::GetSysColorBrush(COLOR_BTNFACE);
   // TODO:  Return a non-NULL brush if the parent's handler should not be called
   return NULL;
}

void CTransparentCheckList::AddString(CString sName)
{
   ListBoxElement element;
   element.m_sName = sName;
   m_ListElements.push_back(element);

   CListBox::AddString((LPCTSTR) sName);
}

int CTransparentCheckList::DeleteString (UINT nIndex)
{
   if (nIndex >= 0 && nIndex < m_ListElements.size())
   {
      m_ListElements.erase(m_ListElements.begin() + nIndex);
   }

   return CListBox::DeleteString(nIndex);
}

int CTransparentCheckList::InsertString(int nIndex, CString sItem)
{
   ListBoxElement element;
   element.m_sName = sItem;

   if (nIndex == -1)
   {
      m_ListElements.push_back(element);
   }
   else if (nIndex >= 0 && nIndex <= (int) m_ListElements.size())
   {
      m_ListElements.insert(m_ListElements.begin() + nIndex, element);
   }

   return CListBox::InsertString(nIndex, (LPCTSTR) sItem);
}

void CTransparentCheckList::ResetContent()
{
   CListBox::ResetContent();

   m_ListElements.clear();
}


void CTransparentCheckList::OnLButtonUp(UINT nFlags, CPoint point)
{
   // TODO: Add your message handler code here and/or call default
   if (!m_bSelItemDrawn)
   {
      Invalidate();
   }

   CListBox::OnLButtonUp(nFlags, point);
}

void CTransparentCheckList::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
   m_bSelItemDrawn = FALSE;
	CListBox::OnLButtonDown(nFlags, point);
}

void CTransparentCheckList::Enable(int nIndex, BOOL bEnable)
{
	if (nIndex >= 0 && nIndex < (int) m_ListElements.size())
	{
		m_ListElements[nIndex].m_bEnabled = bEnable;
		Invalidate();
	}
}

BOOL CTransparentCheckList::IsEnabled(int nIndex)
{
	if (nIndex >= 0 && nIndex < (int) m_ListElements.size())
	{
		return m_ListElements[nIndex].m_bEnabled;
	}

	return FALSE;
}

void CTransparentCheckList::SetCheck(int nIndex, BOOL bChecked)
{
   if (nIndex >= 0 && nIndex < (int) m_ListElements.size())
   {
      if (m_ListElements[nIndex].m_bChecked != bChecked)
      {
         m_ListElements[nIndex].m_bChecked = bChecked;
         Invalidate();
      }
   }
}

BOOL CTransparentCheckList::GetCheck(int nIndex)
{
   if (nIndex >= 0 && nIndex < (int) m_ListElements.size())
   {
      if (m_ListElements[nIndex].m_bEnabled && !m_bSelItemDrawn)
      {
         return !m_ListElements[nIndex].m_bChecked;//this flag is toggled in DrawItem
      }
      else
      {
         return m_ListElements[nIndex].m_bChecked;
      }
   }

   return FALSE;
}

void CTransparentCheckList::PreSubclassWindow()
{
   // TODO: Add your specialized code here and/or call the base class
   ModifyStyleEx(0, WS_EX_TRANSPARENT);
   CListBox::PreSubclassWindow();
}
