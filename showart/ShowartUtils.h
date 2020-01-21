// ShowartUtils.h: interface for the ShowartUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWARTUTILS_H__701F31AE_217A_11D4_B06A_0090279A57DA__INCLUDED_)
#define AFX_SHOWARTUTILS_H__701F31AE_217A_11D4_B06A_0090279A57DA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class ShowartUtils  
{
public:
	static CRect GetFullScreenRect();
	static CRect FitToWindow(const CSize window,const Dib& dib, BOOL stretch);
	static CRect FitToWindow(const CSize window, CSize dib, BOOL stretch);
	static CString BrowseForFolder(void);
};

#endif // !defined(AFX_SHOWARTUTILS_H__701F31AE_217A_11D4_B06A_0090279A57DA__INCLUDED_)
