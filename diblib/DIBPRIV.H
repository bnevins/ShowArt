#ifndef __DIBPRIV_H__
#define __DIBPRIV_H__
#else
#error - dibpriv.h already included!!!
#endif

#ifndef __DIB_H__
#error - You must include "dib.h" BEFORE "dibpriv.h"
#endif

#ifndef _INC_WINDOWSX
#include <windowsx.h>
#endif

#define PALVERSION 0x300
#define MAXPALETTE	256
#define TRACE(x)

static char DibAssertBuffer__[2048];

extern HINSTANCE GetDllInstance();

#ifdef _DEBUG
#define ASSERT(f) \
	do \
	{ \
		if (!(f))\
		{\
			wsprintf(DibAssertBuffer__, "Assertion Failure: (" #f "): %s, Line %d", __FILE__, __LINE__);\
			if(IDABORT == MessageBox(NULL, DibAssertBuffer__, "DibLib DLL Error", MB_ABORTRETRYIGNORE))\
			{\
				FatalAppExit(0, "Assertion Failure in DibLib DLL");\
			}\
		}\
	} while (0) \

#define VERIFY(f)          ASSERT(f)
#else
#define ASSERT(f)	((void)0)
#define VERIFY(f)	((void)0)
#endif

#define GIF_SUPPORT
#define JPG_SUPPORT
#define TIFF_SUPPORT
#define PNG_SUPPORT

#ifdef MIN
#undef MIN
#endif

#define MIN(a,b) ((a) < (b) ? (a) : (b))

extern 	   BYTE huge * Gif2Dib(LPCTSTR filename);
extern "C" BYTE huge * Tiff2Dib(LPCTSTR filename);
extern 	   BYTE huge*  Png2Dib(LPCTSTR filename);

#ifdef JPG_SUPPORT
extern "C"
{
#include "jpegext/jpegext.h"
}
#else
extern "C" BYTE huge* Jpg2Dib(LPCTSTR filename, LPCTSTR outName, int  scalingFactor, int showErrorsInMessageBox);
#endif


