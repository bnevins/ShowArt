#include "STDAFX.H"
#include <wincodec.h>
#include <wrl/client.h>
#include <algorithm>
#include <wincodec.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include "WicUtils.h"

using std::string;
using std::wstring;
using std::vector;
using std::map;
using std::set;
using Microsoft::WRL::ComPtr;

std::vector<WicFormat> WicUtils::GetCannedFormats()
{
	// Canonical list of formats we care about
	struct KnownFormat
	{
		REFGUID container;
		const wchar_t* name;
		const wchar_t* preferredExt;
		vector<const wchar_t*> aliases;
	};

	static const KnownFormat known[] =
	{
		{ GUID_ContainerFormatJpeg, L"JPEG", L".jpg",  { L".jpg", L".jpeg", L".jpe" } },
		{ GUID_ContainerFormatPng,  L"PNG",  L".png",  { L".png" } },
		{ GUID_ContainerFormatBmp,  L"BMP",  L".bmp",  { L".bmp" } },
		{ GUID_ContainerFormatTiff, L"TIFF", L".tif",  { L".tif", L".tiff" } },
		{ GUID_ContainerFormatGif,  L"GIF",  L".gif",  { L".gif" } },
		{ GUID_ContainerFormatWmp,  L"JPEG XR", L".wdp", { L".wdp", L".jxr" } },
		{ GUID_ContainerFormatDds,  L"DDS",  L".dds",  { L".dds" } },
	};

	vector<WicFormat> result;

	ComPtr<IWICImagingFactory> pFactory;
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pFactory))))
		return result;

	for (const auto& k : known)
	{
		// Test that both encoder and decoder exist for this container
		ComPtr<IWICBitmapEncoder> pEnc;
		ComPtr<IWICBitmapDecoder> pDec;

		HRESULT hrEnc = pFactory->CreateEncoder(k.container, NULL, &pEnc);
		HRESULT hrDec = pFactory->CreateDecoder(k.container, NULL, &pDec);

		if (SUCCEEDED(hrEnc) && SUCCEEDED(hrDec))
		{
			WicFormat fmt;
			fmt.name = k.name;
			fmt.preferredExt = k.preferredExt;

			for (const wchar_t* a : k.aliases)
				fmt.allExts.push_back(a);

			result.push_back(move(fmt));
		}
	}

	return result;
}


// get the list of formats that have both encoder and decoder, and their extensions
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
			// Normalize name a bit (remove " Encoder" / " Decoder" suffix if present)
			const wstring encSuffix = L" Encoder";
			const wstring decSuffix = L" Decoder";
			if (name.size() > encSuffix.size() &&
				name.compare(name.size() - encSuffix.size(), encSuffix.size(), encSuffix) == 0)
				name.erase(name.size() - encSuffix.size());
			if (name.size() > decSuffix.size() &&
				name.compare(name.size() - decSuffix.size(), decSuffix.size(), decSuffix) == 0)
				name.erase(name.size() - decSuffix.size());

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

				if (comma == wstring::npos) break;
				start = comma + 1;
			}
		}
		pUnk.Reset();
	}
}

void Trace(const map<wstring, set<wstring>>& encoders, const map<wstring, set<wstring>>& decoders)
{
	/****  Output on my dev computer
C:\DEV\cpp\win\showart\WicUtils.cpp(160) : atlTraceGeneral - ENCODER -- Format: BMP, Extensions: .bmp, .dib, .rle, 
C:\DEV\cpp\win\showart\WicUtils.cpp(160) : atlTraceGeneral - ENCODER -- Format: DDS, Extensions: .dds, 
C:\DEV\cpp\win\showart\WicUtils.cpp(160) : atlTraceGeneral - ENCODER -- Format: GIF, Extensions: .gif, 
C:\DEV\cpp\win\showart\WicUtils.cpp(160) : atlTraceGeneral - ENCODER -- Format: JPEG, Extensions: .exif, .jfif, .jpe, .jpeg, .jpg, 
C:\DEV\cpp\win\showart\WicUtils.cpp(160) : atlTraceGeneral - ENCODER -- Format: Microsoft HEIF, Extensions: .heic, .heif, .hif, 
C:\DEV\cpp\win\showart\WicUtils.cpp(160) : atlTraceGeneral - ENCODER -- Format: Microsoft JPEG XL, Extensions: .jxl, 
C:\DEV\cpp\win\showart\WicUtils.cpp(160) : atlTraceGeneral - ENCODER -- Format: PNG, Extensions: .png, 
C:\DEV\cpp\win\showart\WicUtils.cpp(160) : atlTraceGeneral - ENCODER -- Format: TIFF, Extensions: .tif, .tiff, 
C:\DEV\cpp\win\showart\WicUtils.cpp(160) : atlTraceGeneral - ENCODER -- Format: WMPhoto, Extensions: .jxr, .wdp, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: BMP, Extensions: .bmp, .dib, .rle, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: CUR, Extensions: .cur, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: DDS, Extensions: .dds, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: DNG, Extensions: .dng, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: GIF, Extensions: .gif, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: ICO, Extensions: .ico, .icon, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: JPEG, Extensions: .exif, .jfif, .jpe, .jpeg, .jpg, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: Microsoft HEIF, Extensions: .avci, .avcs, .avif, .avifs, .heic, .heics, .heif, .heifs, .hif, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: Microsoft JPEG XL, Extensions: .jxl, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: Microsoft Raw Image, Extensions: .3fr, .ari, .arw, .bay, .cap, .cr2, .cr3, .crw, .dcr, .dcs, .dng, .drf, .eip, .erf, .fff, .iiq, .k25, .kdc, .mef, .mos, .mrw, .nef, .nrw, .orf, .ori, .pef, .ptx, .pxn, .raf, .raw, .rw2, .rwl, .sr2, .srf, .srw, .x3f, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: Microsoft Webp, Extensions: .webp, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: PNG, Extensions: .png, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: TIFF, Extensions: .tif, .tiff, 
C:\DEV\cpp\win\showart\WicUtils.cpp(177) : atlTraceGeneral - DECODER -- Format: WMPhoto, Extensions: .jxr, .wdp, 

	*/
	for (const auto& pair : encoders)
		{
			const wstring& name = pair.first;
			const set<wstring>& exts = pair.second;

			CString msg;
			msg.Format(_T("ENCODER -- Format: %s, Extensions: "), CString(name.c_str()).GetString());

			for (const auto& ext : exts)
			{
				msg += CString(ext.c_str());
				msg += _T(", ");
			}

			TRACE(_T("%s\n"), (LPCTSTR)msg);
		}

		for (const auto& pair : decoders)
		{
			const wstring& name = pair.first;
			const set<wstring>& exts = pair.second;

			CString msg;
			msg.Format(_T("DECODER -- Format: %s, Extensions: "), CString(name.c_str()).GetString());

			for (const auto& ext : exts)
			{
				msg += CString(ext.c_str());
				msg += _T(", ");
			}

			TRACE(_T("%s\n"), (LPCTSTR)msg);
		}
}

vector<WicFormat> WicUtils::GetFormatsWithEncoderAndDecoder()
{
	map<wstring, set<wstring>> encoders;
	map<wstring, set<wstring>> decoders;

	CollectCodecInfo(WICEncoder, encoders);
	CollectCodecInfo(WICDecoder, decoders);
	Trace(encoders, decoders);

	vector<WicFormat> result;

	// AI generated very unreadable code here  FIXME FIXME 
	for (const auto& encPair : encoders)
	{
		const wstring& name = encPair.first;
		auto decIt = decoders.find(name);
		if (decIt == decoders.end())
			continue;   // no matching decoder

		// Intersection of extensions
		set<wstring> common;
		std::set_intersection(encPair.second.begin(), encPair.second.end(),
			decIt->second.begin(), decIt->second.end(),
			std::inserter(common, common.begin()));

		if (common.empty())
			continue;

		WicFormat fmt;
		fmt.name = name;
		fmt.allExts.assign(common.begin(), common.end());
		fmt.preferredExt = fmt.allExts.front();   // first one as preferred

		result.push_back(std::move(fmt));
	}
	
	return result;
}


