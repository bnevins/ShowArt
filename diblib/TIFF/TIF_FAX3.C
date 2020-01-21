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
 * CCITT Group 3 Facsimile-compatible
 * Compression Scheme Support.
 *
 * This stuff is derived from code by Paul Haeberli and
 * Jef Pozkanzer.
 */
#define	NDEBUG			/* performance tradeoff */

/* 
 * 	fax.h
 *
 * 	Copyright (C) 1989 by Jef Poskanzer.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  This software is provided "as is" without express or
 * implied warranty.
 */
/*
 * The code+length are placed next to each other and
 * first in the structure so that comparisons can be
 * done 32-bits at a time (instead of as two 16-bit
 * compares) -- on machines where it is worthwhile.
 */
typedef struct tableentry {
    short code;
    short length;
    short count;
    short tabid;
} tableentry;

/*
 * Table identifiers are coded so that the decoding
 * algorithm can check the sign bit to decide if the
 * current code is "complete".
 */
#define TWTABLE		-2
#define TBTABLE		-1
#define VRTABLE		0
#define MWTABLE		1
#define MBTABLE		2
#define EXTABLE		3
#define	MINTABID	TWTABLE
#define	MAXTABID	EXTABLE

static struct tableentry twtable[] = {
    { 0x35, 8, 0, TWTABLE },
    { 0x7, 6, 1, TWTABLE },
    { 0x7, 4, 2, TWTABLE },
    { 0x8, 4, 3, TWTABLE },
    { 0xb, 4, 4, TWTABLE },
    { 0xc, 4, 5, TWTABLE },
    { 0xe, 4, 6, TWTABLE },
    { 0xf, 4, 7, TWTABLE },
    { 0x13, 5, 8, TWTABLE },
    { 0x14, 5, 9, TWTABLE },
    { 0x7, 5, 10, TWTABLE },
    { 0x8, 5, 11, TWTABLE },
    { 0x8, 6, 12, TWTABLE },
    { 0x3, 6, 13, TWTABLE },
    { 0x34, 6, 14, TWTABLE },
    { 0x35, 6, 15, TWTABLE },
    { 0x2a, 6, 16, TWTABLE },
    { 0x2b, 6, 17, TWTABLE },
    { 0x27, 7, 18, TWTABLE },
    { 0xc, 7, 19, TWTABLE },
    { 0x8, 7, 20, TWTABLE },
    { 0x17, 7, 21, TWTABLE },
    { 0x3, 7, 22, TWTABLE },
    { 0x4, 7, 23, TWTABLE },
    { 0x28, 7, 24, TWTABLE },
    { 0x2b, 7, 25, TWTABLE },
    { 0x13, 7, 26, TWTABLE },
    { 0x24, 7, 27, TWTABLE },
    { 0x18, 7, 28, TWTABLE },
    { 0x2, 8, 29, TWTABLE },
    { 0x3, 8, 30, TWTABLE },
    { 0x1a, 8, 31, TWTABLE },
    { 0x1b, 8, 32, TWTABLE },
    { 0x12, 8, 33, TWTABLE },
    { 0x13, 8, 34, TWTABLE },
    { 0x14, 8, 35, TWTABLE },
    { 0x15, 8, 36, TWTABLE },
    { 0x16, 8, 37, TWTABLE },
    { 0x17, 8, 38, TWTABLE },
    { 0x28, 8, 39, TWTABLE },
    { 0x29, 8, 40, TWTABLE },
    { 0x2a, 8, 41, TWTABLE },
    { 0x2b, 8, 42, TWTABLE },
    { 0x2c, 8, 43, TWTABLE },
    { 0x2d, 8, 44, TWTABLE },
    { 0x4, 8, 45, TWTABLE },
    { 0x5, 8, 46, TWTABLE },
    { 0xa, 8, 47, TWTABLE },
    { 0xb, 8, 48, TWTABLE },
    { 0x52, 8, 49, TWTABLE },
    { 0x53, 8, 50, TWTABLE },
    { 0x54, 8, 51, TWTABLE },
    { 0x55, 8, 52, TWTABLE },
    { 0x24, 8, 53, TWTABLE },
    { 0x25, 8, 54, TWTABLE },
    { 0x58, 8, 55, TWTABLE },
    { 0x59, 8, 56, TWTABLE },
    { 0x5a, 8, 57, TWTABLE },
    { 0x5b, 8, 58, TWTABLE },
    { 0x4a, 8, 59, TWTABLE },
    { 0x4b, 8, 60, TWTABLE },
    { 0x32, 8, 61, TWTABLE },
    { 0x33, 8, 62, TWTABLE },
    { 0x34, 8, 63, TWTABLE },
};

static struct tableentry mwtable[] = {
    { 0x1b, 5, 64, MWTABLE },
    { 0x12, 5, 128, MWTABLE },
    { 0x17, 6, 192, MWTABLE },
    { 0x37, 7, 256, MWTABLE },
    { 0x36, 8, 320, MWTABLE },
    { 0x37, 8, 384, MWTABLE },
    { 0x64, 8, 448, MWTABLE },
    { 0x65, 8, 512, MWTABLE },
    { 0x68, 8, 576, MWTABLE },
    { 0x67, 8, 640, MWTABLE },
    { 0xcc, 9, 704, MWTABLE },
    { 0xcd, 9, 768, MWTABLE },
    { 0xd2, 9, 832, MWTABLE },
    { 0xd3, 9, 896, MWTABLE },
    { 0xd4, 9, 960, MWTABLE },
    { 0xd5, 9, 1024, MWTABLE },
    { 0xd6, 9, 1088, MWTABLE },
    { 0xd7, 9, 1152, MWTABLE },
    { 0xd8, 9, 1216, MWTABLE },
    { 0xd9, 9, 1280, MWTABLE },
    { 0xda, 9, 1344, MWTABLE },
    { 0xdb, 9, 1408, MWTABLE },
    { 0x98, 9, 1472, MWTABLE },
    { 0x99, 9, 1536, MWTABLE },
    { 0x9a, 9, 1600, MWTABLE },
    { 0x18, 6, 1664, MWTABLE },
    { 0x9b, 9, 1728, MWTABLE },
};

static struct tableentry tbtable[] = {
    { 0x37, 10, 0, TBTABLE },
    { 0x2, 3, 1, TBTABLE },
    { 0x3, 2, 2, TBTABLE },
    { 0x2, 2, 3, TBTABLE },
    { 0x3, 3, 4, TBTABLE },
    { 0x3, 4, 5, TBTABLE },
    { 0x2, 4, 6, TBTABLE },
    { 0x3, 5, 7, TBTABLE },
    { 0x5, 6, 8, TBTABLE },
    { 0x4, 6, 9, TBTABLE },
    { 0x4, 7, 10, TBTABLE },
    { 0x5, 7, 11, TBTABLE },
    { 0x7, 7, 12, TBTABLE },
    { 0x4, 8, 13, TBTABLE },
    { 0x7, 8, 14, TBTABLE },
    { 0x18, 9, 15, TBTABLE },
    { 0x17, 10, 16, TBTABLE },
    { 0x18, 10, 17, TBTABLE },
    { 0x8, 10, 18, TBTABLE },
    { 0x67, 11, 19, TBTABLE },
    { 0x68, 11, 20, TBTABLE },
    { 0x6c, 11, 21, TBTABLE },
    { 0x37, 11, 22, TBTABLE },
    { 0x28, 11, 23, TBTABLE },
    { 0x17, 11, 24, TBTABLE },
    { 0x18, 11, 25, TBTABLE },
    { 0xca, 12, 26, TBTABLE },
    { 0xcb, 12, 27, TBTABLE },
    { 0xcc, 12, 28, TBTABLE },
    { 0xcd, 12, 29, TBTABLE },
    { 0x68, 12, 30, TBTABLE },
    { 0x69, 12, 31, TBTABLE },
    { 0x6a, 12, 32, TBTABLE },
    { 0x6b, 12, 33, TBTABLE },
    { 0xd2, 12, 34, TBTABLE },
    { 0xd3, 12, 35, TBTABLE },
    { 0xd4, 12, 36, TBTABLE },
    { 0xd5, 12, 37, TBTABLE },
    { 0xd6, 12, 38, TBTABLE },
    { 0xd7, 12, 39, TBTABLE },
    { 0x6c, 12, 40, TBTABLE },
    { 0x6d, 12, 41, TBTABLE },
    { 0xda, 12, 42, TBTABLE },
    { 0xdb, 12, 43, TBTABLE },
    { 0x54, 12, 44, TBTABLE },
    { 0x55, 12, 45, TBTABLE },
    { 0x56, 12, 46, TBTABLE },
    { 0x57, 12, 47, TBTABLE },
    { 0x64, 12, 48, TBTABLE },
    { 0x65, 12, 49, TBTABLE },
    { 0x52, 12, 50, TBTABLE },
    { 0x53, 12, 51, TBTABLE },
    { 0x24, 12, 52, TBTABLE },
    { 0x37, 12, 53, TBTABLE },
    { 0x38, 12, 54, TBTABLE },
    { 0x27, 12, 55, TBTABLE },
    { 0x28, 12, 56, TBTABLE },
    { 0x58, 12, 57, TBTABLE },
    { 0x59, 12, 58, TBTABLE },
    { 0x2b, 12, 59, TBTABLE },
    { 0x2c, 12, 60, TBTABLE },
    { 0x5a, 12, 61, TBTABLE },
    { 0x66, 12, 62, TBTABLE },
    { 0x67, 12, 63, TBTABLE },
};

static struct tableentry mbtable[] = {
    { 0xf, 10, 64, MBTABLE },
    { 0xc8, 12, 128, MBTABLE },
    { 0xc9, 12, 192, MBTABLE },
    { 0x5b, 12, 256, MBTABLE },
    { 0x33, 12, 320, MBTABLE },
    { 0x34, 12, 384, MBTABLE },
    { 0x35, 12, 448, MBTABLE },
    { 0x6c, 13, 512, MBTABLE },
    { 0x6d, 13, 576, MBTABLE },
    { 0x4a, 13, 640, MBTABLE },
    { 0x4b, 13, 704, MBTABLE },
    { 0x4c, 13, 768, MBTABLE },
    { 0x4d, 13, 832, MBTABLE },
    { 0x72, 13, 896, MBTABLE },
    { 0x73, 13, 960, MBTABLE },
    { 0x74, 13, 1024, MBTABLE },
    { 0x75, 13, 1088, MBTABLE },
    { 0x76, 13, 1152, MBTABLE },
    { 0x77, 13, 1216, MBTABLE },
    { 0x52, 13, 1280, MBTABLE },
    { 0x53, 13, 1344, MBTABLE },
    { 0x54, 13, 1408, MBTABLE },
    { 0x55, 13, 1472, MBTABLE },
    { 0x5a, 13, 1536, MBTABLE },
    { 0x5b, 13, 1600, MBTABLE },
    { 0x64, 13, 1664, MBTABLE },
    { 0x65, 13, 1728, MBTABLE },
};

static struct tableentry extable[] = {
    { 0x8, 11, 1792, EXTABLE },
    { 0xc, 11, 1856, EXTABLE },
    { 0xd, 11, 1920, EXTABLE },
    { 0x12, 12, 1984, EXTABLE },
    { 0x13, 12, 2048, EXTABLE },
    { 0x14, 12, 2112, EXTABLE },
    { 0x15, 12, 2176, EXTABLE },
    { 0x16, 12, 2240, EXTABLE },
    { 0x17, 12, 2304, EXTABLE },
    { 0x1c, 12, 2368, EXTABLE },
    { 0x1d, 12, 2432, EXTABLE },
    { 0x1e, 12, 2496, EXTABLE },
    { 0x1f, 12, 2560, EXTABLE },
};

#ifdef notdef		/* not doing 2D yet...*/
#define HORIZ	1000
#define PASS	2000

static struct tableentry vrtable[] = {
    { 0x3, 7, -3, VRTABLE },
    { 0x3, 6, -2, VRTABLE },
    { 0x3, 3, -1, VRTABLE },
    { 0x1, 1,  0, VRTABLE },
    { 0x2, 3,  1, VRTABLE },
    { 0x2, 6,  2, VRTABLE },
    { 0x2, 7,  3, VRTABLE },
    { 0x1, 3,  HORIZ, VRTABLE },
    { 0x1, 4,  PASS, VRTABLE },
};
#endif

#define WHASHA	3510
#define WHASHB	1178

#define BHASHA	293
#define BHASHB	2695

#define	FAX3_CLASSF	TIFF_OPT0	/* use Class F protocol */

typedef struct {
	int	data;
	int	bit;
	u_char*	bitmap;
#define HASHSIZE	1021
	tableentry *whash[HASHSIZE];
	tableentry *bhash[HASHSIZE];
} Fax3DecodeState;

typedef struct {
	int	data;
	int	bit;
	u_char*	bitmap;
	u_char	*wruns;
	u_char	*bruns;
} Fax3EncodeState;

#define	EOL	0x1			/* 11 bits of zero, followed by 1 */

#if USE_PROTOTYPES
static	Fax3PreDecode(PTIFF );
static	Fax3Decode(PTIFF, char *, int);
static	int Fax3DecodeRow(PTIFF, u_char *, int);
static	Fax3PreEncode(PTIFF );
static	Fax3PostEncode(PTIFF );
static	Fax3Encode(PTIFF, u_char *, int);
static	Fax3Close(PTIFF );
static	Fax3Cleanup(PTIFF );
#else
static	int Fax3PreEncode(), Fax3Encode(), Fax3PostEncode();
static	int Fax3Decode(), Fax3PreDecode(), Fax3DecodeRow();
static	int Fax3Close(), Fax3Cleanup();
#endif

TIFFInitCCITTFax3(tif)
	PTIFF tif;
{
	tif->tif_stripdecode = Fax3PreDecode;
	tif->tif_decoderow = Fax3Decode;
	tif->tif_stripencode = Fax3PreEncode;
	tif->tif_encoderow = Fax3Encode;
	tif->tif_encodestrip = Fax3PostEncode;
	tif->tif_close = Fax3Close;
	tif->tif_cleanup = Fax3Cleanup;
	tif->tif_options |= FAX3_CLASSF;	/* default */
	tif->tif_flags |= TIFF_NOBITREV;	/* we handle bit reversal */
	return (1);
}

TIFFModeCCITTFax3(tif, isClassF)
	PTIFF tif;
	int isClassF;
{
	if (isClassF)
		tif->tif_options |= FAX3_CLASSF;
	else
		tif->tif_options &= ~FAX3_CLASSF;
}

static int
addtohash(hash, te, n, a, b)
	tableentry *hash[];
	tableentry *te;
	int n, a, b;
{
	while (n--) {
		u_int pos = ((te->length + a) * (te->code + b)) % HASHSIZE;
		if (hash[pos] != 0) {
			TIFFError("Fax3(addtohash)", "Fatal hash collision");
			return (0);
		}
		hash[pos] = te;
		te++;
	}
	return (1);
}

static void
skiptoeol(tif)
	PTIFF tif;
{
	Fax3DecodeState FAR *sp = (Fax3DecodeState FAR *)tif->tif_data;
	register int bit = sp->bit;
	register int data = sp->data;
	int code, len;

	do {
		code = len = 0;
		do {
			if (bit == 0) {
				if (tif->tif_rawcc <= 0) {
					TIFFError(tif->tif_name,
				    "skiptoeol: Premature EOF at scanline %d",
					    tif->tif_row);
					return;
				}
				bit = 0x80;
				data = sp->bitmap[*(u_char FAR *)tif->tif_rawcp++];
			}
			code <<= 1;
			if (data & bit)
				code |= 1;
			bit >>= 1;
			len++;
		} while (code <= 0);
	} while (len < 12 || code != EOL);
	sp->bit = bit;
	sp->data = data;
}

/*
 * Setup state for decoding a strip.
 */
static
Fax3PreDecode(tif)
	PTIFF tif;
{
	register Fax3DecodeState FAR *sp = (Fax3DecodeState FAR *)tif->tif_data;
	TIFFDirectory FAR *td = &tif->tif_dir;

	if (td->td_group3options & GROUP3OPT_2DENCODING) {
		TIFFError(tif->tif_name, "2-D encoding is not supported");
		return (0);
	}
	if (td->td_bitspersample != 1) {
		TIFFError(tif->tif_name,
		    "Bits/sample must be 1 for Group 3 encoding");
		return (0);
	}
	if (sp == NULL) {
		tif->tif_data = malloc(sizeof (Fax3DecodeState));
		if (tif->tif_data == NULL) {
			TIFFError(tif->tif_name,
			    "No space for Fax3 state block");
			return (0);
		}
		sp = (Fax3DecodeState FAR *)tif->tif_data;
		bzero(sp->whash, HASHSIZE*sizeof (tableentry *));
#define	TABSIZE(tab)	(sizeof (tab) / sizeof (tab[0]))
#define	WHITE(tab)	addtohash(sp->whash, tab, TABSIZE(tab), WHASHA, WHASHB)
#define	BLACK(tab)	addtohash(sp->bhash, tab, TABSIZE(tab), BHASHA, BHASHB)
		if (!WHITE(twtable) || !WHITE(mwtable) || !WHITE(extable))
			return (0);
		bzero(sp->bhash, HASHSIZE*sizeof (tableentry *));
		if (!BLACK(tbtable) || !BLACK(mbtable) || !BLACK(extable))
			return (0);
#undef BLACK
#undef WHITE
#undef TABSIZE
		sp->bitmap = (tif->tif_fillorder != td->td_fillorder ? 
		    TIFFBitRevTable : TIFFNoBitRevTable);
	}
	sp->bit = 0;				/* force initial read */
	sp->data = 0;
	if ((tif->tif_options & FAX3_CLASSF) == 0 && tif->tif_curstrip == 0)
		skiptoeol(tif);			/* skip leading EOL */
	return (1);
}

/*
 * Decode the requested amount of data.
 */
static
Fax3Decode(tif, buf, occ)
	PTIFF tif;
	char *buf;
	int occ;
{
	int scanline;
	u_char *row;
	int cc, imagewidth;

	bzero(buf, occ);		/* decoding only sets non-zero bits */
	scanline = tif->tif_scanlinesize;
	imagewidth = tif->tif_dir.td_imagewidth;
	row = (u_char *)buf;
	for (cc = occ; cc > 0; cc -= scanline) {
		if (!Fax3DecodeRow(tif, row, imagewidth))
			return (0);
		row += scanline;
	}
	return (1);
}

/*
 * Fill a span with ones.
 */
static void
fillspan(cp, x, count)
	register char *cp;
	register int x, count;
{
	static unsigned char masks[] =
	    { 0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff };

	cp += x>>3;
	if (x &= 7) {			/* align to byte boundary */
		if (count < 8 - x) {
			*cp++ |= masks[count] >> x;
			return;
		}
		*cp++ |= 0xff >> x;
		count -= 8 - x;
	}
	while (count >= 8) {
		*cp++ = 0xff;
		count -= 8;
	}
	*cp |= masks[count];
}

#define	BITCASE(b)			\
    case b:				\
	code <<= 1;			\
	if (data & b) code |= 1;	\
	len++;				\
	if (code > 0) { bit = (b>>1); break; }

static int
Fax3DecodeRow(tif, buf, npels)
	PTIFF tif;
	u_char *buf;
	int npels;
{
	Fax3DecodeState FAR *sp = (Fax3DecodeState FAR *)tif->tif_data;
	short bit = sp->bit;
	short data = sp->data;
	short x = 0;
	short len = 0;
	short code = 0;
	short count = 0;
	int color = 1;
	int fillcolor = (tif->tif_dir.td_photometric == PHOTOMETRIC_MINISWHITE);
	tableentry *te;
	u_int h;
	static char module[] = "Fax3Decode";

	for (;;) {
		switch (bit) {
	again:	BITCASE(0x80);
		BITCASE(0x40);
		BITCASE(0x20);
		BITCASE(0x10);
		BITCASE(0x08);
		BITCASE(0x04);
		BITCASE(0x02);
		BITCASE(0x01);
		default:
			if (tif->tif_rawcc <= 0) {
			    TIFFError(module,
				"%s: Premature EOF at scanline %d (x %d)",
			        tif->tif_name, tif->tif_row, x);
				return (0);
			}
			data = sp->bitmap[*(u_char FAR *)tif->tif_rawcp++];
			goto again;
		}
		if (len >= 12) {
			if (code == EOL) {
				if (x == 0) {
					TIFFWarning(tif->tif_name,
				    "%s: Ignoring null row at scanline %d",
					    module, tif->tif_row);
					code = len = 0;
					continue;
				}
				sp->bit = bit;
				sp->data = data;
				TIFFWarning(tif->tif_name,
				    "%s: Premature EOL at scanline %d (x %d)",
				    module, tif->tif_row, x);
				return (1);	/* try to resynchronize... */
			}
			if (len > 13) {
				TIFFError(tif->tif_name,
		    "%s: Bad code word (len %d code %d) at scanline %d (x %d)",
				    module, len, code, tif->tif_row, x);
				break;
			}
		}
		if (color) {
			if (len < 4)
				continue;
			h = ((len + WHASHA) * (code + WHASHB)) % HASHSIZE;
			assert(0 <= h && h < HASHSIZE);
			te = sp->whash[h];
		} else {
			if (len < 2)
				continue;
			h = ((len + BHASHA) * (code + BHASHB)) % HASHSIZE;
			assert(0 <= h && h < HASHSIZE);
			te = sp->bhash[h];
		}
		if (te && te->length == len && te->code == code) {
			assert(MINTABID <= te->tabid && te->tabid <= MAXTABID);
			if (te->tabid < 0) {
				count += te->count;
				if (x+count > npels)
					count = npels-x;
				if (count > 0) {
					if (color ^ fillcolor)
						fillspan(buf, x, count);
					x += count;
					if (x >= npels)
						break;
				}
				count = 0;
				color = !color;
			} else
				count += te->count;
			code = len = 0;
		}
	}
	sp->data = data;
	sp->bit = bit;
	skiptoeol(tif);
	return (x == npels);
}

/*
 * CCITT Group 3 FAX Encoding.
 */

static void
putbit(tif, d)
	PTIFF tif;
	int d;
{
	Fax3EncodeState FAR *sp = (Fax3EncodeState FAR *)tif->tif_data;

	if (d) 
		sp->data |= sp->bit;
	sp->bit >>= 1;
	if (sp->bit == 0) {
		if (tif->tif_rawcc >= tif->tif_rawdatasize)
			(void) TIFFFlushData1(tif);	/* XXX */
		*tif->tif_rawcp++ = sp->bitmap[sp->data];
		tif->tif_rawcc++;
		sp->data = 0;
		sp->bit = 0x80;
	}
}

static void
putcode(tif, te)
	PTIFF tif;
	tableentry *te;
{
	Fax3EncodeState FAR *sp = (Fax3EncodeState FAR *)tif->tif_data;
	int bit = sp->bit;
	int data = sp->data;
	u_int mask;

	for (mask = 1 << (te->length-1); mask; mask >>= 1) {
		if (te->code & mask)
			data |= bit;
		bit >>= 1;
		if (bit == 0) {
			if (tif->tif_rawcc >= tif->tif_rawdatasize)
				(void) TIFFFlushData1(tif);	/* XXX */
			*tif->tif_rawcp++ = sp->bitmap[data];
			tif->tif_rawcc++;
			data = 0;
			bit = 0x80;
		}
	}
	sp->data = data;
	sp->bit = bit;
}

static void
puteol(tif)
	PTIFF tif;
{
	int i;

	if (tif->tif_dir.td_group3options & GROUP3OPT_FILLBITS) {
		Fax3EncodeState FAR *sp = (Fax3EncodeState FAR *)tif->tif_data;
		/*
		 * Force bit alignment so EOL will terminate
		 * on a byte boundary.  That is, force the bit
		 * offset to 16-12 = 4, or bit pattern 0x08
		 * before putting out the EOL code.
		 */
		while (sp->bit != 0x08)
			putbit(tif, 0);
	}
	for (i = 0; i < 11; i++)
		putbit(tif, 0);
	putbit(tif, 1);
}

static u_char zeroruns[256] = {
    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,	/* 0x00 - 0x0f */
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,	/* 0x10 - 0x1f */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,	/* 0x20 - 0x2f */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,	/* 0x30 - 0x3f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x40 - 0x4f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x50 - 0x5f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x60 - 0x6f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x70 - 0x7f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x80 - 0x8f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x90 - 0x9f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xa0 - 0xaf */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xb0 - 0xbf */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xc0 - 0xcf */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xd0 - 0xdf */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xe0 - 0xef */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xf0 - 0xff */
};
static u_char oneruns[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x00 - 0x0f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x10 - 0x1f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x20 - 0x2f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x30 - 0x3f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x40 - 0x4f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x50 - 0x5f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x60 - 0x6f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x70 - 0x7f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x80 - 0x8f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x90 - 0x9f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0xa0 - 0xaf */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0xb0 - 0xbf */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,	/* 0xc0 - 0xcf */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,	/* 0xd0 - 0xdf */
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,	/* 0xe0 - 0xef */
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 7, 8,	/* 0xf0 - 0xff */
};

/*
 * Reset encoding state at the start of a strip.
 */
static
Fax3PreEncode(tif)
	PTIFF tif;
{
	register Fax3EncodeState FAR *sp = (Fax3EncodeState FAR *)tif->tif_data;
	TIFFDirectory FAR *td = &tif->tif_dir;

	if (td->td_group3options & GROUP3OPT_2DENCODING) {
		TIFFError(tif->tif_name,
		    "Sorry, can not handle 2-d FAX encoding");
		return (0);
	}
	if (td->td_bitspersample != 1) {
		TIFFError("Fax3PreEncode",
		    "%s: Must have 1 bit/sample", tif->tif_name);
		return (0);
	}
	if (sp == NULL) {
		tif->tif_data = malloc(sizeof (Fax3EncodeState));
		if (tif->tif_data == NULL) {
			TIFFError(tif->tif_name,
			    "No space for Fax3 state block");
			return (0);
		}
		sp = (Fax3EncodeState FAR *)tif->tif_data;
		if (tif->tif_dir.td_photometric == PHOTOMETRIC_MINISWHITE) {
			sp->wruns = zeroruns;
			sp->bruns = oneruns;
		} else {
			sp->wruns = oneruns;
			sp->bruns = zeroruns;
		}
		sp->bitmap = (tif->tif_fillorder != td->td_fillorder ? 
		    TIFFBitRevTable : TIFFNoBitRevTable);
	}
	sp->bit = 0x80;
	sp->data = 0;
	if ((tif->tif_options & FAX3_CLASSF) == 0 && tif->tif_curstrip == 0)
		puteol(tif);
	return (1);
}

static int
findspan(bpp, bits, tab)
	u_char **bpp;
	register int bits;
	register u_char *tab;
{
	register u_char *bp = *bpp;
	register int span;
	int n;

	/*
	 * To find a run of ones or zeros we use tables
	 * that give the number of consecutive ones or
	 * zeros starting from the msb.  The tables are
	 * indexed by byte value so a starting condition
	 * is to first align the search to a byte boundary.
	 */
	if (n = (bits & 7)) {
		/*
		 * NB: bits is counted *down* so we have to
		 *     account for this in calculating the
		 *     bit offset in the byte.
		 */
		span = tab[(*bp++ << (8-n)) & 0xff];
		if (span < n)		/* doesn't extend to edge of byte */
			return (span);
		span = n;		/* table value may be too generous */
		bits -= span;
	} else
		span = 0;
	while (bits >= 8) {
		int len = tab[*bp];
		span += len;
		bits -= len;
		if (len < 8)
			break;
		bp++;
	}
	*bpp = bp;
	return (span);
}

/*
 * Encode a buffer of pixels.
 */
static
Fax3Encode(tif, bp, cc)
	PTIFF tif;
	u_char *bp;
	int cc;
{
	Fax3EncodeState FAR *sp = (Fax3EncodeState FAR *)tif->tif_data;
	int span, bits = cc << 3;		/* XXX */
	tableentry *te;

	while (bits > 0) {
		span = findspan(&bp, bits, sp->wruns);	/* white span */
		bits -= span;
		if (span >= 64) {
			te = &mwtable[(span/64)-1];
			putcode(tif, te);
			span -= te->count;
		}
		putcode(tif, &twtable[span]);
		if (bits == 0)
			break;
		span = findspan(&bp, bits, sp->bruns);	/* black span */
		bits -= span;
		if (span >= 64) {
			te = &mbtable[(span/64)-1];
			putcode(tif, te);
			span -= te->count;
		}
		putcode(tif, &tbtable[span]);
	}
	puteol(tif);
	return (1);
}

static int
Fax3PostEncode(tif)
	PTIFF tif;
{
	Fax3EncodeState FAR *sp = (Fax3EncodeState FAR *)tif->tif_data;

	if (sp->bit != 0x80) {
		if (tif->tif_rawcc >= tif->tif_rawdatasize &&
		    !TIFFFlushData1(tif))
			return (0);
		*tif->tif_rawcp++ = sp->bitmap[sp->data];
		tif->tif_rawcc++;
	}
	return (1);
}

static
Fax3Close(tif)
	PTIFF tif;
{
	if ((tif->tif_options & FAX3_CLASSF) == 0) {	/* append RTC */
		int i;
		for (i = 0; i < 6; i++)
			puteol(tif);
		(void) Fax3PostEncode(tif);
	}
}

static
Fax3Cleanup(tif)
	PTIFF tif;
{
	if (tif->tif_data) {
		free(tif->tif_data);
		tif->tif_data = NULL;
	}
}
