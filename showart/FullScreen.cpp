// FullScreen.cpp: implementation of the FullScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "showart.h"
#include "FullScreen.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FullScreen::FullScreen()
{
	isForcedMaximized	= false;
	isFullScreenMode	= false;
	toolbarWasVisible	= false;
	statusbarWasVisible	= false;
	wasZoomed			= false;
}

//////////////////////////////////////////////////////////////////////

bool FullScreen::isFullScreen()
{
	return isFullScreenMode;
}

//////////////////////////////////////////////////////////////////////

void FullScreen::startFullScreen(CMainFrame* pFrame)
{
	if(isFullScreenMode)
		return;

	ASSERT(pFrame);

	
	bool isMDI = false;

	toolbarWasVisible	= pFrame->m_wndToolBar.ShowWindow(SW_HIDE)		? true : false;
	statusbarWasVisible	= pFrame->m_wndStatusBar.ShowWindow(SW_HIDE)	? true : false;
	wasZoomed			= pFrame->IsZoomed()							? true : false;

	CView* pView = pFrame->GetActiveView();

	if(!pView)
	{
		isMDI = true;
		pView = (((CMDIFrameWnd*)pFrame)->MDIGetActive())->GetActiveView();
	}

	if(!pView)
		return;	// DO SOMETHING??!!??

	CRect rect;
	pFrame->GetWindowRect(&rect);

	if((rect.Width()	< ::GetSystemMetrics(SM_CXSCREEN)) ||
		(rect.Height()	< ::GetSystemMetrics(SM_CYSCREEN)))
	{
		pFrame->ShowWindow(SW_SHOWMAXIMIZED);
		isForcedMaximized = true;

		if(isMDI)
		{
			// update all windows...
			MSG msg;

			while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				::AfxGetThread()->PumpMessage();
			}
			//pView->Invalidate();
		}
	}
	//else // frame already fills the screen...


	pView->Invalidate();
	pView->GetWindowRect(rect);
	CPoint tl = rect.TopLeft();
	pView->SetWindowPos(&CWnd::wndTopMost, tl.x, tl.y, rect.Width(), rect.Height(), SWP_SHOWWINDOW);

	isFullScreenMode = true;
}

//////////////////////////////////////////////////////////////////////
#pragma message("Add restore-to-maximized!!!")

void FullScreen::stopFullScreen(CMainFrame* pFrame)
{
	ASSERT(pFrame);

	if(!isFullScreenMode)
		return;

	isFullScreenMode = false;

	if(toolbarWasVisible)
		pFrame->m_wndToolBar.ShowWindow(SW_RESTORE);

	if(statusbarWasVisible)
		pFrame->m_wndStatusBar.ShowWindow(SW_RESTORE);

	if(isForcedMaximized)
	{
		if(wasZoomed)
			pFrame->ShowWindow(SW_MAXIMIZE);
		else
			pFrame->ShowWindow(SW_RESTORE);

		isForcedMaximized	= false;
		wasZoomed			= false;
	}

	CRect rect(-200, -200, 2000, 2000);
	CWnd::GetDesktopWindow()->RedrawWindow(&rect, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE |RDW_UPDATENOW | RDW_ALLCHILDREN);
}
