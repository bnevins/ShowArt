#pragma once

#ifndef __cplusplus
#error C++ only
#endif

class __declspec( dllexport ) JpegRotater
{
public:
	JpegRotater(const char* inName, const char* outName, int numTurns);
	bool rotate();
private:
	static void setTimes(int from, int to);
	const char*	infileName;
	const char*	outfileName;
	int			numQuarterTurns;
};
