
#include "STDAFX.H"
#include "WinUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void ListFilesInDirectory(LPCTSTR dirName, CStringArray& filepaths )
{
	// BUG ALERT FIXME  Instead of *.* -- get bmp,tif,jpg,png, etc
	// FIXME

	ASSERT( dirName != NULL );
	CFileFind finder;
	CString wildcard( dirName );
	wildcard += _T("\\*.*");
	BOOL working = finder.FindFile( wildcard );
	while ( working )
	{
		working = finder.FindNextFile();

		if ( finder.IsDots() ) {
			continue;
		}

		// Skip sub-directories

		if ( finder.IsDirectory() )
			continue;

		filepaths.Add( finder.GetFilePath() );
	}

// Cleanup file finder

finder.Close();

}

 
void MondoCreateDirectory(const CString& destDir)	// throws CString
{
	// see if it's already there -- this is the usual case so let's 
	// opptimize the code around doing this part fast!

	 if(destDir.IsEmpty())
		throw CString("Directory name is empty");

	FileStatus fs(destDir);

	if(fs.Exists())
	{
		if(fs.IsDirectory())
		{
			// all OK -- file exists and it IS a directory...
			return;
		}
		else
		{
			// Uh-oh -- it must be a file with the name of the desired directory!!
			CString msg;
			msg.Format( "I can't create the directory: %s because a file already exists with that name.\n\n"
						"Shall I delete the file so that I can create the directory?", destDir);
			
			if(IDNO == AfxMessageBox(msg, MB_YESNO))
				throw CString("");

			_unlink(destDir);
		}	
	}

	// FIX THIS!!!!
	if(destDir.Find("..") > 0)	// create dir will freak...
		throw CString("\"..\" for the parent directory isn't supported.  Use a full path name instead.");

	if(::CreateDirectory(destDir, NULL))
	{
		// that was easy!  It was ready to be created!!
		return;
	}
	
	// OK -- we must need to create the directories upstream...

	int index = destDir.ReverseFind('\\');

	if(index < 0)
	{
		// this means we have a  fullpath name like "foo"
		// we're here so that means it couldn't be created.
		ShowLastError();
		throw CString("");
		//return false;
	}
	
	CString subDestDir = destDir.Left(index);

	MondoCreateDirectory(subDestDir);	// may throw...
	MondoCreateDirectory(destDir);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

BOOL IsDir(const CString& name)
{
	FileStatus fs(name);

	if(fs.Exists() && fs.IsDirectory())
		return TRUE;
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////

void ShowLastError(LPCTSTR extra)
{
	LPVOID lpMsgBuf;
	
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	CString msg;
	
	if(extra)
		msg.Format("%s:  %s", extra, lpMsgBuf);
	else
		msg = (LPCTSTR)lpMsgBuf;

	::MessageBox(NULL, msg, "Error", MB_OK | MB_ICONINFORMATION );
	LocalFree( lpMsgBuf );
} 


//////////////////////////////////////////////////////////////////////////
//// Clipboard support

//---------------------------------------------------------------------
//
// Function:   CopyHandle (from SDK DibView sample clipbrd.c)
//
// Purpose:    Makes a copy of the given global memory block.  Returns
//             a handle to the new memory block (NULL on error).
//
//             Routine stolen verbatim out of ShowDIB.
//
// Parms:      h == Handle to global memory to duplicate.
//
// Returns:    Handle to new global memory block.
//
//---------------------------------------------------------------------

HANDLE CopyHandle (HANDLE h)
{	   
	BYTE huge *lpCopy;
	BYTE huge *lp;
	HANDLE     hCopy;
	DWORD      dwLen;

	if (h == NULL)
		return NULL;

	dwLen = ::GlobalSize((HGLOBAL) h);

	if ((hCopy = (HANDLE) ::GlobalAlloc (GHND, dwLen)) != NULL)
	{
		lpCopy = (BYTE huge *) ::GlobalLock((HGLOBAL) hCopy);
		lp     = (BYTE huge *) ::GlobalLock((HGLOBAL) h);

		while (dwLen--)
			*lpCopy++ = *lp++;

		::GlobalUnlock((HGLOBAL) hCopy);
		::GlobalUnlock((HGLOBAL) h);
	}

	return hCopy;
}


/////////////////////////////////////////////////////////////////////////////////////////////////

BOOL Exists(LPCTSTR path) {
	CFileStatus status;

	return CFile::GetStatus( path, status );
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CString NextFileName(CString&  path) {
	int len = path.GetLength();
	int lenofname = path.ReverseFind('.');

	if(lenofname < 0) // WEIRD -- no extension!!
		return path;

	int lenofextanddot = len - lenofname;
	
	for(int i = 1; i < 1000; i++) {
		CString ret;
		ret.Format("%s(%d)%s", path.Left(lenofname), i, path.Right(lenofextanddot));

		if(!Exists(ret))
			return ret;
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////

