#pragma once

#include <vector>
#include <string>

struct WicFormat
{
	std::wstring name;
	std::wstring preferredExt;
	std::vector<std::wstring> allExts;
};

class WicUtils
{
public:
	static void Initialize();

	static const std::vector<WicFormat>& GetSaveFormats();
	static const std::vector<WicFormat>& GetOpenOnlyFormats();
	static const std::vector<WicFormat>& GetAllOpenableFormats();

	// Double-null-terminated filter strings for CFileDialog / OPENFILENAME
	static CString GetOpenFileFilter();   // all openable + *.lst + *.*
	static CString GetSaveFileFilter();   // saveable only + *.*

private:
	static bool initialized;
	static std::vector<WicFormat> saveFormats;
	static std::vector<WicFormat> openOnlyFormats;
	static std::vector<WicFormat> allOpenableFormats;
};