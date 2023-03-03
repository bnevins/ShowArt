// #include this in dib.h iff we're not a DLL
// #include in dib.cpp if we are a DLL

//#ifdef _DLL
#define INLINE
//#else
//#define INLINE inline
//#endif

INLINE 			CArt::~CArt()
{
}		 

/////////////////////////////////////////////////////////////////////////

INLINE int		CArt::OK()
{
	 return ok; 
}

/////////////////////////////////////////////////////////////////////////

INLINE 			CArt::operator int()
{
	 return ok; 
}

/////////////////////////////////////////////////////////////////////////

INLINE WORD 	CArt::Width() const
{
	 return width; 
}																			 

/////////////////////////////////////////////////////////////////////////

INLINE WORD		CArt::Height() const
{
	 return height; 
}

/////////////////////////////////////////////////////////////////////////

INLINE void		CArt::Show(HDC hdc, int x, int y, int w, int h, DWORD rop) 
{
	Show_(hdc, x, y, w, h, rop); 
}

/////////////////////////////////////////////////////////////////////////

INLINE void		CArt::Show(int x, int y, int w, int h, DWORD rop)
{
	 Show_(NULL, x, y, w, h, rop);
}

/////////////////////////////////////////////////////////////////////////

INLINE void		CArt::OK(int newOK)
{
	 ok = newOK;
}		

/////////////////////////////////////////////////////////////////////////

INLINE void		CArt::Width(int newW)
{
	 width = newW;
}

/////////////////////////////////////////////////////////////////////////

INLINE void		CArt::Height(int newH)
{
	 height = newH;
}

/////////////////////////////////////////////////////////////////////////

INLINE HWND& CArt::Owner()
{			 
	return hwndOwner;
}

/////////////////////////////////////////////////////////////////////////

INLINE void 	Dib::ShowWithPal(HDC hdc, int x, int y, int w, int h, DWORD rop)
{
	 ShowWithPal_(hdc, x, y, w, h, rop);
}

/////////////////////////////////////////////////////////////////////////

INLINE void 	Dib::ShowWithPal(int x, int y, int w, int h, DWORD rop)
{
	 ShowWithPal_(NULL, x, y, w, h, rop);
}

/////////////////////////////////////////////////////////////////////////

INLINE void Dib::Gif2Dib(LPCTSTR filename)
{
	lpDib = ::Gif2Dib(filename);
}

/////////////////////////////////////////////////////////////////////////

INLINE void Dib::Tiff2Dib(LPCTSTR filename)
{				 
	lpDib = ::Tiff2Dib(filename);
}			  

/////////////////////////////////////////////////////////////////////////

INLINE void Dib::Jpg2Dib(LPCTSTR filename)
{				 
#ifdef _DEBUG
	char buf[300];
	wsprintf(buf, "Dib::Jpg2Dib(%s, NULL, %s)\n", filename, ShowErrorsInMessageBox() ? "Show Errors In MessageBox" : "Don't show Errors In MessageBox"); 
	//MessageBox(NULL, buf, "", MB_ICONINFORMATION); 
	//TRACE(buf);
#endif

	lpDib = ::Jpg2Dib(filename, NULL, scalingFactor, ShowErrorsInMessageBox());
}			  				  

/////////////////////////////////////////////////////////////////////////

INLINE void Dib::Png2Dib(LPCTSTR filename)
{
	lpDib = ::Png2Dib(filename);
}

/////////////////////////////////////////////////////////////////////////

INLINE const HPALETTE Dib::GetPal() const	  	
{ 
	return newPal; 
} 	 		

////////////////////////////////////////////////////////////////////

INLINE HPALETTE Dib::GetPalCopy()		
{ 
	return Palette(); 
}	 		

////////////////////////////////////////////////////////////////////

INLINE LPCTSTR Dib::Name()
{ 	   
	return fileName; 
}  

///////////////////////////////////////////////////////

INLINE void Dib::Free()
{
	if(!lpDib)
		return;

//#if defined(WIN32) || defined(_WIN32)
     //delete[] lpDib;
//#else     
     GlobalFreePtr(lpDib) ;  // unicode
//#endif

	lpDib = NULL;
}

///////////////////////////////////////////////////////

INLINE DWORD Dib::HeaderSize() const
{
	return ((BITMAPINFOHEADER huge *) lpDib)->biSize;
}	

///////////////////////////////////////////////////////

INLINE BOOL Dib::OnPaletteChanged(HDC hdc)
{
	// return TRUE if re-drawing is needed
	return OnPalette(hdc, TRUE);	// bkgrnd realization...
}

///////////////////////////////////////////////////////

INLINE BOOL Dib::OnQueryNewPalette(HDC hdc)
{
	// return TRUE if re-drawing is needed
	return OnPalette(hdc, FALSE);	// foreground realization...
}

////////////////////////////////////////////////////////////////////



