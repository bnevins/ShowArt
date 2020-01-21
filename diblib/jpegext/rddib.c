/*
 * WBN
 * modified rdbmp.c to use in-memory dibs
 * The other, easier, technique would have been to do a hideous
 * hack like WRITE out the (probably huge) dib to file first.
 *
 */
#define BMP_SUPPORTED
#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */



/* Create the BIZARRE, BYZANTINE add-on message string table!!! */

#define JMESSAGE(code,string)	string ,

static const char * const rddib_message_table[] = {
#include "cderror.h"
  NULL
};



/* Macros to deal with unsigned chars as efficiently as compiler allows */

typedef unsigned char U_CHAR;


#define UCH(x)	((int) (x) & 0xFF)

#define	ReadOK(file,buffer,len)	(JFREAD(file,buffer,len) == ((size_t) (len)))
LOCAL(int)
copy(U_CHAR from, U_CHAR to, int len)
{
	return 0;
}

/* Private version of data source object */

typedef struct _dib_source_struct * dib_source_ptr;

typedef struct _dib_source_struct {
  struct cjpeg_source_struct pub; /* public fields */

  j_compress_ptr cinfo;		/* back link saves passing separate parm */

  JSAMPARRAY colormap;		/* BMP colormap (converted to my format) */

  jvirt_sarray_ptr whole_image;	/* Needed to reverse row order */
  JDIMENSION source_row;	/* Current source row number */
  JDIMENSION row_width;		/* Physical width of scanlines in file */

  int bits_per_pixel;		/* remembers 8- or 24-bit format */
  U_CHAR*	lpDib;
  U_CHAR*	lpDibBits;
} dib_source_struct;


static U_CHAR* megaPointer = NULL;

LOCAL(int)
read_byte()
{
  return *megaPointer++;
}

LOCAL(void)
setReader(U_CHAR* px)
{
	megaPointer = px;
}


LOCAL(void)
read_colormap (dib_source_ptr sinfo, int cmaplen, int mapentrysize)
/* Read the colormap from a BMP file */
{
  int i;

  switch (mapentrysize) {
  case 3:
    /* BGR format (occurs in OS/2 files) */
    for (i = 0; i < cmaplen; i++) {
      sinfo->colormap[2][i] = (JSAMPLE) read_byte();
      sinfo->colormap[1][i] = (JSAMPLE) read_byte();
      sinfo->colormap[0][i] = (JSAMPLE) read_byte();
    }
    break;
  case 4:
    /* BGR0 format (occurs in MS Windows files) */
    for (i = 0; i < cmaplen; i++) {
      sinfo->colormap[2][i] = (JSAMPLE) read_byte();
      sinfo->colormap[1][i] = (JSAMPLE) read_byte();
      sinfo->colormap[0][i] = (JSAMPLE) read_byte();
      (void) read_byte();
    }
    break;
  default:
    ERREXIT(sinfo->cinfo, JERR_BMP_BADCMAP);
    break;
  }
}


/*
 * Read one row of pixels.
 * The image has been read into the whole_image array, but is otherwise
 * unprocessed.  We must read it out in top-to-bottom row order, and if
 * it is an 8-bit image, we must expand colormapped pixels to 24bit format.
 */

METHODDEF(JDIMENSION)
get_8bit_row (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
/* This version is for reading 8-bit colormap indexes */
{
  dib_source_ptr source = (dib_source_ptr) sinfo;
  register JSAMPARRAY colormap = source->colormap;
  JSAMPARRAY image_ptr;
  register int t;
  register JSAMPROW inptr, outptr;
  register JDIMENSION col;

  /* Fetch next row from virtual array */
  source->source_row--;
  image_ptr = (*cinfo->mem->access_virt_sarray)
    ((j_common_ptr) cinfo, source->whole_image,
     source->source_row, (JDIMENSION) 1, FALSE);

  /* Expand the colormap indexes to real data */
  inptr = image_ptr[0];
  outptr = source->pub.buffer[0];
  for (col = cinfo->image_width; col > 0; col--) {
    t = GETJSAMPLE(*inptr++);
    *outptr++ = colormap[0][t];	/* can omit GETJSAMPLE() safely */
    *outptr++ = colormap[1][t];
    *outptr++ = colormap[2][t];
  }

  return 1;
}


METHODDEF(JDIMENSION)
get_24bit_row (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
/* This version is for reading 24-bit pixels */
{
  dib_source_ptr source = (dib_source_ptr) sinfo;
  JSAMPARRAY image_ptr;
  register JSAMPROW inptr, outptr;
  register JDIMENSION col;

  /* Fetch next row from virtual array */
  source->source_row--;
  image_ptr = (*cinfo->mem->access_virt_sarray)
    ((j_common_ptr) cinfo, source->whole_image,
     source->source_row, (JDIMENSION) 1, FALSE);

  /* Transfer data.  Note source values are in BGR order
   * (even though Microsoft's own documents say the opposite).
   */
  inptr = image_ptr[0];
  outptr = source->pub.buffer[0];
  for (col = cinfo->image_width; col > 0; col--) {
    outptr[2] = *inptr++;	/* can omit GETJSAMPLE() safely */
    outptr[1] = *inptr++;
    outptr[0] = *inptr++;
    outptr += 3;
  }

  return 1;
}


/*
 * This method loads the image into whole_image during the first call on
 * get_pixel_rows.  The get_pixel_rows pointer is then adjusted to call
 * get_8bit_row or get_24bit_row on subsequent calls.
 */

METHODDEF(JDIMENSION)
preload_image (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
{
  dib_source_ptr source = (dib_source_ptr) sinfo;
  register FILE *infile = source->pub.input_file;
  register int c;
  register JSAMPROW out_ptr;
  JSAMPARRAY image_ptr;
  JDIMENSION row, col;
  cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;

  /* Read the data into a virtual array in input-file row order. */
  for (row = 0; row < cinfo->image_height; row++) {
    if (progress != NULL) {
      progress->pub.pass_counter = (long) row;
      progress->pub.pass_limit = (long) cinfo->image_height;
      (*progress->pub.progress_monitor) ((j_common_ptr) cinfo);
    }
    image_ptr = (*cinfo->mem->access_virt_sarray)
      ((j_common_ptr) cinfo, source->whole_image,
       row, (JDIMENSION) 1, TRUE);
    out_ptr = image_ptr[0];
    for (col = source->row_width; col > 0; col--) {
      /* inline copy of read_byte() for speed */
      c = read_byte();
      *out_ptr++ = (JSAMPLE) c;
    }
  }
  if (progress != NULL)
    progress->completed_extra_passes++;

  /* Set up to read from the virtual array in top-to-bottom order */
  switch (source->bits_per_pixel) {
  case 8:
    source->pub.get_pixel_rows = get_8bit_row;
    break;
  case 24:
    source->pub.get_pixel_rows = get_24bit_row;
    break;
  default:
    ERREXIT(cinfo, JERR_BMP_BADDEPTH);
  }
  source->source_row = cinfo->image_height;

  /* And read the first row */
  return (*source->pub.get_pixel_rows) (cinfo, sinfo);
}


/*
 * Read the file header; return image size and component count.
 */

METHODDEF(void)
start_input_dib (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
{
  dib_source_ptr source = (dib_source_ptr) sinfo;
  U_CHAR * lpDib;
  U_CHAR * lpDibBits;
  U_CHAR * bmpinfoheader;


#define GET_2B(array,offset)  ((unsigned int) UCH(array[offset]) + \
			       (((unsigned int) UCH(array[offset+1])) << 8))
#define GET_4B(array,offset)  ((INT32) UCH(array[offset]) + \
			       (((INT32) UCH(array[offset+1])) << 8) + \
			       (((INT32) UCH(array[offset+2])) << 16) + \
			       (((INT32) UCH(array[offset+3])) << 24))
  //INT32 bfOffBits;
  INT32 headerSize;
  INT32 biWidth = 0;		/* initialize to avoid compiler warning */
  INT32 biHeight = 0;
  unsigned int biPlanes;
  INT32 biCompression;
  INT32 biXPelsPerMeter,biYPelsPerMeter;
  INT32 biClrUsed = 0;
  int mapentrysize = 0;		/* 0 indicates no colormap */
  INT32 bPad;
  JDIMENSION row_width;


  source->lpDibBits	=  (U_CHAR *)(sinfo->input_file);
  lpDib				= source->lpDib;
  lpDibBits			= source->lpDibBits;
  bmpinfoheader		= lpDib;


  /* The infoheader might be 12 bytes (OS/2 1.x), 40 bytes (Windows),
   * or 64 bytes (OS/2 2.x).  Check the first 4 bytes to find out which.
   */
  headerSize = (INT32) GET_4B(bmpinfoheader,0);
  if (headerSize < 12 || headerSize > 64)
    ERREXIT(cinfo, JERR_BMP_BADHEADER);

  setReader(lpDib + headerSize);
  switch ((int) headerSize) {
  case 12:
    /* Decode OS/2 1.x header (Microsoft calls this a BITMAPCOREHEADER) */
    biWidth = (INT32) GET_2B(bmpinfoheader,4);
    biHeight = (INT32) GET_2B(bmpinfoheader,6);
    biPlanes = GET_2B(bmpinfoheader,8);
    source->bits_per_pixel = (int) GET_2B(bmpinfoheader,10);

    switch (source->bits_per_pixel) {
    case 8:			/* colormapped image */
      mapentrysize = 3;		/* OS/2 uses RGBTRIPLE colormap */
      TRACEMS2(cinfo, 1, JTRC_BMP_OS2_MAPPED, (int) biWidth, (int) biHeight);
      break;
    case 24:			/* RGB image */
      TRACEMS2(cinfo, 1, JTRC_BMP_OS2, (int) biWidth, (int) biHeight);
      break;
    default:
      ERREXIT(cinfo, JERR_BMP_BADDEPTH);
      break;
    }
    if (biPlanes != 1)
      ERREXIT(cinfo, JERR_BMP_BADPLANES);
    break;
  case 40:
  case 64:
    /* Decode Windows 3.x header (Microsoft calls this a BITMAPINFOHEADER) */
    /* or OS/2 2.x header, which has additional fields that we ignore */
    biWidth = GET_4B(bmpinfoheader,4);
    biHeight = GET_4B(bmpinfoheader,8);
    biPlanes = GET_2B(bmpinfoheader,12);
    source->bits_per_pixel = (int) GET_2B(bmpinfoheader,14);
    biCompression = GET_4B(bmpinfoheader,16);
    biXPelsPerMeter = GET_4B(bmpinfoheader,24);
    biYPelsPerMeter = GET_4B(bmpinfoheader,28);
    biClrUsed = GET_4B(bmpinfoheader,32);
    /* biSizeImage, biClrImportant fields are ignored */

    switch (source->bits_per_pixel) {
    case 8:			/* colormapped image */
      mapentrysize = 4;		/* Windows uses RGBQUAD colormap */
      TRACEMS2(cinfo, 1, JTRC_BMP_MAPPED, (int) biWidth, (int) biHeight);
      break;
    case 24:			/* RGB image */
      TRACEMS2(cinfo, 1, JTRC_BMP, (int) biWidth, (int) biHeight);
      break;
    default:
      ERREXIT(cinfo, JERR_BMP_BADDEPTH);
      break;
    }
    if (biPlanes != 1)
      ERREXIT(cinfo, JERR_BMP_BADPLANES);
    if (biCompression != 0)
      ERREXIT(cinfo, JERR_BMP_COMPRESSED);

    if (biXPelsPerMeter > 0 && biYPelsPerMeter > 0) {
      /* Set JFIF density parameters from the BMP data */
      cinfo->X_density = (UINT16) (biXPelsPerMeter/100); /* 100 cm per meter */
      cinfo->Y_density = (UINT16) (biYPelsPerMeter/100);
      cinfo->density_unit = 2;	/* dots/cm */
    }
    break;
  default:
    ERREXIT(cinfo, JERR_BMP_BADHEADER);
    break;
  }

  /* Compute distance to bitmap data --- will adjust for colormap below */
  bPad = (lpDibBits - lpDib) - headerSize;

  /* Read the colormap, if any */
  if (mapentrysize > 0) {
    if (biClrUsed <= 0)
      biClrUsed = 256;		/* assume it's 256 */
    else if (biClrUsed > 256)
      ERREXIT(cinfo, JERR_BMP_BADCMAP);
    /* Allocate space to store the colormap */
    source->colormap = (*cinfo->mem->alloc_sarray)
      ((j_common_ptr) cinfo, JPOOL_IMAGE,
       (JDIMENSION) biClrUsed, (JDIMENSION) 3);
    /* and read it from the file */
    read_colormap(source, (int) biClrUsed, mapentrysize);
    /* account for size of colormap */
    bPad -= biClrUsed * mapentrysize;
  }

  /* Skip any remaining pad bytes */
  if (bPad < 0)			/* incorrect bfOffBits value? */
    ERREXIT(cinfo, JERR_BMP_BADHEADER);
  while (--bPad >= 0) {
    (void) read_byte();
  }

  /* Compute row width in file, including padding to 4-byte boundary */
  if (source->bits_per_pixel == 24)
    row_width = (JDIMENSION) (biWidth * 3);
  else
    row_width = (JDIMENSION) biWidth;
  while ((row_width & 3) != 0) row_width++;
  source->row_width = row_width;

  /* Allocate space for inversion array, prepare for preload pass */
  source->whole_image = (*cinfo->mem->request_virt_sarray)
    ((j_common_ptr) cinfo, JPOOL_IMAGE, FALSE,
     row_width, (JDIMENSION) biHeight, (JDIMENSION) 1);
  source->pub.get_pixel_rows = preload_image;
  if (cinfo->progress != NULL) {
    cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;
    progress->total_extra_passes++; /* count file input as separate pass */
  }

  /* Allocate one-row buffer for returned data */
  source->pub.buffer = (*cinfo->mem->alloc_sarray)
    ((j_common_ptr) cinfo, JPOOL_IMAGE,
     (JDIMENSION) (biWidth * 3), (JDIMENSION) 1);
  source->pub.buffer_height = 1;

  cinfo->in_color_space = JCS_RGB;
  cinfo->input_components = 3;
  cinfo->data_precision = 8;
  cinfo->image_width = (JDIMENSION) biWidth;
  cinfo->image_height = (JDIMENSION) biHeight;
}


/*
 * Finish up at the end of the file.
 */

METHODDEF(void)
finish_input_dib (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
{
  /* no work */
}

/*
 * The module selection routine for BMP format input.
 */

GLOBAL(cjpeg_source_ptr)
jinit_read_dib (j_compress_ptr cinfo)
{
	// World's Most Convoluted Code!!!!

	dib_source_ptr source;
	struct jpeg_error_mgr* err = cinfo->err;

  /* Add some application-specific error messages (from cderror.h) 
   * Note: AAAAIIIYYYYYAAAAARRRRGGGGHHHHHH!
	*/
  err->addon_message_table = rddib_message_table;
  err->first_addon_message = JMSG_FIRSTADDONCODE;
  err->last_addon_message = JMSG_LASTADDONCODE;

  // NOTE:  "client_data" is actually pointing at "lpDib"

  /* Create module interface object */
  source = (dib_source_ptr)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  SIZEOF(dib_source_struct));
  source->cinfo = cinfo;	/* make back link for subroutines */
  source->lpDib = (U_CHAR *)cinfo->client_data;

  /* Fill in method ptrs, except get_pixel_rows which start_input sets */
  source->pub.start_input = start_input_dib;
  source->pub.finish_input = finish_input_dib;

  // KLUDGE ALERT!  Caller will put lpDibBits into cjpeg_source_ptr->input_file
  return (cjpeg_source_ptr) source;
}

