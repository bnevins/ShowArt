// SuperDelete.cpp : implementation file
//

#include "stdafx.h"
#include "Showart.h"
#include "SuperDelete.h"
#include ".\superdelete.h"


// SuperDelete dialog

IMPLEMENT_DYNAMIC(SuperDelete, CDialog)
SuperDelete::SuperDelete(CWnd* pParent /*=NULL*/)
	: CDialog(SuperDelete::IDD, pParent)
	, numchars(4)
{
}

void SuperDelete::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, numcharsSpinner);
	DDX_Text(pDX, IDC_EDIT1, numchars);
	DDV_MinMaxInt(pDX, numchars, 1, 20);
}


BEGIN_MESSAGE_MAP(SuperDelete, CDialog)
END_MESSAGE_MAP()


// SuperDelete message handlers

BOOL SuperDelete::OnInitDialog()
{
	CDialog::OnInitDialog();
	numcharsSpinner.SetRange(1,20);
	numcharsSpinner.SetPos(4);

	return TRUE;
}

void SuperDelete::OnOK()
{
	CDialog::OnOK();
}

int SuperDelete::getNumChars()
{
	return numchars;
}
