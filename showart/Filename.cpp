#include "STDAFX.H"
#include ".\filename.h"

Filename::Filename(LPCTSTR s)
{
	fullpath = s;

	const char* endPath = strrchr(s, '\\');
	const char* startExt = strrchr(s, '.');

	if(!endPath)
	{
		//no path!
		path = "";
	}
	else
	{
		path = fullpath.Left(endPath - s);
	}

	if(!startExt)
	{
		// no extension ?!?
		ext = "";
	}
	else
	{
		ext = fullpath.Right(strlen(s) - (startExt - s) - 1);
	}

	name = fullpath.Mid(path.GetLength() + 1, strlen(s) - 2 - path.GetLength() - ext.GetLength());
}

