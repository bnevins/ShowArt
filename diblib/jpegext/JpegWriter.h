#pragma once

#ifndef __cplusplus
#error C++ only
#endif

class  __declspec( dllexport ) JpegWriter
{
public:
	JpegWriter(unsigned char* pDib, unsigned char * pdibbits, const char* outName);
	JpegWriter(unsigned char* pDib, unsigned char * pdibbits, const char* outName, int q);
	void write();
	void setQuality(int q);
private:
	void						ctor(unsigned char * pdib, unsigned char * pdibbits, const char *name, int q);
	void						ReportAndThrowError(const char* msg);
	void						init();
	void						cleanup();
	unsigned char*				lpDib;
	unsigned char*				lpDibBits;
	const char*					outFileName;
	int							quality;
	static const int			defaultQuality;
};

