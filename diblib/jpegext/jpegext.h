extern unsigned int		GetNumDisplayBitsPerPixel();
unsigned char*			AllocWinMem(unsigned int dwLen);
extern void				Win32ErrorMessage(const char* msg);
extern void				Win32TraceMessage(const char* msg);
unsigned char*			Jpg2Dib(const char* infilename, const char *outfilename, double scalingFactor, int showErrorsInMessageBox);
void					Win32Throw(void);
