#include "stdtif.h"

static HANDLE		hdibCurrent;  /* Handle to current memory DIB						  */
static HANDLE	hbiCurrent;   /* Handle to current bitmap info struct   */

				
BYTE huge * Tiff2Dib(char *fname)
{				 
	int status=0;
	int numcolors=0,j;

	int x = sizeof(short int);

	unsigned short int width, bitsperpix, photometric;	 //BN
	unsigned short int scanlinesize;					 //BN
	unsigned short int height,row;
	DWORD paddedWidth;
	int		padBytes;
	LPBITMAPINFOHEADER lpbi;
	DWORD dwBits,dwLen;
	BYTE huge *pixels, *pwholeDib;
	PTIFF tif;
	RGBQUAD FAR *pRgb=NULL;
	float factor;

	tif = TIFFOpen(fname, "r");
	if(tif==NULL)  
		return NULL;
				   
	//get the dimensions out for use in creating the buffer

	TIFFGetField(tif,TIFFTAG_IMAGELENGTH,   &height);
	TIFFGetField(tif,TIFFTAG_IMAGEWIDTH,    &width);
	TIFFGetField(tif,TIFFTAG_BITSPERSAMPLE, &bitsperpix);
	TIFFGetField(tif,TIFFTAG_PHOTOMETRIC, &photometric);
	
	
	if(photometric==PHOTOMETRIC_RGB)
		bitsperpix *= 3;
	else
		numcolors = 1 << bitsperpix;
	if(bitsperpix!=1 && bitsperpix!=4 && bitsperpix!=8 && bitsperpix!=24) {
		//ErrMsg("TIFF: %d bits per pixel not handled",bitsperpix);
		status=-1;
		goto red_exit;
		}

	//Create the DIB structures
	// put width on paragraph boundary...	
	// Bullshit handling of short->long stuff
	paddedWidth = width;
	paddedWidth *= bitsperpix;
	paddedWidth /= 8;
	padBytes = 0;

	while(paddedWidth & 0X03)
	{
		 paddedWidth++;
		 padBytes++;
	}

   hbiCurrent=GlobalAlloc(GHND, (LONG)sizeof(BITMAPINFOHEADER) +
		numcolors * sizeof(RGBQUAD));
   if (!hbiCurrent)
   {
      goto red_exit;
     } 

   lpbi = (VOID FAR *)GlobalLock (hbiCurrent);
   lpbi->biSize=sizeof(BITMAPINFOHEADER);
	lpbi->biWidth=width;
	lpbi->biHeight=height;
	lpbi->biPlanes=1;
	lpbi->biBitCount= bitsperpix;
	lpbi->biCompression= BI_RGB;
   dwBits= lpbi->biSizeImage = ((DWORD)(height)) * paddedWidth;
   lpbi->biXPelsPerMeter      = 0;					  
   lpbi->biYPelsPerMeter      = 0;
   lpbi->biClrUsed            = numcolors;
   lpbi->biClrImportant       = numcolors;
	
	/* fill in intensities for all palette entry colors */

	if(photometric==PHOTOMETRIC_PALETTE) {
		u_short FAR *redmap;
		u_short FAR *greenmap;
		u_short FAR *bluemap;

		TIFFGetField(tif,TIFFTAG_COLORMAP,&redmap,&greenmap,&bluemap);
		pRgb = (RGBQUAD FAR *) ((LPSTR) lpbi + lpbi->biSize);
		for (j = 0;  j < numcolors;  j++) {
			pRgb[j].rgbRed   = HIBYTE(redmap[j]);
			pRgb[j].rgbGreen = HIBYTE(greenmap[j]);
			pRgb[j].rgbBlue  = HIBYTE(bluemap[j]);
			}
		}
	else if(photometric==PHOTOMETRIC_MINISBLACK) {
		pRgb = (RGBQUAD FAR *) ((LPSTR) lpbi + lpbi->biSize);
		factor = 256.0f / numcolors;
		for (j = 0;  j < numcolors;  j++) {
			pRgb[j].rgbRed =
				pRgb[j].rgbGreen =
				pRgb[j].rgbBlue = (BYTE) (j * factor);
			}
		}
	else if(photometric==PHOTOMETRIC_MINISWHITE) {
		pRgb = (RGBQUAD FAR *) ((LPSTR) lpbi + lpbi->biSize);
		factor = 256.0f / numcolors;
		for (j = 0;  j < numcolors;  j++) {
			pRgb[j].rgbRed =
				pRgb[j].rgbGreen =
				pRgb[j].rgbBlue = (BYTE) ((numcolors-j) * factor);
			}

		}

	dwLen  = lpbi->biSize + (DWORD)numcolors*sizeof(RGBQUAD) + dwBits;
	hdibCurrent=GlobalAlloc(GHND,dwLen);
	
	if(!hdibCurrent)
	{
		//ErrMsg("Couldn't create DIB for TIFF conversion");
		GlobalUnlock(hbiCurrent);
		GlobalFree(hbiCurrent);
		goto red_exit;
	}	
		
	pwholeDib = pixels =(BYTE huge *)GlobalLock(hdibCurrent);
	hmemcpy(pixels,lpbi,(size_t)(dwLen-dwBits));
	GlobalUnlock(hbiCurrent);
	GlobalFree(hbiCurrent);
	hbiCurrent = 0;
	pixels += dwLen-dwBits;
	scanlinesize=TIFFScanlineSize(tif);

	//StartWait();

	{ // simulate C++ var. decls....
	//long offset;
	BYTE huge *pPix;
	DWORD dwOffset;
	//BN
	for (row = 0; row < height; row++)
	{	 							
		int i;
		BYTE huge * px;

		dwOffset= ((DWORD)(height - row - 1)) * paddedWidth;
		//dwOffset=( ((DWORD)(height - row - 1)) * ((DWORD)scanlinesize));
		pPix = pixels + dwOffset;
		px = pPix + paddedWidth - 1;
		
		if(TIFFReadScanline(tif,pPix,row,0)<0)
			break;
		

		for(i = 0; i < padBytes; i++, --px)
			*px = 0;
		
		if(photometric==PHOTOMETRIC_RGB)
		{
			int i;
			BYTE huge *pRed = pPix; 
			BYTE huge *pBlue = pPix + 2;
			BYTE tmp;

			for(i = 0; i < width; i++, pRed += 3, pBlue += 3)
			{
				tmp = *pRed;
				*pRed = *pBlue;
				*pBlue = tmp;
			}		
		}
	}
	}

	//EndWait();

	// GlobalUnlock(hdibCurrent); don't unlock...
	TIFFClose(tif);
   return pwholeDib;	// It's LOCKED DOWN -- caller free's it...
		  
red_exit:
	TIFFClose(tif);
   return NULL;
}		  
//////////////////// old code dump area......////////////

#if 0

static void SwapRGB(BYTE huge * bits, int width)
{
	int i;
	BYTE huge *pRed = bits; 
	BYTE huge *pBlue = bits + 2;
	BYTE tmp;

	if(width <= 0)
		return;

	for(i = 0; i < width; i++, pRed += 3, pBlue += 3)
	{
		tmp = *pRed;
		*pRed = *pBlue;
		*pBlue = tmp;
	}		
}

#define SwapRGB(bits, width)\
{\
	int i;\
	BYTE huge *pRed = bits;\ 
	BYTE huge *pBlue = bits + 2;\
	BYTE tmp;\

	for(i = 0; i < width; i++, pRed += 3, pBlue += 3)\
	{\
		tmp = *pRed;\
		*pRed = *pBlue;\
		*pBlue = tmp;\
	}\		
}
#endif

