#include "jpegextwin.h"

BYTE *AllocWinMem(DWORD dwLen)
{
	return (BYTE  * )GlobalAllocPtr(GMEM_MOVEABLE, dwLen);
}


unsigned int GetNumDisplayBitsPerPixel()
{
	HDC screenDC;
 	int bpp;

   	screenDC = GetDC(NULL);

 	bpp = GetDeviceCaps(screenDC, BITSPIXEL);

 	ReleaseDC(NULL, screenDC);		
	return bpp;
}

void
Win32Throw(void)
{
	RaiseException(0XE0000001,0,0,0);
}

void
Win32ErrorMessage(const char* msg)
{
	MessageBox(GetActiveWindow(), msg, "JPEG Library Error", MB_OK | MB_ICONERROR);
}

void
Win32TraceMessage(const char* msg)
{
#ifdef _DEBUG
  OutputDebugString(msg);
#endif
}