
// DibIterator Class
// go through the painful lists & return appropriate attached dibs...
//
#include "stdafx.h"
#include "showart.h"
#include "doc.h"
#include "dib_iter.h"
#include "WinUtil.h"
#include "Filename.h"
#include ".\dib_iter.h"

static LPCTSTR	ART_TYPES[] = { "JPG", "GIF", "TIF", "BMP" };
const int		NUM_ART_TYPES = sizeof(ART_TYPES) / sizeof(*ART_TYPES);

//////////////////////////////////////////////////////////////////////////

DibLib				TheDibLib;	// GLOBAL OBJECT!!!!
bool				DibLib::dontLoad = false;
DibLib::eSortType	DibLib::m_sortType = DibLib::eName;
BOOL				DibLib::m_descendingSort = FALSE;

/////////////////////////////////////////////////////////////////////////////
	
inline CString RootName(const CString& full)
{
	int index = full.ReverseFind('\\');

	if(index >= 0)
		return full.Right(full.GetLength() - index - 1);
	else
		return full;

}
/////////////////////////////////////////////////////////////////////////////
		
DibLib::DibLib()
{
	srand( (unsigned)time( NULL ) );
	prev = curr = -1;
}	

/////////////////////////////////////////////////////////////////////////////
	
int DibLib::Add(const CString& filename, bool autoDelete)
{
//	ASSERT(aDib && *aDib);
	if(IsList(filename))
		return AddList(filename, autoDelete);

	if(!IsValidArtName(filename))
	{
		return Num() - 1;
	}
	
	DibEntry de;
	if(!de.Set(filename, autoDelete)) // no autoDelete implies auto-load...
		return -1;

	if(curr < 0)
		curr = 0;
	
	if(!dontLoad)
		curr = Num();
	
	dibs.insert(de);
	//++num;

	//VERIFY(num < MAXDIBS);
	
	return Num() - 1;
}	

/////////////////////////////////////////////////////////////////////////////
	
int DibLib::AddList(const CString& filename, bool autoDelete)
{
	// file loaded with filenames -- one per line
	// if the name starts with a drive letter, colon - it is absolute
	// if the name starts with a slash -- then prepend the drive of the list file
	// if the name starts with anything else it is relative to the path of the list file.  Prepend that path.
	int ret = curr;

	try
	{
		char buf[_MAX_PATH * 2];		

		if(filename.GetLength() <= 0)
			throw "bad filename argument";

		ifstream is;
		is.open(filename);
		Filename listfilename(filename);
		CString drive(filename.GetAt(0));
		drive += ":";

		if(!is.is_open())
			throw "Can't open filelist file";
		
		while(!is.eof())
		{
			is.getline(buf, sizeof(buf));

			if(!*buf || *buf == '#') // ignore empty lines and comments
				continue;

			CString name;
			// only 3 cases!
			if(buf[1] == ':')
				name = buf;
			else if(*buf == '\\' || *buf == '/')
				name = drive +  buf;
			else
				name = listfilename.path + '\\' + buf;

			Add(name, autoDelete);
		}
	}
	catch(const char* msg)
	{
		AfxMessageBox(msg);
	}

	return ret;
}	

/////////////////////////////////////////////////////////////////////////////
	
void DibLib::Add(const CStringArray& names, bool autoDelete)
{
	AddOrReplace(names, autoDelete, false);
}	

/////////////////////////////////////////////////////////////////////////////
	
void DibLib::Replace(const CStringArray& names, bool autoDelete)
{
	AddOrReplace(names, autoDelete, true);
}	

/////////////////////////////////////////////////////////////////////////////
	
void DibLib::AddOrReplace(const CStringArray& names, bool autoDelete, bool replace)
{
	int	newNum	= names.GetSize();

	if(replace)
		Clear(newNum);
	
	DontLoad(true);

	for(int i = 0; i < newNum; i++)
	{
		CString name = names[i];
		Add(name, autoDelete);
	}
	
	DontLoad(false);
	Sort();
	
	if(theApp.m_pMainWnd)
		theApp.m_pMainWnd->Invalidate();
}	

/////////////////////////////////////////////////////////////////////////////
	
	  

void DibLib::DontLoad(bool newVal)
{
	dontLoad = newVal;
}

Dib*& DibLib::GetDib(int whichOne)
{
	VERIFY(whichOne >= 0 && whichOne < Num() && Num() >0);
	return dibs[whichOne].pDib;
}

Dib*	DibLib::First()	
{ 
	if(Num() <= 0)
		return pNullDib;

	prev = curr;	
	curr = 0; 
	return Curr(); 
}

Dib*	DibLib::Last()		
{ 
	ASSERT(Num() > 0); 
	prev = curr;	
	curr = Num() - 1; 
	return Curr(); 
}

/////////////////////////////////////////////////////////////////////////////
	
	
Dib	*DibLib::Next()
{
	if(Num() <= 0)
		return Curr();

	prev = curr;	
	
	if(++curr >= Num())
		curr = 0;

	Dib* ret = Curr();

	if(ret != pNullDib && !ret->OK())
	{
		dibs.removeAt(curr);
		
		if(prev > curr)
		{
			--prev;
		}
		return Next();
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////
	
Dib	*DibLib::Prev()
{
	if(Num() <= 0)
		return Curr();

	prev = curr;	
	if(--curr < 0)
		curr = Num() - 1;
	
	//return Curr();
	Dib* ret = Curr();

	if(ret != pNullDib && !ret->OK())
	{
		dibs.removeAt(curr);
		
		if(prev > curr)
		{
			--prev;
		}
		return Prev();
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////
	
void DibLib::ReloadCurr()
{
	int num = Num();
	
	if(num <= 0)
		return;

	ASSERT(num > 0);

	if(curr > num - 1)
		return;

	if(curr < 0)
		return;

	try
	{
		delete dibs[curr].pDib;
	}
	catch(...)
	{
		AfxMessageBox("Delete Error");
	}
	dibs[curr].pDib = NULL;
	Curr();
}	

/////////////////////////////////////////////////////////////////////////////
	
Dib*& DibLib::Curr()
{
	int num = Num();
	
	if(num <= 0)
		return pNullDib;

	ASSERT(num > 0);

	if(curr >= num - 1)
		curr = num - 1;

	if(curr < 0)
		curr = 0;

	Dib*& dib = dibs[curr].Load();

/*
	if(!dontLoad && !dib->OK())
	{
		dibs.removeAt(curr);
		
		if(prev == curr)
		{
			prev = -1;
		}
		else if(prev > curr)
		{
			--prev;
			--curr;
		}
		return Curr();
	}
*/
	if(prev >= 0 && prev != curr && prev < dibs.length() && dibs[prev].pDib)
	{
		try
		{
			delete dibs[prev].pDib;
		}
		catch(...)
		{
			AfxMessageBox("Delete Error");
		}
		dibs[prev].pDib = NULL;
	}
	
	// bn 12/29/03 - added support for putting special info on the file name.

	CString title;
	int scale = Dib::getScale();
    
	if(scale > 1)
		title.Format("%s  *** 1/%d shrinking", dib->Name(), scale);
	else
		title.Format("%s", dib->Name());

	SetTitle(title);
	return dib;
}

/////////////////////////////////////////////////////////////////////////////

void DibLib::SetTitle(const CString& newTitle)
{
	CWnd* pWnd = AfxGetMainWnd();

	if(!pWnd)
		return;

	CString currTitle;
	pWnd->GetWindowText(currTitle);

	if(currTitle != newTitle)
		pWnd->SetWindowText(newTitle);
}

void DibLib::Clear()
{
	dibs.clear();
	prev = curr = -1;
}

void DibLib::Clear(int num)
{
	dibs.clear(num);
	prev = curr = -1;
}

void DibLib::Sort()
{
	Sort((eSortType)Prefs.m_sortType, (eSortDirection)Prefs.m_sortDirection);
}

void DibLib::Sort(eSortType type, eSortDirection direction)
{
	if(direction == eAscending)
		m_descendingSort = false;
	else
		m_descendingSort = true;

	m_sortType = type;

	if(type == eRandom)
		random_sort();
	else
		qsort(dibs.QSortPointer(), Num(), sizeof(DibEntry), sortFunc);
}

int DibLib::sortFunc(const void* e1, const void* e2)
{
	int		ret;
	DibEntry& de1 = *(DibEntry*)e1;
	DibEntry& de2 = *(DibEntry*)e2;

	switch(m_sortType)
	{
		case eSize:		ret = de1.fs->Size() - de2.fs->Size();												break;
		case eDate:		ret = (de1.fs->Timestamp() < de2.fs->Timestamp()) ? -1 : (de1.fs->Timestamp() > de2.fs->Timestamp()) ? 1 : 0;	break;
		case eName:		ret = stricmp(RootName(de1.fullPathName), RootName(de2.fullPathName));		break;
		default:		ASSERT(FALSE); ExitProcess(86); break;
	}

	if(m_descendingSort)
		ret *= -1;

	return ret;
}

void DibLib::DeleteFile()
{
	DeleteFile(curr);
}

void DibLib::DeleteFile(int which)
{
	if(Num() <= 0 || which < 0)
		return;

	//--num;

	//delete dibs[curr].pDib;
	//dibs[curr].pDib = NULL;
	
	CString fileToDelete = dibs[which].fullPathName;

	dibs.removeAt(which);
	if(curr > which)
		--curr;
	if(prev > which)
		--prev;

	if(!::DeleteFile(fileToDelete))
	{
		//ShowLastError();
		TRACE("Couldn't delete %s\n", fileToDelete);
	}

	// note: better not have any dtors that need calling!!!
	//if(num - curr > 0)
		//MoveMemory(dibs + curr, dibs + curr + 1, (num - curr) * sizeof(*dibs));
	

	if(Num() <= 0)
		curr = -1;
}


void DibLib::random_sort()
{
	// ridiculous!!!  use a better array class!!!  or use handles...
	int num = Num();
	if(num <= 1)
		return;

	int iter = num * 2;

	while(--iter)
	{
		DibEntry temp;
		int one, two;
		
		one = rand() % num;
		
		while((two = rand() % num) == one)
			;
		TRACE("** Swapping %d and %d\n", one, two);
		
		temp = dibs[one];
		dibs[one] = dibs[two];
		dibs[two] = temp;
	}
}


int DibLib::GetCurr()
{
	return curr;
}

void DibLib::DecrementCurr()
{
	--curr;

	if( curr < 0)
		curr = 0;
}


void DibLib::DecrementPrev()
{
	if(prev < 0)
		return;
	
	--prev;
}

bool DibLib::IsValidArtName(LPCTSTR fName)
{
	// check last 4 chars...
	int len = strlen(fName);

	if(len < 5 || fName[len - 4] != '.')
		return false;

	CString ext = fName + len - 3;
	ext.MakeUpper();

	for(int i = 0; i < NUM_ART_TYPES; i++)
		if(ext == ART_TYPES[i])
			return true;

	TRACE("Non-Art Filename: %s\n", fName);
	return false;
}

bool DibLib::IsList(const CString & filename)
{
	if(filename.GetLength() < 5)
		return false;

	if(filename.Right(4).CompareNoCase(".lst") == 0)
		return true;

	return false;
}


/////////////////////////////////////////////////////////////////////////////

void DibLib::Skip(int num)
{
	if(curr < 0 || curr >= Num())
		return;

	if(num == 0)
		return;

	prev = curr;	
	curr += num;

	if(curr >= Num() && num >= 0)
	{
		curr = curr - Num();
	}

	else if(curr < 0 && num < 0)
	{
		curr = Num() + curr;		// curr is negative!
	}

	if(curr < 0 || curr >= Num())
		// sanity check!
		curr = prev;

	Dib* ret = Curr();

	if(ret != pNullDib && !ret->OK())
	{
		dibs.removeAt(curr);
		
		if(prev > curr)
		{
			--prev;

		}
		Next();
	}
}

void DibLib::SuperDelete(int howManyChars)
{
	CString chars = getFirstChars(howManyChars);
	
	if(chars.GetLength() != howManyChars)
		return;

	while(1)
	{
		int which = dibs.FindFirst(chars);
			
		if(which < 0)
			break;

		TRACE("Deleting: %s\n", dibs[which].fullPathName);
		DeleteFile(which);
	}
}

void DibLib::SuperSkip(int howManyChars)
{
	CString chars = getFirstChars(howManyChars);
	
	if(chars.GetLength() != howManyChars)
		return;

	int which = dibs.FindLast(chars);

	if(which < 0)
		return;

	curr = which;
	Curr();
}

CString DibLib::getFirstChars(int howManyChars)
{
	int len = dibs.length();
	Dib* dib = Curr();

	if(!dib || dib == pNullDib)
		return "";
	
	CString dibFilename = dib->GetFileName();
	Filename fn(dibFilename);
	CString chars = fn.name.Left(howManyChars);

	return chars;
}

void DibLib::SuperMove(int howManyChars, CString& target)
{
	if(!IsDir(target))
	{
		try
			{
				MondoCreateDirectory(target);
			}
			catch(const CString& msg)
			{
				if(!msg.IsEmpty()) // empty string means user has been alerted...
				{
					AfxMessageBox(msg);
					return;
				}
			}
	}

	CString chars = getFirstChars(howManyChars);
	
	if(chars.GetLength() != howManyChars)
		return;

	while(1)
	{
		int which = dibs.FindFirst(chars);
			
		if(which < 0)
			break;

		CString dibFilename = dibs[which].fullPathName;
		Filename fn(dibFilename);
		CString newName = target + "\\" + fn.name + "." + fn.ext;
		TRACE("Moving %s to %s\n", dibFilename, newName);
		
		if(::MoveFileEx(dibFilename, newName, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
			dibs.removeAt(which);
	}
}
