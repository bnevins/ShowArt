#include "stdwin.h"
#include "dib.h"
#include "dibpriv.h"

static HINSTANCE HinstDLL = NULL;

HINSTANCE GetDllInstance()	
{ 
	if(!HinstDLL)
	{
		HinstDLL = GetModuleHandle("bndib.dll");

		// remember -- an MFC app can link to EITHER one...
		
		if(!HinstDLL)
			HinstDLL = GetModuleHandle("bndibd.dll");
	}
	return HinstDLL;
}

/*
BOOL WINAPI DllEntryPoint( 
	HINSTANCE hinstDLL, 
	DWORD fdwReason, 
	LPVOID lpvReserved)
{
	HinstDLL = hinstDLL;
	return TRUE;
}
*/





