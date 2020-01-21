#pragma once
#include "afxcmn.h"


// SuperSkip dialog

class SuperSkip : public CDialog
{
	DECLARE_DYNAMIC(SuperSkip)

public:
	SuperSkip(CWnd* pParent = NULL);   // standard constructor
	BOOL OnInitDialog();
	int getNumChars();
// Dialog Data
	enum { IDD = IDD_SUPER_SKIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSpinButtonCtrl spinner;
	int numChars;
};
