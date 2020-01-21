// fullscre.cpp : implementation file
//

#include "stdafx.h"
#include "dib_iter.h"
#include "showart.h"
#include "fullscre.h"
#include "KeyCommandHandler.h"
#include "ShowartUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//UINT	FullScreenWin::classStyle = CS_BYTEALIGNWINDOW | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
//CString FullScreenWin::className = "";

/////////////////////////////////////////////////////////////////////////////
// FullScreenWin

FullScreenWin::FullScreenWin(CWnd* pPU, BOOL aStretch, DibLib* aDibLib) 
		: theDibLib(aDibLib), stretch(aStretch), timerID(1007), timerTime(2000)
{														 				  
	ASSERT(theDibLib->Num() > 0);
	wTimer = 0;
	//Dib::ShowErrorsInMessageBox() = 1;

	Create(pPU);	
	theDib = theDibLib->Curr();
}			 	   	   	

/////////////////////////////////////////////////////////////////////////////

FullScreenWin::~FullScreenWin()
{
	ReleaseCapture();	// probably overkill!!
	KillTimer();
}	

/////////////////////////////////////////////////////////////////////////////

BOOL FullScreenWin::Create(CWnd* pParentWnd)
{											
	ASSERT(pParentWnd != NULL);
	HDC infoDC = ::GetDC(NULL);
	width = GetDeviceCaps(infoDC, HORZRES);
	height = GetDeviceCaps(infoDC, VERTRES);
	::ReleaseDC(NULL, infoDC);

	classStyle = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	className = AfxRegisterWndClass(classStyle);

    if(Prefs.debugDisplay)
		return CreateEx( WS_EX_TOPMOST, className, "", 			   
				WS_VISIBLE | WS_POPUP | WS_OVERLAPPED | WS_CHILD,
				0, 0, 640, 80,
				pParentWnd->GetSafeHwnd(), NULL, NULL);
	else
		return CreateEx( WS_EX_TOPMOST, className, "", 			   
		//return CreateEx( WS_EX_TOPMOST, AFX_WNDFRAMEORVIEW, "", 			   
				WS_VISIBLE | WS_POPUP | WS_OVERLAPPED | WS_CHILD,
				0, 0, width, height,
				pParentWnd->GetSafeHwnd(), NULL, NULL);
}									 

/////////////////////////////////////////////////////////////////////////////

//BOOL FullScreenWin::PreCreateWindow(CREATESTRUCT& cs)
//{
	//cs.cx = 640;
	//cs.cy = 480;
	//cs.x = cs.y = 0;
	//cs.style = WS_POPUP | WS_VISIBLE;
	//return 1;
//}

////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(FullScreenWin, CWnd)
	//{{AFX_MSG_MAP(FullScreenWin)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FullScreenWin message handlers


void FullScreenWin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Draw(dc);
   	dc.TextOut(0,0, theDib->Name(), lstrlen(theDib->Name()));
}

/////////////////////////////////////////////////////////////////////////////

void FullScreenWin::Draw(CPaintDC& dc)
{
	VERIFY(theDib);

	LPCTSTR			dibName = theDib->Name();	

	if(!dibName || !*dibName)
	{
		CRect rect;
		GetClientRect(rect);
		DrawBackground(dc);
		dc.SetTextColor(RGB(255,255,0));
		dc.TextOut( rect.Width() / 2, rect.Height() / 2, "No art file!!");
		return;
	}
	
	Dib&			slide = *theDib;
	const	CSize	screen(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
	CSize			dibSize(slide.Width(), slide.Height());
	
	CRect	dibRect	= ShowartUtils::FitToWindow(screen, dibSize, stretch);
	CPoint	origin	= dibRect.TopLeft();
	DrawBackground(dc);	// just before drawing...
	slide.ShowWithPal(dc, origin.x, origin.y, dibRect.Width(), dibRect.Height());			

#ifdef _DEBUG	
		dc.SetTextColor(RGB(255,255,255));
		dc.SetBkColor(RGB(0,0,0));
		CString msg;
		msg.Format("%s actual:(%d,%d) displayed(%d,%d)", slide.Name(), slide.Width(), slide.Height(), dibRect.Width(), dibRect.Height());
		dc.TextOut(0, 0, msg);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void FullScreenWin::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	else if(::IsWindow(m_hWnd))
	{
		if(nChar == 6)	// ^F
			nChar = VK_ESCAPE;

		KeyCommandHandler(nChar, *this, &theDib);
	}
}        

/////////////////////////////////////////////////////////////////////////////

int FullScreenWin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetCapture();
	SetFocus();
	
	return 0;
}

BOOL FullScreenWin::OnEraseBkgnd(CDC* pDC)
{
/*
	CDC& dc = *pDC;
	
	CBrush br(RGB(0,0,0));
	br.UnrealizeObject();
	CBrush *oldBr = dc.SelectObject(&br);
	dc.Rectangle(0,0,width, height);
	dc.SelectObject(oldBr);
*/
	return 1;
}

void FullScreenWin::OnTimer(UINT nIDEvent)
{
	//if(nIDEvent == timerID)
	
	theDib = theDibLib->Next();
	Invalidate(TRUE); 
	CWnd::OnTimer(nIDEvent);
}

void FullScreenWin::DrawBackground(CDC& dc)
{
	CBrush brush(RGB(0,0,0)); 
	CRect rect;
	GetClientRect(rect);
	dc.FillRect(&rect, &brush);
}

void FullScreenWin::OnDestroy() 
{
	CWnd::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////

void FullScreenWin::OnLButtonDown(UINT nFlags, CPoint point)
{					  
	OnKeyDown(VK_LEFT, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////


void FullScreenWin::OnRButtonDown(UINT nFlags, CPoint point) 
{
	OnKeyDown(VK_RIGHT, 0, 0);
	
	//CWnd::OnRButtonDown(nFlags, point);
}

BOOL FullScreenWin::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void FullScreenWin::OnMButtonDown(UINT nFlags, CPoint point) 
{
	stretch = !stretch;
	Invalidate(TRUE);
}

void FullScreenWin::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	/*
	ReleaseCapture();
	DestroyWindow();
	*/
	OnKeyDown(VK_ESCAPE, 0, 0);
}


