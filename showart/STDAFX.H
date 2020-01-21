// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//		are changed infrequently
//

#include <afxwin.h>			// MFC core and standard components
#include <afxext.h> 		// MFC extensions (including VB)
#include <afxcmn.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
typedef vector<int> INTVECTOR;

//#if ! (defined WINVER)
//#if defined _WIN32_WINNT
//#define WINVER _WIN32_WINNT
#if _MSC_VER < 1100
#define bool BOOL
#define true TRUE
#define false FALSE
#endif

#include "dib.h"
#include "KeyCommands.h"
#include "UserPreferences.h"
#include "Globals.h"
#include "FileStatus.h"
