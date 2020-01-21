/*
 * Jpeg2Dib.c
 *
 */


#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jversion.h"		/* for version message */

#include <ctype.h>		/* to declare isprint() */
#include "wrbmp.h"
#include "jpegext.h"

/* Create the add-on message string table. */

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
#define DEFAULT_FMT	FMT_PPM
#endif

static IMAGE_FORMATS requested_fmt;


/*
 * Argument-parsing code.
 * The switch parser is designed to be useful with DOS-style command line
 * syntax, ie, intermixed switches and file names, where only the switches
 * to the left of a given file name affect processing of that file.
 * The main program in this file doesn't actually use this capability...
 */


static const char * progname;	/* program name for error messages */
static char * outfilename;	/* for -outfile switch */



/*
 * Marker processor for COM and interesting APPn markers.
 * This replaces the library's built-in processor, which just skips the marker.
 * We want to print out the marker as text, to the extent possible.
 * Note this code relies on a non-suspending data source.
 */

LOCAL(unsigned int)
jpeg_getc (j_decompress_ptr cinfo)
/* Read next byte */
{
  struct jpeg_source_mgr * datasrc = cinfo->src;

  if (datasrc->bytes_in_buffer == 0) {
    if (! (*datasrc->fill_input_buffer) (cinfo))
      ERREXIT(cinfo, JERR_CANT_SUSPEND);
  }
  datasrc->bytes_in_buffer--;
  return GETJOCTET(*datasrc->next_input_byte++);
}


METHODDEF(boolean)
print_text_marker (j_decompress_ptr cinfo)
{
  boolean traceit = (cinfo->err->trace_level >= 1);
  INT32 length;
  unsigned int ch;
  unsigned int lastch = 0;

  length = jpeg_getc(cinfo) << 8;
  length += jpeg_getc(cinfo);
  length -= 2;			/* discount the length word itself */

  if (traceit) {
    if (cinfo->unread_marker == JPEG_COM)
      fprintf(stderr, "Comment, length %ld:\n", (long) length);
    else			/* assume it is an APPn otherwise */
      fprintf(stderr, "APP%d, length %ld:\n",
	      cinfo->unread_marker - JPEG_APP0, (long) length);
  }

  while (--length >= 0) {
    ch = jpeg_getc(cinfo);
    if (traceit) {
      /* Emit the character in a readable form.
       * Nonprintables are converted to \nnn form,
       * while \ is converted to \\.
       * Newlines in CR, CR/LF, or LF form will be printed as one newline.
       */
      if (ch == '\r') {
	fprintf(stderr, "\n");
      } else if (ch == '\n') {
	if (lastch != '\r')
	  fprintf(stderr, "\n");
      } else if (ch == '\\') {
	fprintf(stderr, "\\\\");
      } else if (isprint(ch)) {
	putc(ch, stderr);
      } else {
	fprintf(stderr, "\\%03o", ch);
      }
      lastch = ch;
    }
  }

  if (traceit)
    fprintf(stderr, "\n");

  return TRUE;
}

typedef unsigned char BYTE;


////////////////////////////////////////////////////////////////////////////

BYTE * setup_bmp(j_decompress_ptr cinfo, boolean hasPalette)
{
	int 			numColorsBits = hasPalette ? 8 : 24;
	int 			numColorsBytes = numColorsBits / 8;
	BYTE  * 	lpDib = NULL;
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



BYTE* Jpg2Dib(const char* infilename, const char *outfilename, double scalingFactor, int showErrorsInMessageBox)
{ 	 	 
  struct 			jpeg_decompress_struct cinfo;
  struct 			jpeg_error_mgr jerr;
  djpeg_dest_ptr 	dest_mgr = NULL;
  FILE * 			input_file;
  FILE * 			output_file;
  JDIMENSION 		num_scanlines;
  BYTE  			*lpDib = NULL;
  
   // if outfilename is NULL -- "write" to memory...(lpDib)

  /* Initialize the JPEG decompression object with default error handling. */

	jerr.showErrorsInMessageBox = showErrorsInMessageBox;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	/* Insert custom marker processor for COM and APP12.
	* APP12 is used by some digital camera makers for textual info,
	* so we provide the ability to display it as text.
	* If you like, additional APPn marker types can be selected for display,
	* but don't try to override APP0 or APP14 this way (see libjpeg.doc).
	*/
	jpeg_set_marker_processor(&cinfo, JPEG_COM, print_text_marker);
	jpeg_set_marker_processor(&cinfo, JPEG_APP0+12, print_text_marker);
  
	if ((input_file = fopen(infilename, READ_BINARY)) == NULL) 
	{
		return NULL;
	}

	output_file = NULL;
	
	if(outfilename)		// WBN
	{
		if ((output_file = fopen(outfilename, WRITE_BINARY)) == NULL)
		{
			char s[1000];
			sprintf(s, "Can't open %s\n", outfilename);	 
			Win32ErrorMessage(s);
			return NULL;
		}
	}
  /* Specify data source for decompression */
  jpeg_stdio_src(&cinfo, input_file);

  /* Read file header, set default decompression parameters */
  (void) jpeg_read_header(&cinfo, TRUE);

	#pragma message("REMOVE ME!!!")
  // Currently the jpeg library will only scale by 1/2, 1/4, 1/8
  // Dib library is all setup for arbitrary scaling.



	cinfo.scale_num = 1;
	// err on the side of making it smaller
	if(scalingFactor >= 0.99)
		cinfo.scale_denom = 1;
	else if(scalingFactor >= 0.5)
		cinfo.scale_denom = 2;
	else if(scalingFactor >= 0.25)
		cinfo.scale_denom = 4;
	else if(scalingFactor >= 0.125)
		cinfo.scale_denom = 8;
	else
		cinfo.scale_denom = 1;
	
  if(GetNumDisplayBitsPerPixel() <= 8)
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

    return lpDib;
} 		 				  	
