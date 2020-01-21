#pragma once
#include <sys/stat.h>

class FileStatus
{
public:
	FileStatus(const char* theFilename);
	bool	Exists();
	bool	IsDirectory();
	long	Timestamp();
	long	Size();
private:
	struct stat statbuf;
	bool	exists;
};

