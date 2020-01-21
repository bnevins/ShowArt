//#include "stdafx.h"
//#include <windows.h>
//#include <windowsx.h>
//#include <commctrl.h>
#include "stdwin.h"

#ifdef WIN32
#define huge
#endif
#include "winmem.h"
static HWND 	hwndProgress = NULL;

BYTE huge *AllocWinMem(DWORD dwLen)
{
	return (BYTE huge * )GlobalAllocPtr(GMEM_MOVEABLE, dwLen);
}

///////////////////////////////////////////////////////////////////////

unsigned long GetNumDisplayColors()
{
	HDC screenDC;
 	unsigned long nColors;
 	
   	screenDC = GetDC(NULL);
 	nColors = 1 << GetDeviceCaps(screenDC,BITSPIXEL);
 	ReleaseDC(NULL, screenDC);		
	return nColors;
}

////////////////////////////////////////////

void win_progress_monitor(int numPass, int numPasses, int percent)
{
	/* jpeg sends in pass 1, 2, 2 for some reason!!! 
	 * percents are 1-99 inclusive
	 */
    static int	old_percent = -1;		// just do the percent the FIRST time...
	percent -= 1;	// turn 1 into 0

	if(!hwndProgress || percent % 10 || percent == old_percent)
		return;

	old_percent = percent;
	SendMessage(hwndProgress, PBM_STEPIT, 0, 0);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateProgressWindow(const char* fname)
{
	//WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_STATICEDGE,
	HMODULE		hApp = GetModuleHandle(NULL);
	int			screenW;
	int			screenH;
	int			winW;
	int			winH;
	int			xPos;
	int			yPos;
	char		caption[200];
	const char* rootName = strrchr(fname, '\\');

	if(!hApp)
		return;

	//if(rootName)
		//fname = ++rootName;
	
	screenW = GetSystemMetrics(SM_CXSCREEN);
	screenH = GetSystemMetrics(SM_CYSCREEN);
	winW = screenW / 3;
	winH = screenH / 15;

	xPos = (screenW / 2) - (winW / 2);
	yPos = (screenH / 2) - (winH / 2);
	
	wsprintf(caption, "Loading %s", fname);
	InitCommonControls();

	hwndProgress = CreateWindowEx(
	WS_EX_TOPMOST | WS_EX_STATICEDGE | WS_EX_TOOLWINDOW,
	PROGRESS_CLASS,
	caption,
	WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_BORDER,
	xPos,
	yPos,
	winW,
	winH,
	NULL, // parent HWND...
	NULL,// HMENU...
	hApp,
	NULL // MDI stuff...
	);
	
	if(hwndProgress)
	{
		SendMessage(hwndProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 30)); 
		SendMessage(hwndProgress, PBM_SETSTEP, (WPARAM) 1, 0); 
 
		//ShowWindow(hwndProgress, SW_NORMAL);
		//SendMessage(hwndProgress, PBM_SETPOS, 0, 0);			
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DestroyProgressWindow()
{
	if(hwndProgress)
	    DestroyWindow(hwndProgress); 
	//SendMessage(hwndProgress, WM_CLOSE, 0, 0);
	hwndProgress = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0

   SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, cb / 2048)); 
    SendMessage(hwndPB, PBM_SETSTEP, (WPARAM) 1, 0); 
 
    // Parse the file. 
    pch = (LPCH) LocalAlloc(LPTR, sizeof(char) * 2048); 
    pchTmp = pch; 
    do { 
        ReadFile(hFile, pchTmp, sizeof(char) * 2048, &cb, 
            (LPOVERLAPPED) NULL); 
        . 
        . // Include here any code that parses the file. 
        . 
 
        // Advance the current position of the progress bar 
        // by the increment. 
        SendMessage(hwndPB, PBM_STEPIT, 0, 0); 
    } while (cb); 
    CloseHandle((HANDLE) hFile); 
 
#endif

	//////////////// following 2 funcs MOVED from jerror.c

/*
 * Error exit handler: must not return to caller.
 *
 * Applications may override this if they want to get control back after
 * an error.  Typically one would longjmp somewhere instead of exiting.
 * The setjmp buffer can be made a private field within an expanded error
 * handler object.  Note that the info needed to generate an error message
 * is stored in the error object, so you can generate the message now or
 * later, at your convenience.
 * You should make sure that the JPEG object is cleaned up (with jpeg_abort
 * or jpeg_destroy) at some point.
 */

void
Win32Throw(void)
{
	DestroyProgressWindow();
	RaiseException(0XE0000001,0,0,0);
}


/*
 * Actual output of an error or trace message.
 * Applications may override this method to send JPEG messages somewhere
 * other than stderr.
 */

void
Win32ErrorMessage(const char* msg)
{
  	DestroyProgressWindow();
	MessageBox(NULL, msg, "JPEG Library Fatal Error", MB_ICONEXCLAMATION);
}

void
Win32TraceMessage(const char* msg)
{
#ifdef _DEBUG
  OutputDebugString(msg);
#endif
}

