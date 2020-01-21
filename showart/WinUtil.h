#pragma once

extern void		MondoCreateDirectory(const CString& destDir);
extern void		ShowLastError(LPCTSTR extra = NULL);
extern HANDLE	CopyHandle(HANDLE h);
extern BOOL		IsDir(const CString& name);
extern void		ListFilesInDirectory(LPCTSTR dirName, CStringArray& filepaths );
extern BOOL		Exists(LPCTSTR path);
extern CString NextFileName(CString&  path);