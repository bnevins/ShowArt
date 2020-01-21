// SuperSkip.cpp : implementation file
//

#include "stdafx.h"
#include "Showart.h"
#include "SuperSkip.h"


// SuperSkip dialog

IMPLEMENT_DYNAMIC(SuperSkip, CDialog)
SuperSkip::SuperSkip(CWnd* pParent /*=NULL*/)
	: CDialog(SuperSkip::IDD, pParent)
	, numChars(4)
{
}


BOOL SuperSkip::OnInitDialog()
{
	CDialog::OnInitDialog();
	spinner.SetRange(1,40);
	spinner.SetPos(Prefs.superSkipNum);

	return TRUE;
}

int SuperSkip::getNumChars()
{
	Prefs.superSkipNum = numChars;
	return numChars;
}

void SuperSkip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, spinner);
	DDX_Text(pDX, IDC_EDIT1, numChars);
	DDV_MinMaxInt(pDX, numChars, 1, 20);
}


BEGIN_MESSAGE_MAP(SuperSkip, CDialog)
END_MESSAGE_MAP()


// SuperSkip message handlers
