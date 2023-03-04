#pragma once

#ifndef __cplusplus
#error C++ only
#endif

bool	isWritable(const char* path);
bool	isReadOnly(const char* path);
void	makeWritable(const char* path);
void	makeReadOnly(const char* path);
boolean	isJPEG(const char* path);
boolean	isBMP( const char* path);
extern void		ChangeFileExtension(CString& newName, LPCTSTR oldFilename, LPCTSTR newExtension);
