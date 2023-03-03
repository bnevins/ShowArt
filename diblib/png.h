#pragma once
#include "pngcmn.h"

class png
{
public:
	png(LPCTSTR fname);
	void* getDibBits();
	DWORD getDibSize();
private:
	//BOOL ReadFileIntoMemory();
	LPCTSTR filename;
	void* dibData = NULL;
	DWORD dibSize;
	//LPBYTE fileBuffer = NULL;
	//DWORD filesize;
	PNG_IMAGE_HEADER imageHeader;
};

