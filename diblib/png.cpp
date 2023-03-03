#include "stdwin.h"
#include "png.h"
#include "pngdecoder.h"

BYTE huge* Png2Dib(LPCTSTR filename)
{
	void* dibData = NULL;
	DWORD dibSize = 0;
	PNG_IMAGE_HEADER imageHeader;

	ReadPngFile(filename, &dibData, &dibSize, &imageHeader);
	return (BYTE*)dibData;
}



