#include "stdwin.h"
#include <io.h>
#include <sys/stat.h>


bool isWritable(const char* path)
{
	return _access(path, 2) == 0;
}

bool isReadOnly(const char* path)
{
	return _access(path, 2) != 0;
}

void makeWritable(const char* path)
{
	_chmod(path, _S_IREAD | _S_IWRITE);
}

void makeReadOnly(const char* path)
{
	_chmod(path, _S_IREAD);
}


static const char* getExtension(const char* path)
{
	if(!path || !*path)
		return NULL;

	const char* px = strrchr(path, '.');

	if(!px)
		return NULL;

	if(!*++px)
		return NULL;
	
	return px;
}

boolean isJPEG(const char* path)
{
	const char* ext =  getExtension(path);

	if(!ext)
		return false;

	if(!stricmp(ext, "jpg") || !stricmp(ext, "jpeg"))
		return true;
	
	return false;
}

boolean isBMP(const char* path)
{
	const char* ext =  getExtension(path);

	if(!ext)
		return false;

	if(!stricmp(ext, "bmp") || !stricmp(ext, "dib"))
		return true;
	
	return false;
}

