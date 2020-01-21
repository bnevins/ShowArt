// ShowartUtils.cpp: implementation of the ShowartUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "STDAFX.H"
#include "showart.h"
#include "ShowartUtils.h"
#include <shlobj.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRect ShowartUtils::FitToWindow(const CSize window, CSize dib, BOOL stretch)
{
	if((dib.cx != window.cx || dib.cy != window.cy)  && stretch)
	{
		double xRatio = ((double)window.cx) / ((double)dib.cx);
		double yRatio = ((double)window.cy) / ((double)dib.cy);

		double ratio = xRatio <= yRatio ? xRatio : yRatio;
		
		dib = CSize((int)(((double)dib.cx) * ratio), (int)(((double)dib.cy) * ratio));
	}

	CPoint			origin(0,0);
	
	if(dib.cx < window.cx)
		origin.x = (window.cx - dib.cx) / 2;

	if(dib.cy < window.cy)
		origin.y = (window.cy - dib.cy) / 2;
	
	return CRect(origin, dib);
}

CRect ShowartUtils::FitToWindow(const CSize window, const Dib& dib, BOOL stretch)
{
	return FitToWindow(window, CSize(dib.Width(), dib.Height()), stretch);
}

CRect ShowartUtils::GetFullScreenRect()
{
	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	return CRect(0, 0, cx - 1, cy - 1);
}
CString ShowartUtils::BrowseForFolder()
{
	char buf[_MAX_PATH + 1];
    LPITEMIDLIST pidlSelected = NULL;
    BROWSEINFO bi = {0};
	bi.ulFlags |= BIF_USENEWUI;
	bi.hwndOwner = AfxGetMainWnd()->m_hWnd;
	pidlSelected = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(pidlSelected, buf);
	return buf;
}
