/* tiffcompat.h 1.8		90/05/18 */

#ifndef _COMPAT_
#define _COMPAT_

/*
 * Copyright (c) 1990 by Sam Leffler.
 * All rights reserved.
 *
 * This file is provided for unrestricted use provided that this
 * legend is included on all tape media and as a part of the
 * software program in whole or part.	 Users may copy, modify or
 * distribute this file at will.
 */

/*
 * This file contains a hodgepodge of definitions and
 * declarations that are needed to provide compatibility
 * between the native system and the base UNIX implementation
 * that the library assumes (~4BSD).  In particular, you
 * can override the standard i/o interface (read/write/lseek)
 * by redefining the ReadOK/WriteOK/SeekOK macros to your
 * liking.
 *
 * NB: This file is a mess.
 */
#if defined(__STDC__) || USE_PROTOTYPES
#include <stdio.h>
#endif
#include <sys/types.h>
#include <fcntl.h>
#ifdef THINK_C
#include <stdlib.h>
#endif


#if defined(SYSV) || defined (MSDOS)
#define L_SET 0
#define L_INCR  SEEK_CUR
#define L_XTND  SEEK_END
#ifdef _WINDOWS
#define bzero(dst,len)		_fmemset(dst, 0, len)
//#define bcopy(src,dst,len) 	  _fmemcpy(dst, src, len)
#define bcopy(src,dst,len) 	  hmemcpy(dst, src, len)
#define bcmp(src, dst, len)	  _fmemcmp(dst, src, len)
#else
#define bzero(dst,len)		memset(dst, 0, len)
#define bcopy(src,dst,len) 	  memcpy(dst, src, len)
#define bcmp(src, dst, len)	  memcmp(dst, src, len)
#endif
#endif

#ifdef BSDTYPES
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
#endif

/*
 * Return an open file descriptor or -1.
 */
#if defined(applec) || defined(THINK_C)
#define TIFFOpenFile(name, mode, prot)  open(name, mode)
#else
#if defined(MSDOS)
//#if defined(_WINDOWS)
#define TIFFOpenFile(name, mode, prot)  _lopen(name, mode)
//#else 
//#define TIFFOpenFile(name, mode, prot)  open(name, mode|O_BINARY, prot)
//#endif
#else
#define TIFFOpenFile(name, mode, prot)  open(name, mode, prot)
#endif
#endif

/*
 * Return the size in bytes of the file
 * associated with the supplied file descriptor.
 */
extern  long TIFFGetFileSize();

#ifndef L_SET
#define L_SET	 0
#define L_INCR 1
#define L_XTND 2
#endif

#ifdef notdef
#define lseek unix_lseek		  /* Mac's Standard 'lseek' won't extend file */
#endif
#ifndef MSDOS
extern  long lseek();
#endif

#ifndef ReadOK
#ifdef _WINDOWS
#define ReadOK(fd, buf, size)   (_hread(fd, (void _huge *)buf, size) == size)
#else							  			 	  
#define ReadOK(fd, buf, size)   (read(fd, (char *)buf, size) == size)
#endif
#endif
#ifndef SeekOK
#ifdef _WINDOWS
#define SeekOK(fd, off) (_llseek(fd, (long)off, L_SET) == (long)off)
#else
#define SeekOK(fd, off) (lseek(fd, (long)off, L_SET) == (long)off)
#endif
#endif
#ifndef WriteOK
#ifdef _WINDOWS
#define WriteOK(fd, buf, size)  (_hwrite(fd, (void _huge *)buf, size) == size)
#else							  			  
#define WriteOK(fd, buf, size)  (write(fd, (char *)buf, size) == size)
#endif
#endif

#if defined(__MACH__) || defined(THINK_C)
extern  void *malloc(size_t size);
extern  void *realloc(void *ptr, size_t size);
#else
#if defined(MSDOS)
#include <malloc.h>
#include <io.h>
//#include <memory.h>
#include <string.h>
#else
extern  char *malloc();
extern  char *realloc();
#endif
#endif

/*
 * dblparam_t is the type that a double precision
 * floating point value will have on the parameter
 * stack (when coerced by the compiler).
 */
#ifdef applec
typedef extended dblparam_t;
#else
typedef double dblparam_t;
#endif

/*
 * Varargs parameter list handling...YECH!!!!
 */
#if defined(__STDC__) && !defined(USE_VARARGS)
#define USE_VARARGS 0
#endif

#if defined(USE_VARARGS)
#if USE_VARARGS
#include <varargs.h>
#define VA_START(ap, parmN)	  va_start(ap)
#else
#include <stdarg.h>
#define VA_START(ap, parmN)	  va_start(ap, parmN)
#endif
#endif /* defined(USE_VARARGS) */

/*
 * How to concatenate lexical tokens with
 * the C preprocessor -- more YECH!
 */
#if defined(MSDOS) || (defined(__STDC__) && !defined(apollo))
#define CAT(a,b) a##b
#else
#define IDENT(x) x
#define CAT(a,b) IDENT(a)b
#endif

#if defined(__STDC__) && !defined(USE_PROTOTYPES)
#define USE_PROTOTYPES 1
#endif
#endif /* _COMPAT_ */
