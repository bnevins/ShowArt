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
 */

#define	ord(e)	((int)e)

/*
 * Initialize the bit fill order, the
 * shift & mask tables, and the byte
 * swapping state according to the file
 * contents and the machine architecture.
 */
static
TIFFInitOrder(tif, magic, bigendian)
	PTIFF tif;
	int magic, bigendian;
{

	/* XXX how can we deduce this dynamically? */
	tif->tif_fillorder = FILLORDER_MSB2LSB;

	tif->tif_typemask[0] = 0;
	tif->tif_typemask[ord(TIFF_BYTE)] = 0xff;
	tif->tif_typemask[ord(TIFF_SHORT)] = 0xffff;
	tif->tif_typemask[ord(TIFF_LONG)] = 0xffffffff;
	tif->tif_typemask[ord(TIFF_RATIONAL)] = 0xffffffff;
	tif->tif_typeshift[0] = 0;
	tif->tif_typeshift[ord(TIFF_LONG)] = 0;
	tif->tif_typeshift[ord(TIFF_RATIONAL)] = 0;
	if (magic == TIFF_BIGENDIAN) {
		tif->tif_typeshift[ord(TIFF_BYTE)] = 24;
		tif->tif_typeshift[ord(TIFF_SHORT)] = 16;
		if (!bigendian)
			tif->tif_flags |= TIFF_SWAB;
	} else {
		tif->tif_typeshift[ord(TIFF_BYTE)] = 0;
		tif->tif_typeshift[ord(TIFF_SHORT)] = 0;
		if (bigendian)
			tif->tif_flags |= TIFF_SWAB;
	}
}

/*
 * Open a TIFF file for read/writing.
 */
PTIFF 
TIFFOpen(name, mode)
	char *name,*mode;
{
	static char module[] = "TIFFOpen";
	PTIFF tif;
	int m, fd, bigendian;

	switch (mode[0]) {
	case 'r':
		m = O_RDONLY;
		if (mode[1] == '+')
			m = O_RDWR;
		break;
	case 'w':
	case 'a':
		m = O_RDWR|O_CREAT;
		if (mode[0] == 'w')
			m |= O_TRUNC;
		break;
	default:
		TIFFError(module, "\"%s\": Bad mode", mode);
		return ((PTIFF )0);
	}
	fd = TIFFOpenFile(name, m, 0666);
	if (fd < 0) {
		TIFFError(module, "%s: Cannot open", name);
		return ((PTIFF )0);
	}
	tif = (PTIFF )malloc(sizeof (TIFF) + strlen(name) + 1);
	if (tif == NULL) {
		TIFFError(module, "%s: Out of memory (TIFF structure)", name);
		(void) close(fd);
		return ((PTIFF )0);
	}
	bzero((char FAR *)tif, sizeof (*tif));
	tif->tif_name = (char FAR *)tif + sizeof (TIFF);
	strcpy(tif->tif_name, name);
	tif->tif_fd = fd;
	tif->tif_mode = m &~ (O_CREAT|O_TRUNC);
	tif->tif_curoff = 0;
	tif->tif_curstrip = -1;		/* invalid strip */
	tif->tif_row = -1;		/* read/write pre-increment */
	{ int one = 1; bigendian = (*(char *)&one == 0); }
	/*
	 * Read in TIFF header.
	 */
	if (!ReadOK(fd, &tif->tif_header, sizeof (TIFFHeader))) {
		int one = 1;

		if (tif->tif_mode == O_RDONLY) {
			TIFFError(name, "Cannot read TIFF header");
			goto bad;
		}
		/*
		 * Setup header and write.
		 */
		tif->tif_header.tiff_magic =  bigendian ?
		    TIFF_BIGENDIAN : TIFF_LITTLEENDIAN;
		tif->tif_header.tiff_version = TIFF_VERSION;
		tif->tif_header.tiff_diroff = 0;	/* filled in later */
		if (!WriteOK(fd, &tif->tif_header, sizeof (TIFFHeader))) {
			TIFFError(name, "Error writing TIFF header");
			goto bad;
		}
		/*
		 * Setup the byte order handling.
		 */
		TIFFInitOrder(tif, tif->tif_header.tiff_magic, bigendian);
		/*
		 * Setup default directory.
		 */
		if (!TIFFDefaultDirectory(tif))
			goto bad;
		tif->tif_diroff = 0;
		return (tif);
	}
	/*
	 * Setup the byte order handling.
	 */
	if (tif->tif_header.tiff_magic != TIFF_BIGENDIAN &&
	    tif->tif_header.tiff_magic != TIFF_LITTLEENDIAN) {
		TIFFError(name,  "Not a TIFF file, bad magic number %d (0x%x)",
		    tif->tif_header.tiff_magic,
		    tif->tif_header.tiff_magic);
		goto bad;
	}
	TIFFInitOrder(tif, tif->tif_header.tiff_magic, bigendian);
	/*
	 * Swap header if required.
	 */
	if (tif->tif_flags & TIFF_SWAB) {
		TIFFSwabShort(&tif->tif_header.tiff_version);
		TIFFSwabLong(&tif->tif_header.tiff_diroff);
	}
	/*
	 * Now check version (if needed, it's been byte-swapped).
	 * Note that this isn't actually a version number, it's a
	 * magic number that doesn't change (stupid).
	 */
	if (tif->tif_header.tiff_version != TIFF_VERSION) {
		TIFFError(name,
		    "Not a TIFF file, bad version number %d (0x%x)",
		    tif->tif_header.tiff_version,
		    tif->tif_header.tiff_version); 
		goto bad;
	}
	/*
	 * Setup initial directory.
	 */
	switch (mode[0]) {
	case 'r':
		tif->tif_nextdiroff = tif->tif_header.tiff_diroff;
		if (TIFFReadDirectory(tif)) {
			tif->tif_rawcc = -1;
			tif->tif_flags |= TIFF_BUFFERSETUP;
			return (tif);
		}
		break;
	case 'a':
		/*
		 * Don't append to file that has information
		 * byte swapped -- we will write data that is
		 * in the opposite order.
		 */
		if (tif->tif_flags & TIFF_SWAB) {
			TIFFError(name,
		"Cannot append to file that has opposite byte ordering");
			goto bad;
		}
		/*
		 * New directories are automatically append
		 * to the end of the directory chain when they
		 * are written out (see TIFFWriteDirectory).
		 */
		if (!TIFFDefaultDirectory(tif))
			goto bad;
		return (tif);
	}
bad:
	tif->tif_mode = O_RDONLY;	/* XXX avoid flush */
	(void) TIFFClose(tif);
	return ((PTIFF )0);
}

TIFFScanlineSize(tif)
	PTIFF tif;
{
	TIFFDirectory FAR *td = &tif->tif_dir;
	long scanline;
	
	scanline = td->td_bitspersample * td->td_imagewidth;
	if (td->td_planarconfig == PLANARCONFIG_CONTIG)
		scanline *= td->td_samplesperpixel;
#define	howmany(x, y)	(((x)+((y)-1))/(y))
	return (howmany(scanline, 8));
}
