// progress.cpp : implementation file
//

#include "stdafx.h"
#include "showart.h"
#include "progress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ProgressDlg dialog


ProgressDlg::ProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ProgressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ProgressDlg)
	DDX_Control(pDX, IDC_PASSES, passCtrl);
	DDX_Control(pDX, IDC_PROGRESS, progCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ProgressDlg, CDialog)
	//{{AFX_MSG_MAP(ProgressDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ProgressDlg message handlers
