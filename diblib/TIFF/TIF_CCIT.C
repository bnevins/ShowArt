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
 * CCITT Group 3 1-D Modified Huffman
 * Run Length Encoding Compression Support
 */    
#pragma alloc_text(TIFF1,TIFFInitCCITTRLE,TIFFInitCCITTRLEW)

TIFFInitCCITTRLE(tif)
	PTIFF tif;
{

	TIFFError(tif->tif_name, "CCITT Group 3 1-Dimensional Modified %s",
	    "Huffman run length encoding not supported");
	return (0);
}

TIFFInitCCITTRLEW(tif)
	PTIFF tif;
{

	TIFFError(tif->tif_name, "CCITT Group 3 1-Dimensional Modified %s",
	    "Huffman run length encoding w/ word alignment");
	return (0);
}
