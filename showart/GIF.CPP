#define NOCOMM
#define NOKANJI
#include "stdafx.h"
//#include <windows.h>
//#include "image2.h"
//#include <stdio.h>
//#include <string.h>
//static HCURSOR hcurSave;

//GIF input routines, adapted for image2 by Martin Heller
//GIF input routines, adapted again by Byron Nevins...
							  
/*
 *
 * This GIF decoder is designed for use with Bert Tyler's FRACTINT
 * program. It should be noted that the "FRACTINT" program only decodes
 * GIF files FRACTINT creates, so this decoder code lacks full generality
 * in the following respects: supports single image, non-interlaced GIF
 * files with no local color maps and no extension blocks.
 *
 * GIF and 'Graphics Interchange Format' are trademarks (tm) of
 * Compuserve, Incorporated, an H&R Block Company.
 *
 *											Tim Wegner
 */

static HANDLE	hdibCurrent;  /* Handle to current memory DIB						  */
static HANDLE	hbiCurrent;   /* Handle to current bitmap info struct   */

static short  init_exp(short  size);
static short  get_next_code(void);
static void  inittable(void);
static void  raster(unsigned int  code);
static void  close_file(void);
static short decoder(short linewidth);
									 
static int get_byte_n=0,get_byte_i=0;
int rowcount;		/* row counter for screen */
extern NPLOGPALETTE pLogPal;
extern BOOL	bMemoryDIB;
static int hin = NULL;
unsigned int height;

int	mapset;
unsigned char dacbox[256][3];	/* Video-DAC (filled in by SETVIDEO) */

static int paletteVGA[16];		/* VGA Palette-to-DAC registers */
static unsigned char decoderline[2049]; /* write-line routines use this */
static unsigned char win_andmask[8];
static unsigned char win_notmask[8];
static unsigned char win_bitshift[8];

int xdots, ydots, colors, maxiter;
int xposition, yposition, win_xoffset, win_yoffset, xpagesize, ypagesize;
int win_xdots, win_ydots;
unsigned char huge *pixels; /* the device-independent bitmap pixels */
int pixels_per_byte;        /* pixels/byte in the pixmap */
long pixels_per_bytem1;     /* pixels / byte - 1 (for ANDing) */
int pixelshift_per_byte;    /* 0, 1, 2, or 3 */
int bytes_per_pixelline;    /* pixels/line / pixels/byte */
long win_bitmapsize;        /* bitmap size, in bytes */
int bad_code_count = 0; 	 /* needed by decoder module */
extern int rowcount;
static BYTE FAR *cbuf = NULL;
static HANDLE hcbuf = NULL;
#define cbufsize 16384

static int strncmp(BYTE* s1, char* s2, int num)
{				   			 
	while(num--)
	{
		if(*s1++ != *s2++)
			return 1;
	}
	return 0;
}

int get_byte(){	//changed to _lread by mh

// BN - BUG:  n was set to 2 when I called for a second file!!
	if(get_byte_n==0) {
		get_byte_n = _lread(hin,cbuf,cbufsize);
		get_byte_i=0;
		//if(SpinTheMessageLoop())
			//return (-1);
		}
	if(get_byte_n) {
		--get_byte_n;
		return (cbuf[get_byte_i++]);
		}
	else
		return (-1);
}

void putcolor(int x, int y, int color){
	long i;

	i = win_ydots - 1 - y;
	i = (i * win_xdots) + x;

	if (x >= 0 && x < xdots && y >= 0 && y < ydots) {
		if (pixelshift_per_byte == 0) {
			pixels[i] = color % colors;
			}
		else {
			unsigned int j;

			j = (unsigned int)(i & pixels_per_bytem1);
			i = i >> pixelshift_per_byte;
			pixels[i] = (pixels[i] & win_notmask[j]) + 
				 (((unsigned char) (color % colors)) << win_bitshift[j]);
			}
		}
}


int put_line(int rownum, int leftpt, int rightpt, unsigned char *localvalues){
	int i, len;
	long startloc;

	len = rightpt - leftpt;
	if (rightpt >= xdots)
		len = xdots - 1 - leftpt;
	startloc = win_ydots - 1 - rownum;
	startloc = (startloc * win_xdots) + leftpt;

	if (rownum < 0 || rownum >= ydots || leftpt < 0) {
		return (0);
		}

	if (pixelshift_per_byte == 0) {
/*		for (i = 0;  i <= len;  i++)
			pixels[startloc + i] = localvalues[i]; */
		_fmemcpy(pixels+startloc,localvalues,len);	//mh--for efficiency
		}
	else {
		unsigned int j;
		long k;
		for (i = 0;  i <= len;  i++) {
			k = startloc + i;
			j = (unsigned int)(k & pixels_per_bytem1);
			k = k >> pixelshift_per_byte;
			pixels[k] = (pixels[k] & win_notmask[j]) + 
				 (((unsigned char) (localvalues[i] % colors)) << win_bitshift[j]);
			}					//for
		}						//else
	putcolor(leftpt, rownum, localvalues[0]);
	return (1);
}

int out_line(unsigned char *localvalues, int numberofdots){
	return (put_line(rowcount++, 0, numberofdots, localvalues));
}


/* Main entry decoder */

BYTE huge *Gif2Dib(const char *filename){
	unsigned numcolors;
	unsigned char buffer[16];
	unsigned width, finished;
	LPBITMAPINFOHEADER lpbi;
	RGBQUAD FAR       *pRgb;
	int status = 0;
	int i, j, planes;
	DWORD dwBits, dwLen;
   OFSTRUCT	of;
	BYTE huge *pWholeDib;
   /* initialize the row count for write-lines */
	rowcount = 0;

   /* zero out the full write-line */
	for (width = 0;  width < 2049;  width++)
		decoderline[width] = (BYTE)0;

   /* Open the file -- changed to OpenFile by mh */
    hin = OpenFile (filename, (LPOFSTRUCT)&of, OF_READ);
	 if(hin== -1) {	
		//AfxTrace("Can't open file '%ls'", (LPSTR)filename);
		return NULL;						   
		}	   
	 hcbuf=GlobalAlloc(GHND,cbufsize);
	 cbuf=(BYTE FAR *)GlobalLock(hcbuf);
	 if(cbuf==NULL) {
		//AfxTrace("Can't allocate file buffer");
		close_file();
		return NULL;
		}	   

   /* Get the screen description */
	for (i = 0;  i < 13;  i++) {
		if ((buffer[i] = (unsigned char) get_byte()) < 0) {
			close_file();
			return NULL;
			}	   
		}

	if (strncmp(buffer, "GIF87a", 3) || 	/* use updated GIF specs */
		 buffer[3] < '0' || buffer[3] > '9' || 
		 buffer[4] < '0' || buffer[4] > '9' || 
		 buffer[5] < 'A' || buffer[5] > 'z') {
		close_file();
		return NULL;
		}	   

	planes = (buffer[10] & 0xF) + 1;

	if ((buffer[10] & 0x80) == 0)	/* color map (better be!) */
		{
		close_file();
		return NULL;
		}	   
	numcolors = 1 << planes;

	for (i = 0;  i < (int)numcolors;  i++) {
		for (j = 0;  j < 3;  j++) {
			if ((buffer[j] = (unsigned char) get_byte()) < 0) {
				close_file();
				return NULL;
				}	   
			if (dacbox[0][0] != 255)
				dacbox[i][j] = buffer[j] >> 2;
			}
		}

   /* Now display one or more GIF objects */
	finished = 0;
	while (!finished) {
		switch (get_byte()) {
	  case ';':				/* End of the GIF dataset */

			finished = 1;
			status = 0;
			break;

	  case '!':				/* GIF Extension Block */
			get_byte();		/* read (and ignore) the ID */
			while ((i = get_byte()) > 0)	/* get the data length */
				for (j = 0;  j < i;  j++)
					get_byte();	/* flush the data */
			break;

	  case ',':
	 /*
	  * Start of an image object. Read the image description.
	  */

			for (i = 0;  i < 9;  i++) {
				if ((buffer[i] =  get_byte()) < 0) {
					status = -1;
					break;
					}
				}
			if (status < 0) {
				finished = 1;
				break;
				}

			width = buffer[4] | buffer[5] << 8;
			height = buffer[6] | buffer[7] << 8;
// fill in DIB stuff
			xdots = width;
			ydots = height;
			colors = numcolors;
			if (colors > 16)
				colors = 256;
			if (colors > 2 && colors < 16)
				colors = 16;
			win_xdots = (xdots + 3) & 0xFFFC;
			win_ydots = ydots;
			pixelshift_per_byte = 0;
			pixels_per_byte = 1;
			pixels_per_bytem1 = 0;
			if (colors == 16) {
				win_xdots = (xdots + 7) & 0xFFF8;
				pixelshift_per_byte = 1;
				pixels_per_byte = 2;
				pixels_per_bytem1 = 1;
				win_andmask[0] = 0xF0;
				win_notmask[0] = 0xF;
				win_bitshift[0] = 4;
				win_andmask[1] = 0xF;
				win_notmask[1] = 0xF0;
				win_bitshift[1] = 0;
				}
			if (colors == 2) {
				win_xdots = (xdots + 31) & 0xFFE0;
				pixelshift_per_byte = 3;
				pixels_per_byte = 8;
				pixels_per_bytem1 = 7;
				win_andmask[0] = 0x80;
				win_notmask[0] = 0x7F;
				win_bitshift[0] = 7;
				for (i = 1;  i < 8;  i++) {
					win_andmask[i] = win_andmask[i - 1] >> 1;
					win_notmask[i] = (win_notmask[i - 1] >> 1) + 0x80;
					win_bitshift[i] = win_bitshift[i - 1] - 1;
					}
				}
			bytes_per_pixelline = win_xdots >> pixelshift_per_byte;

			hbiCurrent = GlobalAlloc(GHND, (LONG) sizeof(BITMAPINFOHEADER) + 
				 colors * sizeof(RGBQUAD));
			if (!hbiCurrent)
				return NULL;
					   
			lpbi = (LPBITMAPINFOHEADER) GlobalLock(hbiCurrent);
			lpbi->biSize = sizeof(BITMAPINFOHEADER);
			lpbi->biWidth = width;
			lpbi->biHeight = height;
			lpbi->biPlanes = 1;	//nb: NOT equal to planes from GIF
			lpbi->biBitCount = 8 / pixels_per_byte;
			lpbi->biCompression = BI_RGB;
			dwBits = 
				 lpbi->biSizeImage = (DWORD) bytes_per_pixelline * win_ydots;
			lpbi->biXPelsPerMeter = 0;
			lpbi->biYPelsPerMeter = 0;
			lpbi->biClrUsed = colors;
			lpbi->biClrImportant = colors;

			win_bitmapsize = (((long) win_xdots * (long) win_ydots) >> pixelshift_per_byte) + 1;

    			/* fill in intensities for all palette entry colors */

			pRgb = (RGBQUAD FAR *) ((LPSTR) lpbi + lpbi->biSize);
			for (i = 0;  i < colors;  i++) {
				pRgb[i].rgbRed = ((BYTE) dacbox[i][0]) << 2;
				pRgb[i].rgbGreen = ((BYTE) dacbox[i][1]) << 2;
				pRgb[i].rgbBlue = ((BYTE) dacbox[i][2]) << 2;
				}

			dwLen = lpbi->biSize + (DWORD) colors * sizeof(RGBQUAD) + dwBits;
			hdibCurrent = GlobalAlloc(GHND, dwLen);
			if (!hdibCurrent) {
				//AfxTrace("Couldn't create DIB for GIF conversion");
				finished = 1;
				status = -1;
				break;
				}
			pWholeDib = pixels = (unsigned char huge *) GlobalLock(hdibCurrent);
			hmemcpy(pixels, lpbi, (size_t) (dwLen - dwBits));
			GlobalUnlock(hbiCurrent);
			GlobalFree(hbiCurrent);
			lpbi = NULL;
			hbiCurrent = 0;
			pixels += dwLen - dwBits;

			//StartWait();
		//ProfStart();
			decoder(width);	//this does the grunt work
		//ProfStop();
			//EndWait();

			//GlobalUnlock(hdibCurrent);
			finished = 1;
			break;

	  default:
			status = -1;
			finished = 1;
			break;
			}
		}
	close_file();
	
	if(hbiCurrent)
	{
		GlobalUnlock(hbiCurrent);
		GlobalFree(hbiCurrent);
		hbiCurrent = 0;
	}			  
		
	if(status)
	{
		if(hdibCurrent)
		{	
			GlobalUnlock(hdibCurrent);
			GlobalFree(hdibCurrent);
			hdibCurrent = 0;
		}			  	 
		return NULL;
	}	
	else
		return pWholeDib;	
}

static void close_file(){	//changed to _lclose by mh
	_lclose(hin);  
	hin=NULL;
	if(hcbuf) {
		GlobalUnlock(hcbuf);
		GlobalFree(hcbuf);
		cbuf=NULL;
		hcbuf=NULL;
		}
	get_byte_n=0;
	get_byte_i=0;
}	


/* DECODE.C - An LZW decoder for GIF
 * Copyright (C) 1987, by Steven A. Bennett
 *
 * Permission is given by the author to freely redistribute and include
 * this code in any program as long as this credit is given where due.
 *
 * In accordance with the above, I want to credit Steve Wilhite who wrote
 * the code which this is heavily inspired by...
 *
 * GIF and 'Graphics Interchange Format' are trademarks (tm) of
 * Compuserve, Incorporated, an H&R Block Company.
 *
 * Release Notes: This file contains a decoder routine for GIF images
 * which is similar, structurally, to the original routine by Steve Wilhite.
 * It is, however, somewhat noticably faster in most cases.
 *
 == This routine was modified for use in FRACTINT in two ways.
 ==
 == 1) The original #includes were folded into the routine strictly to hold
 ==    down the number of files we were dealing with.
 ==
 == 2) The 'stack', 'suffix', 'prefix', and 'buf' arrays were changed from
 ==    static and 'malloc()'ed to external only so that the assembler
 ==    program could use the same array space for several independent
 ==    chunks of code.	Also, 'stack' was renamed to 'dstack' for TASM
 ==    compatibility.
 ==
 == 3) The 'out_line()' external function has been changed to reference
 ==    '*outln()' for flexibility (in particular, 3D transformations)
 ==
 == 4) A call to 'keypressed()' has been added after the 'outln()' calls
 ==    to check for the presenc of a key-press as a bail-out signal
 ==
 == (Bert Tyler and Timothy Wegner)
 */
#define LOCAL static
#define IMPORT extern

#define FAST register

typedef unsigned short UWORD;
typedef char TEXT;
typedef unsigned char UTINY;
typedef unsigned long ULONG;
typedef int INT;


/* Various error codes used by decoder
 * and my own routines...   It's okay
 * for you to define whatever you want,
 * as long as it's negative...  It will be
 * returned intact up the various subroutine
 * levels...
 */
#define OUT_OF_MEMORY -10
#define BAD_CODE_SIZE -20
#define READ_ERROR -1
#define WRITE_ERROR -2
#define OPEN_ERROR -3
#define CREATE_ERROR -4



/* IMPORT INT get_byte()
 *
 *   - This external (machine specific) function is expected to return
 * either the next byte from the GIF file, or a negative number, as
 * defined in ERRS.H.
 */
//IMPORT INT get_byte();

/* IMPORT INT out_line(pixels, linelen)
 *     UBYTE pixels[];
 *     INT linelen;
 *
 *   - This function takes a full line of pixels (one byte per pixel) and
 * displays them (or does whatever your program wants with them...).  It
 * should return zero, or negative if an error or some other event occurs
 * which would require aborting the decode process...  Note that the length
 * passed will almost always be equal to the line length passed to the
 * decoder function, with the sole exception occurring when an ending code
 * occurs in an odd place in the GIF file...  In any case, linelen will be
 * equal to the number of pixels passed...
 */
//IMPORT INT out_line();
INT (*outln)(UTINY *, INT) = out_line;

/* IMPORT INT bad_code_count;
 *
 * This value is the only other global required by the using program, and
 * is incremented each time an out of range code is read by the decoder.
 * When this value is non-zero after a decode, your GIF file is probably
 * corrupt in some way...
 */
//IMPORT INT bad_code_count;

/* whups, here are more globals, added by PB: */
INT skipxdots; /* 0 to get every dot, 1 for every 2nd, 2 every 3rd, ... */
INT skipydots; /* ditto for rows */

#undef NULL
#define NULL   0L
#define MAX_CODES   4095

/* Static variables */
LOCAL short curr_size;			  /* The current code size */
LOCAL short clear;			  /* Value for a clear code */
LOCAL short ending;			  /* Value for a ending code */
LOCAL short newcodes;			  /* First available code */
LOCAL short top_slot;			  /* Highest code for current size */
LOCAL short slot;			  /* Last read code */

/* The following static variables are used
 * for seperating out codes
 */
LOCAL short navail_bytes = 0;		  /* # bytes left in block */
LOCAL short nbits_left = 0;		  /* # bits left in current byte */
LOCAL UTINY b1; 			  /* Current byte */
LOCAL UTINY byte_buff[257];		  /* Current block, reuse shared mem */
LOCAL UTINY *pbytes;			  /* Pointer to next byte in block */

LOCAL LONG code_mask[13] = {
     0,
     0x0001, 0x0003,
     0x0007, 0x000F,
     0x001F, 0x003F,
     0x007F, 0x00FF,
     0x01FF, 0x03FF,
     0x07FF, 0x0FFF
     };


/* This function initializes the decoder for reading a new image.
 */

LOCAL short init_exp(short size)
{
	curr_size = size + 1;
	top_slot = 1 << curr_size;
	clear = 1 << size;
	ending = clear + 1;
	slot = newcodes = ending + 1;
	navail_bytes = nbits_left = 0;
	return (0);
}

/* get_next_code()
 * - gets the next code from the GIF file.  Returns the code, or else
 * a negative number in case of file errors...
 */

LOCAL short get_next_code(){
	short i, x;
	ULONG ret;

	if (nbits_left == 0) {
		if (navail_bytes <= 0) {

	 /* Out of bytes in current block, so read next block
	  */
			pbytes = byte_buff;
			if ((navail_bytes = get_byte()) < 0)
				return (navail_bytes);
			else if (navail_bytes) {
				for (i = 0;  i < navail_bytes;  ++i) {
					if ((x = get_byte()) < 0)
						return (x);
					byte_buff[i] = (UTINY)x;
					}
				}
			}
		b1 = *pbytes++;
		nbits_left = 8;
		--navail_bytes;
		}

	ret = b1 >> (8 - nbits_left);
	while (curr_size > nbits_left) {
		if (navail_bytes <= 0) {

	 /* Out of bytes in current block, so read next block
	  */
			pbytes = byte_buff;
			if ((navail_bytes = get_byte()) < 0)
				return (navail_bytes);
			else if (navail_bytes) {
				for (i = 0;  i < navail_bytes;  ++i) {
					if ((x = get_byte()) < 0)
						return (x);
					byte_buff[i] = (UTINY)x;
					}
				}
			}
		b1 = *pbytes++;
		ret |= b1 << nbits_left;
		nbits_left += 8;
		--navail_bytes;
		}
	nbits_left -= curr_size;
	ret &= code_mask[curr_size];
	return ((short) (ret));
}


/* The reason we have these seperated like this instead of using
 * a structure like the original Wilhite code did, is because this
 * stuff generally produces significantly faster code when compiled...
 * This code is full of similar speedups...  (For a good book on writing
 * C for speed or for space optomisation, see Efficient C by Tom Plum,
 * published by Plum-Hall Associates...)
 */

/*
I removed the LOCAL identifiers in the arrays below and replaced them
with 'extern's so as to declare (and re-use) the space elsewhere.
The arrays are actually declared in the assembler source.
						Bert Tyler

I put them back -- m heller
*/

UTINY dstack[MAX_CODES + 1];	      /* Stack for storing pixels */
UTINY suffix[MAX_CODES + 1];	      /* Suffix table */
UWORD prefix[MAX_CODES + 1];	      /* Prefix linked list */
//extern UTINY decoderline[2];		      /* decoded line goes here */

/* short decoder(linewidth)
 *    short linewidth;		    * Pixels per line of image *
 *
 * - This function decodes an LZW image, according to the method used
 * in the GIF spec.  Every *linewidth* "characters" (ie. pixels) decoded
 * will generate a call to out_line(), which is a user specific function
 * to display a line of pixels.  The function gets its codes from
 * get_next_code() which is responsible for reading blocks of data and
 * seperating them into the proper size codes.	Finally, get_byte() is
 * the global routine to read the next byte from the GIF file.
 *
 * It is generally a good idea to have linewidth correspond to the actual
 * width of a line (as specified in the Image header) to make your own
 * code a bit simpler, but it isn't absolutely necessary.
 *
 * Returns: 0 if successful, else negative.  (See ERRS.H)
 *
 */

static short decoder(short linewidth){
	FAST UTINY *sp, *bufptr;
	UTINY *buf;
	FAST short code, fc, oc, bufcnt;
	short c, size, ret;
	short xskip, yskip;

   /* Initialize for decoding a new image...
    */
	if ((size = get_byte()) < 0)
		return (size);
	if (size < 2 || 9 < size)
		return (BAD_CODE_SIZE);
	init_exp(size);
	xskip = yskip = 0;

   /* Initialize in case they forgot to put in a clear code.
    * (This shouldn't happen, but we'll try and decode it anyway...)
    */
	oc = fc = 0;

	buf = decoderline;

   /* Set up the stack pointer and decode buffer pointer
    */
	sp = dstack;
	bufptr = buf;
	bufcnt = linewidth;

   /* This is the main loop.  For each code we get we pass through the
    * linked list of prefix codes, pushing the corresponding "character" for
    * each code onto the stack.  When the list reaches a single "character"
    * we push that on the stack too, and then start unstacking each
    * character for output in the correct order.  Special handling is
    * included for the clear code, and the whole thing ends when we get
    * an ending code.
    */

	while ((c = get_next_code()) != ending)
		{

      /* If we had a file error, return without completing the decode
       */
		if (c < 0)
			return (0);

      /* If the code is a clear code, reinitialize all necessary items.
       */
		if (c == clear) {
			curr_size = size + 1;
			slot = newcodes;
			top_slot = 1 << curr_size;

	 /* Continue reading codes until we get a non-clear code
	  * (Another unlikely, but possible case...)
	  */
			while ((c = get_next_code()) == clear)
				;

	 /* If we get an ending code immediately after a clear code
	  * (Yet another unlikely case), then break out of the loop.
	  */
			if (c == ending)
				break;

	 /* Finally, if the code is beyond the range of already set codes,
	  * (This one had better NOT happen...	I have no idea what will
	  * result from this, but I doubt it will look good...) then set it
	  * to color zero.
	  */
			if (c >= slot)
				c = 0;

			oc = fc = c;

	 /* And let us not forget to put the char into the buffer... */
			*sp++ = (UTINY)c;		/* let the common code outside the if else stuff it */
			}
		else 
			{

	 /* In this case, it's not a clear code or an ending code, so
	  * it must be a code code...  So we can now decode the code into
	  * a stack of character codes. (Clear as mud, right?)
	  */
			code = c;

	 /* Here we go again with one of those off chances...  If, on the
	  * off chance, the code we got is beyond the range of those already
	  * set up (Another thing which had better NOT happen...) we trick
	  * the decoder into thinking it actually got the last code read.
	  * (Hmmn... I'm not sure why this works...  But it does...)
	  */
			if (code >= slot) {
				if (code > slot)
					++bad_code_count;
				code = oc;
				*sp++ = (UTINY)fc;
				}

	 /* Here we scan back along the linked list of prefixes, pushing
	  * helpless characters (ie. suffixes) onto the stack as we do so.
	  */
			while (code >= newcodes) {
				*sp++ = suffix[code];
				code = prefix[code];
				}

	 /* Push the last character on the stack, and set up the new
	  * prefix and suffix, and if the required slot number is greater
	  * than that allowed by the current bit size, increase the bit
	  * size.  (NOTE - If we are all full, we *don't* save the new
	  * suffix and prefix...  I'm not certain if this is correct...
	  * it might be more proper to overwrite the last code...
	  */
			*sp++ = (UTINY)code;
			if (slot < top_slot) {
				suffix[slot] = (UTINY)(fc = code);
				prefix[slot++] = oc;
				oc = c;
				}
			if (slot >= top_slot)
				if (curr_size < 12) {
					top_slot <<= 1;
					++curr_size;
					}
			}

      /* Now that we've pushed the decoded string (in reverse order)
       * onto the stack, lets pop it off and put it into our decode
       * buffer...  And when the decode buffer is full, write another
       * line...
       */
		while (sp > dstack) {
			--sp;
			if (--xskip < 0) {
				xskip = skipxdots;
				*bufptr++ = *sp;
				}
			if (--bufcnt == 0)	/* finished an input row? */
				{
				if (--yskip < 0) {
					if ((ret = (*outln)(buf, bufptr - buf)) < 0)
						return (ret);
					yskip = skipydots;
					}
				bufptr = buf;
				bufcnt = linewidth;
				xskip = 0;
				}
			}
		}
   /* PB note that if last line is incomplete, we're not going to try
      to emit it;  original code did, but did so via out_line and therefore
      couldn't have worked well in all cases... */
	return (0);
}

