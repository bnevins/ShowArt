#include "stdwin.h"
#include "png.h"
#include "pngdecoder.h"

png::png(LPCTSTR fname) : filename(fname)
{

	//if (ReadFileIntoMemory())
		//GetDibBits();

	ReadPngFile(filename, &dibData, &dibSize, &imageHeader);
}

void* png::getDibBits() { return dibData; }
DWORD png::getDibSize() { return dibSize;  }

#if 0
BOOL png::ReadFileIntoMemory()
{
	// check file
	HANDLE hFile;
	BOOL ret = FALSE;

	__try
	{
		hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
			__leave;

		filesize = GetFileSize(hFile, NULL);

		if (filesize == INVALID_FILE_SIZE || filesize < 9)
			__leave;

		fileBuffer = (LPBYTE)malloc(filesize);

		if (fileBuffer == NULL)
			__leave;

		DWORD numBytesRead;

		if (!ReadFile(hFile, fileBuffer, filesize, &numBytesRead, NULL))
			__leave;

		if (numBytesRead != filesize)
			__leave;

		ret = TRUE;
	}
	__finally
	{
		if (hFile != INVALID_HANDLE_VALUE)
			CloseHandle(hFile);

		if (!ret && fileBuffer)
			free(fileBuffer);
	}

	return ret;
}
void png::GetDibBits()
{




}
#endif

