#pragma once
// showart.h : main header file for the SHOWART application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define WM_REALIZE 	(WM_USER + 0)

/////////////////////////////////////////////////////////////////////////////
// CShowartApp:
// See showart.cpp for the implementation of this class
//

extern Dib		NullDib;
extern Dib*	pNullDib;


//#define _MDI

#ifdef _MDI
#define CBaseFrameWnd	CMDIFrameWnd
#else
#define CBaseFrameWnd	CFrameWnd
#endif

class CMainFrame;
class CShowartView;
class CShowartDoc;

class CShowartApp : public CWinApp
{
	CMultiDocTemplate *bmpTemplate;
	//BOOL globalStretch;
public:
	CMainFrame* getMainFrame();
	CShowartView* getActiveView();
	CShowartDoc* getDocument();
	BOOL WriteSubProfileBool(const CStringArray& sects, const CString & entry, bool value);
	BOOL WriteSubProfileString(const CStringArray& sects, const CString& entry, const CString& value);
	BOOL WriteSubProfileInt(const CStringArray& sects, const CString& entry, int value);
	CString GetSubProfileString(const CStringArray& sects, const CString& entry, const CString& defaultValue = "");
	UINT GetSubProfileInt(const CStringArray& sects, const CString& entry, int defaultValue);
	bool GetSubProfileBool(const CStringArray& sects, const CString& entry, bool defaultValue);
	CShowartApp();
	static const char* VERSION;
	BOOL OnDDECommand(LPTSTR lpszCommand);
// Overrides
	virtual BOOL InitInstance();
    BOOL DoPromptFileName(LPSTR fileName, UINT nIDSTitle, DWORD lFlags,
						BOOL bOpenFileDialog, CDocTemplate* pTemplate);
	BOOL ProcessShellCommand(CCommandLineInfo& rCmdInfo);
    //virtual int ExitInstance();
#if (_MSC_VER < 1000)
    POSITION		GetFirstDocTemplatePosition()		{ return m_templateList.GetHeadPosition();	}
	CDocTemplate*	GetNextDocTemplate(POSITION& pos)	{ return (CDocTemplate *)m_templateList.GetNext(pos);	}
#endif

// Implementation

	//{{AFX_MSG(CShowartApp)
	afx_msg void OnAppAbout();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnFileCloseall();
	afx_msg void OnUpdateFileCloseall(CCmdUI* pCmdUI);
	afx_msg void OnFilePreferences();
	afx_msg void OnFileKeyCommands();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString getDirName(const char* fname);
	bool IsOneFilename(const char* name);
	CString ParseFilenames(const char* mega, CStringArray& names);
	void SetShellKey(LPCTSTR key, LPCTSTR value);
	void SetupShellRegistry();
	HKEY GetSubSectionKey(const CString& sectName, const CString& subSectName);
	HKEY GetSubSectionKey(HKEY hSecKey, const CString& subSectName);
	HKEY GetSubSectionKey(const CStringArray& sects);
	CString PrepareRegError(const CString& header, const CStringArray& sects);
	static const int FILENAME_BUFFER_SIZE;
	LPCTSTR GetOpenDir();
protected:
	void ParseCommandLine( CCommandLineInfo& rCmdInfo );
private:
	static const CString True;
	static const CString False;
};

/////////////////////////////////////////////////////////////////////////////
// VB-Event extern declarations

//{{AFX_VBX_REGISTER()
//}}AFX_VBX_REGISTER

/////////////////////////////////////////////////////////////////////////////

extern CShowartApp NEAR theApp;

