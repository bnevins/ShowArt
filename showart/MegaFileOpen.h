#if !defined(AFX_MEGAFILEOPEN_H__584C7731_BD21_11D0_8A80_B22965000000__INCLUDED_)
#define AFX_MEGAFILEOPEN_H__584C7731_BD21_11D0_8A80_B22965000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MegaFileOpen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMegaFileOpen dialog

class CMegaFileOpen : public CFileDialog
{
	DECLARE_DYNAMIC(CMegaFileOpen)

public:
	CMegaFileOpen(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	//{{AFX_MSG(CMegaFileOpen)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEGAFILEOPEN_H__584C7731_BD21_11D0_8A80_B22965000000__INCLUDED_)
