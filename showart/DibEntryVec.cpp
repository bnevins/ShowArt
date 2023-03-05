#include "stdafx.h"
#include "ShowArt.h"
#include "DibEntryVec.h"
#include "Filename.h"
#include "DibEntryVec.h"

int DibEntryVec::defaultAllocSize = 100;

/////////////////////////////////////////////////////////////////////////////
	
DibEntry&	DibEntry::operator=(const DibEntry& src)
{
	delete pDib;
	pDib = src.pDib;
	const_cast<DibEntry*>(&src)->pDib = NULL;;
	//src.pDib = NULL;
	strncpy(fullPathName, src.fullPathName, _MAX_PATH);
	autoDelete = src.autoDelete;
	delete fs;
	fs = new FileStatus(src.fullPathName);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
	
bool DibEntry::operator==(const DibEntry& src) const
{
	return 
		pDib		== src.pDib						&&
		autoDelete	== src.autoDelete				&&
		strcmp(fullPathName, src.fullPathName) == 0;
}

/////////////////////////////////////////////////////////////////////////////
	
bool DibEntry::operator<(const DibEntry& src) const
{
	return stricmp(fullPathName, src.fullPathName) < 0;
}


/////////////////////////////////////////////////////////////////////////////
	
DibEntry::DibEntry()							
{ 
	*fullPathName = 0;
	pDib = NULL; 
	autoDelete = true;
	fs = 0;
}

/////////////////////////////////////////////////////////////////////////////
	
DibEntry::DibEntry(const CString& filename, bool autodel)
{ 
	strcpy(fullPathName, filename);
	pDib = NULL; 
	autoDelete = autodel;
	fs = 0; 
}

/////////////////////////////////////////////////////////////////////////////
	
DibEntry::~DibEntry()
{
	//if(autoDelete)
		delete pDib;
		delete fs;
}

/////////////////////////////////////////////////////////////////////////////
	
BOOL DibEntry::Set(const CString& name, bool autodel)
{
	CFile cf(name, CFile::modeRead);
	fs = new FileStatus(name);

	if(!fs->Exists())
		return FALSE;

	// BUGFIX!!!  Feb 2023 if there are thousands of files this REALLY slowed things down.  AND time was never used!!!!
	//long time = fs->Timestamp();
	
	strcpy(fullPathName, name);
	autoDelete = autodel;

	if(!autoDelete)
		Load();
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
	
Dib*& DibEntry::Load()
{
	ASSERT(*fullPathName);
	static bool in_process = false;
	
	if(in_process)
		return pNullDib;

	if(!pDib)
	{
		static int skanky = 0;
		in_process = true;
		TRACE("%d  Attempting to open:  %s\n", ++skanky, (LPCTSTR)fullPathName);
		
		try 
		{
 			pDib = new Dib(fullPathName);	// indirectly calls this func. recursively!
			
			if (Prefs.autoCopyPng)
			{
				CAtlString jpgname;
				pDib->AutoSavePng(jpgname);

				if (!jpgname.IsEmpty())
				{
					CString s;
					s.Format("Auto-saved %s as %s", fullPathName, jpgname);
					AfxMessageBox(s);
				}
			}
			in_process = false;
		}
		catch(...) 
		{
			pDib = NULL;
		}

		if(!pDib)
			return pNullDib;

		// note a 'bad' pDib may be returned check by --->  if(*pDib) ....
	}
	return pDib;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

DibEntryVec::DibEntryVec(int aAllocSize) : allocSize(aAllocSize)
{
	data = 0;
	clear();
}

DibEntryVec::~DibEntryVec()
{
	delete[] data;
}

DibEntry& DibEntryVec::operator [ ](int i)
{
	ASSERT(i < numUsed && i >= 0);
	return data[i];
}

void DibEntryVec::insert(const DibEntry & de)
{
	if(numUsed >= numAlloc)
		Resize();

	data[numUsed++] = de;
}

int DibEntryVec::length()
{
	return numUsed;
}

void DibEntryVec::clear()
{
	delete[] data;
	numAlloc = allocSize;
	numUsed = 0;
	data = new DibEntry[allocSize];
}

void DibEntryVec::clear(int newAllocSize)
{
	setAllocSize(newAllocSize);
	clear();
}

void DibEntryVec::removeAt(int which)
{
	ASSERT(which >= 0 && which < numUsed);
	--numUsed;

	delete data[which].pDib;
	data[which].pDib = 0;

	for(int i = which; i < numUsed; i++)
	{
		memcpy(data + i, data + i + 1, sizeof(DibEntry));
		
		//data[i] = data[i+1];
	}
	data[numUsed].pDib = 0;	// so it doesn't get deleted!!!
	data[numUsed].fs = 0;
	data[numUsed] = DibEntry();
}

void DibEntryVec::Resize()
{
	ASSERT(numUsed >= numAlloc);
	DibEntry* old = data;
	numAlloc += allocSize;

	data = new DibEntry[numAlloc];

	for(int i = 0; i < numUsed; i++)
	{
		data[i] = old[i];
	}

	delete[] old;
}

void* DibEntryVec::QSortPointer()
{
	return data;
}

void DibEntryVec::setAllocSize(int newAllocSize)
{
	if(newAllocSize > defaultAllocSize)
		allocSize = ((newAllocSize / 10) + 1) * 10;
	else
		allocSize = defaultAllocSize;
}

void DibEntryVec::Find(LPCTSTR chars, int* results)
{
	CString what(chars);
	int whatLen = what.GetLength();
	int count = 0;

	for(int i = 0; i < numUsed; i++)
	{
		Filename fn(data[i].fullPathName);
		CString starts = fn.name.Left(whatLen);

		if(starts.CompareNoCase(what) == 0)
		{
			results[count++] = i;
		}
	}
	TRACE("COUNT: %d\n", count);
	results[count] = -1;
}

int DibEntryVec::FindFirst(CString& what)
{
	int whatLen = what.GetLength();

	for(int i = 0; i < numUsed; i++)
	{
		Filename fn(data[i].fullPathName);
		CString starts = fn.name.Left(whatLen);

		if(starts.CompareNoCase(what) == 0)
		{
			return i;
		}
	}

	return -1;
}
int DibEntryVec::FindLast(CString& what)
{
	int whatLen = what.GetLength();
	int first	= FindFirst(what);

	if(first < 0)
		return first;

	for(int i = first; i < numUsed; i++)
	{
		Filename fn(data[i].fullPathName);
		CString starts = fn.name.Left(whatLen);

		if(starts.CompareNoCase(what) != 0)
		{
			// no match -- we're done
			return i;
		}
	}

	return numUsed - 1;
}
