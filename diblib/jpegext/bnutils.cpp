#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/utime.h>

void copyFileTimes(int from, int to)
{
	// first -- get the times of the 'from' file
	struct _stat statBuffer;
	_fstat(from, &statBuffer);

	// prepare another ugly C-structure with the times...
	struct _utimbuf utimBuffer;
	utimBuffer.actime	= statBuffer.st_atime;
	utimBuffer.modtime	= statBuffer.st_mtime;
	
	//Now -- set the "to" file to have these dates...
	
	_futime(to, &utimBuffer);
}

