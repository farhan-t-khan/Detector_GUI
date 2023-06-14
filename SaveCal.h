#pragma once


// SaveCal dialog

class SaveCal : public CDialogEx
{
	DECLARE_DYNAMIC(SaveCal)

public:
	SaveCal(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SaveCal();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
