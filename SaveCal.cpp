// SaveCal.cpp : implementation file
//

#include "stdafx.h"
#include "SaveCal.h"
#include "afxdialogex.h"
#include "resource.h"


// SaveCal dialog

IMPLEMENT_DYNAMIC(SaveCal, CDialogEx)

SaveCal::SaveCal(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

SaveCal::~SaveCal()
{
}

void SaveCal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SaveCal, CDialogEx)
END_MESSAGE_MAP()


// SaveCal message handlers
