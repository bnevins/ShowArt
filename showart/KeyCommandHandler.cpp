// KeyCmd.cpp: implementation of the KeyCommandHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "STDAFX.H"
#include "showart.h"
#include "KeyCommandHandler.h"
#include "dib_iter.h"
#include "WinUtil.h"
#include "Filename.h"
/*
#include "mainfrm.h"
#include "doc.h"
#include "dib_iter.h"
#include "view.h"
#include "fullscre.h"
*/

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static CString StripPath(const CString& fname)
{
	int indexName;

	if((indexName = fname.ReverseFind('\\')) < 0)
		return fname;
	else
		return fname.Right(fname.GetLength() - indexName - 1);
}

static CString GetParentDirName(const CString& fname)
{
	// TODO -- call the below method and simplify this one.  Jan 2010
	int indexName;

	if((indexName = fname.ReverseFind('\\')) < 0)
		return fname; // TODO this looks like a fat bug!!!
	else
	{
		CString s = fname.Left(indexName);

		if((indexName = s.ReverseFind('\\')) < 0)
			return fname;

		return s.Right(s.GetLength() - indexName - 1);
	}
}



KeyCommandHandler::KeyCommandHandler(UINT nChar, CWnd& aWnd, Dib** aPPDib) 
		: wnd(aWnd), ppDib(aPPDib)
{
	if( (nChar >= 'B' && nChar <= 'Z') || 
			(nChar >= '0' && nChar <= '9') )
	{
		static CString outFileName;
		static OSVERSIONINFO info;
		bool useBatchFile = Prefs.UseBatchFile();

		if(outFileName.IsEmpty())
		{
			info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx(&info); 
			
			if(info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
			{
				GetWindowsDirectory(outFileName.GetBuffer(_MAX_PATH + 1), _MAX_PATH);
				outFileName.ReleaseBuffer();
			}
			else if(info.dwPlatformId == VER_PLATFORM_WIN32_NT)
			{
				GetEnvironmentVariable("USERPROFILE", outFileName.GetBuffer(_MAX_PATH + 1), _MAX_PATH);
				outFileName.ReleaseBuffer();
			}
			
			if(!outFileName.IsEmpty())
				outFileName += "\\Desktop\\show.bat";
			else
				outFileName = "c:\\show.bat";
		}
		
		static ofstream TempFile(outFileName, ios::app);	// gets closed automatically...
		char cmd = static_cast<char>(nChar);
		KeyCommands::Mode mode = Prefs.GetKeyMode(nChar);
		
		if(mode == KeyCommands::Mode::List)
		{
			TRACE("LIST MODE!!!!!!!!!!!!!!");
			const CString listname = Prefs.GetExpandedKeyCommand(cmd);
			ofstream list(listname, ios::app);
			// Chop off the drive designator so it is independent of drive letters
			// Annoying problem using encrypted drives mounted as different drive letters.

			LPCTSTR name = TheDibLib.Curr()->Name();
			Filename artfilename(name);
			Filename listfilename(listname);

			// June 22, 2019
			// SIMPLIFY -- always put full path inside the listfile
			list << name << endl;

#if 0
			int listpathlen = strlen(listfilename.path);

			// relative path
			if(strncmp(artfilename.path, listfilename.path, listpathlen) == 0)
				name += listpathlen + 1;
			
			// relative path to the same drive
			else if(strlen(name) > 2 && name[1] == ':' && strnicmp(listname, name, 1) == 0)
				name += 2;

			list << name << endl;
#endif
		}
		else if(mode == KeyCommands::Mode::Index)
		{
			TRACE("INDEX MODE!!!!!!!!!!!!!!");
			const CString destDir = Prefs.GetExpandedKeyCommand(cmd);
			CString destName = destDir;
			destName += '\\';
			destName += GetParentDirName(TheDibLib.Curr()->Name());
			destName += ".jpg";

			if(!IsDir(destDir))
			{
				try
				{
					MondoCreateDirectory(destDir);
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
		
			if(destDir.GetLength())
			{
				if(useBatchFile)
				{
					TempFile << "del" <<  '\t' << "\"" << destName << "\"" << endl;
					TempFile << "copy" << '\t' << "\"" << TheDibLib.Curr()->Name()  << "\"" << '\t' << "\"" << destName << "\"" << endl;
				}
				else
				{
					CopyFile(TheDibLib.Curr()->Name(), destName, FALSE);
				}
			}
		}
		else // Move or Copy
		{
			TRACE("COPY MODE!!!!!!!!!!!!!!");

			const CString destDir = Prefs.GetExpandedKeyCommand(cmd);
			CString destName = destDir;
			destName += '\\';
			destName += StripPath(TheDibLib.Curr()->Name());

			if(!IsDir(destDir))
			{
				try
				{
					MondoCreateDirectory(destDir);
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
		
			if(destDir.GetLength())
			{
				CFileStatus status;
				
				if( Prefs.safeCopyMode && Exists(destName) ) {
					// file exists already.  don't overwrite it.
					destName = NextFileName(destName);

				}
  

				if(mode == KeyCommands::Mode::Move)
				{
					if(useBatchFile)
						TempFile << "move" << '\t' << "\"" << TheDibLib.Curr()->Name() << "\"" << '\t' << "\"" << destName << "\"" << endl;
					else
						::MoveFileEx(TheDibLib.Curr()->Name(), destName, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);
				}
				else
				{
					if(useBatchFile)
						TempFile << "copy" << '\t' << "\"" << TheDibLib.Curr()->Name() << "\"" << '\t' << "\"" << destName << "\"" << endl;
					else
						::CopyFile(TheDibLib.Curr()->Name(), destName, FALSE);

				}
			}
		}
	}									   
	
	switch(nChar)
	{
		int del;
		case VK_DELETE:
 						//AfxMessageBox("DEL key pressed.");
						del = TheDibLib.GetCurr();

        				
						//if(::IsWindow(wnd.m_hWnd))
							//wnd.Invalidate(TRUE); 	
						if(del >= 0) 
							TheDibLib.DeleteFile(del);

						PrevDib();
						NextDib();
						break;
		case VK_LEFT:
		case VK_UP:
        case VK_PRIOR:
						PrevDib();
						break;
		case VK_RIGHT:	
		case VK_DOWN:
        case VK_NEXT:
		default:
						NextDib();
        				
	}
}        

/////////////////////////////////////////////////////////////////////////////

KeyCommandHandler::~KeyCommandHandler()
{

}




void KeyCommandHandler::NextDib()
{
	PrevOrNextDib(true);
}

void KeyCommandHandler::PrevDib()
{
	PrevOrNextDib(false);
}

void KeyCommandHandler::PrevOrNextDib(bool isNext)
{
	Dib* pDib;

	if(isNext)
		pDib = TheDibLib.Next();
	else
		pDib = TheDibLib.Prev();
	
	if(ppDib)
		*ppDib = pDib;

	if(::IsWindow(wnd.m_hWnd))
		wnd.Invalidate(TRUE); 
}
