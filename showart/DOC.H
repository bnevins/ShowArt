// doc.h : interface of the CShowartDoc class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "dib_iter.h"

class CShowartDoc : public CDocument
{
//	Dib *theDib;

protected: // create from serialization only
	CShowartDoc();
	DECLARE_DYNCREATE(CShowartDoc)
	virtual	BOOL	OnNewDocument();
	virtual	BOOL	OnOpenDocument(const char *fname);
	virtual	void	OnCloseDocument();
					  				

public:
	LPCTSTR GetFileName();

	virtual ~CShowartDoc();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
    Dib		*&GetDib();
    //HPALETTE	*GetPal()	{ if(theDib) return theDib->GetPal(); else return 0; }
    void	SetDib(Dib* newDib);
	SIZE	GetSize();			   	
#ifdef _DEBUG					 	   	
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif
// Generated message map functions
protected:
	//{{AFX_MSG(CShowartDoc)
	afx_msg void OnRotate90();
	afx_msg void OnRotate180();
	afx_msg void OnRotate270();
	afx_msg void OnUpdateRotate90(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRotate270(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRotate180(CCmdUI* pCmdUI);
	afx_msg void OnSkipForward();
	afx_msg void OnSkipBack();
	afx_msg void OnSkipForwardMore();
	afx_msg void OnSkipBackMore();
	afx_msg void OnSuperDelete();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnViewShrink2();
	//}}AFX_MSG
	BOOL OnSaveDocument(LPCTSTR lpszPathName);

	DECLARE_MESSAGE_MAP()
private:
	void Invalidate();
	void OnRotate(int numQuarterTurns);
	void OnUpdateRotate(CCmdUI* pCmdUI);
	int dibIndex;
	static int superMoveNumChars;
	static CString superMoveTarget;
public:
	afx_msg void OnViewNoShrink();
	afx_msg void OnViewShrink4();
	afx_msg void OnViewShrink8();
private:
	void OnViewShrink(double scale);
public:
	afx_msg void OnSuperSkip();
	afx_msg void OnSuperMove();
	afx_msg void OnSuperSkipDefault();
};


/////////////////////////////////////////////////////////////////////////////
