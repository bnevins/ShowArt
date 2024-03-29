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
 * Scanline-oriented Write Support
 */

#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))
#define	howmany(x, y)	(((x)+((y)-1))/(y))

#define	STRIPINCR	20		/* expansion factor on strip array */

#if USE_PROTOTYPES
static	TIFFWriteCheck(PTIFF , char []);
static	TIFFBufferSetup(PTIFF , char []);
static	TIFFGrowStrips(PTIFF , int, char []);
static	TIFFAppendToStrip(PTIFF , u_int, u_char FAR *, u_int);
#else
static	TIFFWriteCheck();
static	TIFFBufferSetup();
static	TIFFGrowStrips();
static	TIFFAppendToStrip();
#endif

/*VARARGS3*/
TIFFWriteScanline(tif, buf, row, sample)
	register PTIFF tif;
	u_char FAR *buf;
	u_int row, sample;
{
	static char module[] = "TIFFWriteScanline";
	register TIFFDirectory FAR *td;
	int strip, status, imagegrew = 0;

	if (!TIFFWriteCheck(tif, module))
		return (-1);
	/*
	 * Handle delayed allocation of data buffer.  This
	 * permits it to be sized more intelligently (using
	 * directory information).
	 */
	if ((tif->tif_flags & TIFF_BUFFERSETUP) == 0) {
		if (!TIFFBufferSetup(tif, module))
			return (-1);
		tif->tif_flags |= TIFF_BUFFERSETUP;
	}
	td = &tif->tif_dir;
	/*
	 * Extend image length if needed
	 * (but only for PlanarConfig=1).
	 */
	if (row >= td->td_imagelength) {	/* extend image */
		if (td->td_planarconfig == PLANARCONFIG_SEPARATE) {
			TIFFError(tif->tif_name,
		"Can not change \"ImageLength\" when using separate planes");
			return (-1);
		}
		td->td_imagelength = row+1;
		imagegrew = 1;
	}
	/*
	 * Calculate strip and check for crossings.
	 */
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE) {
		if (sample >= td->td_samplesperpixel) {
			TIFFError(tif->tif_name,
			    "%d: Sample out of range, max %d",
			    sample, td->td_samplesperpixel);
			return (-1);
		}
		strip = sample*td->td_stripsperimage + row/td->td_rowsperstrip;
	} else
		strip = row / td->td_rowsperstrip;
	if (strip != tif->tif_curstrip) {
		/*
		 * Changing strips -- flush any data present.
		 */
		if (tif->tif_rawcc > 0 && !TIFFFlushData(tif))
			return (-1);
		tif->tif_curstrip = strip;
		/*
		 * Watch out for a growing image.  The value of
		 * strips/image will initially be 1 (since it
		 * can't be deduced until the imagelength is known).
		 */
		if (strip >= td->td_stripsperimage && imagegrew)
			td->td_stripsperimage =
			    howmany(td->td_imagelength, td->td_rowsperstrip);
		tif->tif_row =
		    (strip % td->td_stripsperimage) * td->td_rowsperstrip;
		if (tif->tif_stripencode && !(*tif->tif_stripencode)(tif))
			return (-1);
	}
	/*
	 * Check strip array to make sure there's space.
	 * We don't support dynamically growing files that
	 * have data organized in separate bitplanes because
	 * it's too painful.  In that case we require that
	 * the imagelength be set properly before the first
	 * write (so that the strips array will be fully
	 * allocated above).
	 */
	if (strip >= td->td_nstrips && !TIFFGrowStrips(tif, 1, module))
		return (-1);
	/*
	 * Ensure the write is either sequential or at the
	 * beginning of a strip (or that we can randomly
	 * access the data -- i.e. no encoding).
	 */
	if (row != tif->tif_row) {
		if (tif->tif_seek) {
			if (row < tif->tif_row) {
				/*
				 * Moving backwards within the same strip:
				 * backup to the start and then decode
				 * forward (below).
				 */
				tif->tif_row = (strip % td->td_stripsperimage) *
				    td->td_rowsperstrip;
				tif->tif_rawcp = tif->tif_rawdata;
			}
			/*
			 * Seek forward to the desired row.
			 */
			if (!(*tif->tif_seek)(tif, row - tif->tif_row))
				return (-1);
			tif->tif_row = row;
		} else {
			TIFFError(tif->tif_name,
		    "Compression algorithm does not support random access");
			return (-1);
		}
	}
	status = (*tif->tif_encoderow)(tif, buf, tif->tif_scanlinesize);
	tif->tif_row++;
	return (status);
}

/*
 * Encode the supplied data and write it to the
 * specified strip.  There must be space for the
 * data; we don't check if strips overlap!
 *
 * NB: Image length must be setup before writing; this
 *     interface does not support automatically growing
 *     the image on each write (as TIFFWriteScanline does).
 */
TIFFWriteEncodedStrip(tif, strip, data, cc)
	register PTIFF tif;
	u_int strip;
	u_char FAR *data;
	u_int cc;
{
	static char module[] = "TIFFWriteEncodedStrip";

	if (!TIFFWriteCheck(tif, module))
		return (-1);
	if (strip >= tif->tif_dir.td_nstrips) {
		TIFFError(module, "%s: Strip %d out of range, max %d",
		    tif->tif_name, strip, tif->tif_dir.td_nstrips);
		return (-1);
	}
	/*
	 * Handle delayed allocation of data buffer.  This
	 * permits it to be sized more intelligently (using
	 * directory information).
	 */
	if ((tif->tif_flags & TIFF_BUFFERSETUP) == 0) {
		if (!TIFFBufferSetup(tif, module))
			return (-1);
		tif->tif_flags |= TIFF_BUFFERSETUP;
	}
	tif->tif_curstrip = strip;
	if (tif->tif_encodestrip && !(*tif->tif_encodestrip)(tif))
		return (0);
	/*
	 * Note that this assumes that encoding routines
	 * can handle multiple scanlines.  All the "standard"
	 * ones in the library do!
	 */
	if (!(*tif->tif_encoderow)(tif, data, cc))
		return (0);
	if (tif->tif_dir.td_fillorder != tif->tif_fillorder &&
	    (tif->tif_flags & TIFF_NOBITREV) == 0)
		TIFFReverseBits(tif->tif_rawdata, tif->tif_rawcc);
	if (tif->tif_rawcc > 0 &&
	    !TIFFAppendToStrip(tif, strip, tif->tif_rawdata, tif->tif_rawcc))
		return (-1);
	tif->tif_rawcc = 0;
	tif->tif_rawcp = tif->tif_rawdata;
	return (cc);
}

/*
 * Write the supplied data to the specified strip.
 * There must be space for the data; we don't check
 * if strips overlap!
 *
 * NB: Image length must be setup before writing; this
 *     interface does not support automatically growing
 *     the image on each write (as TIFFWriteScanline does).
 */
TIFFWriteRawStrip(tif, strip, data, cc)
	PTIFF tif;
	u_int strip;
	u_char FAR *data;
	u_int cc;
{
	static char module[] = "TIFFWriteRawStrip";

	if (!TIFFWriteCheck(tif, module))
		return (-1);
	if (strip >= tif->tif_dir.td_nstrips) {
		TIFFError(module, "%s: Strip %d out of range, max %d",
		    tif->tif_name, strip, tif->tif_dir.td_nstrips);
		return (-1);
	}
	return (TIFFAppendToStrip(tif, strip, data, cc) ? -1 : cc);
}

/*
 * Verify file is writable and that the directory
 * information is setup properly.  In doing the latter
 * we also "freeze" the state of the directory so
 * that important information is not changed.
 */
static
TIFFWriteCheck(tif, module)
	register PTIFF tif;
	char module[];
{
	if (tif->tif_mode == O_RDONLY) {
		TIFFError(module, "%s: File not open for writing",
		    tif->tif_name);
		return (0);
	}
	/*
	 * On the first write verify all the required information
	 * has been setup and initialize any data structures that
	 * had to wait until directory information was set.
	 * Note that a lot of our work is assumed to remain valid
	 * because we disallow any of the important parameters
	 * from changing after we start writing (i.e. once
	 * TIFF_BEENWRITING is set, TIFFSetField will only allow
	 * the image's length to be changed).
	 */
	if ((tif->tif_flags & TIFF_BEENWRITING) == 0) {
		if (!TIFFFieldSet(tif, FIELD_IMAGEDIMENSIONS)) {
			TIFFError(module,
			    "%s: Must set \"ImageWidth\" before writing data",
			    tif->tif_name);
			return (0);
		}
		if (!TIFFFieldSet(tif, FIELD_PLANARCONFIG)) {
			TIFFError(module,
		    "%s: Must set \"PlanarConfiguration\" before writing data",
			    tif->tif_name);
			return (0);
		}
		if (tif->tif_dir.td_stripoffset == NULL) {
			register TIFFDirectory FAR *td = &tif->tif_dir;

			td->td_stripsperimage =
			    (td->td_rowsperstrip == 0xffffffff ||
			     td->td_imagelength == 0) ? 1 :
			     howmany(td->td_imagelength, td->td_rowsperstrip);
			td->td_nstrips = td->td_stripsperimage;
			if (td->td_planarconfig == PLANARCONFIG_SEPARATE)
				td->td_nstrips *= td->td_samplesperpixel;
			td->td_stripoffset = (u_long FAR *)
			    malloc(td->td_nstrips * sizeof (u_long));
			td->td_stripbytecount = (u_long FAR *)
			    malloc(td->td_nstrips * sizeof (u_long));
			if (td->td_stripoffset == NULL ||
			    td->td_stripbytecount == NULL) {
				td->td_nstrips = 0;
				TIFFError(module,
				    "%s: No space for strip arrays",
				    tif->tif_name);
				return (0);
			}
			/*
			 * Place data at the end-of-file
			 * (by setting offsets to zero).
			 */
			bzero((char FAR *)td->td_stripoffset,
			    td->td_nstrips * sizeof (u_long));
			bzero((char FAR *)td->td_stripbytecount,
			    td->td_nstrips * sizeof (u_long));
			TIFFSetFieldBit(tif, FIELD_STRIPOFFSETS);
			TIFFSetFieldBit(tif, FIELD_STRIPBYTECOUNTS);
		}
		tif->tif_flags |= TIFF_BEENWRITING;
	}
	return (1);
}

/*
 * Setup the raw data buffer used for encoding.
 */
static
TIFFBufferSetup(tif, module)
	PTIFF tif;
	char module[];
{
	int scanline;

	tif->tif_scanlinesize = scanline = TIFFScanlineSize(tif);
	/*
	 * Make raw data buffer at least 8K
	 */
	if (scanline < 8*1024)
		scanline = 8*1024;
	tif->tif_rawdata = malloc(scanline);
	if (tif->tif_rawdata == NULL) {
		TIFFError(module, "%s: No space for output buffer",
		    tif->tif_name);
		return (0);
	}
	tif->tif_rawdatasize = scanline;
	tif->tif_rawcc = 0;
	tif->tif_rawcp = tif->tif_rawdata;
	return (1);
}

/*
 * Grow the strip data structures by delta strips.
 */
static
TIFFGrowStrips(tif, delta, module)
	PTIFF tif;
	int delta;
	char module[];
{
	TIFFDirectory FAR *td = &tif->tif_dir;

	assert(td->td_planarconfig == PLANARCONFIG_CONTIG);
	td->td_stripoffset = (u_long FAR *)realloc(td->td_stripoffset,
	    (td->td_nstrips + delta) * sizeof (u_long));
	td->td_stripbytecount = (u_long FAR *)realloc(td->td_stripbytecount,
	    (td->td_nstrips + delta) * sizeof (u_long));
	if (td->td_stripoffset == NULL || td->td_stripbytecount == NULL) {
		td->td_nstrips = 0;
		TIFFError(module, "%s: No space to expand strip arrays",
		    tif->tif_name);
		return (0);
	}
	bzero(td->td_stripoffset+td->td_nstrips, delta*sizeof (u_long));
	bzero(td->td_stripbytecount+td->td_nstrips, delta*sizeof (u_long));
	td->td_nstrips += delta;
	return (1);
}

/*
 * Append the data to the specified strip.
 *
 * NB: We don't check that there's space in the
 *     file (i.e. that strips do not overlap).
 */
static
TIFFAppendToStrip(tif, strip, data, cc)
	PTIFF tif;
	u_int strip;
	u_char FAR *data;
	u_int cc;
{
	TIFFDirectory FAR *td = &tif->tif_dir;
	static char module[] = "TIFFAppendToStrip";

	if (td->td_stripoffset[strip] == 0 || tif->tif_curoff == 0) {
		/*
		 * No current offset, set the current strip.
		 */
		if (td->td_stripoffset[strip] != 0) {
			if (!SeekOK(tif->tif_fd, td->td_stripoffset[strip])) {
				TIFFError(module,
				    "%s: Seek error at scanline %d",
				    tif->tif_name, tif->tif_row);
				return (0);
			}
		} else
			td->td_stripoffset[strip] =
			    lseek(tif->tif_fd, 0L, L_XTND);
		tif->tif_curoff = td->td_stripoffset[strip];
	}
	if (!WriteOK(tif->tif_fd, data, cc)) {
		TIFFError(module, "%s: Write error at scanline %d",
		    tif->tif_name, tif->tif_row);
		return (0);
	}
	tif->tif_curoff += cc;
	td->td_stripbytecount[strip] += cc;
	return (1);
}

/*
 * Flush buffered data to the file.
 */
TIFFFlushData(tif)
	PTIFF tif;
{
	if ((tif->tif_flags & TIFF_BEENWRITING) == 0)
		return (0);
	if (tif->tif_encodestrip && !(*tif->tif_encodestrip)(tif))
		return (0);
	return (TIFFFlushData1(tif));
}

/*
 * Internal version of TIFFFlushData that can be
 * called by ``encodestrip routines'' w/o concern
 * for infinite recursion.
 */
TIFFFlushData1(tif)
	register PTIFF tif;
{
	if (tif->tif_dir.td_fillorder != tif->tif_fillorder &&
	    (tif->tif_flags & TIFF_NOBITREV) == 0)
		TIFFReverseBits(tif->tif_rawdata, tif->tif_rawcc);
	if (!TIFFAppendToStrip(tif,
	    tif->tif_curstrip, tif->tif_rawdata, tif->tif_rawcc))
		return (0);
	tif->tif_rawcc = 0;
	tif->tif_rawcp = tif->tif_rawdata;
	return (1);
}
