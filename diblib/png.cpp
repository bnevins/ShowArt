#include "stdwin.h"
#include "png.h"
#include "pngdecoder.h"

#define huge


BYTE huge* Png2Dib(LPCTSTR filename)
{
	png thePng(filename);
	return (BYTE*)thePng.getDibBits();
}

png::png(LPCTSTR fname) : filename(fname)
{

	//if (ReadFileIntoMemory())
		//GetDibBits();

	ReadPngFile(filename, &dibData, &dibSize, &imageHeader);
}

void* png::getDibBits() { return dibData; }
DWORD png::getDibSize() { return dibSize;  }



