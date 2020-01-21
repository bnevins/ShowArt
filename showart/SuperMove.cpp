// SuperMove.cpp : implementation file
//

#include "stdafx.h"
#include "Showart.h"
#include "SuperMove.h"
#include "ShowartUtils.h"


// SuperMove dialog

IMPLEMENT_DYNAMIC(SuperMove, CDialog)
	SuperMove::SuperMove(int nc, CString& aTarget)
	: CDialog(SuperMove::IDD, NULL)
	, numChars(nc)
	, target(_T(aTarget))
{
}

SuperMove::~SuperMove()
{
}

void SuperMove::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, numChars);
	DDV_MinMaxUInt(pDX, numChars, 1, 20);
	DDX_Control(pDX, IDC_SPIN1, numCharsSpinner);
	DDX_Text(pDX, IDC_WHERE, target);
}


BEGIN_MESSAGE_MAP(SuperMove, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBrowse)
END_MESSAGE_MAP()


// SuperMove message handlers

BOOL SuperMove::OnInitDialog()
{
	CDialog::OnInitDialog();

	numCharsSpinner.SetRange(1,20);
	numCharsSpinner.SetPos(4);

	return TRUE;
}
int SuperMove::getNumChars()
{
	return numChars;
}
CString& SuperMove::getTarget()
{
	return target;
}
void SuperMove::OnBrowse()
{
	CString folder = ShowartUtils::BrowseForFolder();

	if(folder.GetLength() > 0)
	{
		target = folder;
		UpdateData(FALSE);
	}
}
