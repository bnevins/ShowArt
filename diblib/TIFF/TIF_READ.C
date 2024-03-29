#include "stdtif.h"

/*
 * Copyright (c) 1988, 1990 by Sam Leffler.
 * All rights reserved.
 *
 * This file is provided for unrestricted use provided that this
 * legend is included on all tape media and as a part of the
 * software program in whole or part.  Users may copy, modify or
 * distribute this file at will.
 */

/*
 * TIFF Library.
 * Scanline-oriented Read Support
 */

#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))

#if USE_PROTOTYPES
static	TIFFSeek(PTIFF , u_int, u_int);
static	u_long TIFFReadRawStrip1(PTIFF , u_int, u_char huge *, u_long, char []);
static	TIFFFillStrip(PTIFF , u_int);				   			
static	int TIFFStartStrip(PTIFF , u_int);
#else
static	TIFFSeek();
static	u_long TIFFReadRawStrip1();
static	TIFFFillStrip();
static	TIFFStartStrip();
#endif

/*VARARGS3*/
TIFFReadScanline(tif, buf, row, sample)
	register PTIFF tif;
	void huge *buf;
	u_int row, sample;
{
	int e;

	if (tif->tif_mode == O_WRONLY) {
		TIFFError(tif->tif_name, "File not open for reading");
		return (-1);
	}
	if (e = TIFFSeek(tif, row, sample)) {
		/*
		 * Decompress desired row into user buffer
		 */
		e = (*tif->tif_decoderow)(tif, buf, tif->tif_scanlinesize);
		tif->tif_row++;
	}
	return (e ? 1 : -1);
}

/*
 * Seek to a random row+sample in a file.
 */
static
/*VARARGS2*/
TIFFSeek(tif, row, sample)
	register PTIFF tif;
	u_int row, sample;
{
	register TIFFDirectory FAR *td = &tif->tif_dir;
	int strip;

	if (row >= td->td_imagelength) {	/* out of range */
		TIFFError(tif->tif_name, "%d: Row out of range, max %d",
		    row, td->td_imagelength);
		return (0);
	}
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE) {
		if (sample >= td->td_samplesperpixel) {
			TIFFError(tif->tif_name,
			    "%d: Sample out of range, max %d",
			    sample, td->td_samplesperpixel);
			return (0);
		}
		strip = sample*td->td_stripsperimage + row/td->td_rowsperstrip;
	} else
		strip = row / td->td_rowsperstrip;
	if (strip != tif->tif_curstrip) { 	/* different strip, refill */
		if (!TIFFFillStrip(tif, strip))
			return (0);
	} else if (row < tif->tif_row) {
		/*
		 * Moving backwards within the same strip: backup
		 * to the start and then decode forward (below).
		 *
		 * NB: If you're planning on lots of random access within a
		 * strip, it's better to just read and decode the entire
		 * strip, and then access the decoded data in a random fashion.
		 */
		if (!TIFFStartStrip(tif, strip))
			return (0);
	}
	if (row != tif->tif_row) {
		if (tif->tif_seek) {
			/*
			 * Seek forward to the desired row.
			 */
			if (!(*tif->tif_seek)(tif, row - tif->tif_row))
				return (0);
			tif->tif_row = row;
		} else {
			TIFFError(tif->tif_name,
		    "Compression algorithm does not support random access");
			return (0);
		}
	}
	return (1);
}

/*
 * Read a strip of data and decompress the specified
 * amount into the user-supplied buffer.
 */
TIFFReadEncodedStrip(tif, strip, buf, size)
	PTIFF tif;
	u_int strip;
	void huge *buf;
	u_int size;
{
	TIFFDirectory FAR *td = &tif->tif_dir;

	if (tif->tif_mode == O_WRONLY) {
		TIFFError(tif->tif_name, "File not open for reading");
		return (-1);
	}
	if (strip >= td->td_nstrips) {
		TIFFError(tif->tif_name, "%d: Strip out of range, max %d",
		    strip, td->td_nstrips);
		return (-1);
	}
	if (size == (u_int)-1)
		size = td->td_rowsperstrip * tif->tif_scanlinesize;
	else if (size > td->td_rowsperstrip * tif->tif_scanlinesize)
		size = td->td_rowsperstrip * tif->tif_scanlinesize;
	return (TIFFFillStrip(tif, strip) && 
	    (*tif->tif_decoderow)(tif, buf, size) ? size : -1);
}


/*
 * Read a strip of data from the file.
 */
u_long TIFFReadRawStrip(tif, strip, buf, size)
	PTIFF tif;
	u_int strip;
	void huge *buf;
	u_int size;
{
	static char module[] = "TIFFReadRawStrip";
	TIFFDirectory FAR *td = &tif->tif_dir;
	u_long bytecount;
	DWORD dwDibSize = size;

	if (tif->tif_mode == O_WRONLY) {
		TIFFError(tif->tif_name, "File not open for reading");
		return (-1);
	}
	if (strip >= td->td_nstrips) {
		TIFFError(tif->tif_name, "%d: Strip out of range, max %d",
		    strip, td->td_nstrips);
		return (-1);
	}
	bytecount = td->td_stripbytecount[strip];
	if (size == (u_int)-1)
		size = bytecount;
	else if (bytecount > size)
		bytecount = size;
	return (TIFFReadRawStrip1(tif, strip, buf, bytecount, module));
}

#include <windowsx.h>
static u_long
TIFFReadRawStrip1(tif, strip, buf, size, module)
	PTIFF tif;
	u_int strip;
	BYTE huge *buf;
	u_long size;
	char module[];
{
	long dwDibSize = size;
	
	if (!SeekOK(tif->tif_fd, tif->tif_dir.td_stripoffset[strip])) {
		TIFFError(module, "%s: Seek error at scanline %d, strip %d",
		    tif->tif_name, tif->tif_row, strip);
		return (-1);
	}
	
	if(size > 10000)
	{	
		long foo;
		//SeekOK(tif->tif_fd, 0);
		 //BYTE huge * buf;
		//buf = (BYTE huge *) GlobalAllocPtr(GMEM_MOVEABLE, 63000);
		//_lclose(tif->tif_fd);
		//tif->tif_fd = _lopen("test.tif", READ);		   
		foo = _hread(tif->tif_fd, buf, dwDibSize);
		//GlobalFreePtr(buf);
		if (foo != dwDibSize)
		//if (_hread(tif->tif_fd, buf, dwDibSize) != dwDibSize)
		
		{						   			  			   				 
			TIFFError(module, "%s: Read error at scanline %d",
			tif->tif_name, tif->tif_row);
			return (-1);
		}	
	}	
	  
	else if (!ReadOK(tif->tif_fd, buf, size)) {
		TIFFError(module, "%s: Read error at scanline %d",
		    tif->tif_name, tif->tif_row);
		return (-1);
	}
	return (size);
}

/*
 * Read the specified strip and setup for decoding. 
 * The data buffer is expanded, as necessary, to
 * hold the strip's data.
 */
static
TIFFFillStrip(tif, strip)
	PTIFF tif;
	u_int strip;
{
	static char module[] = "TIFFFillStrip";
	TIFFDirectory FAR *td = &tif->tif_dir;
	u_long bytecount;

	/*
	 * Expand raw data buffer, if needed, to
	 * hold data strip coming from file
	 * (perhaps should set upper bound on
	 *  the size of a buffer we'll use?).
	 */
	bytecount = td->td_stripbytecount[strip];
	if (bytecount > tif->tif_rawdatasize) 
	{
		tif->tif_curstrip = -1;		/* unknown state */
		if (tif->tif_rawdata) 
		{
			GlobalFreePtr(tif->tif_rawdata);
			tif->tif_rawdata = NULL;
		}
		tif->tif_rawdatasize = roundup(bytecount, 1024);
		
		
		tif->tif_rawdata = (BYTE huge *) GlobalAllocPtr(GMEM_MOVEABLE, tif->tif_rawdatasize);
						   
		if (tif->tif_rawdata == NULL) {
			TIFFError(module,
			    "%s: No space for data buffer at scanline %d",
			    tif->tif_name, tif->tif_row);
			tif->tif_rawdatasize = 0;
			return (0);
		}
	}
	if (TIFFReadRawStrip1(tif,
	    strip, tif->tif_rawdata, bytecount, module) != bytecount)
		return (0);
	if (td->td_fillorder != tif->tif_fillorder &&
	    (tif->tif_flags & TIFF_NOBITREV) == 0)
		TIFFReverseBits(tif->tif_rawdata, bytecount);
	return (TIFFStartStrip(tif, strip));
}

/*
 * Set state to appear as if a
 * strip has just been read in.
 */
static
int TIFFStartStrip(tif, strip)
	PTIFF tif;
	u_int strip;
{
	TIFFDirectory FAR *td = &tif->tif_dir;

	tif->tif_curstrip = strip;
	tif->tif_row = (strip % td->td_stripsperimage) * td->td_rowsperstrip;
	tif->tif_rawcp = tif->tif_rawdata;
	tif->tif_rawcc = td->td_stripbytecount[strip];
	return (tif->tif_stripdecode == NULL || (*tif->tif_stripdecode)(tif));
}
