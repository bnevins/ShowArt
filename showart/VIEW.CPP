// view.cpp : implementation of the CShowartView class
//

#include "stdafx.h"
#include "showart.h"
#include "mainfrm.h"
#include "doc.h"
#include "dib_iter.h"
#include "view.h"
#include "KeyCommandHandler.h"
#include "ShowartUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowartView

IMPLEMENT_DYNCREATE(CShowartView, CScrollView)

BEGIN_MESSAGE_MAP(CShowartView, CScrollView)
	//{{AFX_MSG_MAP(CShowartView)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_COMMAND(ID_VIEW_FITTOWINDOW, OnViewFittowindow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FITTOWINDOW, OnUpdateViewFittowindow)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_WALLPAPER, OnWallpaper)
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_MESSAGE(WM_REALIZE, OnRealize)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowartView construction/destruction

CShowartView::CShowartView() : timerID(1008)
{							
	wTimer = 0;
}

CShowartView::~CShowartView()
{               
	Prefs.Write();
	KillTimer();
}

/////////////////////////////////////////////////////////////////////////////
// CShowartView drawing

void CShowartView::OnDraw(CDC* pDC)
{
		MyDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////

void CShowartView::MyDraw(CDC* pDC)
{
	static Dib* prevDib = NULL;

	CShowartDoc* pDoc = GetDocument();
	Dib *dib = pDoc->GetDib();
	HDC hDC = pDC->m_hDC;

	if(!dib)
		return;
	
	if(prevDib != dib)
		SetScrollSizes();
	
	prevDib = dib;

	CMainFrame* pAppFrame = GetMainFrame();
	CView* pView = pAppFrame->GetActiveView();

	CRect r;
	GetClientRect(r);

	CRect	dibRect	= ShowartUtils::FitToWindow(r.Size(), *dib, Globals::stretch);
	CPoint	origin	= dibRect.TopLeft();

	// black background for fullscreen...
	if(pAppFrame->IsFullScreen())
	{
		CBrush brush(RGB(0,0,0)); 
		pDC->FillRect(&r, &brush);
	}

	dib->ShowWithPal(hDC, origin.x, origin.y, dibRect.Width(), dibRect.Height());			
}

/////////////////////////////////////////////////////////////////////////////
// CShowartView printing

BOOL CShowartView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CShowartView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CShowartView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


void CShowartView::OnActivateView(BOOL bActivate, CView* pActivateView,
	  				CView* pDeactiveView)
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	
	if (bActivate)
	{
		ASSERT(pActivateView == this);
		OnRealize((WPARAM)m_hWnd, 0);   // same as SendMessage(WM_DOREALIZE);
	}
}




LRESULT CShowartView::OnRealize(WPARAM wParam, LPARAM)

{
	Dib		 *dib;
	HPALETTE hPal;
	ASSERT(wParam != NULL);
	CShowartDoc* pDoc = GetDocument();
	
	dib = pDoc->GetDib();
	
	
	
 	if(!dib || !dib->OK())
		return 0L;  // must be a new document
    
    hPal = dib->GetPal();
    
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT(pAppFrame->IsKindOf(RUNTIME_CLASS( CMainFrame )));

	CClientDC appDC(pAppFrame);
	// All views but one should be a background palette.
	// wParam contains a handle to the active view, so the SelectPalette
	// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
	HPALETTE oldPal = ::SelectPalette(appDC.m_hDC, hPal, ((HWND)wParam) != m_hWnd);
				   						 
	if (oldPal)
	{		  
		UINT nColorsChanged = RealizePalette(appDC.m_hDC);
		if (nColorsChanged > 0)// && (HWND)wParam == m_hWnd)
			pDoc->UpdateAllViews(NULL);
		SelectPalette(appDC.m_hDC, oldPal, TRUE);
	}						
	else
	{
		TRACE0("SelectPalette failed in CShowArtView::OnPaletteChanged()\n");
	}									   

	return 0L;
}




/////////////////////////////////////////////////////////////////////////////
// CShowartView diagnostics

#ifdef _DEBUG
void CShowartView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CShowartView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CShowartDoc* CShowartView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShowartDoc)));
	return (CShowartDoc*) m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShowartView message handlers

void CShowartView::OnViewFullscreen()
{      
	GetMainFrame()->ShowFullScreen();
}



void CShowartView::OnViewFittowindow()
{
	Globals::stretch = Globals::stretch ? 0 : 1;
	SetScrollSizes();
				   
	if(Globals::stretch)

		Invalidate(FALSE);	// don't do background...
	else	
		Invalidate(TRUE);	// do background stretched could have been bigger...
		
		
		
#if 0	
	if(stretch)
		Invalidate(FALSE);	// don't do background...
	else	
		Invalidate(TRUE);	// do background stretched could have been bigger...
	if(stretch)
	{	
		SetScaleToFitSize(GetDocument()->GetSize());
		//SIZE sz;
		//RECT r;
		//GetClientRect(&r);
		//sz.cx = r.right;
		//sz.cy = r.bottom;
		
		//SetScaleToFitSize(sz);
	}
	else
		SetScrollSizes(MM_TEXT, GetDocument()->GetSize());
#endif
}				   			   				

void CShowartView::OnUpdateViewFittowindow(CCmdUI* pC)
{
	pC->SetCheck(Globals::stretch);
}

void CShowartView::OnEditCopy()
{
	CClientDC  dc(this); 
	CShowartDoc* pDoc = GetDocument();
	Dib *dib = pDoc->GetDib();
	HPALETTE hpal = dib->GetPalCopy();
	HBITMAP hBMP = dib->GetDDB(dc.m_hDC, hpal);

	if (OpenClipboard())
	{
		BeginWaitCursor();
		EmptyClipboard();
		SetClipboardData (CF_DIB, dib->GetHandleCopy());
		SetClipboardData (CF_BITMAP, hBMP);
		SetClipboardData (CF_PALETTE, hpal);
		CloseClipboard();
		EndWaitCursor();
	}	
}

void CShowartView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	CShowartDoc* pDoc = GetDocument();
	Dib *dib = pDoc->GetDib();
	
	pCmdUI->Enable(dib && *dib);
}

void CShowartView::OnInitialUpdate()
{
	SetScrollSizes();
}				   

void CShowartView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{					 
	//if(stretch)
		SetScrollSizes();
	
	Invalidate(TRUE);
}				   

void CShowartView::OnSize(UINT nType, int cx, int cy)
{	  
	if(Globals::stretch && cx > 0 && cy > 0)
		CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));
		
	CScrollView::OnSize(nType, cx, cy);
}

void 	CShowartView::SetScrollSizes()
{
	if(Globals::stretch)
	{
		CRect r;
		
		GetClientRect(&r);
		
		// just doesn't work!!!
		//SetScaleToFitSize(r.Size()); 
		//SetScaleToFitSize(GetDocument()->GetDocSize());
		
		CScrollView::SetScrollSizes(MM_TEXT, r.Size());
	}                       
	else	
		CScrollView::SetScrollSizes(MM_TEXT, GetDocument()->GetSize());
}



void CShowartView::OnWallpaper() 
{
	Dib* pDib = GetDocument()->GetDib();
	LPCTSTR fname = pDib->GetFileName();
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)fname, SPIF_SENDCHANGE);
}

/**  old version
void CShowartView::OnWallpaper() 
{
	//static char rootName[] = "ShowartWallpaper.bmp";
	//char		dirName[1024];
	Dib* pDib = GetDocument()->GetDib();
	//GetWindowsDirectory(dirName, sizeof(dirName) - lstrlen(rootName) - 2);
	//DWORD WINAPI GetTempPath(  _In_  DWORD  nBufferLength,  _Out_ LPTSTR lpBuffer)

	LPCTSTR dirName = pDib->GetFileName();
	// All versions of Windows >= 7.0 allow jpg files for thew wallpaper name!  No need to create the stupid temporary
	// bmp file any longer!
	// getting the version looks too complicated
	// TODO check version

	if(dirName[lstrlen(dirName) - 1] != '\\')
		lstrcat(dirName, "\\");
	
	lstrcat(dirName, rootName);
	LPCSTR error;
	
	if(Globals::stretch)
	{
		SIZE size = pDib->GetFitToScreenSize();
		LPCTSTR error = pDib->Write(dirName, size.cx, size.cy);

		if(error)
			AfxMessageBox("Can't write");
	}
	else
		pDib->Write(dirName, 90);
	
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)dirName, SPIF_SENDCHANGE);
	//SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, dirName, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	//SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, dirName, SPIF_UPDATEINIFILE);
}
*/


void CShowartView::OnTimer(UINT nIDEvent)
{
	//if(nIDEvent == timerID)
	TheDibLib.Next();
	Invalidate(TRUE); 
    CScrollView::OnTimer(nIDEvent);
}

void CShowartView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_CONTROL || nChar == VK_SHIFT)
	{
		super::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
		
	if(wTimer)
		KillTimer();

	if(nChar == 'A')
		SetTimer();
	if(nChar == VK_ESCAPE)
		GetMainFrame()->ShowFullScreen();
	else
		KeyCommandHandler(nChar, *this);
}

LRESULT CShowartView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_MBUTTONDOWN)
	{
		Globals::stretch = !Globals::stretch;
		SetScrollSizes();
		Invalidate(TRUE);		
	}

	return CScrollView::WindowProc(message, wParam, lParam);
}

CMainFrame* CShowartView::GetMainFrame()
{
	return DYNAMIC_DOWNCAST(CMainFrame, AfxGetApp()->m_pMainWnd);
}

///////////////////////////////////////////////////////////////////////

void CShowartView::OnLButtonDown(UINT nFlags, CPoint point) 
{
#pragma message("Nov 2006 -- Left Mouse != Left Arrow --> This still does not work!  FIX ME!!" __FILE__)
	// August 2006
	// Annoying bug -- LButton isn't acting like VK_LEFT !?!
	KeyCommandHandler(VK_LEFT, *this);
	//OnKeyDown(VK_LEFT, 0, 0);	
	//CScrollView::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////

void CShowartView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	TheDibLib.Skip(-4); // OnLButtonDown already decremented once
	Invalidate();	
}

///////////////////////////////////////////////////////////////////////

void CShowartView::OnRButtonDown(UINT nFlags, CPoint point) 
{
#if 0 
	//-- removed 2/28/03
	CMenu menu;
	menu.LoadMenu(IDR_CONTEXTMENU);
	CMenu* pPopup = menu.GetSubMenu(0);
	
	if(!pPopup)
		return;

	ClientToScreen(&point);
	pPopup->TrackPopupMenu (TPM_LEFTALIGN |
	 TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y,this);
#endif
	// August 2006
	// Annoying bug -- LButton isn't acting like VK_LEFT !?!
	KeyCommandHandler(VK_RIGHT, *this);
	//OnKeyDown(VK_RIGHT, 0, 0);	
	//CScrollView::OnRButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////

void CShowartView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	TheDibLib.Skip(4); // OnRButtonDown already incremented once
	Invalidate();	
}

///////////////////////////////////////////////////////////////////////

