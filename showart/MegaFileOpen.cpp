// MegaFileOpen.cpp : implementation file
//

#include "stdafx.h"
#include "showart.h"
#include "MegaFileOpen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMegaFileOpen

IMPLEMENT_DYNAMIC(CMegaFileOpen, CFileDialog)

CMegaFileOpen::CMegaFileOpen(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	memset(&m_ofn, 0, sizeof(m_ofn)); // initialize structure to 0/NULL
	m_szFileName[0] = '\0';
	m_szFileTitle[0] = '\0';
	m_pofnTemp = NULL;
	m_bOpenFileDialog = bOpenFileDialog;
	m_nIDHelp = bOpenFileDialog ? AFX_IDD_FILEOPEN : AFX_IDD_FILESAVE;
	m_ofn.lStructSize = sizeof(m_ofn);
	m_ofn.lpstrFile = m_szFileName;
	m_ofn.nMaxFile = _countof(m_szFileName);
	m_ofn.lpstrDefExt = lpszDefExt;
	m_ofn.lpstrFileTitle = (LPTSTR)m_szFileTitle;
	m_ofn.nMaxFileTitle = _countof(m_szFileTitle);
	m_ofn.Flags |= dwFlags | OFN_ENABLEHOOK;
	if (!afxData.bWin4 && AfxHelpEnabled())
		m_ofn.Flags |= OFN_SHOWHELP;
	if (afxData.bWin4)
	{
		m_ofn.Flags |= OFN_EXPLORER;
		m_ofn.hInstance = AfxGetResourceHandle();
	}
	m_ofn.lpfnHook = (COMMDLGPROC)_AfxCommDlgProc;

	// setup initial file name
	if (lpszFileName != NULL)
		lstrcpyn(m_szFileName, lpszFileName, _countof(m_szFileName));

	// Translate filter into commdlg format (lots of \0)
	if (lpszFilter != NULL)
	{
		m_strFilter = lpszFilter;
		LPTSTR pch = m_strFilter.GetBuffer(0); // modify the buffer in place
		// MFC delimits with '|' not '\0'
		while ((pch = _tcschr(pch, '|')) != NULL)
			*pch++ = '\0';
		m_ofn.lpstrFilter = m_strFilter;
		// do not call ReleaseBuffer() since the string contains '\0' characters
	}

		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
}


BEGIN_MESSAGE_MAP(CMegaFileOpen, CFileDialog)
	//{{AFX_MSG_MAP(CMegaFileOpen)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

