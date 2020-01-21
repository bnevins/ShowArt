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
 *
 * "Null" Compression Algorithm Support.
 */

#if USE_PROTOTYPES
static	int DumpModeEncode(PTIFF , u_char *, int);
static	int DumpModeDecode(PTIFF , BYTE huge *, int);
static	int DumpModeSeek(PTIFF , int);
#else
static	int DumpModeEncode(), DumpModeDecode(), DumpModeSeek();
#endif
#pragma alloc_text(TIFF1,TIFFInitDumpMode,DumpModeEncode,DumpModeDecode,DumpModeSeek)

/*
 * Initialize dump mode.
 */
TIFFInitDumpMode(tif)
	register PTIFF tif;
{
	tif->tif_decoderow = DumpModeDecode;
	tif->tif_encoderow = DumpModeEncode;
	tif->tif_seek = DumpModeSeek;
	return (1);
}

/*
 * Encode a scanline of pixels.
 */
static int
DumpModeEncode(tif, pp, cc)
	register PTIFF tif;
	u_char *pp;
	int cc;
{
	if (tif->tif_rawcc + cc > tif->tif_rawdatasize)
		if (!TIFFFlushData(tif))
			return (-1);
	bcopy(pp, tif->tif_rawcp, cc);
	if (tif->tif_flags & TIFF_SWAB) {
		switch (tif->tif_dir.td_bitspersample) {
		case 16:
			assert((cc & 3) == 0);
			TIFFSwabArrayOfShort((u_short FAR *)tif->tif_rawcp, cc/2);
			break;
		case 32:
			assert((cc & 15) == 0);
			TIFFSwabArrayOfLong((u_long FAR *)tif->tif_rawcp, cc/4);
			break;
		}
	}
	tif->tif_rawcp += cc;
	tif->tif_rawcc += cc;
	return (1);
}

/*
 * Decode a scanline of pixels.
 */
static int
DumpModeDecode(tif, buf, cc)
	register PTIFF tif;
	BYTE huge *buf;
	int cc;
{
	if (tif->tif_rawcc < cc) {
		TIFFError(tif->tif_name,
		    "DumpModeDecode: Not enough data for scanline %d",
		    tif->tif_row);
		return (0);
	}
	bcopy(tif->tif_rawcp, buf, cc);
	if (tif->tif_flags & TIFF_SWAB) {
		switch (tif->tif_dir.td_bitspersample) {
		case 16:
			assert((cc & 3) == 0);
			TIFFSwabArrayOfShort((u_short *)buf, cc/2);
			break;
		case 32:
			assert((cc & 15) == 0);
			TIFFSwabArrayOfLong((u_long *)buf, cc/4);
			break;
		}
	}
	tif->tif_rawcp += cc;
	tif->tif_rawcc -= cc;
	return (1);
}

/*
 * Seek forwards nrows in the current strip.
 */
static int
DumpModeSeek(tif, nrows)
	register PTIFF tif;
	int nrows;
{
	tif->tif_rawcp += nrows * tif->tif_scanlinesize;
	tif->tif_rawcc -= nrows * tif->tif_scanlinesize;
	return (1);
}
