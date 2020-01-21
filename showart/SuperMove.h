#pragma once
#include "afxcmn.h"


// SuperMove dialog

class SuperMove : public CDialog
{
	DECLARE_DYNAMIC(SuperMove)

public:
	SuperMove(int nc, CString& aTarget);   // standard constructor
	virtual ~SuperMove();
	int getNumChars();
	CString& getTarget();
// Dialog Data
	enum { IDD = IDD_SUPER_MOVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int numChars;
	CSpinButtonCtrl numCharsSpinner;
	virtual BOOL OnInitDialog();
	CString target;
	afx_msg void OnBrowse();
};
