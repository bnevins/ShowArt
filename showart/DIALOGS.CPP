// dialogs.cpp : implementation file
//

#include "stdafx.h"
#include "showart.h"
#include "dialogs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

const UINT RadioStartDirection	= IDC_RADIO1;
const UINT RadioEndDirection	= IDC_RADIO2;
const UINT RadioStartType		= IDC_RADIO3;
const UINT RadioEndType			= IDC_RADIO6;

/////////////////////////////////////////////////////////////////////////////
// PreferencesDialog dialog

PreferencesDialog::PreferencesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(PreferencesDialog::IDD, pParent)
	, defaultDir(_T(""))
	, useDefaultDir(FALSE)
{
	m_defaultFitToWindow	= Prefs.defaultFitToWindow;
	m_debug					= Prefs.debug;
	m_autoTime				= Prefs.autoTime;
	m_showErrorsInMessageBox= Prefs.showErrorsInMessageBox;
	m_jpegQuality			= Prefs.jpegQuality;
	defaultDir				= Prefs.defaultDir;
	useDefaultDir			= Prefs.useDefaultDir;
	safeCopyMode			= Prefs.safeCopyMode;
}

void PreferencesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN3, m_jpegQualitySpinner);
	DDX_Control(pDX, IDC_SPIN2, m_spinner);
	DDX_Check(pDX, IDC_CHECK2, m_defaultFitToWindow);
	DDX_Check(pDX, IDC_DEBUG_DISPLAY, m_debug);
	DDX_Text(pDX, IDC_AUTOTIME, m_autoTime);
	DDV_MinMaxUInt(pDX, m_autoTime, 1, 18000);
	DDX_Check(pDX, IDC_SKIP_BAD_ART, m_showErrorsInMessageBox);
	DDX_Text(pDX, IDC_JPEG_QUALITY, m_jpegQuality);
	DDV_MinMaxUInt(pDX, m_jpegQuality, 25, 95);
	DDX_Text(pDX, IDC_DEFAULT_DIR, defaultDir);
	DDX_Check(pDX, IDC_DEFAULT_DIR_ALWAYS_OPEN, useDefaultDir);
	DDX_Check(pDX, IDC_SAFECOPY, safeCopyMode);
}

BEGIN_MESSAGE_MAP(PreferencesDialog, CDialog)
	//{{AFX_MSG_MAP(PreferencesDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PreferencesDialog message handlers

BOOL PreferencesDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_sortDirection = Prefs.m_sortDirection;
	m_sortType		= Prefs.m_sortType;
	m_sortChanged = false;

	CheckRadioButton(RadioStartDirection, RadioEndDirection, 
				RadioStartDirection + m_sortDirection);
	CheckRadioButton(RadioStartType, RadioEndType, RadioStartType + m_sortType);

	m_spinner.SetRange(1, 18000);	// 5 minute maximum...
	m_spinner.SetPos(Prefs.autoTime);
	m_jpegQualitySpinner.SetRange(25, 95);
	m_jpegQualitySpinner.SetPos(Prefs.jpegQuality);

#if !defined(_DEBUG)
	//m_debugDisplayCtrl.ShowWindow(SW_HIDE);
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void PreferencesDialog::OnOK() 
{
	int newDir			= GetCheckedRadioButton(RadioStartDirection, RadioEndDirection);
	int newType			= GetCheckedRadioButton(RadioStartType, RadioEndType);

	m_sortChanged = false;

	// note that we have the RB's ID -- we need the offset...
	if(newDir > 0)
		newDir -= RadioStartDirection;
	if(newType > 0)
		newType -= RadioStartType;
	
	CDialog::OnOK();

	if((newType != m_sortType) || (newDir != m_sortDirection))
	{
		m_sortChanged = true;
		m_sortDirection = newDir;
		m_sortType		= newType;
	}
}
