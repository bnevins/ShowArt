#pragma once
#include <vector>
#include <string>

struct WicFormat
{
	std::wstring name;              // "JPEG", "PNG", ...
	std::wstring preferredExt;      // ".jpg"
	std::vector<std::wstring> allExts; // { ".jpg", ".jpeg", ".jpe" }
	static std::vector<WicFormat> GetSupportedFormats();
	static CString GetSupportedFormatsString();

};

class WicUtils
{
public:
	// Returns formats that support both encoding and decoding
	static std::vector<WicFormat> GetCannedFormats();
	static std::vector<WicFormat> GetFormatsWithEncoderAndDecoder();
};