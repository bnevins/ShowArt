/****************************************************************************
 * 																							 *
 *  FILE : image2.H															  *
 * 																							 *
 *  DESCRIPTION : Header/include file for image2 example.								*
 * 																							 *
 ****************************************************************************/

#ifdef _MT
#define _FAR_ _far
#else
#define _FAR_
#endif

#if !defined(_VA_LIST_DEFINED) && !defined(__VARARGS_H) && !defined(__STDARG_H) && !defined(_VARARGS_H_INCLUDED) && !defined(_STDARG_H_INCLUDED)
typedef char _FAR_ *va_list;
#define _VA_LIST_DEFINED
#define __STDARG_H
#define __VARARGS_H
#endif

/* Macro to restrict a given value to an upper or lower boundary value */
#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/* Macro to swap two values */
#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))

/* Macro to find the minimum of two values */
#define MIN(x,y) (((x) <= (y)) : x ? y)

/* Macros to display/remove hourglass cursor for lengthy operations */
#define StartWait() hcurSave = SetCursor(hCursor=LoadCursor(NULL,IDC_WAIT));\
																		  bAbortRequested=FALSE
#define EndWait()   SetCursor(hCursor=hcurSave)

/* WinFlags, __WinFlags is a ABS external, thus the need for the & */
/* #define WinFlags		  ((WORD)(&_WinFlags)) */
/* The above trick only works in MSC. Removed for compatibility and
		  replaced in the body with GetWinFlags() --mh */

#define MINBAND 50		 /* Minimum band size used by the program */
#define BANDINCREMENT	20 		/* Decrement for band size while trying		  */
													  /* to determine optimum band size. */

/* Bit values for the DIB attributes flag (fFileOptions). Also used as control
 * IDs for the radiobuttons for DIB bitcount in the File/Open dialog
 */
#define F_1BPP   DLGOPEN_1BPP
#define F_4BPP   DLGOPEN_4BPP
#define F_8BPP   DLGOPEN_8BPP
#define F_24BPP	DLGOPEN_24BPP

/* Bit values for the DIB attributes flag (fFileOptions), also used as control
 * IDs for the radiobuttons for DIB compression type in the File/Open dialog
 */
#define F_RLE4   DLGOPEN_RLE4
#define F_RLE8   DLGOPEN_RLE8
#define F_RGB				 DLGOPEN_RGB

#ifndef SEEK_CUR
/* flags for _lseek */
#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0
#endif

/* struct to be passed in for the SETDIBSCALING printer escape */
typedef struct {
		  short ScaleMode;
		  short dx, dy;
} DIBPARAMS;

/* Menu IDs */


#define IDM_OPEN			1002
#define IDM_EXIT			1003
#define IDM_ABOUT 1004
#define IDM_READ_FILE 1005

#define IDM_PRINT   1010
#define IDM_COPY			1012
#define IDM_MEMORYDIB	1014
#define IDM_SAVE			1015
#define IDM_CUT 1016
#define IDM_UNDO 1017
#define IDM_DEL 1018
#define IDM_RICHTEXT 1019

#define IDM_PASTEDIB 	1020
#define IDM_PASTEDDB 	1021
#define IDM_PASTEPAL 	1022
#define IDM_PASTECLIP	1023

#define IDM_GRAYSCALE 1030
#define IDM_COLOR 		 1031
#define IDM_BLUR						1032
#define IDM_SHARPEN		 1033
#define IDM_CLEANUP		 1034

#define IDM_HELP_INDEX			  1090
#define IDM_HELP_KEYBOARD		  1091
#define IDM_HELP_HELP			  1092

#define IDI_APP					  2000

#define ABOUTBOX								 100
#define RAWDLG 					  110
#define GRAYDLG					  120
#define ADJCOLOR								 130
#define toparse					  140

extern HWND hWndApp; 						  /* The handle to the app. window			 */

#define MAXREAD  32768									 /* Number of bytes to be read during */
																 /* each read operation.								 */

/* Header signatutes for various resources */
#define BFT_ICON			 0x4349	/* 'IC' */
#define BFT_BITMAP 0x4d42			/* 'BM' */
#define BFT_CURSOR 0x5450			/* 'PT' */

/* macro to determine if resource is a DIB */
#define ISDIB(bft) ((bft) == BFT_BITMAP)

/* Macro to align given value to the closest DWORD (unsigned long ) */
#define ALIGNULONG(i)	((i+3)/4*4)

/* Macro to determine to round off the given value to the closest byte */
#define WIDTHBYTES(i)	((i+31)/32*4)

#define PALVERSION				  0x300
#define MAXPALETTE		256		 /* max. # supported palette entries */

/********** THE FOLLOWING ARE USED IN DLGOPEN.C  ************************/

/* IDs for controls in the DlgOpen dialog */
#define DLGOPEN_EDIT 	101
#define DLGOPEN_FILE_LISTBOX	  102
#define DLGOPEN_DIR_LISTBOX	  103
#define DLGOPEN_PATH 	104
#define DLGOPEN_TEXT 							105
#define DLGOPEN_FOLDOUT 						106
#define DLGOPEN_BIG										  107
#define DLGOPEN_SMALL							108

#define DLGOPEN_OPTION							0xF000
#define DLGOPEN_1BPP 	0x0001
#define DLGOPEN_4BPP 	0x0002
#define DLGOPEN_8BPP 	0x0004
#define DLGOPEN_24BPP			  0x0008
#define DLGOPEN_RLE4 	0x0010
#define DLGOPEN_RLE8 	0x0020
#define DLGOPEN_RGB				  0x0040

#define DLGOPEN_OPTION8 						0x0080

/*  flags:
 * 		The LOWORD is the standard FileOpen() flags (OF_*)
 * 		the HIWORD can be any of the following:
 */
#define OF_MUSTEXIST 0x00010000 /* file must exist if the user hits Ok	*/
#define OF_NOSHOWSPEC	0x00020000		 /* DO NOT Show search spec in the edit box*/
#define OF_SHOWSPEC		0x00000000		 /* Show the search spec in the edit box */
#define OF_SAVE 0x00040000 	  /* Ok button will say "Save"        */
#define OF_OPEN 0x00080000 	  /* Ok button will say "Open"        */
#define OF_NOOPTIONS 0x00100000 /* Disable the options fold out					  */

/* Attributes for DlgDirLst() */
#define ATTRFILELIST 0x0000							/* include files only				 */
#define ATTRDIRLIST		0xC010						/* directories and drives ONLY */
#define CBEXTMAX			6								/* Number of bytes in "\*.txt" */


#define IDF(id)	((id) & ~DLGOPEN_OPTION)  /* extracts flag from control ID */
#define FID(f)   ((f)  |  DLGOPEN_OPTION) 		/* extracts control ID from flag */

/***************** GLOBAL VARIABLES *************************/

extern char 				achFileName[128]; /* File pathname										  */
extern DWORD				dwOffset;			/* Current position if DIB file pointer		 */
extern RECT 				rcClip;			  /* Current clip rectangle.										 */
extern BOOL 				fPalColors;   /* TRUE if the current DIB's color table   */
													  /* contains palette indexes not rgb values */
extern BOOL 				bDIBToDevice; /* Use SetDIBitsToDevice() to BLT data.   */
extern BOOL 				bLegitDraw;   /* We have a valid bitmap to draw 					  */
extern WORD 				wTransparent; /* Mode of DC												  */
extern char 				szAppName[];  /* App. name 										*/
extern HPALETTE hpalCurrent;	/* Handle to current palette								*/
extern HANDLE				hdibCurrent;  /* Handle to current memory DIB						  */
extern HBITMAP  hbmCurrent;	/* Handle to current memory BITMAP								  */
extern HANDLE				hbiCurrent;   /* Handle to current bitmap info struct   */
extern DWORD				dwStyle; 		  /* Style bits of the App. window								 */
extern HANDLE	 hdibPrior; /* for UNDO */

extern int							  file_format; /* Identifies type of raster file */
#define FI_DIB 0
#define FI_GIF 1
#define FI_TIF 2
#define FI_TGA 3
#define FI_PCX 4
#define FI_VICAR 5
#define FI_RAW 6

extern BOOL bAbortRequested;
extern HCURSOR hCursor;
extern BOOL bCanUndo;

/***************** FUNCTION DECLARATIONS *******************/

DWORD PASCAL lread(int fh, VOID FAR *pv, DWORD ul);
DWORD PASCAL lwrite(int fh, VOID FAR *pv, DWORD ul);

/***********************************************************/
/* Declarations of functions used in image2.c and menucomm modules */
/***********************************************************/

long _export FAR PASCAL WndProc (HWND, unsigned, WORD, LONG) ;
BOOL							MenuCommand (HWND hWnd, WORD wParam);
int							InitDIB (HWND);
void							FreeDib (void);
int identify_file_format(void);
BOOL SpinTheMessageLoop(void);

/***********************************************************/
/* Declarations of functions used in dib.c module					 */
/***********************************************************/

HANDLE			  OpenDIB (LPSTR szFile);
BOOL				  WriteDIB (LPSTR szFile,HANDLE hdib);
WORD				  PaletteSize (VOID FAR * pv);
WORD				  DibNumColors (VOID FAR * pv);
HPALETTE 		 CreateDibPalette (HANDLE hdib);
HPALETTE 		 CreateBIPalette (LPBITMAPINFOHEADER lpbi);
HANDLE			  DibFromBitmap (HBITMAP hbm, DWORD biStyle, WORD biBits, HPALETTE hpal);
HANDLE			  BitmapFromDib (HANDLE hdib, HPALETTE hpal);
BOOL				  DibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, LONG rop);
BOOL				  StretchDibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, int dx1, int dy1, LONG rop);
BOOL				  DibInfo (HANDLE hdib,LPBITMAPINFOHEADER lpbi);
HANDLE			  ReadDibBitmapInfo (int fh);
BOOL				  DrawBitmap (HDC hdc, int x, int y, HBITMAP hbm, DWORD rop);

/***********************************************************/
/* Declarations of functions used in drawdib. c module			 */
/***********************************************************/

void							PrintDIB (HWND hWnd, HDC hDC, int x, int y, int dx, int dy);
void							AppPaint (HWND hWnd, HDC hDC, int x, int y);
int							ErrMsg (PSTR sz,...);
BOOL							fDialog (int id, HWND hwnd, FARPROC fpfn);
void							BandDIB (HWND hWnd, HDC hDC, int x, int y);
BOOL		 FAR PASCAL AppAbout (HWND, unsigned, WORD, LONG) ;
HANDLE						CopyHandle (HANDLE h);
void							SizeWindow (HWND hWnd);
void							GetRealClientRect (HWND hwnd, PRECT lprc);
void							SetScrollRanges (HWND hwnd);
void							DrawSelect (HDC hdc, BOOL fDraw);
void				  PASCAL NormalizeRect (RECT *prc);
void							TrackMouse (HWND hwnd, POINT pt);
HBITMAP			 CopyBitmap (HBITMAP hbm);
HPALETTE 		 CopyPalette (HPALETTE hpal);
HBITMAP			 CropBitmap (HBITMAP hbm, PRECT prc);
HANDLE						RenderFormat (int cf);
HANDLE						RealizeDibFormat (DWORD biStyle, WORD biBits);

/***********************************************************/
/* Declarations of functions used in the print.c module	 */
/***********************************************************/

BOOL	  PASCAL InitPrinting (HDC hDC, HWND hWnd, HANDLE hInst, LPSTR msg);
void	  PASCAL TermPrinting (HDC hDC);
HDC	  PASCAL GetPrinterDC (void);

/***********************************************************/
/* Declarations of functions used in the dlgopen.c module  */
/***********************************************************/

int FAR PASCAL DlgfnOpen();
int FAR PASCAL DlgOpenFile (
		  HWND				hwndParent,
		  char				*szTitle,
		  DWORD	 flags,
		  char				*szExtIn,
		  char				*szFileNameIn,
		  WORD				*pfOpt
);


/* Global Declarations for GIF.C module */

int	  get_byte(void);
void	  putcolor(int x,int y,int  color);
int	  put_line(int rownum,int leftpt,int rightpt,unsigned char *localvalues);
int	  out_line(unsigned char *localvalues,int numberofdots);
void	  spindac(int direction,int step);
int	  gifview(HWND hWnd);
short decoder(short linewidth);

/* Debugging Stuff */

void OkMsgBox (char *szCaption, char *szFormat, ...);


/* PCX, TGA, TIFF, VICAR, and RAW stuff */

int  pcxview(HWND hWnd);
int  vicarview(unsigned int  hWnd);
int  rawview(unsigned int		  hWnd);
int  tiffview(unsigned int  hWnd);
int  tgaview(unsigned int		  hWnd);

/* Processing stuff */
void GrayScale(HWND hWnd);
void Color			(HWND hWnd);
void Sharpen  (HWND hWnd);
void Blur			(HWND hWnd);
void Cleanup  (HWND hWnd);

/* editing stuff */
void ClearRectToBackground(LPRECT lprect);
void RenderCurrentStuff(HWND hWnd);
void DragTheClipping(HWND hWnd,HANDLE h,HBITMAP hbm,HPALETTE hpal,
													 BOOL fStretchClipping);
HPALETTE MergePalettes(HPALETTE hpalCurrent,HPALETTE hpal);
HANDLE MergeImages(HANDLE hdibCurrent,HANDLE h,HPALETTE hpal,
													 RECT rcClip,BOOL fStretchClipping);
long _export FAR PASCAL FloatPicWndProc(HWND, unsigned, WORD, LONG);

#define _WINDOWS 1

