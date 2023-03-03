#include "stdwin.h"

//#include <windows.h>
//#include <assert.h>
//#include <strstrea.h>
#include "stdio.h"
#include "resource.h"
#include "dib.h" 
#include "dibpriv.h"
#include "dib.inl"
#include "JpegRotater.h"
#include "utils.h"
#include "JpegWriter.h"
#include "png.h"
//#include <gdiplus.h>

//using namespace Gdiplus;

//#ifndef _DLL
//#error Setup for use as DLL only!!!!
//#endif


#ifndef TIFF_SUPPORT
extern "C" BYTE huge * Tiff2Dib(LPCTSTR fname)
{
	return NULL;
}
#endif

#ifndef GIF_SUPPORT
// otherwise link in gif.obj .....
BYTE huge* Gif2Dib(LPCTSTR filename)
{			
	 return NULL;
}	 
#endif

#ifndef JPG_SUPPORT
// otherwise link in jpeg library...
extern "C" BYTE huge* Jpg2Dib(LPCTSTR filename, LPCTSTR outName, double scalingFactor, int showErrorsInMessageBox)
{			
	 return NULL;
}	 
#endif

#define Delay(x)
#if 0
void	Delay(int msecs)
{
	DWORD	start = GetTickCount();
	DWORD	end = start + msecs;
	
	while(GetTickCount() < end)
		;
}	
#endif


// FOO FOO FOO
BYTE huge* testPNG(LPCTSTR fname) {

	png thePng(fname);
	return (BYTE*)thePng.getDibBits();
}


int CArt::showErrorsInMessageBox = FALSE; 


////////////////////////////////////////////////////////////////////////
//  CArt Member Functions...
////////////////////////////////////////////////////////////////////////

CArt::CArt()
{	  	 
	width =		0; 
	height =	0;
	ok = 		0;	   
	hwndOwner = NULL;
}			 

////////////////////////////////////////////////////////////////////////

void* CArt::operator new( size_t nSize )
{
	return ::new BYTE[nSize];
}

////////////////////////////////////////////////////////////////////////////

void CArt::operator delete( void* p )
{	  	 
	::delete[] p;
}

////////////////////////////////////////////////////////////////////////////

void	CArt::Fill(HDC hdc, POINT oldp, POINT newp)
{					   			   			  
	// fill in area not covered by current bmp placement...
											   
	RECT	rcFill;
	int 	leftToRight = 0;
				   	   	  
	if(oldp.x == newp.x && oldp.y == newp.y)
		return;
		
		//
		// 1. do the rect on the SIDE
		//
	rcFill.top    = oldp.y;
	rcFill.bottom = oldp.y + Height();
	
	if(newp.x >= oldp.x)			// left to right
	{
		rcFill.left   = oldp.x; 
		rcFill.right  = newp.x;	
		leftToRight = 1;
	}	
	else								// right to left...
	{
		rcFill.left   = newp.x + Width();
		rcFill.right  = oldp.x + Width();
	}		
	
	FillRect(hdc, &rcFill, (HBRUSH)GetStockObject(BLACK_BRUSH));	
		//
		// 2. do top OR bottom rect...
		//
	if(leftToRight)	
		rcFill.right = rcFill.left + Width();        // Top or bottom bits
	else
		rcFill.left = rcFill.right - Width();	
	
	if( newp.y > oldp.y )		 	 
	{		   
		rcFill.bottom = newp.y;                       // Top bits
	}					
	else
	{
		rcFill.top = newp.y + Height();            // Bottom bits
		rcFill.bottom = oldp.y + Height();
	}							  
	FillRect(hdc, &rcFill, (HBRUSH)GetStockObject(BLACK_BRUSH));
}

SIZE CArt::GetFitToScreenSize()
{
	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

	int dw = Width();
	int dh = Height();
	int sw = rect.right;
	int sh = rect.bottom;
	
	SIZE ret;
	ret.cx = dw;
	ret.cy = dh;

	if(dw == sw && dh == sh)
		return ret;

	double xRatio = ((double)sw) / ((double)dw);
	double yRatio = ((double)sh) / ((double)dh);

	double ratio = xRatio <= yRatio ? xRatio : yRatio;
	
	ret.cx = (int)(((double)dw) * ratio);
	ret.cy = (int)(((double)dh) * ratio);
	return ret;
}

////////////////////////////////////////////////////////////////////////////

/* static */ 
int&	CArt::ShowErrorsInMessageBox() 
{ 
		return showErrorsInMessageBox;
}

////////////////////////////////////////////////////////////////////////
//  Dib Member Functions...
////////////////////////////////////////////////////////////////////////

double Dib::scalingFactor = 1.0;

ArtType Dib::GetType()
{	 	 
	// nothing fancy -- just look at the filename extension...

	LPCTSTR fname = GetFileName();

	if(!fname)
		return unknown;

	LPCTSTR ext = strrchr(fname, '.');

	if(!ext || ext - fname + 1 >= lstrlen(fname))
		return unknown;

	++ext;	// now it's SAFELY pointed at first char of extension
	 		   					   		   	 
	if(!lstrcmpi(ext, "DIB")  || !lstrcmpi(ext, "BMP"))
	   	return bmp;
	   			
	else if(!lstrcmpi(ext, "TIF"))
		return tiff;
				 
	else if(!lstrcmpi(ext, "GIF"))
		return gif;
				
	else if(!lstrcmpi(ext, "JPG") || !lstrcmpi(ext, "JPEG"))
		return jpeg;

	return unknown;
}

////////////////////////////////////////////////////////////////////////

LPCTSTR Dib::GetFileName()
{	 	 
	return fileName;
}

////////////////////////////////////////////////////////////////////////

void Dib::ctorBefore()
{	 	 
	// common code for ALL the constructors...
	lpDib 		= NULL;
	lpDibBits	= NULL;
	hRes 		= NULL; 
	fileName 	= NULL;	
	oldPal		= NULL;
	newPal		= NULL;
	OK(0);
	Width(0);
	Height(0);
}

////////////////////////////////////////////////////////////////////////////

void Dib::ctorAfter()
{	 	 	  
 	// only do once!
	VERIFY(newPal == NULL);
	VERIFY(oldPal == NULL);
	VERIFY(lpDibBits == NULL);
	VERIFY(Width() == 0);
	VERIFY(Height() == 0);
	VERIFY(OK() == 0);
	
	// Now that we have lpDib (or not!) -- continue construction...
	
	if(!lpDib)
	{
		OK(0);
		return;
	}	
	
	OK(1);
	setWidthAndHeight();
	setDibBitsAddr();
	newPal = Palette();
	oldPal = newPal;
}

////////////////////////////////////////////////////////////////////////////

Dib::Dib()
{		 
	ctorBefore();
	ctorAfter();
}					   

/* ====================================================================== */

Dib::Dib(LPCTSTR filename)
{		 			 
	ctorBefore();
	Read(filename);
}
/////////////////////////////////////////////////////////////////////////

void	Dib::Read(LPCTSTR szFileName)
{			 	
	int len = lstrlen(szFileName);
					  
	if(len < 5)	 
		return;
		
	fileName = new CHAR[len + 1];
	lstrcpy(fileName, szFileName); 
					  
	HCURSOR oldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	
	ArtType type = GetType();

	try
	{
		switch(type)
		{
		case jpeg:
			Jpg2Dib(fileName);	break;
		case bmp:
	   		Bmp2Dib(fileName);	break;			 				 	  
		case tiff:	
			lpDib = testPNG(fileName); break;
			//Tiff2Dib(fileName);	break;
		case gif:				 
			Gif2Dib(fileName);	break;
		default:
			return; // FIXME ?????
		}
	}
	catch(...)
	{
		// IMPORTANT!!!  
		// This try/vatch used to be in dib.inl in Jpg2Dib() -- an inline
		// method.  Which doesn't work AT ALL in the release version!
		// Need to keep try/catch in non-inline code!

		TRACE("Caught an Exception");

	}

	ctorAfter();
	SetCursor(oldCursor);
}

////////////////////////////////////////////////////////////////////////////

Dib::Dib(HGLOBAL hDib)
{		 
	// copy global-memory -- caller has to clean-up hDib...
	
	ctorBefore();
	CopyHandle(hDib);	// sets lpDib...
	ctorAfter();
}	
	
////////////////////////////////////////////////////////////////////

Dib::Dib(HINSTANCE hInstance, int res_num)
{	 								 	  	   			   	
	ctorBefore();
	
	HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(res_num), TEXT( "DIB" ));  // unicode

	if(hResource)
	{
		hRes = LoadResource(hInstance, hResource);
		
		if(hRes)
		{
			lpDib = (BYTE huge *)LockResource(hRes);
			
			if(lpDib)
				lpDib += sizeof(BITMAPFILEHEADER);
		}
	}	
	
	ctorAfter();
}	
	
////////////////////////////////////////////////////////////////////////////

Dib::~Dib()
{
	ClearAll();
}

////////////////////////////////////////////////////////////////////////////

void Dib::ClearAll()
{
	if(hRes)
	{  	
		// loaded as a resource -- don't free lpDib !!!
		if(lpDib)
			UnlockResource(hRes);
		FreeResource(hRes);
	}				  
	else if(lpDib)
		Free();
			
	if(newPal)
		DeleteObject(newPal);	
	
	if(fileName)
		delete[] fileName;
		
	ctorBefore();	
}	                

/* ====================================================================== */

void Dib::setWidthAndHeight()
{	 	  	   		 
	Width(0); 
	Height(0);
	
	VERIFY(lpDib);
			   
     if (HeaderSize() == sizeof (BITMAPCOREHEADER))
     {
		  Width( (WORD) (((BITMAPCOREHEADER huge *) lpDib)->bcWidth) );
		  Height( (WORD) (((BITMAPCOREHEADER huge *) lpDib)->bcHeight) );
	 }	  	  
     else 		  
     {
		  Width( (WORD) (((BITMAPINFOHEADER huge *) lpDib)->biWidth) );
		  Height( (WORD) (((BITMAPINFOHEADER huge *) lpDib)->biHeight) );
	 }	  	  
}		  														

////////////////////////////////////////////////////////////////////////

void	Dib::ShowWithPal_(HDC hdc, int x, int y, int w, int h, DWORD rop)
{														   
	BOOL ownDC = FALSE;
	
	if(hdc == NULL)
	{
		ownDC = TRUE;
		VERIFY(::IsWindow(Owner()));		// you HAVE to send in a real HDC if no owner window attached...
		hdc = ::GetDC(Owner());	// could be NULL !!!
	}
	
	if(!newPal)
		Show(hdc, x, y, w, h, rop);
	
	else
	{	
		Realize(hdc);
		Show(hdc, x, y, w, h, rop);
		UnRealize(hdc);	 	
	}	
	
	if(ownDC)
		::ReleaseDC(Owner(), hdc);
}

////////////////////////////////////////////////////////////////////////

void	Dib::Show_(HDC hdc, int x, int y, int w, int h, DWORD rop)
{				  							 		 
	BOOL ownDC = FALSE;
	
	if(hdc == NULL)
	{
		ownDC = TRUE;
		hdc = ::GetDC(Owner());	// could be NULL !!!
	}
	
	if (hdc && OK() && lpDib)
	{	
		SetStretchBltMode (hdc, COLORONCOLOR) ;
		
		if(w && h)	// stretching
		   	StretchDIBits (hdc, x, y, w, h,
				0, 0, Width(), Height(),   	 
				/* (LPSTR) */ lpDibBits,
				(LPBITMAPINFO) lpDib,
				newPal ? DIB_RGB_COLORS : DIB_PAL_COLORS, rop) ;
		else					
		   	SetDIBitsToDevice (hdc, x, y, Width(), Height(), 0, 0,
				0, Height(), (LPTSTR) lpDibBits,  // unicode
				(LPBITMAPINFO) lpDib,
				newPal ? DIB_RGB_COLORS : DIB_PAL_COLORS) ;
				//DIB_PAL_COLORS) ;
	}
	
	if(ownDC)
		::ReleaseDC(Owner(), hdc);
}

////////////////////////////////////////////////////////////////////////

UINT	Dib::Realize(HDC hdc, int background)
{			 		 
	if(!newPal) 
		return FALSE;			// 16 or 24 bit bmp
		
	oldPal = SelectPalette(hdc, newPal, background);
	return RealizePalette(hdc);			   	 				
}

////////////////////////////////////////////////////////////////////////

void	Dib::UnRealize(HDC hdc)
{			 		 
	if(!newPal) 
		return;			// 16 or 24 bit bmp
		
	if(oldPal)
		SelectPalette(hdc, oldPal, TRUE);
								   
	oldPal = 0;
}

////////////////////////////////////////////////////////////////////////

BOOL Dib::OnPalette(HDC hdc, int background)
{
	// private function!!!
	// return TRUE if the dib needs repainting - caller must arrange it...
														
	if(!newPal) 
		return FALSE;			// 16 or 24 bit bmp
		
	int iColorsChanged = Realize(hdc, background);
  	UnRealize(hdc);					  			   
	
	if(iColorsChanged)
		return TRUE;
		
	return FALSE;	
}					

/* ====================================================================== */

void Dib::setDibBitsAddr()
{	 	  				 
	VERIFY(lpDib);
			  
     DWORD dwNumColors, dwColorTableSize ;
     WORD  wBitCount ;

     if (HeaderSize() == sizeof (BITMAPCOREHEADER))
	  {
	  wBitCount = ((BITMAPCOREHEADER huge *) lpDib)->bcBitCount ;

	  if (wBitCount != 24)
	       dwNumColors = 1L << wBitCount ;
	  else
	       dwNumColors = 0 ;

	  dwColorTableSize = dwNumColors * sizeof (RGBTRIPLE) ;
	  }
     else
	  {
	  wBitCount = ((BITMAPINFOHEADER huge *) lpDib)->biBitCount ;

	  if (HeaderSize() >= 36)
	       dwNumColors = ((BITMAPINFOHEADER huge *) lpDib)->biClrUsed ;
	  else
	       dwNumColors = 0 ;

	  if (dwNumColors == 0)
	       {
	       if (wBitCount != 24)
		    dwNumColors = 1L << wBitCount ;
	       else
		    dwNumColors = 0 ;
	       }

	  dwColorTableSize = dwNumColors * sizeof (RGBQUAD) ;
	  }

     lpDibBits = lpDib + HeaderSize() + dwColorTableSize ;
}

/* ====================================================================== */

void	Dib::Bmp2Dib(LPCTSTR szFileName)
{			 	
     BITMAPFILEHEADER bmfh ;
     DWORD            dwDibSize, dwOffset, dwHeaderSize ;
     int              hFile ;
     WORD             wDibRead ;

	 //OK(0);
	 	
     if (HFILE_ERROR == (hFile = _lopen (szFileName, OF_READ | OF_SHARE_DENY_WRITE)))
	     return;


     if (_lread (hFile, (LPSTR) &bmfh, sizeof (BITMAPFILEHEADER)) != 
				       sizeof (BITMAPFILEHEADER))
	  {
	  _lclose (hFile) ;
	  return ;
	  }		 

     if (bmfh.bfType != * (WORD *) "BM" )
	  {							   		 
	  _lclose (hFile) ;
	  return ;
	  }		 

     dwDibSize = bmfh.bfSize - sizeof (BITMAPFILEHEADER) ;
	 lpDib = NULL;
#ifdef _DEBUG
	//TRACE("bfSize = %ul\n", bmfh.bfSize);
#endif

//#if defined(WIN32) || defined(_WIN32)
     //lpDib = new BYTE[dwDibSize]; 
//#else     
     lpDib = (BYTE huge * ) GlobalAllocPtr (GMEM_MOVEABLE | GMEM_SHARE, dwDibSize) ;
//#endif
     if (lpDib == NULL)
	  {
	  _lclose (hFile) ;
	  return ;
	  }		 

     dwOffset = 0 ;

     while (dwDibSize > 0)
	  {
	  wDibRead = (WORD) MIN (32768ul, dwDibSize) ;

	  if (wDibRead != _lread (hFile, (LPSTR) (lpDib + dwOffset), wDibRead))
	       {
	       _lclose (hFile) ;
			Free();
	       return ;
	       }	  

	  dwDibSize -= wDibRead ;
	  dwOffset  += wDibRead ;
	  }

     _lclose (hFile) ;

     dwHeaderSize = HeaderSize() ;

     if (dwHeaderSize < 12 || (dwHeaderSize > 12 && dwHeaderSize < 16))
	  {
	  Free();
	  return ;
	  }
	  //OK(1);
}

/* ====================================================================== */

LPCTSTR Dib::Write(LPCTSTR szFileName, int w, int h, LPCTSTR label)
{			 	
	HBITMAP hbm = GetDDB(w, h, label);
	DDB ddb(hbm);
	return ddb.Write(szFileName);	// returns NULL or an error string...
}

/* ====================================================================== */

BOOL Dib::Write(LPCTSTR szFileName, int jpegQuality)
{	
	try
	{
		if(isBMP(szFileName))
		{
			WriteBMP(szFileName);
		}
		else if(isJPEG(szFileName))
		{
			WriteJpg(szFileName, jpegQuality);
		}
		else
		{
			// default -- write out a jpeg
			WriteJpg(szFileName, jpegQuality);
		}
	}
	catch(...)
	{
		// Error message was shown to user already.
		// if the problem was a read-only file -- this won't delete it...
		_unlink(szFileName);	
		return FALSE;
	}
	return TRUE;
}

/* ====================================================================== */

void	Dib::WriteBMP(LPCTSTR outfilename)
{			 	
	// ANY error -- throw an int...
	HFILE hFile;

	if(HFILE_ERROR == (hFile = _lcreat(outfilename, 0)))
	{
		Win32ErrorMessage("Error creating file");
		throw 1;
	}



	BITMAPFILEHEADER  bfh;
	
	bfh.bfType = MAKEWORD('B', 'M');
	bfh.bfOffBits = (lpDibBits - lpDib) + sizeof(BITMAPFILEHEADER);
	bfh.bfSize = ImageSize() + bfh.bfOffBits; 
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	
	BITMAPINFOHEADER& bih = *((LPBITMAPINFOHEADER)lpDib);

	if (_lwrite (hFile, (LPCSTR)&bfh, sizeof (BITMAPFILEHEADER)) != sizeof (BITMAPFILEHEADER))
	{
		Win32ErrorMessage("Error writing BITMAPFILEHEADER");
		_lclose(hFile);
		throw 2;
	}		 
	const LONG size = bfh.bfSize - sizeof(BITMAPFILEHEADER);
	
	if(_hwrite(hFile, (const char huge *)lpDib, size) != size)
	{
		Win32ErrorMessage("Error writing image bits");
		_lclose(hFile);
		throw 3;
	}

	_lclose (hFile) ;
}
	
///////////////////////////////////////////////////////////////////////	

void Dib::WriteJpg(LPCTSTR outfilename, int jpegQuality)
{
	int			headerSize	= HeaderSize();
	BYTE huge*	lpPal		= lpDib + headerSize;
	int			palSize		= lpPal - lpDibBits; // usually 0

	JpegWriter writer(lpDib, lpDibBits, outfilename, jpegQuality);
	writer.write();
}



	
///////////////////////////////////////////////////////////////////////	

DWORD Dib::ImageSize()
{			  
	LPBITMAPINFOHEADER	lpbi;
	LPBITMAPCOREHEADER	lpbc;
	double				bytesPerPixel;
	
	if(!lpDib)
		return 0;
		
	lpbi = ((LPBITMAPINFOHEADER) lpDib);
	lpbc = ((LPBITMAPCOREHEADER) lpDib);
								 
    /*	With the BITMAPINFO format headers, the size of the palette
     *	is in biClrUsed, whereas in the BITMAPCORE - style headers, it
     *	is dependent on the bits per pixel ( = 2 raised to the power of
     *	bits/pixel).
     */
     
	if (lpbi->biSize != sizeof(BITMAPCOREHEADER))
		// LPBITMAPINFOHEADER
		bytesPerPixel = (double)(lpbi->biBitCount) / 8.0;
	else 
		bytesPerPixel = (double)(lpbc->bcBitCount) / 8.0;

	/* image rows are padded with zeros to a 4-byte boundary... */
	WORD rowWidth = Width();	// width in PIXELS
	rowWidth *= (WORD)bytesPerPixel;	// width in bytes before padding...
	
	while(rowWidth & 3)
		rowWidth++;
	/* whew - now rowWidth = width in bytes + pad bytes... */
	
	return rowWidth * Height();
}
		
///////////////////////////////////////////////////////////////////////	

HPALETTE Dib::Palette()
{			  
	HLOCAL		hlocal;
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER) lpDib;
	LOGPALETTE          *pPal;
	HPALETTE            hpal = 0;
	WORD                nNumColors;
	//BYTE                red;
	//BYTE                green;
	//BYTE                blue;
	int                 i;
	RGBQUAD        FAR *pRgb;

	if (!lpbi)
		return NULL;
		
					   
	if (lpbi->biSize != sizeof(BITMAPINFOHEADER))
		return NULL;
	
    /* Get a pointer to the color table and the number of colors in it */
	pRgb = (RGBQUAD FAR *) ((LPTSTR) lpbi + (WORD) lpbi->biSize);  // unicode
	nNumColors = NumColors();
	
	if(nNumColors == 0)	// > 8 bit bmp, NOT AN ERROR - should have errors return something besides NULL ??
		return NULL;
							   
	if (nNumColors) 
	{
		// Allocate for the logical palette structure
		hlocal = LocalAlloc(LPTR, sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
		pPal = (LOGPALETTE *) LocalLock(hlocal);
		
		if (!pPal)
			return NULL;

		//static TCHAR foo[100];  // unicode
		//wsprintf(foo, TEXT( "nNumColors= %d\n\r" ), nNumColors);  // unicode
		//OutputDebugString(foo);
		//wsprintf(foo, TEXT( "PALVERSION= %d\n\r" ), PALVERSION);  // unicode
		//OutputDebugString(foo);
		pPal->palNumEntries = nNumColors;
		pPal->palVersion = PALVERSION;
		
		/* Fill in the palette entries from the DIB color table and
		 * create a logical color palette.
		 */
		 
		for (i = 0;  i < (int)nNumColors;  i++) 
		{
			// BN: note this is brain-dead -- do ONE hmemcpy to copy
			// all in one line...  TBD TBD
			//
			pPal->palPalEntry[i].peRed = pRgb[i].rgbRed;
			pPal->palPalEntry[i].peGreen = pRgb[i].rgbGreen;
			pPal->palPalEntry[i].peBlue = pRgb[i].rgbBlue;
			pPal->palPalEntry[i].peFlags = (BYTE) 0;  // unicode
		}
		hpal = CreatePalette(pPal);
		LocalFree(hlocal);
	}		  	  // end of num colors > 0
#if 0	
	else if (lpbi->biBitCount == 24) 
	{	
		/* A 24 bitcount DIB has no color table entries so, set the number of
		 * to the maximum value (256).
		 */
		nNumColors = MAXPALETTE;
		hlocal = LocalAlloc(LPTR, sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
		pPal = (LOGPALETTE *) LocalLock(hlocal);
		//pPal = (LOGPALETTE *) App.localAlloc(sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY), 0);
		//pPal = (LOGPALETTE *) LocalAlloc(LPTR, sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
		if (!pPal)
			return NULL;

		pPal->palNumEntries = nNumColors;
		pPal->palVersion = PALVERSION;

		red = green = blue = 0;

	/* Generate 256 (= 8*8*4) RGB combinations to fill the palette
	 * entries.
	 */
		for (i = 0;  i < (int)pPal->palNumEntries;  i++) {
			pPal->palPalEntry[i].peRed = red;
			pPal->palPalEntry[i].peGreen = green;
			pPal->palPalEntry[i].peBlue = blue;
			pPal->palPalEntry[i].peFlags = (BYTE) 0;  // unicode

			if (!(red += 32))
				if (!(green += 32))
					blue += 64;
			}
		hpal = CreatePalette(pPal);
		//App.localFree(pPal);
		LocalFree(hlocal);
		}			  
#endif		
	return hpal;	   
}

//////////////////////////////////////////////////////////////////////////

const HGLOBAL Dib::GetHandle()
{	   				  
	return GlobalPtrHandle(lpDib);
}	

//////////////////////////////////////////////////////////////////////////

HGLOBAL Dib::GetHandleCopy()
{	   				  
	BYTE huge *lpCopy;  // unicode
	BYTE huge *lpSrc;  // unicode
	HGLOBAL     hCopy;
	DWORD      dwLen;

	if (!lpDib)
		return 0;
			   
	dwLen = ::GlobalSize(GetHandle());
						 
	if ((hCopy = ::GlobalAlloc(GHND, dwLen)) != NULL)
	{			 			  
		lpCopy = (BYTE huge *) ::GlobalLock((HGLOBAL) hCopy);  // unicode
		lpSrc = lpDib;
		  
		while (dwLen--)
			*lpCopy++ = *lpSrc++;

		::GlobalUnlock((HGLOBAL) hCopy);
	}

	return hCopy;
}

/****************************************************************************
 *									    *
 *  FUNCTION   : Dib::NumColors()
 *				 		   			    *
 *  PURPOSE    : Determines the number of colors in the DIB by looking at   *
 *		 the BitCount filed in the info block.			    *
 *									    *
 *  RETURNS    : The number of colors in the DIB.			    *
 *									    *
 ****************************************************************************/

WORD Dib::NumColors()
{	 				   
	ASSERT(lpDib);
	
	int 		bits;
	LPBITMAPINFOHEADER	lpbi;
	LPBITMAPCOREHEADER	lpbc;
	
	lpbi = ((LPBITMAPINFOHEADER) lpDib);
	lpbc = ((LPBITMAPCOREHEADER) lpDib);
								 
    /*	With the BITMAPINFO format headers, the size of the palette
     *	is in biClrUsed, whereas in the BITMAPCORE - style headers, it
     *	is dependent on the bits per pixel ( = 2 raised to the power of
     *	bits/pixel).
     */
	if (lpbi->biSize != sizeof(BITMAPCOREHEADER)) {
		if (lpbi->biClrUsed != 0)
			return (WORD) lpbi->biClrUsed;
		bits = lpbi->biBitCount;
		}
	else 
		bits = lpbc->bcBitCount;

	switch (bits) {
  case 1:
		return 2;
  case 4:
		return 16;
  case 8:
		return 256;
  default:
		/* A 16 or 24 bitcount DIB has no color table */
		return 0;
		}
}

//////////////////////////////////////////////////////////////////////////

void Dib::CopyHandle(HGLOBAL hDib)
{	   			  	  		
	DWORD      dwLen;  
	lpDib = NULL;		// just to be sure...

	if (!hDib)
		return;
			   
	dwLen = ::GlobalSize(hDib);

//#if defined(WIN32) || defined(_WIN32)
     //lpDib = new BYTE[dwLen];  // unicode
//#else     
     lpDib = (BYTE huge * ) GlobalAllocPtr (GMEM_MOVEABLE, dwLen) ;  // unicode
//#endif
						 
	if(lpDib)
	{	 	   	 			  										
		BYTE huge *lpSrc = (BYTE huge *) ::GlobalLock(hDib);  // unicode
		  				 
		hmemcpy(lpDib, lpSrc, dwLen);
		::GlobalUnlock(hDib);
	}				   
}		   

/****************************************************************************
 *									    *
 *  FUNCTION   : BitmapFromDib(HANDLE hdib, HPALETTE hpal)		    *
 *									    *
 *  PURPOSE    : Will create a DDB (Device Dependent Bitmap) given a global *
 *		 handle to a memory block in CF_DIB format		    *
 *									    *
 *  RETURNS    : A handle to the DDB.					    *
 *									    *
 ****************************************************************************/

HBITMAP Dib::GetDDB(HDC hdc, HPALETTE hp)
{					
	HPALETTE old_pal = 0;
	
	if(!hp)
		hp = GetPal();
	
	if((int)(hp) == -1)
		hp = 0;

	LPBITMAPINFOHEADER	lpbi = (LPBITMAPINFOHEADER)lpDib;
	HBITMAP		hbm;

	SetStretchBltMode (hdc, COLORONCOLOR) ;
	
	if(hp)
	{
		old_pal = SelectPalette(hdc, hp, FALSE);
		RealizePalette(hdc);				  
	}	
					  
	hbm = CreateDIBitmap(hdc,
		 lpbi,
		 (LONG) CBM_INIT,		
		 (LPTSTR) lpDib + lpbi->biSize + PaletteSize(),  // unicode
		 (LPBITMAPINFO)lpDib,						
		 DIB_RGB_COLORS); 

	if(hp)
		SelectPalette(hdc, old_pal, FALSE);
		
	return hbm;		   
}

////////////////////////////////////////////////////////////////////////////

HBITMAP Dib::GetDDB(int w, int h)
{										   
	return GetDDB(w, h, NULL);
}

////////////////////////////////////////////////////////////////////////////

HBITMAP Dib::GetDDB(int w, int h, LPCTSTR label)
{										   
	return GetDDB(::GetDC(NULL), w, h, 0, label);
}

////////////////////////////////////////////////////////////////////////////

HBITMAP Dib::GetDDB(HDC hdc, int w, int h, HPALETTE hp)
{
	return GetDDB(hdc, w, h, hp, NULL);
}
	
////////////////////////////////////////////////////////////////////////////

HBITMAP Dib::GetDDB(HDC hdc, int w, int h, HPALETTE hp, LPCTSTR label)
{										   
	if(!hp)
		hp = GetPal();

	if((int)(hp) == -1)
		hp = 0;
		
	HBITMAP		hbmSrc, hbmOldSrc, hbmOldDest, hbmNew;
    HDC     hdcSrc, hdcDest;
    BITMAP  bm;
	HPALETTE oldPalSrc = 0;
	HPALETTE oldPalDest = 0;
	HPALETTE old_pal = 0;
					
	hbmSrc = GetDDB(hdc, hp);		// get an unstretched bitmap...
	
	if((w == 0 && h == 0) || (w == (int)Width() && h == (int)Height()))
		if(!label)
			return hbmSrc;
		
	if(w == 0)
		w = Width();	

	if(h == 0)
		h = Height();	

    hdcSrc = CreateCompatibleDC(hdc);
    hdcDest = CreateCompatibleDC(hdcSrc);
    
    if(hp)
    {
		old_pal = SelectPalette(hdc, hp, FALSE);
		RealizePalette(hdc);				  
		oldPalSrc = SelectPalette(hdcSrc, hp, FALSE);
		RealizePalette(hdcSrc);
		oldPalDest = SelectPalette(hdcDest, hp, FALSE);
		RealizePalette(hdcDest);
	}	
    
	SetStretchBltMode (hdc, COLORONCOLOR) ;
	SetStretchBltMode (hdcDest, COLORONCOLOR) ;
	SetStretchBltMode (hdcSrc, COLORONCOLOR) ;
	
    GetObject(hbmSrc, sizeof(BITMAP), &bm);
    hbmOldSrc = (HBITMAP)SelectObject(hdcSrc, hbmSrc);
    hbmNew = CreateCompatibleBitmap(hdcSrc, w, h);
    hbmOldDest = (HBITMAP)SelectObject(hdcDest, hbmNew);
	
    StretchBlt(hdcDest, 0, 0, w, h, hdcSrc, 0, 0,
        bm.bmWidth, bm.bmHeight, SRCCOPY);

	if(label)
	{
		::SetBkColor(hdcDest, RGB(0, 0, 0));
		::SetTextColor(hdcDest, RGB(255, 255, 0));
		RECT rect = { 0, 0, w, h};
		::DrawText(hdcDest, label, -1, &rect, DT_RIGHT);
		//TextOut(hdcDest, 0, 0, label, lstrlen(label));
	}
    
	SelectObject(hdcDest, hbmOldDest);
    SelectObject(hdcSrc, hbmOldSrc);
    
    if(hp)
    {
		SelectPalette(hdcDest, oldPalDest, FALSE);
		SelectPalette(hdcSrc, oldPalSrc, FALSE);
		SelectPalette(hdc, old_pal, FALSE);
	}	
    DeleteDC(hdcDest);
    DeleteDC(hdcSrc);
	DeleteObject(hbmSrc);
	//UnRealize(hdc);	// GDI Bug...????
    return hbmNew;
}

/****************************************************************************
 *									    *
 *  FUNCTION   :  PaletteSize(VOID FAR * pv)				    *
 *									    *
 *  PURPOSE    :  Calculates the palette size in bytes. If the info. block  *
 *		  is of the BITMAPCOREHEADER type, the number of colors is  *
 *		  multiplied by 3 to give the palette size, otherwise the   *
 *		  number of colors is multiplied by 4.								*
 *									    *
 *  RETURNS    :  Palette size in number of bytes.			    *
 *									    *
 ****************************************************************************/

WORD Dib::PaletteSize()
{
	LPBITMAPINFOHEADER lpbi;
	WORD	       numColors;
				   
	lpbi = (LPBITMAPINFOHEADER) lpDib;
	numColors = NumColors();
	//numColors = DibNumColors(lpbi);
	
	if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
		return numColors * sizeof(RGBTRIPLE);
	else 	   
		return numColors * sizeof(RGBQUAD);
}			   

////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Dib::DumpHeader(ostream* outStream)
{
	BOOL isString = FALSE;
	if(!outStream)
	{
		outStream = new ostrstream;
		isString = TRUE;
	}
		
	ostream& mesg = *outStream;								  
	
	if(!lpDib)
	{
		TRACE("lpDib is NULL - No Header!!!!\n");
		return;
	}	
	LPBITMAPINFOHEADER pbi = (LPBITMAPINFOHEADER)lpDib;
	BITMAPINFOHEADER& bi = *pbi;
	if(bi.biSize != sizeof(BITMAPINFOHEADER))
	{
		mesg << "Not a BITMAPINFOHEADER!!!" << endl;
	}	 				  	   
	else
	{
        mesg << "biSize =" << bi.biSize << endl;
        mesg << "biWidth =" << bi.biWidth << endl;
        mesg << "biHeight =" << bi.biHeight << endl;
        mesg << "biPlanes =" << bi.biPlanes << endl;
        mesg << "biBitCount =" << bi.biBitCount << endl;
        mesg << "biCompression =" << bi.biCompression << endl;
        mesg << "biSizeImage =" << bi.biSizeImage << endl;
        mesg << "biXPelsPerMeter =" << bi.biXPelsPerMeter << endl;
        mesg << "biYPelsPerMeter =" << bi.biYPelsPerMeter << endl;
        mesg << "biClrUsed =" << bi.biClrUsed << endl;
        mesg << "biClrImportant =" << bi.biClrImportant << endl;
	}
	
	if(isString)
	{
		LPTSTR str = ((ostrstream *)(outStream))->str();
		TRACE(str);
		free(str);
		delete outStream;
	}
}	
#endif

////////////////////////////////////////////////////////////////////////

DDB::DDB(HINSTANCE hInstance, UINT resID)
{ 
#ifdef USING_MFC
	if(!hInstance)
		hInstance = AfxGetInstanceHandle();
#endif
	ASSERT(hInstance);
	m_hbmp = LoadBitmap(hInstance, MAKEINTRESOURCE(resID));
	init();
}

////////////////////////////////////////////////////////////////////////

DDB::DDB(HBITMAP aBmp) : m_hbmp(aBmp)	
{ 
	init();
}

////////////////////////////////////////////////////////////////////////

void DDB::init()
{ 
	
	if(!m_hbmp)
		return;

	BITMAP    bm;
	
	GetObject(m_hbmp, sizeof(BITMAP), (LPTSTR) &bm);  // unicode
	Width( bm.bmWidth);
	Height( bm.bmHeight);
	OK(1);
}

////////////////////////////////////////////////////////////////////////

DDB::~DDB()
{	  			   	
	if(m_hbmp)
	   	DeleteObject(m_hbmp);
}					 

////////////////////////////////////////////////////////////////////////

void	DDB::Show_(HDC hdc, int x, int y, int w, int h, DWORD rop)
{											  
	HDC       hdcBits;
	BITMAP    bm;
	//BOOL      f;

	if(OK())
	{
		SetStretchBltMode (hdc, COLORONCOLOR) ;
		hdcBits = CreateCompatibleDC(hdc);
		SetStretchBltMode (hdcBits, COLORONCOLOR) ;
		GetObject(m_hbmp, sizeof(BITMAP), (LPTSTR) &bm);  // unicode
		SelectObject(hdcBits, m_hbmp);
		SetStretchBltMode (hdcBits, COLORONCOLOR) ;
							  
		if(w && h)	// stretching
			StretchBlt(hdc, x, y, w, h, hdcBits, 0, 0, bm.bmWidth, bm.bmHeight, rop);
		else
			BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcBits, 0, 0, rop);
		
		if(Width() == 0 || Height() == 0)
		{
			Width( bm.bmWidth);
			Height( bm.bmHeight);
		}		  
							  	
		DeleteDC(hdcBits);
	}	
}

////////////////////////////////////////////////////////////////////////

void	DDB::Wipe(HDC hdc, int x, int y, int timing)
{			 							 	  
	HDC       hdcBits;
	BITMAP    bm;
	const int orig_x = x;
	const int chunkWidth = 1;
	//BOOL      f;	   

	if(OK())
	{
		SetStretchBltMode (hdc, COLORONCOLOR) ;
		hdcBits = CreateCompatibleDC(hdc);
		SetStretchBltMode (hdcBits, COLORONCOLOR) ;
		GetObject(m_hbmp, sizeof(BITMAP), (LPTSTR) &bm);  // unicode
		SelectObject(hdcBits, m_hbmp);
		SetStretchBltMode (hdcBits, COLORONCOLOR) ;
							  
							  
		// timing = timing * 10 / bm.bmHeight;
		int numChunks = bm.bmWidth / chunkWidth ;
		int leftover = bm.bmWidth % chunkWidth;			  
			   			   			
		for(int i = 0; i < numChunks; i++, x += chunkWidth)
		{
			BitBlt(hdc, x, y, chunkWidth, bm.bmHeight, hdcBits, x - orig_x, 0, SRCCOPY);
			Delay(timing);
		}	
			
		if(leftover)					  									
		{
			x -= chunkWidth;
			BitBlt(hdc, x, y, leftover, bm.bmHeight, hdcBits, x - orig_x, 0, SRCCOPY);
		}
		
		if(Width() == 0 || Height() == 0)
		{
			Width( bm.bmWidth);
			Height( bm.bmHeight);
		}		  
							  	
		DeleteDC(hdcBits);
	}	
}
	
///////////////////////////////////////////////////////////////////////	

LPCTSTR DDB::Write(LPCTSTR filename)
{			 	
	HANDLE hFile = CreateFile(filename, 
                   GENERIC_READ | GENERIC_WRITE, 
                   (DWORD) 0, 
                   (LPSECURITY_ATTRIBUTES) NULL, 
                   CREATE_ALWAYS, 
                   FILE_ATTRIBUTE_NORMAL, 
                   (HANDLE) NULL); 
 
    if (hFile == INVALID_HANDLE_VALUE) 
        return "Can't create file";

	LPCTSTR ret = Write(hFile);

    /* Close the .BMP file. */ 
 
    if(!CloseHandle(hFile)) 
           return "Can't close the file";

	return ret;
}
	
///////////////////////////////////////////////////////////////////////	

LPCTSTR DDB::Write(HANDLE hFile)
{
	try
	{
		PBITMAPINFO pbi = CreateBitmapInfoStruct();

		CreateBMPFile(hFile, pbi);
	}
	catch(LPCTSTR s)
	{
		return s;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////

PBITMAPINFO DDB::CreateBitmapInfoStruct()
{
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 
 
    /* Retrieve the bitmap's color format, width, and height. */ 
 
    if (!GetObject(m_hbmp, sizeof(BITMAP), (LPSTR)&bmp))
		throw "Get Object Error";
 
    /* Convert the color format to a count of bits. */ 
 
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else 
        cClrBits = 32; 
 
    /* 
     * Allocate memory for the BITMAPINFO structure. (This structure 
     * contains a BITMAPINFOHEADER structure and an array of RGBQUAD data 
     * structures.) 
     */ 
 
    if (cClrBits != 24) 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (2^cClrBits)); 
 
    /* 
     * There is no RGBQUAD array for the 24-bit-per-pixel format. 
     */ 
 
    else 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER)); 

	if(!pbmi)
		throw "Error allocating PBITMAPINFO";

    /* Initialize the fields in the BITMAPINFO structure. */ 
 
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 

    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = 2^cClrBits; 
 
    /* If the bitmap is not compressed, set the BI_RGB flag. */ 
    pbmi->bmiHeader.biCompression = BI_RGB; 
 
    /* 
     * Compute the number of bytes in the array of color 
     * indices and store the result in biSizeImage. 
     */ 
 
    pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) /8 
                                  * pbmi->bmiHeader.biHeight 
                                  * cClrBits; 
 
    /* 
     * Set biClrImportant to 0, indicating that all of the 
     * device colors are important. 
     */ 
 
    pbmi->bmiHeader.biClrImportant = 0; 
 
    return pbmi; 
}

////////////////////////////////////////////////////////////////////////

void DDB::CreateBMPFile(HANDLE hFile, PBITMAPINFO pbi)
{
	if(!hFile || hFile == INVALID_HANDLE_VALUE)
		throw "Bad File Handle";

	if(!pbi)
		throw "Null PBITMAPINFO";

    BITMAPFILEHEADER hdr;       /* bitmap file-header */ 
    PBITMAPINFOHEADER pbih;     /* bitmap info-header */ 
    LPBYTE lpBits;              /* memory pointer */ 
    DWORD dwTotal;              /* total count of bytes */ 
    DWORD cb;                   /* incremental count of bytes */ 
    BYTE *hp;                   /* byte pointer */ 
    DWORD dwTmp; 
	const DWORD MAXWRITE = 16384;
	HDC		hDC = ::GetDC(NULL);
 
	if(!hDC)
		throw "Null HDC";

    pbih = (PBITMAPINFOHEADER) pbi; 
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);    
	
	if (!lpBits) 
         throw "GlobalAlloc Error";
 
    /* 
     * Retrieve the color table (RGBQUAD array) and the bits 
     * (array of palette indices) from the DIB. 
     */ 
 
	if(!m_hbmp)
		throw "Null HBITMAP";

    if (!GetDIBits(hDC, m_hbmp, 0, (WORD) pbih->biHeight, 
                   lpBits, pbi, DIB_RGB_COLORS)) 
        throw "GetDIBits";
 
    /* Create the .BMP file. */ 
 
    hdr.bfType = 0x4d42;        /* 0x42 = "B" 0x4d = "M" */ 
 
    /* Compute the size of the entire file. */ 
 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                 pbih->biSize + pbih->biClrUsed 
                 * sizeof(RGBQUAD) + pbih->biSizeImage); 
 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 
 
    /* Compute the offset to the array of color indices. */ 
 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                    pbih->biSize + pbih->biClrUsed 
                    * sizeof (RGBQUAD); 
 
    /* Copy the BITMAPFILEHEADER into the .BMP file. */ 
 
    if (!WriteFile(hFile, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
       (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL)) 
       throw "WriteFile";
 
    /* Copy the BITMAPINFOHEADER and RGBQUAD array into the file. */ 
 
    if (!WriteFile(hFile, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
                  + pbih->biClrUsed * sizeof (RGBQUAD), 
                  (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL)) 
       throw "WriteFile";
 
    /* Copy the array of color indices into the .BMP file. */ 
 
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    while (cb > MAXWRITE)  { 
            if (!WriteFile(hFile, (LPSTR) hp, (int) MAXWRITE, 
                          (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL)) 
		       throw "WriteFile";
            cb-= MAXWRITE; 
            hp += MAXWRITE; 
    } 
    if (!WriteFile(hFile, (LPSTR) hp, (int) cb, 
         (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL)) 
	       throw "WriteFile";
 
 
    /* Free memory. */    
	GlobalFree((HGLOBAL)lpBits);
} 

////////////////////////////////////////////////////////////////////////

QuickDib::QuickDib(HDC hDC, HINSTANCE hInstance, int res_num, HPALETTE hp)
{																	   	
	ddb = NULL;
	
	Dib dib(hInstance, res_num);
	
	if(dib == 0)
	   	return;
		
	if(hp)	
		ddb = new DDB(dib.GetDDB(hDC, hp));
	else	
		ddb = new DDB(dib.GetDDB(hDC));
									
	if(ddb && *ddb)	 
		OK(1);
}

////////////////////////////////////////////////////////////////////////

QuickDib::QuickDib(HDC hDC, TCHAR* filename, HPALETTE hp)  // unicode
{
 	ddb = NULL;
	
	Dib dib(filename);
			
	if(dib == 0)
	   	return;
		
	if(hp)	
		ddb = new DDB(dib.GetDDB(hDC, hp));
	else	
		ddb = new DDB(dib.GetDDB(hDC));
					 
	if(ddb && *ddb)
		OK(1);
}							

////////////////////////////////////////////////////////////////////////

QuickDib::QuickDib(HDC hDC, TCHAR* filename, int w, int h, HPALETTE hp)  // unicode
{
 	ddb = NULL;
	
	Dib dib(filename);
			
	if(dib == 0)
	   	return;
		
	if(hp)	
		ddb = new DDB(dib.GetDDB(hDC, w, h, hp));
	else	
		ddb = new DDB(dib.GetDDB(hDC, w, h));
					 
	if(ddb && *ddb)
		OK(1);
}							
	
////////////////////////////////////////////////////////////////////////
		
QuickDib::~QuickDib()
{
	delete ddb;
}

////////////////////////////////////////////////////////////////////////
		
void QuickDib::Show_(HDC hdc, int x, int y, int w, int h, DWORD rop)
{
	if(ddb)
		ddb->Show(hdc, x, y, w, h, rop);
}  
			 				   		  		 			 
////////////////////////////////////////////////////////////////////////
		
#if 0
HBITMAP PASCAL DuplicateBitmap(HBITMAP hbmSrc)
{
    HBITMAP hbmpOldSrc, hbmpOldDest, hbmpNew;
    HDC     hdcSrc, hdcDest;
    BITMAP  bmp;

    hdcSrc = CreateCompatibleDC(NULL);
    hdcDest = CreateCompatibleDC(hdcSrc);

    GetObject(hbmpSrc, sizeof(BITMAP), &bmp);

    hbmpOldSrc = SelectObject(hdcSrc, hbmpSrc);

    hbmpNew = CreateCompatibleBitmap(hdcSrc, bmp.bmWidth,
        bmp.bmHeight);

    hbmpOldDest = SelectObject(hdcDest, hbmpNew);


    BitBlt(hdcDest, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcSrc, 0, 0,
        SRCCOPY);

    SelectObject(hdcDest, hbmpOldDest);
    SelectObject(hdcSrc, hbmpOldSrc);

    DeleteDC(hdcDest);
    DeleteDC(hdcSrc);

    return hbmpNew;
}
#endif
double  Dib::getScale()
{
	return scalingFactor;
}

void  Dib::setScale(double num)
{
	scalingFactor = num;
}

bool Dib::rotate(int numQuarterTurnsCW)
{
	LPCTSTR oldname		= GetFileName();
	LPCTSTR	suffix		= "_rotated.jpg";
	FILE*	newFile = tmpfile();
	char newname[4096];

	// Huge bug fixed at last!  It used to make newname's size
	// equal to the lenght of oldname plus FIVE!!
	// OLD:  char*	newname		= new char[lstrlen(oldname) + 5];

	if(lstrlen(oldname) + lstrlen(suffix) + 1 > sizeof(newname))
	{
		MessageBox(NULL, "Filename is too long in Dib::Rotate.  Contact support.", "Error", MB_ICONEXCLAMATION);
		return false;
	}
	try
	{
		lstrcpy(newname, oldname);
		lstrcat(newname, suffix);

		// if the original file is read-only -- do this:
		// 1. make it writable
		// 2. rotate
		// 3. copy the new one over
		// 4. make it read-only again
		
		bool wasReadOnly = false;

		if(isReadOnly(oldname))
		{
			wasReadOnly = true;
			makeWritable(oldname);
		}

		JpegRotater r(oldname, newname, numQuarterTurnsCW);
		
		if(!r.rotate())
			return false;

		if(remove(oldname) != 0)
			return false;

		if(rename(newname, oldname) != 0)
			return false;

		if(wasReadOnly)
			makeReadOnly(oldname);

		return true;
	}
	catch(...)
	{
		MessageBox(NULL, "Unexpected Error in Dib::Rotate", "Error", MB_ICONEXCLAMATION);
		return false;
	}
}
