#include "STDAFX.H"
#include "WicUtils.h"

#include <wincodec.h>
#include <wrl/client.h>
#include <map>
#include <set>
#include <algorithm>
#include <vector>

using std::wstring;
using std::vector;
using std::map;
using std::set;
using Microsoft::WRL::ComPtr;

// static members
bool              WicUtils::initialized = false;
vector<WicFormat> WicUtils::saveFormats;
vector<WicFormat> WicUtils::openOnlyFormats;
vector<WicFormat> WicUtils::allOpenableFormats;

/////////////////////////////////////////////////////////////////////////

static void CollectCodecInfo(DWORD componentType,
	map<wstring, set<wstring>>& nameToExts)
{
	ComPtr<IWICImagingFactory> pFactory;
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pFactory))))
		return;

	ComPtr<IEnumUnknown> pEnum;
	if (FAILED(pFactory->CreateComponentEnumerator(componentType,
		WICComponentEnumerateDefault, &pEnum)))
		return;

	ComPtr<IUnknown> pUnk;
	ULONG fetched = 0;

	while (pEnum->Next(1, &pUnk, &fetched) == S_OK && fetched)
	{
		ComPtr<IWICBitmapCodecInfo> pInfo;
		if (SUCCEEDED(pUnk.As(&pInfo)))
		{
			WCHAR nameBuf[256] = {};
			WCHAR extBuf[512] = {};
			UINT actual = 0;

			pInfo->GetFriendlyName(_countof(nameBuf), nameBuf, &actual);
			pInfo->GetFileExtensions(_countof(extBuf), extBuf, &actual);

			wstring name = nameBuf;

			const wstring encSuffix = L" Encoder";
			const wstring decSuffix = L" Decoder";

			if (name.size() >= encSuffix.size() &&
				name.compare(name.size() - encSuffix.size(), encSuffix.size(), encSuffix) == 0)
			{
				name.erase(name.size() - encSuffix.size());
			}
			else if (name.size() >= decSuffix.size() &&
				name.compare(name.size() - decSuffix.size(), decSuffix.size(), decSuffix) == 0)
			{
				name.erase(name.size() - decSuffix.size());
			}

			wstring all(extBuf);
			size_t start = 0;
			while (start < all.size())
			{
				size_t comma = all.find(L',', start);
				wstring one = (comma == wstring::npos)
					? all.substr(start)
					: all.substr(start, comma - start);

				while (!one.empty() && iswspace(one.front())) one.erase(one.begin());
				while (!one.empty() && iswspace(one.back()))  one.pop_back();

				if (!one.empty())
				{
					if (one.front() != L'.')
						one.insert(one.begin(), L'.');
					std::transform(one.begin(), one.end(), one.begin(), ::towlower);
					nameToExts[name].insert(one);
				}

				if (comma == wstring::npos)
					break;
				start = comma + 1;
			}
		}
		pUnk.Reset();
	}
}

static wstring ChoosePreferredExtension(const vector<wstring>& exts)
{
	static const wchar_t* preferredOrder[] =
	{
		L".jpg", L".png", L".tif", L".bmp", L".gif",
		L".heic", L".jxl", L".webp", L".wdp", L".dds"
	};

	for (const wchar_t* pref : preferredOrder)
	{
		for (const auto& e : exts)
		{
			if (e == pref)
				return e;
		}
	}
	return exts.empty() ? L"" : exts.front();
}

/////////////////////////////////////////////////////////////////////////

void WicUtils::Initialize()
{
	if (initialized)
		return;

	map<wstring, set<wstring>> encoders;
	map<wstring, set<wstring>> decoders;

	CollectCodecInfo(WICEncoder, encoders);
	CollectCodecInfo(WICDecoder, decoders);

	// Formats that have both encoder and decoder
	for (const auto& encPair : encoders)
	{
		const wstring& name = encPair.first;
		auto decIt = decoders.find(name);
		if (decIt == decoders.end())
			continue;

		set<wstring> common;
		std::set_intersection(
			encPair.second.begin(), encPair.second.end(),
			decIt->second.begin(), decIt->second.end(),
			std::inserter(common, common.begin()));

		if (common.empty())
			continue;

		WicFormat fmt;
		fmt.name = name;
		fmt.allExts.assign(common.begin(), common.end());
		fmt.preferredExt = ChoosePreferredExtension(fmt.allExts);

		saveFormats.push_back(std::move(fmt));
	}

	// Decoder-only formats
	for (const auto& decPair : decoders)
	{
		const wstring& name = decPair.first;
		if (encoders.find(name) != encoders.end())
			continue;

		WicFormat fmt;
		fmt.name = name;
		fmt.allExts.assign(decPair.second.begin(), decPair.second.end());
		fmt.preferredExt = ChoosePreferredExtension(fmt.allExts);

		openOnlyFormats.push_back(std::move(fmt));
	}

	// Combined list
	allOpenableFormats = saveFormats;
	allOpenableFormats.insert(allOpenableFormats.end(),
		openOnlyFormats.begin(), openOnlyFormats.end());

	initialized = true;
}

/////////////////////////////////////////////////////////////////////////

const vector<WicFormat>& WicUtils::GetSaveFormats()
{
	if (!initialized)
		Initialize();
	return saveFormats;
}

const vector<WicFormat>& WicUtils::GetOpenOnlyFormats()
{
	if (!initialized)
		Initialize();
	return openOnlyFormats;
}

const vector<WicFormat>& WicUtils::GetAllOpenableFormats()
{
	if (!initialized)
		Initialize();
	return allOpenableFormats;
}

/////////////////////////////////////////////////////////////////////////
// Filter strings for CFileDialog

static CString BuildFilter(const vector<WicFormat>& formats, bool includeListFiles)
{
	std::vector<TCHAR> buf;
	buf.reserve(4096);

	auto append = [&](LPCTSTR s)
		{
			while (s && *s)
				buf.push_back(*s++);
		};

	auto add = [&](LPCTSTR label, LPCTSTR wild)
		{
			append(label);
			buf.push_back(_T('\0'));
			append(wild);
			buf.push_back(_T('\0'));
		};

	// "All Supported Images" using preferred extensions only
	CString allWild;
	for (const auto& f : formats)
	{
		if (f.preferredExt.empty())
			continue;
		if (!allWild.IsEmpty())
			allWild += _T(';');
		allWild += _T('*');
		allWild += f.preferredExt.c_str();
	}
	if (allWild.IsEmpty())
		allWild = _T("*.jpg;*.png;*.bmp;*.tif;*.gif");

	add(_T("All Supported Images"), allWild);

	// One entry per format
	for (const auto& f : formats)
	{
		CString wild;
		for (size_t i = 0; i < f.allExts.size(); ++i)
		{
			if (i) wild += _T(';');
			wild += _T('*');
			wild += f.allExts[i].c_str();
		}
		if (wild.IsEmpty())
			continue;

		CString label;
		label.Format(_T("%s (%s)"), CString(f.name.c_str()).GetString(), (LPCTSTR)wild);
		add(label, wild);
	}

	if (includeListFiles)
		add(_T("List Files (*.lst)"), _T("*.lst"));

	CString allFilter;
	if (!allFilter.LoadString(AFX_IDS_ALLFILTER) || allFilter.IsEmpty())
		allFilter = _T("All Files (*.*)");
	add(allFilter, _T("*.*"));

	buf.push_back(_T('\0'));  // final extra null

	return CString(buf.data(), (int)buf.size());
}

CString WicUtils::GetOpenFileFilter()
{
	return BuildFilter(GetAllOpenableFormats(), true);
}

CString WicUtils::GetSaveFileFilter()
{
	return BuildFilter(GetSaveFormats(), false);
}