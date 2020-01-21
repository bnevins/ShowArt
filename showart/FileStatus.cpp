#include "STDAFX.H"

#include ".\filestatus.h"

FileStatus::FileStatus(const char* filename)
{
	if(stat(filename, &statbuf) < 0)
		exists = false;
	else
		exists = true;
}

long FileStatus::Timestamp()
{
	if(!exists)
		return 0;

	return statbuf.st_mtime;
}

bool FileStatus::Exists()
{
	return exists;
}

bool FileStatus::IsDirectory()
{
	if(!exists)
		return false;

	return (statbuf.st_mode & S_IFMT) == S_IFDIR;
}

long FileStatus::Size()
{
	if(!exists)
		return 0;

	return statbuf.st_size;
}

