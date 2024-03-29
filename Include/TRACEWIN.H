////////////////////////////////////////////////////////////////
// TRACEWIN Copyright 1995 Microsoft Systems Journal. 
// If this program works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// TRACEWIN header file. Include this file in any application you want to
// use TRACEWIN. #include-ing this file will make diagnostic output go
// to the TRACEWIN applet if it is running.
//
// You should only #include this file one place in your app.
// 
// You must also call CMfxTrace::Init as the first thing in your app's 
// InitInstance function.
//
//
#define TRACEWND_CLASSNAME "MfxTraceWindow"
#define TRACEWND_MESSAGE   "*WM_TRACE_MSG"

#ifdef _DEBUG

class CMfxTrace : public CFile {
	static CMfxTrace theTracer;	// one-and-only tracing object
	CMfxTrace();						// private constructor
public:
	virtual void Write(const void* lpBuf, UINT nCount);
	static void Init();
};

// The one and only tracer
CMfxTrace CMfxTrace::theTracer;

//////////////////
// Constructor installs itself as file afxDump
//
CMfxTrace::CMfxTrace()
{
	ASSERT(this==&theTracer);
}

/////////////////
// Initialize tracing. Replace global afxDump.m_pFile with me.
//
void CMfxTrace::Init()
{
	if (afxDump.m_pFile == NULL) {
		afxDump.m_pFile = &theTracer;
	} else if (afxDump.m_pFile != &theTracer) {
		TRACE("afxDump is already using a file: TRACEWIN not installed.\n");
	}
}

//////////////////
// Override Write function to Write to TRACEWIN applet instead of file.
//
void CMfxTrace::Write(const void* lpBuf, UINT /* nCount */)
{
	if (!afxTraceEnabled)
		return;

	CWnd *pTraceWnd = CWnd::FindWindow(TRACEWND_CLASSNAME, NULL);
	if (pTraceWnd) 
	{
#if 0
		COPYDATASTRUCT cds;
		cds.dwData = ID_COPYDATA_TRACEMSG;
		cds.cbData = nCount;
		cds.lpData = (void *)lpBuf;
		pTraceWnd->SendMessage(WM_COPYDATA, (AfxGetApp()->m_pMainWnd->GetSafeHwnd(), 
			(LPARAM)&cds);		
		
#endif		
		static UINT WM_TRACE_MSG = RegisterWindowMessage(TRACEWND_MESSAGE);

		// Found Trace window: send message there as a global atom.
		// Delete atom after use.
		//
		ATOM atom = GlobalAddAtom((LPCSTR)lpBuf);
		pTraceWnd->SendMessage(WM_TRACE_MSG, (WPARAM)atom);
		GlobalDeleteAtom(atom);

	} else {
		// No trace window: do normal debug thing
		//
		::OutputDebugString((LPCSTR)lpBuf);	
	}
}

#endif // _DEBUG
