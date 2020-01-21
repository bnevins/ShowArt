// fullscre.h : header file
//

 
/////////////////////////////////////////////////////////////////////////////
// FullScreenWin window
class DibIterator;

class FullScreenWin : public CWnd
{
	typedef CWnd super;
	WORD	wTimer;
	const 	WORD timerID, timerTime;
	DibLib	*theDibLib;
	Dib 	*theDib;
    BOOL 	stretch;	// just like the stretch in the view class...
	int		width, height;		// w & h of screen in pixels...

protected:
	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    BOOL 	Create(CWnd* pParentWnd);		// overload
public:
			FullScreenWin(CWnd* pPU, BOOL aStretch, DibLib* aDibLib);
			virtual ~FullScreenWin();
    void	SetDib(Dib* adib);
    void	SetTimer()	{ wTimer = ::SetTimer(m_hWnd, timerID, Prefs.autoTime * 1000, NULL); }
    void	KillTimer()	{ if(wTimer)  ::KillTimer(m_hWnd, timerID); wTimer = 0; }
									 						 
	// Don't put any user-stuff below here....
protected:
	//{{AFX_MSG(FullScreenWin)
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void	Draw(CPaintDC& dc);
	void	DrawBackground(CDC& dc);
	CString className;
	UINT	classStyle;
//	int		originalProgressMonitor;
};

/////////////////////////////////////////////////////////////////////////////

			
