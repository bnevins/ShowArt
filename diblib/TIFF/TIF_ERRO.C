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
//#define NOCOMM
//#define NOKANJI
//#include <windows.h>
#pragma alloc_text(TIFF1,TIFFError)

void
#if USE_PROTOTYPES
TIFFError(char FAR *module, char *fmt, ...)
#else
/*VARARGS2*/
TIFFError(module, fmt, va_alist)
	char FAR *module;
	char *fmt;
	va_dcl
#endif
{
#ifdef _WINDOWS
	va_list ap;
	char szBuffer[100];

	VA_START(ap, fmt);
	vsprintf(szBuffer, fmt, ap);
	MessageBox(GetActiveWindow(), szBuffer, module, MB_OK);
	va_end(ap);
#else
	va_list ap;

	if (module != NULL)
		fprintf(stderr, "%s: ", module);
	VA_START(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, ".\n");
#endif
}
