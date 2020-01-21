// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#define STRICT
#define VNET
#include <windows.h>
#include <assert.h>

#ifdef VNET
	#include <strstream>
	#include <iostream>
	using namespace std;
#else
	#include <strstrea.h>
#endif

typedef unsigned char BYTE;

