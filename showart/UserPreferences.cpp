// prefer.cpp : implementation file
//

#include "stdafx.h"
//#include "showart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

char	*UserPreferences::sectName			= "Preferences"; 
UserPreferences Prefs;


UserPreferences::UserPreferences()
{
	m_sortType			= 0;
	m_sortDirection		= 0;
	defaultFitToWindow	= TRUE;
	debug				= FALSE;
	autoTime			= 10;
	jpegQuality			= 75;
	*openDir			= 0;
	*saveDir			= 0;
	showErrorsInMessageBox	= FALSE;
	useDefaultDir		= FALSE;
	defaultDir			= "";
	safeCopyMode		= FALSE;
	autoCopyPng			= FALSE;
}	

/////////////////////////////////////////////////////////////////////////////////


void UserPreferences::Read()
{
	m_sortType			= AfxGetApp()->GetProfileInt(sectName, "SortType",				FALSE);
	m_sortDirection		= AfxGetApp()->GetProfileInt(sectName, "SortDirection",			FALSE);
	defaultFitToWindow	= AfxGetApp()->GetProfileInt(sectName, "DefaultFitToWindow",	FALSE);
	debug 	  			= AfxGetApp()->GetProfileInt(sectName, "Debug",					FALSE);
	autoTime			= AfxGetApp()->GetProfileInt(sectName, "AutoTime",				10);
	jpegQuality			= AfxGetApp()->GetProfileInt(sectName, "JpegQuality",			75);
	showErrorsInMessageBox	= AfxGetApp()->GetProfileInt(sectName, "ShowErrorsInMessageBox", TRUE);
	useDefaultDir		= AfxGetApp()->GetProfileInt(sectName, "UseDefaultDir",			FALSE);
	defaultDir			= AfxGetApp()->GetProfileString(sectName, "UserDefaultOpenDir", NULL);
	superSkipNum		= AfxGetApp()->GetProfileInt(sectName, "DefaultSuperSkip",		8);
	safeCopyMode		= AfxGetApp()->GetProfileInt(sectName, "SafeCopyMode",			FALSE);
	autoCopyPng			= AfxGetApp()->GetProfileInt(sectName, "AutoCopyPng",			FALSE);
	// if none openDir[0] = 0.....
	lstrcpy(openDir, AfxGetApp()->GetProfileString(sectName, "DefaultOpenDir", NULL));
	lstrcpy(saveDir, AfxGetApp()->GetProfileString(sectName, "DefaultSaveDir", NULL));
	
	keys.Read();
	Globals::stretch = defaultFitToWindow;
}				   	  									 			 			  

/////////////////////////////////////////////////////////////////////////////////

UserPreferences::~UserPreferences()
{
	Write();
}

/////////////////////////////////////////////////////////////////////////////////

void UserPreferences::Write()
{
	CWinApp* pApp = AfxGetApp();

	if(!pApp)
		return;

	pApp->WriteProfileInt(sectName,		"SortType",				m_sortType);
	pApp->WriteProfileInt(sectName,		"SortDirection",		m_sortDirection);
	pApp->WriteProfileInt(sectName,		"DefaultFitToWindow",	defaultFitToWindow);
	pApp->WriteProfileInt(sectName,		"Debug",				debug);
	pApp->WriteProfileInt(sectName,		"AutoTime",				autoTime);
	pApp->WriteProfileInt(sectName,		"JpegQuality",			jpegQuality);
	pApp->WriteProfileInt(sectName,		"ShowErrorsInMessageBox",showErrorsInMessageBox);
	pApp->WriteProfileInt(sectName,		"UseDefaultDir",		useDefaultDir);
	pApp->WriteProfileString(sectName,	"UserDefaultOpenDir",	defaultDir);
	pApp->WriteProfileString(sectName,	"DefaultOpenDir",		openDir);
	pApp->WriteProfileString(sectName,	"DefaultSaveDir",		saveDir);
	pApp->WriteProfileInt(sectName,		"DefaultSuperSkip",		superSkipNum);
	pApp->WriteProfileInt(sectName,		"SafeCopyMode",			safeCopyMode);
	pApp->WriteProfileInt(sectName,		"AutoCopyPng",			autoCopyPng);

	keys.Write();
}										   					  				 



void UserPreferences::SetKeyScheme(const CString & scheme)
{
	keys.SetCurrentScheme(scheme);
}

void UserPreferences::FillSchemesBox(CComboBox& box)
{
	keys.FillSchemesBox(box);
}

void UserPreferences::WriteScheme()
{
	keys.WriteScheme();
}

