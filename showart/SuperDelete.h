#pragma once
#include "afxcmn.h"


// SuperDelete dialog

class SuperDelete : public CDialog
{
	DECLARE_DYNAMIC(SuperDelete)

public:
	SuperDelete(CWnd* pParent = NULL);   // standard constructor
	CSpinButtonCtrl		numcharsSpinner;
	BOOL				OnInitDialog();
	void				OnOK(void);
	int					getNumChars();
	enum				{ IDD = IDD_SUPER_DELETE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	int	numchars;
};
