#include "stdafx.h"
#include <DDraw.h>
#include "dib_iter.h"
#include "showart.h"
#include "DirectDrawWin.h"
#include "DDUtil.h"
#include "KeyCommandHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//UINT	DirectDrawWin::classStyle = CS_BYTEALIGNWINDOW | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
//CString DirectDrawWin::className = "";

/////////////////////////////////////////////////////////////////////////////
// DirectDrawWin

LPDIRECTDRAW            lpDD;           // DirectDraw object
LPDIRECTDRAWSURFACE     lpDDSPrimary;   // DirectDraw primary surface
//LPDIRECTDRAWSURFACE     lpDDSBack;       // Offscreen 
LPDIRECTDRAWSURFACE     lpDDSBackBMP;       // Offscreen surface 1 - the mega-sprite
//LPDIRECTDRAWSURFACE     lpDDSCum;       // Offscreen surface 1 - the mega-sprite
//LPDIRECTDRAWSURFACE     lpDDSOne;       // Offscreen surface 1 - the mega-sprite
//LPDIRECTDRAWSURFACE     lpDDSTwo;       // Offscreen surface 2 - CLEAN copy of curr. bkgrnd
//LPDIRECTDRAWSURFACE     lpDDSThree;       // Offscreen surface 3 - copy of curr. bkgrnd + sprite
LPDIRECTDRAWPALETTE     lpDDPal;        // DirectDraw palette



DirectDrawWin::DirectDrawWin(CWnd* pPU, BOOL aStretch, DibLib* aDibLib) 
		: theDibLib(aDibLib), stretch(aStretch), timerID(1007), timerTime(2000)
{														 				  
	ASSERT(theDibLib->Num() > 0);
	screenW			= 640;
	screenH			= 480;
	screenRect		= CRect(0,0,screenW,screenH);
	bitsPerPixel	= 32;
	wTimer			= 0;
	Dib::ShowErrorsInMessageBox() = 1;

	theDib = theDibLib->Curr();

	Create(pPU);	
}			 	   	   	

/////////////////////////////////////////////////////////////////////////////

DirectDrawWin::~DirectDrawWin()
{
	ReleaseCapture();	// probably overkill!!
	KillTimer();

    if( lpDD != NULL )
    {
        if( lpDDSPrimary != NULL )
        {
            lpDDSPrimary->Release();
            lpDDSPrimary = NULL;
        }
        if( lpDDSBackBMP != NULL )
        {		 
            lpDDSBackBMP->Release();
            lpDDSBackBMP = NULL;
        }
		
        if( lpDDPal != NULL )
        {
            lpDDPal->Release();
            lpDDPal = NULL;
        }
        lpDD->Release();
        lpDD = NULL;
    }
}	

/////////////////////////////////////////////////////////////////////////////

BOOL DirectDrawWin::Create(CWnd* pParentWnd)
{											
	ASSERT(pParentWnd != NULL);
	HDC infoDC = ::GetDC(NULL);
	//width = GetDeviceCaps(infoDC, HORZRES);
	//height = GetDeviceCaps(infoDC, VERTRES);
	::ReleaseDC(NULL, infoDC);

	classStyle = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	className = AfxRegisterWndClass(classStyle);

	BOOL ret;

    if(Prefs.debugDisplay)
		ret = CreateEx( WS_EX_TOPMOST, className, "", 			   
				WS_VISIBLE | WS_POPUP | WS_OVERLAPPED | WS_CHILD,
				0, 0, 640, 80,
				pParentWnd->GetSafeHwnd(), NULL, NULL);
	else
		ret = CreateEx( WS_EX_TOPMOST,  className, "", 			   
				WS_VISIBLE | WS_POPUP | WS_OVERLAPPED | WS_CHILD,
				0, 0, 2048,2048,
				pParentWnd->GetSafeHwnd(), NULL, NULL);

	ShowWindow(SW_SHOWNORMAL);
	UpdateWindow();

	if(ret)
		return DDInit() ? TRUE : FALSE;
	else
		return FALSE;
}									 

/////////////////////////////////////////////////////////////////////////////

bool DirectDrawWin::DDInit()
{	
	/*
     * create the main DirectDraw object
     */
    HWND			hwnd	= m_hWnd;
    HRESULT			ddrval;
    DDSURFACEDESC   ddsd;
	if(!hwnd || !::IsWindow(hwnd))
		return false;

	ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
    if( ddrval != DD_OK )
    {
        return false;
    }

    ddrval = lpDD->SetCooperativeLevel( hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
    if( ddrval != DD_OK )
    {
        return false;
    }

    // Set the video mode...
    ddrval = lpDD->SetDisplayMode( screenW, 480, bitsPerPixel);
    if( ddrval != DD_OK )
    {
        return false;
    }

    // Create the primary surface
	memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof( ddsd );
    
	ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    ;

    //ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    //ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    //ddsd.dwBackBufferCount = 1;

    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
    if( ddrval != DD_OK )
    {
		return false;
    }

/*
	DDSCAPS		ddsCaps;
    ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	ddrval = lpDDSPrimary->GetAttachedSurface(&ddsCaps, &lpDDSBack);
    if( ddrval != DD_OK )					   
    {
        return false;
    }
*/	
	DDDraw();
	
	/*
    lpDDPal = DDLoadPalette(lpDD, szCumBMP);

    if (lpDDPal)
    {
        lpDDSPrimary->SetPalette(lpDDPal);
    }
*/
	/*
	lpDDSBackBMP = DDLoadBitmap(lpDD, szBackBMP, 0, 0);
    if( lpDDSBackBMP == NULL )
    {
        return initFail(hwnd);
    }
	lpDDSCum = DDLoadBitmap(lpDD, szCumBMP, 0, 0);
    if( lpDDSCum == NULL )
    {
        return initFail(hwnd);
    }
*/
    // set color key to black
    //DDSetColorKey(lpDDSCum, RGB(0,0,0));

#if 0
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwHeight = 64;
    ddsd.dwWidth = 64;
    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSTwo, NULL );
    if( ddrval != DD_OK )
    {
        return initFail(hwnd);
    }
	// BN
    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSThree, NULL );
    if( ddrval != DD_OK )
    {
        return initFail(hwnd);
    }
	
	DDSCAPS		ddsCaps;
    ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	ddrval = lpDDSPrimary->GetAttachedSurface(&ddsCaps, &lpDDSBack);
    if( ddrval != DD_OK )					   
    {
        return initFail(hwnd);
    }
	//RECT screenRect = { 0,0,640,480 };

//	ddrval = lpDDSBack->BltFast( 0, 0, lpDDSPrimary,
            // &screenRect, DDBLTFAST_NOCOLORKEY);
			
#endif
	return true;
}
////////////////////////////////////////////////////////////////////////////

//BOOL DirectDrawWin::PreCreateWindow(CREATESTRUCT& cs)
//{
	//cs.cx = 640;
	//cs.cy = 480;
	//cs.x = cs.y = 0;
	//cs.style = WS_POPUP | WS_VISIBLE;
	//return 1;
//}

////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(DirectDrawWin, CWnd)
	//{{AFX_MSG_MAP(DirectDrawWin)
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
// DirectDrawWin message handlers

void DirectDrawWin::DDDraw()
{
	if(!theDib)
		return;
	HDC hdc = ::GetDC(m_hWnd);
	DDB theDDB = DDB(theDib->GetDDB(hdc, (HPALETTE)-1));

	if(!theDDB.m_hbmp)
		return;

	lpDDSBackBMP = DDLoadBitmap(lpDD, theDDB.m_hbmp, 0, 0);
	CRect r(0,0,100,100);
	try
	{
		HRESULT ddrval = lpDDSPrimary->BltFast(0, 0, lpDDSBackBMP, r, DDBLTFAST_NOCOLORKEY);
	}
	catch(...)
	{
		int x = 5;
	}
	
	/*ddrval = lpDDSPrimary->Flip( NULL, DDFLIP_WAIT );

	if( ddrval == DDERR_SURFACELOST )
	{
		RestoreAll();
	}
*/
}

void DirectDrawWin::OnPaint()
{
	//DDDraw();
	CWnd::OnPaint();
	return;
/*
	CPaintDC dc(this); // device context for painting
	
	//SetCapture();
	
	if(theDib && *theDib)
		if(stretch)
			theDib->ShowWithPal(dc.m_hDC,0, 0, screenW, screenH, SRCCOPY);
		else
			Draw(dc);
			//theDib->ShowWithPal(dc.m_hDC);
	else
		TRACE0("The view's dib is bad!!! (DirectDrawWin::OnPaint())");
			
   	dc.TextOut(0,0, theDib->Name(), lstrlen(theDib->Name()));
*/
}

/////////////////////////////////////////////////////////////////////////////

void DirectDrawWin::Draw(CPaintDC& dc)
{
	VERIFY(theDib);
	LPCTSTR		dibName = theDib->Name();	
	CRect rect;
	GetClientRect(rect);

	if(!dibName || !*dibName)
	{
		DrawBackground(dc);
		dc.SetTextColor(RGB(255,255,0));
		dc.TextOut( rect.Width() / 2, rect.Height() / 2, "No art file!!");
		return;
	}
	
	Dib& slide = *theDib;

	int			dibW = slide.Width();
	int			dibH = slide.Height();
	int			xOrigin = 0;
	int			yOrigin = 0;

	int			tooBigX = (dibW - 1) / screenW + 1;
	int			tooBigY = (dibH - 1) / screenH + 1;
	int			tooBigXY = tooBigX >= tooBigY ? tooBigX : tooBigY;
	
	if(Prefs.keepAspectRatio && tooBigXY > 1)
	{
		int divFactor = 1;

		if(tooBigXY <= 2)
			divFactor = 2;
		else if(tooBigXY <= 4)
			divFactor = 4;
		else if(tooBigXY <= 8)
			divFactor = 8;
		else if(tooBigXY <= 16)
			divFactor = 16;
		else if(tooBigXY <= 32)
			divFactor = 32;
		else
			divFactor = 1;	// too huge!!

		dibW /= divFactor;
		dibH /= divFactor;
	}
	else if(Prefs.keepAspectRatio)
	{
		int			tooSmallX = screenW / dibW;
		int			tooSmallY = screenH / dibH;
		int			tooSmallXY = tooSmallX <= tooSmallY ? tooSmallX : tooSmallY;

		dibW *= tooSmallXY;
		dibH *= tooSmallXY;
	}

	if(dibW < screenW)
		xOrigin = (screenW - dibW) / 2;

	if(dibH < screenH)
		yOrigin = (screenH - dibH) / 2;
		
	//TRACE("ClientRect: %d, %d\n", rect.Width(), rect.Height());

		DrawBackground(dc);	// just before drawing...
		
		if(tooBigXY)
			slide.ShowWithPal(dc, xOrigin, yOrigin, dibW, dibH);
		else
			slide.ShowWithPal(dc, xOrigin, yOrigin);

#ifdef _DEBUG	
		dc.SetTextColor(RGB(255,255,255));
		dc.SetBkColor(RGB(0,0,0));
		CString msg;
		msg.Format("%s actual:(%d,%d) displayed(%d,%d)", slide.Name(), slide.Width(), slide.Height(), dibW, dibH);
		dc.TextOut(0, 0, msg);
#endif
}

/////////////////////////////////////////////////////////////////////////////


void DirectDrawWin::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
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

int DirectDrawWin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetCapture();
	SetFocus();
	
	return 0;
}

BOOL DirectDrawWin::OnEraseBkgnd(CDC* pDC)
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

void DirectDrawWin::OnTimer(UINT nIDEvent)
{
	//if(nIDEvent == timerID)
	
	theDib = theDibLib->Next();
	Invalidate(TRUE); 
	CWnd::OnTimer(nIDEvent);
}

void DirectDrawWin::DrawBackground(CDC& dc)
{
	CBrush brush(RGB(0,255,255)); 
	CRect rect;
	GetClientRect(rect);
	dc.FillRect(&rect, &brush);
}

void DirectDrawWin::OnDestroy() 
{
	CWnd::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////

void DirectDrawWin::OnLButtonDown(UINT nFlags, CPoint point)
{					  
	OnKeyDown(VK_LEFT, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////


void DirectDrawWin::OnRButtonDown(UINT nFlags, CPoint point) 
{
	OnKeyDown(VK_RIGHT, 0, 0);
	
	//CWnd::OnRButtonDown(nFlags, point);
}

BOOL DirectDrawWin::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void DirectDrawWin::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//Prefs.ToggleKeepAspectRatio();
	Invalidate(TRUE);
	//CWnd::OnMButtonDown(nFlags, point);
}

void DirectDrawWin::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	/*
	ReleaseCapture();
	DestroyWindow();
	*/
	OnKeyDown(VK_ESCAPE, 0, 0);
}


static void finiObjects( void )
{
	//return;

} /* finiObjects */


void DirectDrawWin::RestoreAll()
{
    HRESULT     ddrval;

    ddrval = lpDDSPrimary->Restore();
    
	if( ddrval != DD_OK )
		return;
	
	ddrval = lpDDSBackBMP->Restore();

    if( ddrval != DD_OK )
		return;
	
    //DDReLoadBitmap(lpDDSBackBMP, szBackBMP);

}
