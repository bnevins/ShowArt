// doc.cpp : implementation of the CShowartDoc class
//

#include "stdafx.h"
#include "showart.h"
#include "doc.h"
#include "dib_iter.h"
#include "SuperDelete.h"
#include "SuperSkip.h"
#include "SuperMove.h"
#include ".\doc.h"
#include "Winutil.h"
//#include "JpegRotater.h"
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
	
/////////////////////////////////////////////////////////////////////////////
// CShowartDoc

IMPLEMENT_DYNCREATE(CShowartDoc, CDocument)

BEGIN_MESSAGE_MAP(CShowartDoc, CDocument)
	//{{AFX_MSG_MAP(CShowartDoc)
	ON_COMMAND(ID_ROTATE_90, OnRotate90)
	ON_COMMAND(ID_ROTATE_180, OnRotate180)
	ON_COMMAND(ID_ROTATE_270, OnRotate270)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_90, OnUpdateRotate90)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_270, OnUpdateRotate270)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_180, OnUpdateRotate180)
	ON_COMMAND(ID_SKIP_FORWARD, OnSkipForward)
	ON_COMMAND(ID_SKIP_BACK, OnSkipBack)
	ON_COMMAND(ID_SKIP_FORWARD_MORE, OnSkipForwardMore)
	ON_COMMAND(ID_SKIP_BACK_MORE, OnSkipBackMore)
	ON_COMMAND(ID_SUPER_DELETE, OnSuperDelete)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_SHRINK2, OnViewShrink2)
	ON_COMMAND(ID_VIEW_NO_SHRINK, OnViewNoShrink)
	ON_COMMAND(ID_VIEW_SHRINK4, OnViewShrink4)
	ON_COMMAND(ID_VIEW_SHRINK8, OnViewShrink8)
	ON_COMMAND(ID_SUPER_SKIP, OnSuperSkip)
	ON_COMMAND(ID_SUPER_MOVE, OnSuperMove)
	ON_COMMAND(ID_SUPER_SKIP_DEFAULT, OnSuperSkipDefault)
END_MESSAGE_MAP()


	int CShowartDoc::superMoveNumChars;
	CString CShowartDoc::superMoveTarget;


/////////////////////////////////////////////////////////////////////////////
// CShowartDoc construction/destruction

CShowartDoc::CShowartDoc()
{
	dibIndex = -1;
	//theDib = NULL;
}

CShowartDoc::~CShowartDoc()
{
	/*
	if(theDib)
	{	
		//TheDibLib.Remove(theDib);
		delete theDib;
	}
	*/
}

 BOOL CShowartDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CShowartDoc::OnOpenDocument(const char *fname)
{


	TheDibLib.Add(fname);
	return TRUE;
	/*
	if(!CDocument::OnNewDocument())
		return FALSE;

	dibIndex = 0;
	//dibIndex = TheDibLib.Add(fname);
	//UpdateAllViews(NULL); 

	if(dibIndex < 0)	// error
		return FALSE;

	SetPathName(fname);
	return TRUE;
	*/
}   
/////////////////////////////////////////////////////////////////////////////

void CShowartDoc::OnCloseDocument()
{	
	// DOESN'T GET CALLED?????							  
	//TheDibLib.Remove(theDib);
	CDocument::OnCloseDocument();
}

/////////////////////////////////////////////////////////////////////////////

void CShowartDoc::SetDib(Dib* aDib)
{
	Dib*& pDib = GetDib();
	
	if(pDib && pDib != pNullDib)
		delete pDib;
	
	pDib = aDib;
}

/////////////////////////////////////////////////////////////////////////////

SIZE CShowartDoc::GetSize()
{             
	SIZE sz;
	Dib*& pDib = GetDib();

	if(pDib)
	{
		sz.cx = pDib->Width();
		sz.cy = pDib->Height();
	}
	else
	{
		sz.cx = 0;
		sz.cy = 0;
	}
	return sz;
}

/////////////////////////////////////////////////////////////////////////////
// CShowartDoc serialization

#pragma message ("Can't delete CShowartDoc::Serialize() for some reason ??!!??" __FILE__)
void CShowartDoc::Serialize(CArchive& ar)
{
	AfxMessageBox("ERROR -- Serialize should NEVER be called!!");
}


/////////////////////////////////////////////////////////////////////////////

Dib*&  CShowartDoc::GetDib()	
{ 
#ifdef _MDI
	ASSERT(dibIndex >= 0);
	return TheDibLib.GetDib(dibIndex); 
#else
	return TheDibLib.Curr(); 
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CShowartDoc diagnostics

#ifdef _DEBUG
void CShowartDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShowartDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShowartDoc commands


void CShowartDoc::OnRotate90() 
{
	OnRotate(1);
}

void CShowartDoc::OnRotate180() 
{
	OnRotate(2);
}

void CShowartDoc::OnRotate270() 
{
	OnRotate(3);
}

void CShowartDoc::OnUpdateRotate90(CCmdUI* pCmdUI) 
{
	OnUpdateRotate(pCmdUI);
}

void CShowartDoc::OnUpdateRotate270(CCmdUI* pCmdUI) 
{
	OnUpdateRotate(pCmdUI);
}

void CShowartDoc::OnUpdateRotate180(CCmdUI* pCmdUI) 
{
	OnUpdateRotate(pCmdUI);
}

void CShowartDoc::OnUpdateRotate(CCmdUI* pCmdUI)
{
	// only allowed for JPEGs

	Dib*& pdib = TheDibLib.Curr();

	if(pdib->GetType() == jpeg)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CShowartDoc::OnRotate(int numQuarterTurns)
{
	Dib*& pdib = TheDibLib.Curr();

	if(!pdib)
		return;

	if(pdib->rotate(numQuarterTurns))
	{
		TheDibLib.ReloadCurr();
		Invalidate();
	}
}

void CShowartDoc::OnSkipForward() 
{
	TheDibLib.Skip(5);
	Invalidate();
}

void CShowartDoc::OnSkipBack() 
{
	TheDibLib.Skip(-5);
	Invalidate();
}

void CShowartDoc::Invalidate()
{
	CWnd* wnd = AfxGetMainWnd();

	if(wnd)
		wnd->Invalidate();
}

void CShowartDoc::OnSkipForwardMore() 
{
	TheDibLib.Skip(20);
	Invalidate();
}

void CShowartDoc::OnSkipBackMore() 
{
	TheDibLib.Skip(-20);
	Invalidate();
}


/////////////////////////////////////////////////////////////////////////////

LPCTSTR CShowartDoc::GetFileName()
{
	// this might be NULL -- e.g. if no dib is loaded...
	return GetDib()->GetFileName();
}

/////////////////////////////////////////////////////////////////////////////

void CShowartDoc::OnFileSave() 
{
	// make sure the filename is set in the super-class
	// since the super-class will be calling 
	// CShowartDoc::OnSaveDocument() eventually...

	LPCTSTR name = GetFileName();

	if(name)
		SetPathName(name);

	CDocument::OnFileSave();
}

/////////////////////////////////////////////////////////////////////////////

#pragma message ("TEMP CODE HERE--> " __FILE__ )
void CShowartDoc::OnFileSaveAs() 
{
	CDocument::OnFileSaveAs();
}


/////////////////////////////////////////////////////////////////////////////

BOOL CShowartDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if(!lpszPathName || !*lpszPathName)
		return FALSE;

	Dib& dib = *GetDib();
	WORD numBits = dib.NumColorBits();

	// JPEG library I have will throw an Exception if bits isn't 8 or 24!
	if (numBits != 8 && numBits != 24)
	{
		CString message;
		message.Format("The JPEG Library can only save images with 8 or 24 bits per pixel.  This image has %u bit%s.  Sorry!", numBits, numBits == 1 ? "" : "s");
		AfxMessageBox(message);
		return FALSE;
	}

	return GetDib()->Write(lpszPathName, Prefs.jpegQuality);
}


void CShowartDoc::OnViewShrink2()
{
	OnViewShrink(1.0 / 2.0);
}

void CShowartDoc::OnViewNoShrink()
{
	OnViewShrink(1.0);
}

void CShowartDoc::OnViewShrink4()
{
	OnViewShrink(1.0 / 4.0);
}

void CShowartDoc::OnViewShrink8()
{
	OnViewShrink(1.0 / 8.0);
}

void CShowartDoc::OnViewShrink(double scale)
{
	Dib::setScale(scale);
	TheDibLib.ReloadCurr();
	Invalidate();
}

void CShowartDoc::OnSuperDelete() 
{
	SuperDelete dlg;
	int result = dlg.DoModal();

	if(result == IDOK)
	{
		int numchars = dlg.getNumChars();
		TheDibLib.SuperDelete(numchars);
		Invalidate();
	}
}

void CShowartDoc::OnSuperSkip()
{
	SuperSkip dlg;
	int result = dlg.DoModal();

	if(result == IDOK)
	{
		int numchars = dlg.getNumChars();
		Prefs.Write();
		TRACE("NUMCHARS: %d\n", numchars);
		TheDibLib.SuperSkip(numchars);
		Invalidate();
	}
}

void CShowartDoc::OnSuperMove()
{
	if(superMoveNumChars <= 0)
		superMoveNumChars = 5;

	SuperMove dlg(superMoveNumChars, superMoveTarget);
	int result = dlg.DoModal();

	if(result == IDOK)
	{
		superMoveNumChars = dlg.getNumChars();
		superMoveTarget = dlg.getTarget();
		TheDibLib.SuperMove(superMoveNumChars, superMoveTarget);
		Invalidate();
	}
}

void CShowartDoc::OnSuperSkipDefault()
{
	TheDibLib.SuperSkip(Prefs.superSkipNum);
		Invalidate();
}
