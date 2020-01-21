#ifndef __DIB_ITER_H__
#define __DIB_ITER_H__
// DibIterator Class
// go through the painful lists & return appropriate attached dibs...
//

#ifndef __DIBENTRYVEC_H__
#include "DibEntryVec.h"
#endif

class	DibLib
{
	DibEntryVec dibs;
	//DibEntry dibs[MAXDIBS + 1];
	
protected:	
	int				curr;
	//int				num;
	static bool		dontLoad;
public:
	void SuperDelete(int howManyChars);
	void	Skip(int howMany);
	void Clear(int num);
	void Replace(const CStringArray& names, bool autoDelete = true);
	void Add(const CStringArray& names, bool autoDelete = true);
	int GetCurr();
	void DeleteFile(int which);
	void DeleteFile();
	int AddList(const CString& filename, bool autoDelete = true);
	enum eSortType { eName, eSize, eDate, eRandom };
	enum eSortDirection { eAscending, eDescending };
	void Sort();
	void Sort(eSortType type, eSortDirection direction);
	void Clear();
	Dib*&			GetDib(int whichOne);
	static void		 DontLoad(bool newVal);
	DibLib();
	~DibLib() { }
	int				Add(const CString& filename, bool autoDelete = true);
	//void	Remove()	{ if(curr >= 0) Remove(lib[curr]); }
	//void	Remove(Dib *aDib);
	//void	RemoveAll()	{ curr = -1; num = 0; }
	Dib				*First();
	Dib				*Last();
	int				Num()  						{ return dibs.length(); } 					   
	
	virtual Dib		*Next();							  				  	
	virtual Dib		*Prev();
	virtual Dib		*&Curr();
	void			ReloadCurr();
private:
	bool IsList(const CString& filename);
	bool			IsValidArtName(LPCTSTR fName);
	void			DecrementPrev();
	void			DecrementCurr();
	void			random_sort();
	static int		sortFunc(const void* e1, const void* e2);
	void			SetTitle(const CString& newTitle);
	int				prev;
	static eSortType	m_sortType;
	static BOOL			m_descendingSort;
	void AddOrReplace(const CStringArray& names, bool autoDelete, bool replace);

public:
	void SuperSkip(int howManyChars);
private:
	CString getFirstChars(int numChars);
public:
	void SuperMove(int numChars, CString& target);
};				 

extern DibLib	TheDibLib;	// GLOBAL OBJECT!!!!
#endif