// showart.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <afxpriv.h>
#include "showart.h"

#include "mainfrm.h"
#include "doc.h"
#include "view.h"
#include "dialogs.h"
//#include "tracewin.h"
#include "keydlg.h"
//#include "doc.h"
#include "registration.h"
#include "Winutil.h"
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif
 
char	Scratch[1000];

static void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt);

static char *GetFileDirName(char *src, char *dest);
static char *GetNextFilename(char *src, char *dest);
static void PathOnly(LPTSTR name);

const int	CShowartApp::FILENAME_BUFFER_SIZE = 500000;
const char* CShowartApp::VERSION = "9.0\nFebruary 2023";
const CString CShowartApp::True = "true";
const CString CShowartApp::False = "false";
Dib		NullDib;
Dib*	pNullDib = &NullDib;

/////////////////////////////////////////////////////////////////////////////
// CShowartApp

BEGIN_MESSAGE_MAP(CShowartApp, CWinApp)
	//{{AFX_MSG_MAP(CShowartApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_CLOSEALL, OnFileCloseall)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSEALL, OnUpdateFileCloseall)
	ON_COMMAND(ID_FILE_PREFERENCES, OnFilePreferences)
	ON_COMMAND(ID_FILE_KEY_COMMANDS, OnFileKeyCommands)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	// Global help commands
	ON_COMMAND(ID_HELP_INDEX, CWinApp::OnHelpIndex)
	ON_COMMAND(ID_HELP_USING, CWinApp::OnHelpUsing)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CWinApp::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CWinApp::OnHelpIndex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowartApp construction

CShowartApp::CShowartApp()
{
// temp fixme
#ifdef _DEBUG

#else
	//AfxMessageBox("CShowartApp::CShowartApp()");
#endif

	TRACE("CShowartApp Constructor\n");

	bmpTemplate = NULL;
	//pMain = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CShowartApp object

CShowartApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CShowartApp initialization

BOOL CShowartApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//AfxMessageBox("Attach!!");
#ifdef _DEBUG
//	CMfxTrace::Init();
#endif
	// DebugBreak();
	// gave up trying to figure out double-clicking on a file.
	// hard to test and debug

	TRACE("%s -- InitInstance!!\n", __argv[0]);

	SetDialogBkColor();        // set dialog background color to gray
	SetRegistryKey(_T("Elf Software"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CShowartDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CShowartView));
	AddDocTemplate(pDocTemplate);
    
	// moved Prefs.read here so that the stretch option will display correctly.
	// It used to be called AFTER the first file is loaded for Win-called instances of this App...

	Prefs.Read();
	Dib::ShowErrorsInMessageBox() = Prefs.showErrorsInMessageBox;

	//DebugBreak();
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	SetupShellRegistry();
	//Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
	return TRUE;
}

int CShowartApp::ExitInstance() 
{
	//Gdiplus::GdiplusShutdown(m_gdiplusToken);
	return CWinApp::ExitInstance();
}

// Copy and paste from CWinApp in order to change one line.
// namely stop adding to MRU list in OpenDocumentFile() below
BOOL CShowartApp::ProcessShellCommand(CCommandLineInfo& rCmdInfo)
{
	BOOL bResult = TRUE;
	switch (rCmdInfo.m_nShellCommand)
	{
	case CCommandLineInfo::RestartByRestartManager:
		// Re-register with the restart manager using the restart identifier from the command line
		RegisterWithRestartManager(SupportsApplicationRecovery(), rCmdInfo.m_strRestartIdentifier);

		// Call RestartIntance, which should reopen any previously opened document(s) and
		// optionally load the autosaved versions after querying the user about loading.
		if (RestartInstance())
			break;
		// If RestartInstance returns FALSE, then fall through to FileNew case, so
		// a new document is created--otherwise the main frame will not be created.

	case CCommandLineInfo::FileNew:
		if (!AfxGetApp()->OnCmdMsg(ID_FILE_NEW, 0, NULL, NULL))
			OnFileNew();
		if (m_pMainWnd == NULL)
			bResult = FALSE;
		break;

	// If we've been asked to open a file, call OpenDocumentFile()
	case CCommandLineInfo::FileOpen:
		if (!OpenDocumentFile(rCmdInfo.m_strFileName, FALSE))
			bResult = FALSE;
		break;

	// If the user wanted to print, hide our main window and
	// fire a message to ourselves to start the printing
	case CCommandLineInfo::FilePrintTo:
	case CCommandLineInfo::FilePrint:
		m_nCmdShow = SW_HIDE;
		ASSERT(m_pCmdInfo == NULL);
		if(OpenDocumentFile(rCmdInfo.m_strFileName, FALSE))
		{
			m_pCmdInfo = &rCmdInfo;
			ENSURE_VALID(m_pMainWnd);
			m_pMainWnd->SendMessage(WM_COMMAND, ID_FILE_PRINT_DIRECT);
			m_pCmdInfo = NULL;
		}
		bResult = FALSE;
		break;

	// If we're doing DDE print or print to, start up without a new document and hide ourselves
	case CCommandLineInfo::FileDDENoShow:
		m_pCmdInfo = (CCommandLineInfo*)(UINT_PTR)m_nCmdShow;
		m_nCmdShow = SW_HIDE;
		break;

	// If we're doing DDE open, start up without a new document, but don't hide ourselves -- this causes the
	// icon in the Windows 7 taskbar to start in the wrong position and slide into the right position.
	case CCommandLineInfo::FileDDE:
		break;

	// If we've been asked to register, exit without showing UI.
	// Registration was already done in InitInstance().
	case CCommandLineInfo::AppRegister:
		{
			Register();			
			bResult = FALSE;    // that's all we do

			// If nobody is using it already, we can use it.
			// We'll flag that we're unregistering and not save our state
			// on the way out. This new object gets deleted by the
			// app object destructor.

			if (m_pCmdInfo == NULL)
			{
				m_pCmdInfo = new CCommandLineInfo;
				m_pCmdInfo->m_nShellCommand = CCommandLineInfo::AppUnregister;
			}
			break;
		}

	// If we've been asked to unregister, unregister and then terminate
	case CCommandLineInfo::AppUnregister:
		{
			BOOL bUnregistered = Unregister();

			if (!rCmdInfo.m_bRunEmbedded)
			{
				if (bUnregistered)
					AfxMessageBox(AFX_IDP_UNREG_DONE);
				else
					AfxMessageBox(AFX_IDP_UNREG_FAILURE);
			}
			bResult = FALSE;    // that's all we do

			// If nobody is using it already, we can use it.
			// We'll flag that we're unregistering and not save our state
			// on the way out. This new object gets deleted by the
			// app object destructor.

			if (m_pCmdInfo == NULL)
			{
				m_pCmdInfo = new CCommandLineInfo;
				m_pCmdInfo->m_nShellCommand = CCommandLineInfo::AppUnregister;
			}
		}
		break;
	}
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	VersionMessage;
	//}}AFX_DATA

// Implementation
protected:
	BOOL OnInitDialog( );
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	VersionMessage = _T("");
	//}}AFX_DATA_INIT
}

BOOL CAboutDlg::OnInitDialog( )
{
	
	VersionMessage.Format("Showart Version %s\n%s Version", CShowartApp::VERSION,
#ifdef _DEBUG
	"Debug");
#else
	"Release");
#endif

	CDialog::OnInitDialog();

	return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_VERSION, VersionMessage);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CShowartApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// VB-Event registration
// (calls to AfxRegisterVBEvent will be placed here by ClassWizard)

//{{AFX_VBX_REGISTER_MAP()
//}}AFX_VBX_REGISTER_MAP

/////////////////////////////////////////////////////////////////////////////
// CShowartApp commands


void CShowartApp::OnEditPaste()
{
	//getDocument
	//CShowartDoc *pDoc = (CShowartDoc *)bmpTemplate->OpenDocumentFile(NULL);
	//CShowartDoc *pDoc = (CShowartDoc*)(pMain->GetActiveView()->GetDocument());
	CMainFrame*		pMainFrm	= getMainFrame();
	CShowartView*	pView		= getActiveView();
	CShowartDoc*	pDoc		= getDocument();

	if (pMainFrm->OpenClipboard())
	{
		BeginWaitCursor();

		HGLOBAL hNewDib = ::GetClipboardData(CF_DIB);
		
		if(!hNewDib)
		{		  
			CloseClipboard();
			EndWaitCursor();
			return;
		}
	
		Dib* newDib = new Dib(hNewDib);
		CloseClipboard();
//#if 0	
		pDoc->SetDib(newDib);
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle("Paste");
		//SetScrollSizes();
		//pDoc->GetActiveView()->OnDoRealize((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
//#endif

		EndWaitCursor();
	}

}

void CShowartApp::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
}



/////////////////////////////////////////////////////////////////////////////


void CShowartApp::OnUpdateFileCloseall(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable(!m_templateList.IsEmpty());
	pCmdUI->Enable(GetFirstDocTemplatePosition() != NULL);

}

/////////////////////////////////////////////////////////////////////////////

void CShowartApp::OnFileCloseall()
{
	POSITION templPos, docPos;
	templPos = GetFirstDocTemplatePosition();
	
	while (templPos)
	{
		//CMultiDocTemplate* pTemplate = (CMultiDocTemplate*)m_templateList.GetNext(templPos);
		CMultiDocTemplate* pTemplate = (CMultiDocTemplate*)GetNextDocTemplate(templPos);
		ASSERT_VALID(pTemplate);
	 
		docPos = pTemplate->GetFirstDocPosition();
		
		while (docPos)
		{
			CDocument* pDoc = pTemplate->GetNextDoc(docPos);
			ASSERT_VALID(pDoc);
			
			pTemplate->RemoveDocument(pDoc);
			pDoc->OnCloseDocument();
		}
	}
}	
	
void DisplayMultiFileStrings(char* newName)
{
	char* px = newName;
	CString sum = "File Name String(s):\n";
	
	while(1)
	{
		CString s = px;

		if(s.IsEmpty())
			break;

		sum += s;
		sum += '\n';

		px += s.GetLength() + 1;
	}

	AfxMessageBox(sum);
}

//////////////////////////////////////////////////////////////////////////

bool CShowartApp::IsOneFilename(const char * name)
{
	// input:
	// r:\art\foo.jpg00
	// r:\art0foo.jpg0goo.jpg00

	// look for a single 0

	ASSERT(*name);

	while(*name)
		name++;

	// first case -- we're testing for the SECOND zero
	if(*(name + 1) == 0)
		return true;
	
	return false;
}

//////////////////////////////////////////////////////////////////////////

CString CShowartApp::ParseFilenames(const char * mega, CStringArray & names)
{
	// input:
	// r:\art\foo.jpg00
	// r:\art0foo.jpg0goo.jpg00

	if(IsOneFilename(mega))
	{
		names.Add(mega);
		return getDirName(mega);
	}

	// OK -- we have multiple filenames...

	CString dirName = mega;
	const char*	px		= mega + dirName.GetLength() + 1;

	dirName += '\\';

	while(true)
	{
		if(!*px)
			return mega;  // found "\0\0"...

		CString s = dirName + px;
		names.Add(s);
		px += strlen(px) + 1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////

void CShowartApp::OnFileOpen()
{
	// cut & paste verbatim from MFC - had to do it in order to call my modified version
	// of DoPromptFileName() - which is NOT virtual...
	// prompt the user (with all document templates)

	TRACE("**** OnFileOpen()\n");

	char* newName = new char[FILENAME_BUFFER_SIZE];
	*newName = 0;	

	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_DONTADDTORECENT /* | OFN_FILEMUSTEXIST */ , TRUE, NULL))
		return; // open cancelled
    
	if(!*newName)
		return;

	CStringArray names;

	CString dirName = ParseFilenames(newName, names);

	if(dirName.GetLength() > 0)
	{
		lstrcpy(Prefs.openDir, dirName);
		Prefs.Write();
	}

	delete[] newName;

	int num = names.GetSize();

	if(num < 1)
		return;

	if (Prefs.openAllFilesInFolder)
	{
		names.RemoveAll();
		ListFilesInDirectory(dirName, names);
	}
	TheDibLib.Replace(names);
}

//////////////////////////////////////////////////////////////////////////////////
 
 // prompt for file name - used for open and save as
BOOL CShowartApp::DoPromptFileName(LPSTR fileName, UINT nIDSTitle, DWORD lFlags,
	BOOL bOpenFileDialog, CDocTemplate* pTemplate)
		// if pTemplate==NULL => all document templates
{                                                     
	// Changes are noted, o/w this is cut-code from MFC CWinApp ...
	static DWORD oldFilterIndex = 1;	// TODO:  save in profile file...
	
	CFileDialog dlgFile(bOpenFileDialog);
	
	CString title;
	VERIFY(title.LoadString(nIDSTitle));
	
	dlgFile.m_ofn.Flags |= lFlags;
	
	if(bOpenFileDialog)
	{
		LPCTSTR dir = GetOpenDir();

		if(dir)
		    dlgFile.m_ofn.lpstrInitialDir = dir;
	}
	else
	{
		if(*Prefs.saveDir)
	    	dlgFile.m_ofn.lpstrInitialDir = Prefs.saveDir;
	}
    // manually adding a (multiple) extension   BN 12/93
	CString strFilter = "Art Files";
	strFilter += (char) '\0';
	strFilter += "*.bmp;*.dib;*.gif;*.tif;*.jpg;*.png";
	strFilter += (char) '\0';
	// End Change -- BN
	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		AppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		// do for all doc template
		  
		//POSITION pos = m_templateList.GetHeadPosition();
		POSITION pos = GetFirstDocTemplatePosition();
		while (pos)
		{
			CDocTemplate* pdt = GetNextDocTemplate(pos);

			AppendFilterSuffix(strFilter, dlgFile.m_ofn,
				pdt, NULL);
		}
	}

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (char)'\0';        // next string please
	strFilter += "*.*";
	strFilter += (char)'\0';        // next string please

	strFilter += "List Files (*.lst)";
	strFilter += (char) '\0';
	strFilter += "*.lst";
	strFilter += (char) '\0';
	
	strFilter += (char)'\0';        // last string
	dlgFile.m_ofn.nMaxCustFilter++;


	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.hwndOwner = AfxGetApp()->m_pMainWnd->GetSafeHwnd();
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName; 
	dlgFile.m_ofn.nFilterIndex = oldFilterIndex;
	dlgFile.m_ofn.nMaxFile = FILENAME_BUFFER_SIZE;
	
	#if(WINVER >= 0x0400)
	dlgFile.m_ofn.Flags |= OFN_ALLOWMULTISELECT | OFN_NOVALIDATE | OFN_EXPLORER;// BN
	//dlgFile.m_ofn.Flags |= OFN_ALLOWMULTISELECT | OFN_PATHMUSTEXIST | OFN_EXPLORER;// BN
	#else
	dlgFile.m_ofn.Flags |= OFN_ALLOWMULTISELECT | OFN_PATHMUSTEXIST;// BN
	#endif

	DWORD pickFolderCheckboxID = 60000;
	CString checkboxText = "Open all files in the folder";
	
	if(bOpenFileDialog)
		dlgFile.AddCheckButton(pickFolderCheckboxID, checkboxText, Prefs.openAllFilesInFolder);

	BOOL bRet = dlgFile.DoModal() == IDOK ? TRUE : FALSE;

	oldFilterIndex = dlgFile.m_ofn.nFilterIndex;

	if (bOpenFileDialog)
		dlgFile.GetCheckButtonState(pickFolderCheckboxID, Prefs.openAllFilesInFolder); // openAllFilesInFolder REFERENCE sent in!
	
	if(bRet == IDOK)
	{
		LPTSTR dir;
		
		if(bOpenFileDialog)
			dir = Prefs.openDir;
		else
			dir = Prefs.saveDir;

		lstrcpy(dir, dlgFile.GetPathName());
		PathOnly(dir);
	}
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////

static char *GetFileDirName(char *src, char *dest)
{	   				   			
	if(!src || !*src || !dest)
		return NULL;

#if(WINVER >= 0x0400)
	while(*src)		// allow embedded blanks in name...
#else
	while(*src && *src != ' ')
#endif
		*dest++ = *src++;
				   
	*dest = 0;

#if(WINVER >= 0x0400)
#pragma message("IS THIS THE BUG ??????????????????" __FILE__)
	if(!*src && *(src + 1))
#else		   
	if(*src)
#endif	
		return src + 1; // Returns a pointer to begin of next filename (or null)
	else
		return NULL;	
}	

/////////////////////////////////////////////////////////////////////////////

static void PathOnly(LPTSTR name)
{
	// find the last '\' & end the string.
	// failing that find the last ':' & end the string
	LPTSTR px;

	if( (px = strrchr(name, '\\')) || (px = strrchr(name, ':')) )
		*++px = 0;
	
	else
		// WOW! No Path!!
		*name = 0;
}
	
/////////////////////////////////////////////////////////////////////////////

static char *GetNextFilename(char *src, char *dest)
{									
	if(!src || !dest)
		return NULL;
		
	while(*src && *src != ' ')
		*dest++ = *src++;
				   
	*dest = 0;
#if(WINVER >= 0x0400)
	if(!*src && *(src + 1))
#else		   
	if(*src)
#endif	
		return src + 1; // Returns a pointer to begin of next filename (or null)
	else
		return src;	
}	  

/////////////////////////////////////////////////////////////////////////////

static void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((const char*)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPSTR)(const char*)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (char)'\0';       // next string please
		filter += "*" + strFilterExt;
		filter += (char)'\0';       // next string please
		ofn.nMaxCustFilter++;
	}
}


void CShowartApp::OnFilePreferences()
{
	PreferencesDialog dlg;
	
	if(IDOK == dlg.DoModal())
	{
		Prefs.defaultFitToWindow	= dlg.m_defaultFitToWindow;
		Prefs.debug					= dlg.m_debug;
		Prefs.showErrorsInMessageBox= dlg.m_showErrorsInMessageBox;
		Prefs.autoTime				= dlg.m_autoTime;
		Prefs.m_sortDirection		= dlg.m_sortDirection;
		Prefs.m_sortType			= dlg.m_sortType;
		Prefs.jpegQuality			= dlg.m_jpegQuality;
		Prefs.useDefaultDir			= dlg.useDefaultDir;
		Prefs.defaultDir			= dlg.defaultDir;
		Prefs.safeCopyMode			= dlg.safeCopyMode;
		Prefs.autoCopyPng			= dlg.autoCopyPng;
		Prefs.Write();
		
		if(dlg.m_sortChanged)
		{
			TheDibLib.Sort();
			Dib* prevDib = TheDibLib.Curr();

			if(TheDibLib.First() != prevDib)
				m_pMainWnd->Invalidate();
		}
	}
	Dib::ShowErrorsInMessageBox() = Prefs.showErrorsInMessageBox;
}

void CShowartApp::OnFileKeyCommands() 
{
	KeyDlg dlg;
	dlg.DoModal();
}


void CShowartApp::ParseCommandLine(CCommandLineInfo & rCmdInfo)
{
	CWinApp::ParseCommandLine(rCmdInfo); // this will set some good stuff -- like New vs. Open
	
	
	// todo todo todo
	//return;

	for (int i = 1; i < __argc; i++)
	{
		bool isList = false;
		CString param = __targv[i];
		CString s;
		s.Format("CmdLine Arg  {%s}\n", param);
		TRACE(s);
		
		if(param.Right(4).CompareNoCase(".LST") == 0)
			isList = true;

	#ifdef _MDI
		if(isList)
			TheDibLib.AddList(param, false);
		else
			TheDibLib.Add(param, false);
	#else
		if(isList)
			TheDibLib.AddList(param, true);
		else
			TheDibLib.Add(param, true);
	#endif
	}
}
BOOL CShowartApp::WriteSubProfileBool(const CStringArray& sects, const CString & entry, bool value)
{
	if(value)
		return WriteSubProfileString(sects, entry, True);
	else
		return WriteSubProfileString(sects, entry, False);
}
BOOL CShowartApp::WriteSubProfileInt(const CStringArray& sects, const CString & entry, int value)
{
	HKEY hSubSecKey	= GetSubSectionKey(sects);
	int num = sects.GetCount();

	if(hSubSecKey == NULL)
	{
		CString msg = PrepareRegError("Couldn't get subsection: ", sects);
		TRACE(msg);
		return FALSE;
	}

	LONG lResult = RegSetValueEx(hSubSecKey, entry, NULL, REG_DWORD,
			(LPBYTE)&value, sizeof(value));

	RegCloseKey(hSubSecKey);
	return lResult == ERROR_SUCCESS;
}

BOOL CShowartApp::WriteSubProfileString(const CStringArray& sects, const CString & entry, const CString & value)
{
	HKEY hSubSecKey	= GetSubSectionKey(sects);
	int num = sects.GetCount();

	if(hSubSecKey == NULL)
	{
		CString msg = PrepareRegError("Couldn't get subsection: ", sects);
		TRACE(msg);
		return FALSE;
	}

	LONG lResult = RegSetValueEx(hSubSecKey, entry, NULL, REG_SZ,
				(LPBYTE)(LPCTSTR)value, (value.GetLength()+1)*sizeof(TCHAR));

	RegCloseKey(hSubSecKey);
	return lResult == ERROR_SUCCESS;
}

CString CShowartApp::GetSubProfileString(const CStringArray& sects, const CString& entry, const CString& defaultValue)
{
	HKEY hSubSecKey	= GetSubSectionKey(sects);
	int num = sects.GetCount();

	if(hSubSecKey == NULL)
	{
		CString msg = PrepareRegError("Couldn't get subsection: ", sects);
		TRACE(msg);
		return defaultValue;
	}

	CString strValue;
	DWORD dwType, dwCount;

	LONG lResult = RegQueryValueEx(hSubSecKey, (LPTSTR)(LPCTSTR)entry, NULL, &dwType,
		NULL, &dwCount);
	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_SZ);
		lResult = RegQueryValueEx(hSubSecKey, (LPTSTR)(LPCTSTR)entry, NULL, &dwType,
			(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
		strValue.ReleaseBuffer();
	}
	RegCloseKey(hSubSecKey);
	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_SZ);
		return strValue;
	}
	return defaultValue;
}


UINT CShowartApp::GetSubProfileInt(const CStringArray& sects, const CString& entry, int defaultValue)
{

	HKEY hSubSecKey	= GetSubSectionKey(sects);
	int num = sects.GetCount();

	if(hSubSecKey == NULL)
	{
		CString msg = PrepareRegError("Couldn't get subsection: ", sects);
		TRACE(msg);
		return (UINT)defaultValue;
	}

	DWORD dwValue;
	DWORD dwType;
	DWORD dwCount = sizeof(DWORD);
	
	LONG lResult = RegQueryValueEx(hSubSecKey, (LPTSTR)(LPCTSTR)entry, NULL, &dwType,
		(LPBYTE)&dwValue, &dwCount);
	RegCloseKey(hSubSecKey);

	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_DWORD);
		ASSERT(dwCount == sizeof(dwValue));
		return (UINT)dwValue;
	}
	return (UINT)defaultValue;
}
bool CShowartApp::GetSubProfileBool(const CStringArray& sects, const CString& entry, bool defaultValue)
{
	CString cs;

	if(defaultValue)
		cs = GetSubProfileString(sects, entry, True);
	else
		cs = GetSubProfileString(sects, entry, False);

	if(cs == True)
		return true;

	return false;
}

/** DEPRECATED
*/
HKEY CShowartApp::GetSubSectionKey(const CString & sectName, const CString & subSectName)
{
	CStringArray arr;
	arr.Add(sectName);
	arr.Add(subSectName);
	return GetSubSectionKey(arr);
}

HKEY CShowartApp::GetSubSectionKey(const CStringArray& sects)
{
	// caller has to close the returned key.  All other keys along
	// the way are closed here.
	int num = sects.GetCount();

	if(num < 2)
		return NULL;

	HKEY hSecKey	= GetSectionKey(sects[0]);

	if(!hSecKey)
		return NULL;

	HKEY hSubSecKey = NULL;

	for(int i = 1; i < num; i++)
	{
		if(!hSecKey)
			return NULL;

		hSubSecKey = GetSubSectionKey(hSecKey, sects[i]);
		RegCloseKey(hSecKey);
		hSecKey = hSubSecKey;
	}
	
	return hSubSecKey;
}

HKEY CShowartApp::GetSubSectionKey(HKEY hSecKey, const CString& subSectName)
{
	HKEY hSubSecKey = NULL;
	
	if (hSecKey)
	{
		DWORD dw;

		RegCreateKeyEx(hSecKey, subSectName, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hSubSecKey, &dw);
	}
	
	return hSubSecKey;
}
	
CString CShowartApp::PrepareRegError(const CString& header, const CStringArray& sects)
{
	CString msg = header;
	int num = sects.GetCount();

	for(int i = 0; i < num; i++)
	{
		if(i)
			msg += "/";
		
		msg += sects[i];
	}

	return msg;
}
	

void CShowartApp::SetupShellRegistry()
{
	Registration r;
	
	
	CString openCmd;

	AfxGetModuleShortFileName(AfxGetInstanceHandle(), openCmd);

	openCmd += " \"%1\"";

	SetShellKey("Showart.Document", "Showart Pix");
	SetShellKey("Showart.Document\\shell\\open\\ddeexec", "[open(\"%1\")]");
	SetShellKey("Showart.Document\\shell\\open\\command", openCmd);
	SetShellKey(".lst", "Showart.Document");
}

void CShowartApp::SetShellKey(LPCTSTR key, LPCTSTR value)
{
	if (ERROR_SUCCESS != ::RegSetValue(HKEY_CLASSES_ROOT, key, REG_SZ,
			  value, lstrlen(value) * sizeof(TCHAR)))
	{
		TRACE("Warning: registration database update failed for key '%s'.\n",
			key);
	}
}


CString CShowartApp::getDirName(const char * fname)
{
	if(fname == 0 || lstrlen(fname) <= 0)
		return "";

	const char* index = strrchr(fname, '\\');

	if(!index)
		return "";

	CString s = fname;

	return s.Left(index - fname);
}

BOOL CShowartApp::OnDDECommand(LPTSTR lpszCommand)
{
	//AfxMessageBox("DDE!!!");
   if (CWinApp::OnDDECommand(lpszCommand))
      return TRUE;

   // Handle any DDE commands recognized by your application
   // and return TRUE.  See implementation of CWinApp::OnDDEComand
   // for example of parsing the DDE command string.

   // Return FALSE for any DDE commands you do not handle.
   return FALSE;
 }

LPCTSTR CShowartApp::GetOpenDir()
{
	// if they want to ALWAYS use the same open dir...
	if(Prefs.useDefaultDir && Prefs.defaultDir.GetLength() > 0)
		return Prefs.defaultDir;

	if(*Prefs.openDir)
		return Prefs.openDir;

	return NULL;
}


CMainFrame* CShowartApp::getMainFrame() 
{
	
	// TODO dynamic cast syntax needed!!
//	CMainFrame* pMainFrm = (CMainFrame*)m_pMainWnd;
	ASSERT(m_pMainWnd);
	ASSERT(m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	return (CMainFrame*) m_pMainWnd;
}



CShowartView* CShowartApp::getActiveView()
{
	CShowartView* pView = (CShowartView*)getMainFrame()->GetActiveView();
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(CShowartView)));
	return pView;
}

CShowartDoc* CShowartApp::getDocument()
{
	CShowartDoc* pDoc = (CShowartDoc*)getActiveView()->GetDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CShowartDoc)));
	return pDoc;
}

