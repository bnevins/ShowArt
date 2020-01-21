#include "stdtif.h"

/*
 * Copyright (c) 1988, 1990 by Sam Leffler.
 * All rights reserved.
 *
 * This file is provided for unrestricted use provided that this
 * legend is included on all tape media and as a part of the
 * software program in whole or part.	Users may copy, modify or
 * distribute this file at will.
 */

/*
 * TIFF Library.
 */

#pragma alloc_text(TIFF1,TIFFClose)

void
TIFFClose(tif)
	PTIFF tif;
{
	if (tif->tif_mode != O_RDONLY)
		/*
		 * Flush buffered data and directory (if dirty).
		 */
		TIFFFlush(tif);
	if (tif->tif_cleanup)
		(*tif->tif_cleanup)(tif);
	TIFFFreeDirectory(tif);
	if (tif->tif_rawdata)
		GlobalFreePtr(tif->tif_rawdata);
	(void) close(tif->tif_fd);
	free((char FAR *)tif);
}

