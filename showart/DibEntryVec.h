#ifndef __DIBENTRYVEC_H__
#define __DIBENTRYVEC_H__

typedef int(*SortingFunction)(const void*, const void*);

struct DibEntry
{
				DibEntry();							
				DibEntry(const CString& filename, bool autodel = true);
				~DibEntry();
				DibEntry(const DibEntry& src)	{ *this = src; }
	DibEntry&	operator=(const DibEntry& src);
	bool		operator==(const DibEntry& src) const;
	bool		operator<(const DibEntry& src) const;
	BOOL		Set(const CString& name, bool autodel = true);
	Dib*&		Load();
	Dib*		pDib;
	char		fullPathName[_MAX_PATH];
	bool		autoDelete;
	FileStatus*	fs;
};


class DibEntryVec
{
public:
	void clear(int newAllocSize);
	void* QSortPointer();
	void removeAt(int which);
	void clear();
						DibEntryVec(int aAllocSize = defaultAllocSize);
						~DibEntryVec();
	DibEntry&			operator[](int i);
	//const DibEntry&		operator[](int i) const;
	void				insert(const DibEntry& de);
	int					length();
	void				Find(LPCTSTR chars, int* results);
	int					FindFirst(CString& what);
	int					FindLast(CString& what);
private:
	void setAllocSize(int newAllocSize);
	void Resize();
	int numUsed;
	int numAlloc;
	int					allocSize;
	static int			defaultAllocSize;
	DibEntry* data;
public:
};




#endif