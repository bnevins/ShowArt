#pragma once

class Filename
{
public:
	Filename(LPCTSTR s);
	CString fullpath;
	CString path;
	CString name;
	CString ext;
};
