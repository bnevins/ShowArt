// KeyCommandHandler.h: interface for the KeyCommandHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYCMD_H__79A66911_CFF3_11D0_8AA8_F4D3F7000000__INCLUDED_)
#define AFX_KEYCMD_H__79A66911_CFF3_11D0_8AA8_F4D3F7000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class KeyCommandHandler  
{
public:
	KeyCommandHandler(UINT nChar, CWnd& aWnd, Dib** aPPDib = NULL);
	virtual ~KeyCommandHandler();

private:
	void PrevOrNextDib(bool isNext);
	void PrevDib();
	Dib**	ppDib;
	CWnd&	wnd;
	void	NextDib();
};

#endif // !defined(AFX_KEYCMD_H__79A66911_CFF3_11D0_8AA8_F4D3F7000000__INCLUDED_)
