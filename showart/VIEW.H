// view.h : interface of the CShowartView class
//
/////////////////////////////////////////////////////////////////////////////

class CShowartView : public CScrollView
{  
	typedef CScrollView super;
	WORD	wTimer;
	const 	WORD timerID;

	BOOL	stretch;
protected: // create from serialization only
	CShowartView();
	DECLARE_DYNCREATE(CShowartView)

// Attributes
public:
	CShowartDoc* GetDocument();
    LRESULT CShowartView::OnRealize(WPARAM wParam, LPARAM);
	virtual ~CShowartView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	// used to get WM_MBUTTONDOWN....	
	void		SetScrollSizes(); // hides base class virtual func.
	void		Stretch(BOOL newStretch)	{ stretch = newStretch; }
	BOOL		Stretch()					{ return stretch; }
#ifdef _DEBUG	   					 
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Printing support
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Generated message map functions
protected:
	//{{AFX_MSG(CShowartView)
	afx_msg void OnViewFullscreen();
	afx_msg void OnViewFittowindow();
	afx_msg void OnUpdateViewFittowindow(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnWallpaper();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CMainFrame* GetMainFrame();
//	DirectDrawWin* pDDW;
	void	SetTimer()	{ wTimer = ::SetTimer(m_hWnd, timerID, Prefs.autoTime * 1000, NULL); }
    void	KillTimer()	{ if(wTimer)  ::KillTimer(m_hWnd, timerID); wTimer = 0; }
	void	MyDraw(CDC* pDC);
};

#ifndef _DEBUG	// debug version in view.cpp
inline CShowartDoc* CShowartView::GetDocument()
   { return (CShowartDoc*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
