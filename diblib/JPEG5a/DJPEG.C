// Windows stuff...
// if you include windows.h, big problem because of the clashing typedefs
// for "boolean" -- probably would require search/replace of *all*
// 
   
typedef unsigned char BYTE;


//#define HAVE_BOOLEAN
#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jversion.h"		/* for version message */
#include "wrbmp.h"
#include "winmem.h"
#include "djpeg.h"

//extern void RaiseException(DWORD,DWORD,DWORD, CONST DWORD *lpArguments); 
		  
#define BMP_SUPPORTED
#undef GIF_SUPPORTED
#undef PPM_SUPPORTED
#undef RLE_SUPPORTED
#undef TARGA_SUPPORTED
 
#include <ctype.h>		/* to declare isupper(),tolower(),isprint() */
#ifdef USE_SETMODE
#include <fcntl.h>		/* to declare setmode()'s parameter macros */
/* If you have setmode() but not <io.h>, just delete this line: */
#include <io.h>			/* to declare setmode() */
#endif


#ifdef DONT_USE_B_MODE		/* define mode parameters for fopen() */
#define READ_BINARY	"r"
#define WRITE_BINARY	"w"
#else
#define READ_BINARY	"rb"
#define WRITE_BINARY	"wb"
#endif

#ifndef EXIT_FAILURE		/* define exit() codes if not provided */
#define EXIT_FAILURE  1
#endif
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS  0
#endif
#ifndef EXIT_WARNING
#define EXIT_WARNING  2
#endif


/* Create the add-on message string table. */

void usage()		{ exit(1); }

LOCAL int
parse_switches (j_decompress_ptr cinfo, int argc, char **argv,
		int last_file_arg_seen, boolean for_real);

#define JMESSAGE(code,string)	string ,

static const char * const cdjpeg_message_table[] = {
#include "cderror.h"
  NULL
};


/*
 * This list defines the known output image formats
 * (not all of which need be supported by a given version).
 * You can change the default output format by defining DEFAULT_FMT;
 * indeed, you had better do so if you undefine PPM_SUPPORTED.
 */

typedef enum {
	FMT_BMP,		/* BMP format (Windows flavor) */
	FMT_GIF,		/* GIF format */
	FMT_OS2,		/* BMP format (OS/2 flavor) */
	FMT_PPM,		/* PPM/PGM (PBMPLUS formats) */
	FMT_RLE,		/* RLE format */
	FMT_TARGA,		/* Targa format */
	FMT_TIFF		/* TIFF format */
} IMAGE_FORMATS;

#ifndef DEFAULT_FMT		/* so can override from CFLAGS in Makefile */
#define DEFAULT_FMT	FMT_BMP
#endif					

static IMAGE_FORMATS 	requested_fmt;
static const char 		*progname;			/* program name for error messages */
static char 			*outfilename;		/* for -outfile switch */
static BYTE huge 		*lpPixels = NULL;

////////////////////////////////////////////

METHODDEF void
progress_monitor (j_common_ptr cinfo)
{
#if 0
  cd_progress_ptr prog = (cd_progress_ptr) cinfo->progress;
  int total_passes = prog->pub.total_passes + prog->total_extra_passes;
  int percent_done = (int) (prog->pub.pass_counter*100L/prog->pub.pass_limit);
  char buf[256];
  
  if (percent_done != prog->percent_done) {
    prog->percent_done = percent_done;
    if (total_passes > 1) {
      sprintf(buf, "\rPass %d/%d: %3d%% ",
	      prog->pub.completed_passes + prog->completed_extra_passes + 1,
	      total_passes, percent_done);
    } else {
      sprintf(buf, "\r %3d%% ", percent_done);
    }
    //OutputDebugString(buf);
  }
#endif
	  cd_progress_ptr	prog = (cd_progress_ptr) cinfo->progress;
	  int				total_passes = prog->pub.total_passes + prog->total_extra_passes;
	  int				percent_done = (int) (prog->pub.pass_counter*100L/prog->pub.pass_limit);
	  int				passNum = 1;
  
	  if (percent_done == prog->percent_done)
		  return;

	  if(total_passes > 1)
		  passNum = prog->pub.completed_passes + prog->completed_extra_passes + 1;

	  win_progress_monitor(passNum, total_passes, percent_done);
}

////////////////////////////////////////////////////////////////////////////

BYTE huge* setup_bmp(j_decompress_ptr cinfo, boolean hasPalette)
{
	int 			numColorsBits = hasPalette ? 8 : 24;
	int 			numColorsBytes = numColorsBits / 8;
	BYTE huge * 	lpDib = NULL;
	unsigned long 	dwLen;
	//unsigned long 	rowWidth = cinfo->image_width * numColorsBytes;	// padded to 4-byte boundary for every row...
	unsigned long 	rowWidth = cinfo->output_width * numColorsBytes;	// padded to 4-byte boundary for every row...

	while(rowWidth & 3)
		rowWidth++;

	dwLen = 40 /* sizeof(BITMAPINFOHEADER) */ + 
			(hasPalette ? 256 * 4 /* sizeof(RGBQUAD) */  : 0) + 
			cinfo->output_height * rowWidth;
						 
	//lpDib = (BYTE huge * )GlobalAllocPtr(GMEM_MOVEABLE, dwLen);
	lpDib = AllocWinMem(dwLen);
	
	return lpDib;	
}

///////////////////////////////////////////////////////////////////////



static struct jpeg_progress_mgr progMgr;

BYTE huge *
Jpg2Dib(const char* infilename, const char *outfilename, int useProgressMonitor)
{ 	 	 
  struct 			jpeg_decompress_struct cinfo;
  struct 			jpeg_error_mgr jerr;
  djpeg_dest_ptr 	dest_mgr = NULL;
  FILE * 			input_file;
  FILE * 			output_file;
  JDIMENSION 		num_scanlines;
  BYTE huge 		*lpDib = NULL;
  
   // if outfilename is NULL -- "write" to memory...(lpDib)

  /* Initialize the JPEG decompression object with default error handling. */

  // 3/12/96
if(useProgressMonitor)
	CreateProgressWindow(infilename);

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  progMgr.progress_monitor = progress_monitor;
  cinfo.progress = &progMgr;
  
	if ((input_file = fopen(infilename, READ_BINARY)) == NULL) 
	{
		return NULL;
		//RaiseException(0XE0000001,0,0,0);
		//exit(EXIT_FAILURE);
	}

	output_file = NULL;
	
	if(outfilename)		// WBN
	{
		if ((output_file = fopen(outfilename, WRITE_BINARY)) == NULL)
		{
			fprintf(stderr, "%s: can't open %s\n", progname, outfilename);	 
			exit(EXIT_FAILURE);
		}
	}
  /* Specify data source for decompression */
  jpeg_stdio_src(&cinfo, input_file);

  /* Read file header, set default decompression parameters */
  (void) jpeg_read_header(&cinfo, TRUE);

  if(GetNumDisplayColors() <= 256)
	  cinfo.quantize_colors = TRUE;
  
  cinfo.do_fancy_upsampling = 0;
							  
  dest_mgr = jinit_write_bmp(&cinfo, FALSE);
  
	if(output_file)
	{
		SetNoFile(FALSE, NULL);	// yukkkk!
		dest_mgr->output_file = output_file;
	}	
	else			// WBN
	{
		
		if(cinfo.out_color_space != JCS_RGB || cinfo.quantize_colors)
			lpDib = setup_bmp(&cinfo, TRUE);
		else	
			lpDib = setup_bmp(&cinfo, FALSE);

		SetNoFile(TRUE, lpDib);	// yukkkk!
		 dest_mgr->output_file = (FILE *)lpDib;	
	}
  
  jpeg_start_decompress(&cinfo);
  
  /* Write output file header */
  (*dest_mgr->start_output) (&cinfo, dest_mgr);

    
  /* Process data */
  while (cinfo.output_scanline < cinfo.output_height) {
	num_scanlines = jpeg_read_scanlines(&cinfo, dest_mgr->buffer,
										dest_mgr->buffer_height);
    (*dest_mgr->put_pixel_rows) (&cinfo, dest_mgr, num_scanlines);
  }


  /* Finish decompression and release memory.
   * I must do it in this order because output module has allocated memory
   * of lifespan JPOOL_IMAGE; it needs to finish before releasing memory.
   */

  (*dest_mgr->finish_output) (&cinfo, dest_mgr);
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  /* Close files, if we opened them */
    fclose(input_file);
    
    if(output_file)			// WBN
    	fclose(output_file);
    	
    
  DestroyProgressWindow();
    return lpDib;
	
//  if(jerr.num_warnings)
  //{
  	//free(lpDib);
  	//return NULL;
  //}
} 		 				  	



