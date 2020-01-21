#if defined(WIN32) || defined(_WIN32)
#define huge
#endif

#ifdef __cplusplus
extern "C" 
#endif

BYTE huge * Jpg2Dib(const char* infilename, const char *outfilename, int useProgressMonitor);
		   
