// mainfrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef FULLSCREEN__H__
#include "FullScreen.h"
#endif

class CMainFrame : public CBaseFrameWnd
{
	DECLARE_DYNCREATE(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Implementation
public:
	void toggleFullScreen();
	void stopFullScreen();
	void startFullScreen();
	bool isFullScreen();
	virtual ~CMainFrame();
	virtual CView* GetActiveView();
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

private:
	friend		FullScreen;
	FullScreen	fullScreen;
protected:	// control bar embedded members
	CStatusBar	m_wndStatusBar;
	CToolBar	m_wndToolBar;
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnEditEmptyclipboard();
	afx_msg void OnClose();
	afx_msg void OnFileNew();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////