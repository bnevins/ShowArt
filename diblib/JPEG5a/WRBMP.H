#if defined(WIN32) || defined(_WIN32)
	#ifndef huge
		#define huge
	#endif
#endif

void SetNoFile(int, unsigned char huge *);
int NoFile();
	
