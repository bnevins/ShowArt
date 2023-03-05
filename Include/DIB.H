#ifndef __DIB_H__
#define __DIB_H__

#ifndef WIN32
#error 32-bit only!!
#endif

#ifndef _INC_IOSTREAM
#include <iostream>
#endif

#ifdef WIN32
#define huge
#define BNEXPORT  __declspec( dllexport )
#else
#define BNEXPORT _export
#endif

enum ArtType
{
	jpeg, tiff, bmp, gif, pngtype, unknown
};

class	BNEXPORT CArt
{

public:								  		 			   			  							  
	virtual 		~CArt();
	SIZE			GetFitToScreenSize();
	int				OK();   				
	operator 		int();				
	WORD 			Width() const;		
	WORD			Height() const;		
	HWND&			Owner();		// use for read and write!!
	void* operator	new( size_t nSize );
	void operator	delete( void* p );
	// for filling old posn. edges with background brush...					
	void			Fill(HDC hdc, POINT oldp, POINT newp);
	
	virtual void 	Show(HDC hdc,	int x = 0, int y = 0, int w = 0, int h = 0, DWORD rop = SRCCOPY);
	virtual void 	Show(			int x = 0, int y = 0, int w = 0, int h = 0, DWORD rop = SRCCOPY);
	static	int&	ShowErrorsInMessageBox();

protected:						 			 
				CArt();		
	void		OK(int newOK);
	void		Width(int newW);
	void		Height(int newH);
	void		setFilename(LPCTSTR fname);
	LPCTSTR		getFilename();
		  						
	// KEEP THIS ONE PROTECTED!!!!!!!!
	virtual void Show_(HDC	hdc, int x, int y, int w, int h, DWORD rop) = 0;
private:	
	static int	showErrorsInMessageBox;
	WORD		width;
	WORD		height;
	int			ok;
	HWND		hwndOwner;		// window that "owns" the CArt, if any - for getting HDC's...  
	LPCTSTR		filename;
};								  		   		 		  

 class	BNEXPORT DDB : public CArt
{
public:											   		
					DDB();
					DDB(HINSTANCE hInstance, UINT resID);
					DDB(HBITMAP aBmp);
	void			Wipe(HDC hdc, int x, int y, int timing);
	virtual 		~DDB();	 	   				  		 			   		   	  
	HBITMAP			m_hbmp;
	LPCTSTR 		Write(LPCTSTR fileName);
	LPCTSTR 		Write(HANDLE hFile);
protected:	
	virtual void 	Show_(HDC	hdc, int x, int y, int w, int h, DWORD rop);  
	void			init();
private:
	void			CreateBMPFile(HANDLE hFile, PBITMAPINFO pbi);
	PBITMAPINFO		CreateBitmapInfoStruct();
};
			 
class	BNEXPORT Dib : public CArt
{
public:	
	static void		setScale(double num);
	static double   getScale();
	bool rotate(int numQuarterTurnsCW);
					Dib();
					Dib(LPCTSTR filename);			   				  	
					Dib(HGLOBAL hDib); // CF_DIB			   				  	
					Dib(HINSTANCE hInstance, int res_num);
	virtual 		~Dib();
	//void 			ShowWithPal(HDC	hdc = NULL);	 

	virtual void 	ShowWithPal(HDC hdc, int x = 0, int y = 0, int w = 0, int h = 0,
			 DWORD rop = SRCCOPY);
	virtual void 	ShowWithPal(int x = 0, int y = 0, int w = 0, int h = 0,
			 DWORD rop = SRCCOPY);
								 
	BOOL			OnPaletteChanged(HDC hdc);
	BOOL 			OnQueryNewPalette(HDC hdc);
	UINT 			Realize(HDC hdc, int background = FALSE);							   			  				 
	void			UnRealize(HDC hdc);							   			  
	LPCTSTR		 	Name();
	HBITMAP 		GetDDB(int w, int h);
	HBITMAP 		GetDDB(int w, int h, LPCTSTR label);
	HBITMAP 		GetDDB(HDC hdc, int w, int h, HPALETTE hpal = 0);
	HBITMAP 		GetDDB(HDC hdc, int w, int h, HPALETTE hpal, LPCTSTR label);
	HBITMAP 		GetDDB(HDC hdc, HPALETTE hpal = 0);
	const HPALETTE	GetPal()	const;
	HPALETTE 		GetPalCopy();
	const HGLOBAL 	GetHandle();
	HGLOBAL 		GetHandleCopy();
	LPCTSTR			Write(LPCTSTR szFileName, int w, int h, LPCTSTR label = NULL);
	BOOL			Write(LPCTSTR fileName, int jpegQuality);
	DWORD			ImageSize();
	void			Read(LPCTSTR szFileName);
	void			ClearAll();
	void			AutoSavePng(CAtlString& newname, int quality);
	LPCTSTR			GetFileName();
	ArtType			GetType();
	WORD			NumColorBits();
#ifdef _DEBUG
	void			DumpHeader(ostream* outStream = NULL);
#endif
protected:					 
	virtual void 	Show_(HDC	hdc, int x, int y, int w, int h, DWORD rop);
	virtual void 	ShowWithPal_(HDC	hdc, int x, int y, int w, int h, DWORD rop);
private:
	void			WriteBMP(LPCTSTR outfilename);
	void			WriteJpg(LPCTSTR outfilename, int jpegQuality);
	void			WriteJpgFromOther(LPCTSTR outfilename, int jpegQuality);
	void			ReLoad();
	static double	scalingFactor;
	TCHAR			*fileName;  // unicode
	HGLOBAL			hRes;		// handle to dib in memory from LoadResource...
    BYTE huge *		lpDibBits ; // unicode
	HPALETTE		oldPal, newPal;
	BYTE huge*		lpDib;
	void			ctorBefore();	// code common to ALL constructors!!
	void			ctorAfter(); 	// code common to ALL constructors!!
	void			setDibBitsAddr();
	DWORD			HeaderSize() const;
	HPALETTE 		Palette();	// allocates memory!!!
	WORD 			PaletteSize();
	void 			setWidthAndHeight(); 
	//void 			setPal(HPALETTE aPal);
	BOOL 			OnPalette(HDC hdc, int background);
	WORD			NumColors();
	void			CopyHandle(HGLOBAL hDib);
	void			Free();		// free memory...
	void			Bmp2Dib(LPCTSTR filename);
	void			Gif2Dib(LPCTSTR filename);
	void			Tiff2Dib(LPCTSTR filename);
	void			Jpg2Dib(LPCTSTR filename);
	void			Png2Dib(LPCTSTR filename);
};

#if 0
class BNEXPORT DibXY : public Dib
{ 
public:
	DibXY(int x = 0, int y = 0) : Dib(), xPos(x), yPos(y){}
	DibXY(LPCTSTR filename,int x = 0, int y = 0) : Dib(filename), xPos(x), yPos(y){}
	DibXY(HGLOBAL hDib,int x = 0, int y = 0) : Dib(hDib), xPos(x), yPos(y){}
	DibXY(HINSTANCE hInstance, int res_num,int x = 0, int y = 0) : Dib(hInstance, res_num), xPos(x), yPos(y){}
	virtual 		~DibXY()	{}																  			
	//operator CRect()	   		{ return CRect(xPos, yPos, xPos + Width(), yPos + Height()); }
	int&	X()					{ return xPos; }	// a REFERENCE!!!
	int&	Y()					{ return yPos; }	// a REFERENCE!!!
protected:						 
	virtual void 	Show_(HDC	hdc, int x, int y, int w, int h, DWORD rop)
					{ Dib::Show_(hdc, xPos, yPos, w, h, rop); }
private:															   
	int 	xPos;
	int 	yPos;	
};	
#endif

////////////////////////////////////////////////////////////////////
						 	  			   			 
class	BNEXPORT QuickDib : public CArt
{				   		  
	DDB		*ddb;
public:	
	QuickDib(HDC hDC, TCHAR* fname, HPALETTE = 0);  // unicode
	QuickDib(HDC hDC, TCHAR* filename, int w, int h, HPALETTE hp);  // unicode
	QuickDib(HDC hDC, HINSTANCE hInstance, int res_num, HPALETTE hp);
	virtual ~QuickDib();
	virtual void 	Show_(HDC hdc, int x, int y, int w, int h, DWORD rop);  
	//operator HBITMAP()	{ return ddb->m_hbmp; }
};								  	  

#endif
