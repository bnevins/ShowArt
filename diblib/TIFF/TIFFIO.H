/* tiffio.h 1.27	90/05/22 */

/*
 * Copyright (c) 1988, 1990 by Sam Leffler.
 * All rights reserved.
 *
 * This file is provided for unrestricted use provided that this
 * legend is included on all tape media and as a part of the
 * software program in whole or part.	Users may copy, modify or
 * distribute this file at will.
 */

#ifndef _TIFFIO_
#define	_TIFFIO_

#ifdef WIN32
//#define hmemcpy memcpy
#define huge
#define _far
//#define FAR
#endif

/* Option selections, M. Heller, June 1991 */
#define BSDTYPES			 1
#define USE_PROTOTYPES	 1
#define USE_VARARGS		 0
//#define __STDC__			1
#define _WINDOWS			 1
#ifndef MSDOS
#define MSDOS				 1
#endif
#ifndef M_I86
#define M_I86	 1
#endif
/*
 * TIFF I/O Library Definitions.
 */
#include "tiffcomp.h"
#include "tiff.h"

/* Windows mods, M. Heller, June 1991 */

#ifdef _WINDOWS

#ifndef FAR
#define NOCOMM
#define NOKANJI
#undef NULL
#undef TRUE
#undef FALSE
#include <windows.h>
#include <windowsx.h>
#endif

#include <assert.h>
#define exit(X) assert(1)
#undef malloc
#undef free
#undef strcpy
#undef strlen
#undef realloc
#define malloc(a) _fmalloc(a)
#define realloc(a,b) _frealloc(a,b)
#define free(a) _ffree(a)
#define strcpy(a,b) lstrcpy(a,b)
#define strlen(a) lstrlen(a)

#else

#define FAR

#endif

#define PTIFF TIFF FAR *


/*
 * Internal format of a TIFF directory entry.
 */
typedef	struct {
		  u_short  td_subfiletype;
		  u_short  td_imagewidth, td_imagelength;
		  u_short  td_bitspersample;
		  u_short  td_compression;
		  u_short  td_photometric;
		  u_short  td_threshholding;
		  u_short  td_fillorder;
		  u_short  td_orientation;
		  u_short  td_samplesperpixel;
		  u_short  td_predictor;
		  u_long   td_rowsperstrip;
		  u_long   td_minsamplevalue, td_maxsamplevalue;  /* maybe float? */
		  float td_xresolution, td_yresolution;
		  u_short  td_resolutionunit;
		  u_short  td_planarconfig;
		  float td_xposition, td_yposition;
		  u_long   td_group3options;
		  u_long   td_group4options;
		  u_short  td_pagenumber[2];
		  u_short  td_grayresponseunit;
		  u_short  td_colorresponseunit;
		  u_short  td_matteing;
		  u_short  td_cleanfaxdata;
		  u_short  td_badfaxrun;
		  u_long   td_badfaxlines;
		  u_short  FAR *td_grayresponsecurve; /* u_short for now (maybe float?) */
		  u_short  FAR *td_redresponsecurve;  /* u_short for now (maybe float?) */
		  u_short  FAR *td_greenresponsecurve; /* u_short for now (maybe float?) */
		  u_short  FAR *td_blueresponsecurve; /* u_short for now (maybe float?) */
		  u_short  FAR *td_redcolormap;
		  u_short  FAR *td_greencolormap;
		  u_short  FAR *td_bluecolormap;
#ifdef notdef
/* not yet used/supported */
		  float td_whitepoint[2];
		  float td_primarychromaticities[6];
#endif
		  char	FAR *td_documentname;
		  char	FAR *td_artist;
		  char	FAR *td_datetime;
		  char	FAR *td_hostcomputer;
		  char	FAR *td_imagedescription;
		  char	FAR *td_make;
		  char	FAR *td_model;
		  char	FAR *td_software;
		  char	FAR *td_pagename;
		  u_long   td_fieldsset[2];  /* bit vector of fields that are set */
		  u_long   td_stripsperimage;
		  u_long   td_nstrips;	  /* size of offset & bytecount arrays */
		  u_long 	FAR *td_stripoffset;
		  u_long 	FAR *td_stripbytecount;
} TIFFDirectory;

/*
 * Field flags used to indicate fields that have
 * been set in a directory, and to reference fields
 * when manipulating a directory.
 */
/* multi-entry fields */
#define	FIELD_IMAGEDIMENSIONS		0
#define	FIELD_CELLDIMENSIONS 	1		/* XXX */
#define	FIELD_RESOLUTION		2
#define	FIELD_POSITION 		3
/* single-entry fields */
#define	FIELD_SUBFILETYPE 	4
#define	FIELD_BITSPERSAMPLE		5
#define	FIELD_COMPRESSION 	6
#define	FIELD_PHOTOMETRIC 	7
#define	FIELD_THRESHHOLDING		8
#define	FIELD_FILLORDER			9		/* XXX */
#define	FIELD_DOCUMENTNAME		10
#define	FIELD_IMAGEDESCRIPTION		11
#define	FIELD_MAKE			12
#define	FIELD_MODEL 		13
#define	FIELD_ORIENTATION 	14
#define	FIELD_SAMPLESPERPIXEL		15
#define	FIELD_ROWSPERSTRIP		16
#define	FIELD_MINSAMPLEVALUE 	17
#define	FIELD_MAXSAMPLEVALUE 	18
#define	FIELD_PLANARCONFIG		19
#define	FIELD_PAGENAME 		20
#define	FIELD_GRAYRESPONSEUNIT		21
#define	FIELD_GRAYRESPONSECURVE 	22
#define	FIELD_GROUP3OPTIONS		23
#define	FIELD_GROUP4OPTIONS		24
#define	FIELD_RESOLUTIONUNIT 	25
#define	FIELD_PAGENUMBER		26
#define	FIELD_COLORRESPONSEUNIT 	27
#define	FIELD_COLORRESPONSECURVE	28
#define	FIELD_STRIPBYTECOUNTS		29
#define	FIELD_STRIPOFFSETS		31
#define	FIELD_COLORMAP 		32
#define FIELD_PREDICTOR 		33
#define FIELD_ARTIST 		34
#define FIELD_DATETIME			35
#define FIELD_HOSTCOMPUTER 	36
#define FIELD_SOFTWARE			37
#define	FIELD_MATTEING 		38
#define	FIELD_BADFAXLINES 	39
#define	FIELD_CLEANFAXDATA		40
#define	FIELD_BADFAXRUN			41
#define	FIELD_LAST			FIELD_BADFAXRUN

#define	TIFFFieldSet(tif, field) \
			((tif)->tif_dir.td_fieldsset[field/32] & (1L<<(field&0x1f)))
#define	TIFFSetFieldBit(tif, field) \
			((tif)->tif_dir.td_fieldsset[field/32] |= (1L<<(field&0x1f)))

typedef	struct {
		  char	FAR *tif_name; 	  /* name of open file */
		  short tif_fd;		  /* open file descriptor */
		  short tif_mode; 	  /* open mode (O_*) */
		  char  tif_fillorder;	  /* natural bit fill order for machine */
		  char  tif_options; 	  /* compression-specific options */
		  short tif_flags;
#define	TIFF_DIRTYHEADER	0x1	/* header must be written on close */
#define	TIFF_DIRTYDIRECT	0x2	/* current directory must be written */
#define	TIFF_BUFFERSETUP	0x4	/* data buffers setup */
#define	TIFF_BEENWRITING	0x8	/* written 1+ scanlines to file */
#define	TIFF_SWAB		0x10	/* byte swap file information */
#define	TIFF_NOBITREV		0x20	/* inhibit bit reversal logic */
		  long  tif_diroff;	  /* file offset of current directory */
		  long  tif_nextdiroff; 	  /* file offset of following directory */
		  TIFFDirectory tif_dir;	  /* internal rep of current directory */
		  TIFFHeader tif_header;	  /* file's header block */
		  int   tif_typeshift[6]; /* data type shift counts */
		  long  tif_typemask[6];  /* data type masks */
		  long  tif_row;	  /* current scanline */
		  int   tif_curstrip;	  /* current strip for read/write */
		  long  tif_curoff;	  /* current offset for read/write */
/* compression scheme hooks */
		  int   (*tif_stripdecode)();   /* strip decoding routine (pre) */
		  int   (*tif_decoderow)();  /* scanline decoding routine */
		  int   (*tif_stripencode)();   /* strip encoding routine (pre) */
		  int   (*tif_encoderow)();  /* scanline encoding routine */
		  int   (*tif_encodestrip)();   /* strip encoding routine (post) */
		  int   (*tif_close)(); 	  /* cleanup-on-close routine */
		  int   (*tif_seek)();	  /* position within a strip routine */
		  int   (*tif_cleanup)(); /* routine called to cleanup state */
		  char  FAR *tif_data;	  /* compression scheme private data */
/* input/output buffering */
		  int   tif_scanlinesize; /* # of bytes in a scanline */
		  BYTE huge *tif_rawdata;	  /* raw data buffer */
		  long  tif_rawdatasize;  /* # of bytes in raw data buffer */
		  BYTE huge  *tif_rawcp;   /* current spot in raw buffer */
		  long  tif_rawcc;	  /* bytes unread from raw buffer */
} TIFF;

/* generic option bit names */
#define	TIFF_OPT0	0x1
#define	TIFF_OPT1	0x2
#define	TIFF_OPT2	0x4
#define	TIFF_OPT3	0x8
#define	TIFF_OPT4	0x10
#define	TIFF_OPT5	0x20
#define	TIFF_OPT6	0x40
#define	TIFF_OPT7	0x80

#ifndef NULL
#define	NULL	0
#endif

extern u_char TIFFBitRevTable[256];
extern u_char TIFFNoBitRevTable[256];

#if defined(c_plusplus) || defined(__cplusplus) || defined(__STDC__) || USE_PROTOTYPES
#if defined(__cplusplus)
extern "C" {
#endif
extern	void TIFFClose(PTIFF);
extern	int TIFFFlush(PTIFF);
extern	int TIFFFlushData(PTIFF);
extern	int TIFFGetField(PTIFF, int, ...);
extern	int TIFFReadDirectory(PTIFF);
extern	int TIFFScanlineSize(PTIFF);
extern	int TIFFSetDirectory(PTIFF, int);
extern	int TIFFSetField(PTIFF, long, ...);
extern	int TIFFWriteDirectory(PTIFF);
#if defined(c_plusplus) || defined(__cplusplus)
extern	PTIFF TIFFOpen(const char *, const char *);
extern	void TIFFError(const char FAR *, const char *, ...);
extern	void TIFFWarning(const char FAR *, const char *, ...);
extern	void TIFFPrintDirectory(PTIFF, FILE *, int = 0, int = 0, int = 0);
extern	int TIFFReadScanline(PTIFF, u_char FAR *, u_int, u_int = 0);
extern	int TIFFWriteScanline(PTIFF, u_char FAR *, u_int, u_int = 0);
#else
extern	PTIFF TIFFOpen(char *, char *);
extern	void TIFFError(char FAR *, char *, ...);
extern	void TIFFWarning(char FAR *, char *, ...);
extern	void TIFFPrintDirectory(PTIFF, FILE *, int, int, int);
extern	int TIFFReadScanline(PTIFF, u_char FAR *, u_int, u_int);
extern	int TIFFWriteScanline(PTIFF, u_char FAR *, u_int, u_int);
#endif
extern	int TIFFReadEncodedStrip(PTIFF, u_int, u_char  FAR *, u_int);
extern	u_long TIFFReadRawStrip(PTIFF, u_int, u_char  FAR *, u_int);
extern	int TIFFWriteEncodedStrip(PTIFF, u_int, u_char	FAR *, u_int);
extern	int TIFFWriteRawStrip(PTIFF, u_int, u_char  FAR *, u_int);
#if defined(__cplusplus)
}
#endif
#else
extern	void TIFFClose();
extern	PTIFF TIFFOpen();
extern	void TIFFError();
extern	int TIFFFlush();
extern	int TIFFFlushData();
extern	int TIFFGetField();
extern	void TIFFPrintDirectory();
extern	int TIFFReadDirectory();
extern	int TIFFReadScanline();
extern	int TIFFScanlineSize();
extern	int TIFFSetDirectory();
extern	int TIFFSetField();
extern	void TIFFWarning();
extern	int TIFFWriteDirectory();
extern	int TIFFWriteScanline();
#endif
#endif
