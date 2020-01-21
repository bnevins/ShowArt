// mainfrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "showart.h"
#include "mainfrm.h"
#include "FullScreen.h"
#include "dib_iter.h"
#include "fullscre.h"
#include "Filename.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBaseFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBaseFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_COMMAND(ID_EDIT_EMPTYCLIPBOARD, OnEditEmptyclipboard)
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	//ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_VIEW_FITTOWINDOW,
		ID_SEPARATOR,
	ID_FILE_PRINT,	   
	ID_APP_ABOUT,
	//ID_CONTEXT_HELP,
};

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,			// status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{           
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE("Failed to create toolbar\n");
		return -1;		// fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE("Failed to create status bar\n");
		return -1;		// fail to create
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBaseFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBaseFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnQueryNewPalette()
{
	CView* pView = GetActiveView();
	
	if(pView == NULL)
		return FALSE;

	// just notify the target view
	pView->SendMessage(WM_REALIZE, (WPARAM)pView->m_hWnd);
	return TRUE;
}

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd)
{
	CBaseFrameWnd::OnPaletteChanged(pFocusWnd);
	CView* pView = GetActiveView();
	
	if(pView != NULL)		// notify all child windows that the palette has changed
		SendMessageToDescendants(WM_REALIZE, (WPARAM)pView->m_hWnd);
}


void CMainFrame::OnEditEmptyclipboard()
{
	if (OpenClipboard())
	{
		EmptyClipboard();
		CloseClipboard();
	}
}
	
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.x = AfxGetApp()->GetProfileInt("MainFrameWindow", "x", 25);
	cs.y = AfxGetApp()->GetProfileInt("MainFrameWindow", "y", 25);
	cs.cx = AfxGetApp()->GetProfileInt("MainFrameWindow", "w", 600);
	cs.cy = AfxGetApp()->GetProfileInt("MainFrameWindow", "h", 350);
	return CBaseFrameWnd::PreCreateWindow(cs);	
}	

void CMainFrame::OnClose()
{
 	RECT r;
	GetWindowRect(&r);
	
	if(!IsIconic() && !IsZoomed())
	{
		AfxGetApp()->WriteProfileInt("MainFrameWindow", "x", r.left);
		AfxGetApp()->WriteProfileInt("MainFrameWindow", "y", r.top);
		AfxGetApp()->WriteProfileInt("MainFrameWindow", "w", r.right - r.left);
		AfxGetApp()->WriteProfileInt("MainFrameWindow", "h", r.bottom - r.top);
	}	
	CBaseFrameWnd::OnClose();
}



CView* CMainFrame::GetActiveView()
{
	CView* ret = NULL;
#ifdef _MDI
	CMDIChildWnd* pMDIChildWnd = MDIGetActive();
	
	if (pMDIChildWnd)
		ret = pMDIChildWnd->GetActiveView();
#else
	ret = CBaseFrameWnd::GetActiveView();
#endif
	return ret;
}

void CMainFrame::OnFileNew() 
{
	// TODO: Add your command handler code here
	
}

/*
void CMainFrame::OnDropFiles(HDROP hDropInfo) 
{
	// Ancient Bug -- never got an invalidate for dropped files!!
	CBaseFrameWnd::OnDropFiles(hDropInfo);

	if(theApp.m_pMainWnd)
		theApp.m_pMainWnd->Invalidate();	
}
*/

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	// WBN 10/3/2004
	// D&D -- with Ctrl Key down means ADD but don't delete
	// the existing opened files

	SetActiveWindow();      // activate us first !
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	CStringArray filenames;

	bool replace = ::GetKeyState(VK_CONTROL) >= 0;
	
	for (UINT iFile = 0; iFile < nFiles; iFile++)
	{
		CString fname;
		::DragQueryFile(hDropInfo, iFile, fname.GetBuffer(_MAX_PATH), _MAX_PATH);
		fname.ReleaseBuffer();
		filenames.Add(fname);
	}
	::DragFinish(hDropInfo);

	// WBN Oct 3, 2004
	// persist the directory

	Filename fn(filenames.GetAt(0));
	CString& path = fn.path;

	if(path.GetLength() > 0)
	{
		lstrcpy(Prefs.openDir, path);
		Prefs.Write();
	}

	if(replace)
		TheDibLib.Replace(filenames);
	else
		TheDibLib.Add(filenames);
}


bool CMainFrame::isFullScreen()
{
	return fullScreen.isFullScreen();
}

void CMainFrame::startFullScreen()
{
	fullScreen.startFullScreen(this);
}

void CMainFrame::stopFullScreen()
{
	fullScreen.stopFullScreen(this);
}

void CMainFrame::toggleFullScreen()
{
	if(isFullScreen())
		stopFullScreen();
	else
		startFullScreen();
}
