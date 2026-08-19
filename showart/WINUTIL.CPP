
#include "STDAFX.H"
#include "WinUtil.h"
#pragma comment(lib, "Windowscodecs.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//#include <windows.h>
//#include <wincodec.h>
//#include <atlstr.h>
//#include <iostream>



/**
 * LoadImageToDIB
 * * Decodes an image file (HEIC, PNG, JPG, etc.) using WIC and returns a
 * Global Memory handle to a Packed DIB (BITMAPINFOHEADER + Pixel Data).
 * * @param strPath: Full filesystem path to the image file.
 * @return: HGLOBAL containing the packed DIB, or NULL on failure.
 */
HGLOBAL LoadImageToDIB(const CString& strPath) {
	HRESULT hr = S_OK;
	IWICImagingFactory* pFactory = NULL;
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pFrame = NULL;
	IWICFormatConverter* pConverter = NULL;

	HGLOBAL hDib = NULL;

	// 1. Initialize WIC Factory
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pFactory)
	);

	if (FAILED(hr)) return NULL;

	// FIX for Error E0413: 
	// Ensure the path is converted to a wide string (LPCWSTR) which WIC strictly 
	// requires, bypassing the issue in legacy MBCS/Multi-Byte MFC projects.
	CStringW wstrPath(strPath);

	// 2. Create decoder from the file path
	// WIC automatically detects the format (PNG, HEIC, etc.) via the file header
	hr = pFactory->CreateDecoderFromFilename(
		wstrPath,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&pDecoder
	);

	if (SUCCEEDED(hr)) {
		hr = pDecoder->GetFrame(0, &pFrame);
	}

	if (SUCCEEDED(hr)) {
		// 3. Convert image to a standard DIB-compatible pixel format (32bpp BGR)
		hr = pFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr)) {
		hr = pConverter->Initialize(
			pFrame,
			GUID_WICPixelFormat32bppBGR, // Standard 32-bit DIB format
			WICBitmapDitherTypeNone,
			NULL,
			0.0f,
			WICBitmapPaletteTypeCustom
		);
	}

	if (SUCCEEDED(hr)) {
		UINT width, height;
		pConverter->GetSize(&width, &height);

		// 4. Calculate buffer size for Packed DIB
		// Stride must be DWORD aligned for DIBs, but 32bpp is naturally aligned
		UINT stride = width * 4;
		UINT imageSize = stride * height;
		UINT totalSize = sizeof(BITMAPINFOHEADER) + imageSize;

		// 5. Allocate Global Memory
		hDib = GlobalAlloc(GHND, totalSize);
		if (hDib) {
			LPBYTE pData = (LPBYTE)GlobalLock(hDib);
			if (pData) {
				// 6. Setup BITMAPINFOHEADER
				PBITMAPINFOHEADER pbi = (PBITMAPINFOHEADER)pData;
				pbi->biSize = sizeof(BITMAPINFOHEADER);
				pbi->biWidth = width;
				pbi->biHeight = -(LONG)height; // Negative for top-down DIB
				pbi->biPlanes = 1;
				pbi->biBitCount = 32;
				pbi->biCompression = BI_RGB;
				pbi->biSizeImage = imageSize;

				// 7. Copy pixels directly into the buffer after the header
				hr = pConverter->CopyPixels(
					NULL,
					stride,
					imageSize,
					pData + sizeof(BITMAPINFOHEADER)
				);

				GlobalUnlock(hDib);

				if (FAILED(hr)) {
					GlobalFree(hDib);
					hDib = NULL;
				}
			}
		}
	}

	// Cleanup WIC resources
	if (pConverter) pConverter->Release();
	if (pFrame) pFrame->Release();
	if (pDecoder) pDecoder->Release();
	if (pFactory) pFactory->Release();

	return hDib;
}

/* Usage Example:
--------------
void DisplayImage(CString path) {
	HGLOBAL hDib = LoadImageToDIB(path);
	if (hDib) {
		LPVOID pBuffer = GlobalLock(hDib);
		PBITMAPINFOHEADER pbi = (PBITMAPINFOHEADER)pBuffer;
		LPVOID pPixels = (LPBYTE)pBuffer + pbi->biSize;

		// Use with StretchDIBits or CreateDIBitmap
		// ...

		GlobalUnlock(hDib);
		GlobalFree(hDib);
	}
}
*/
void ListFilesInDirectory(LPCTSTR dirName, CStringArray& filepaths )
{
	// BUG ALERT FIXME  Instead of *.* -- get bmp,tif,jpg,png, etc
	// FIXME

	ASSERT( dirName != NULL );
	CFileFind finder;
	CString wildcard( dirName );
	wildcard += _T("\\*.*");
	BOOL working = finder.FindFile( wildcard );
	while ( working )
	{
		working = finder.FindNextFile();

		if ( finder.IsDots() ) {
			continue;
		}

		// Skip sub-directories

		if ( finder.IsDirectory() )
			continue;

		filepaths.Add( finder.GetFilePath() );
	}

// Cleanup file finder

finder.Close();

}

 
void MondoCreateDirectory(const CString& destDir)	// throws CString
{
	// see if it's already there -- this is the usual case so let's 
	// opptimize the code around doing this part fast!

	 if(destDir.IsEmpty())
		throw CString("Directory name is empty");

	FileStatus fs(destDir);

	if(fs.Exists())
	{
		if(fs.IsDirectory())
		{
			// all OK -- file exists and it IS a directory...
			return;
		}
		else
		{
			// Uh-oh -- it must be a file with the name of the desired directory!!
			CString msg;
			msg.Format( "I can't create the directory: %s because a file already exists with that name.\n\n"
						"Shall I delete the file so that I can create the directory?", destDir);
			
			if(IDNO == AfxMessageBox(msg, MB_YESNO))
				throw CString("");

			_unlink(destDir);
		}	
	}

	// FIX THIS!!!!
	if(destDir.Find("..") > 0)	// create dir will freak...
		throw CString("\"..\" for the parent directory isn't supported.  Use a full path name instead.");

	if(::CreateDirectory(destDir, NULL))
	{
		// that was easy!  It was ready to be created!!
		return;
	}
	
	// OK -- we must need to create the directories upstream...

	int index = destDir.ReverseFind('\\');

	if(index < 0)
	{
		// this means we have a  fullpath name like "foo"
		// we're here so that means it couldn't be created.
		ShowLastError();
		throw CString("");
		//return false;
	}
	
	CString subDestDir = destDir.Left(index);

	MondoCreateDirectory(subDestDir);	// may throw...
	MondoCreateDirectory(destDir);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

BOOL IsDir(const CString& name)
{
	FileStatus fs(name);

	if(fs.Exists() && fs.IsDirectory())
		return TRUE;
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////

void ShowLastError(LPCTSTR extra)
{
	LPVOID lpMsgBuf;
	
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	CString msg;
	
	if(extra)
		msg.Format("%s:  %s", extra, lpMsgBuf);
	else
		msg = (LPCTSTR)lpMsgBuf;

	::MessageBox(NULL, msg, "Error", MB_OK | MB_ICONINFORMATION );
	LocalFree( lpMsgBuf );
} 


//////////////////////////////////////////////////////////////////////////
//// Clipboard support

//---------------------------------------------------------------------
//
// Function:   CopyHandle (from SDK DibView sample clipbrd.c)
//
// Purpose:    Makes a copy of the given global memory block.  Returns
//             a handle to the new memory block (NULL on error).
//
//             Routine stolen verbatim out of ShowDIB.
//
// Parms:      h == Handle to global memory to duplicate.
//
// Returns:    Handle to new global memory block.
//
//---------------------------------------------------------------------

HANDLE CopyHandle (HANDLE h)
{	   
	BYTE huge *lpCopy;
	BYTE huge *lp;
	HANDLE     hCopy;
	DWORD      dwLen;

	if (h == NULL)
		return NULL;

	dwLen = ::GlobalSize((HGLOBAL) h);

	if ((hCopy = (HANDLE) ::GlobalAlloc (GHND, dwLen)) != NULL)
	{
		lpCopy = (BYTE huge *) ::GlobalLock((HGLOBAL) hCopy);
		lp     = (BYTE huge *) ::GlobalLock((HGLOBAL) h);

		while (dwLen--)
			*lpCopy++ = *lp++;

		::GlobalUnlock((HGLOBAL) hCopy);
		::GlobalUnlock((HGLOBAL) h);
	}

	return hCopy;
}


/////////////////////////////////////////////////////////////////////////////////////////////////

BOOL Exists(LPCTSTR path) {
	CFileStatus status;

	return CFile::GetStatus( path, status );
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CString NextFileName(CString&  path) {
	int len = path.GetLength();
	int lenofname = path.ReverseFind('.');

	if(lenofname < 0) // WEIRD -- no extension!!
		return path;

	int lenofextanddot = len - lenofname;
	
	for(int i = 1; i < 1000; i++) {
		CString ret;
		ret.Format("%s(%d)%s", path.Left(lenofname), i, path.Right(lenofextanddot));

		if(!Exists(ret))
			return ret;
	}

}
