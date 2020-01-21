#include "JpegWriter.h"
extern "C"
{
#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jversion.h"		/* for version message */
#include "jpegext.h"
}

const int JpegWriter::defaultQuality = 80;
/*
 * Take the in-memory Dib and write it out as a JPEG file.
 * @param dib
 * @param outfilename
 * throws stuff!
 */

JpegWriter::JpegWriter(unsigned char * pdib, unsigned char * pdibbits, const char *name)
{ 	 	 
	ctor(pdib, pdibbits, name, defaultQuality);
}

/////////////////////////////////////////////////////////////////////

JpegWriter::JpegWriter(unsigned char * pdib, unsigned char * pdibbits, const char *name, int q)
{ 	 	 
	ctor(pdib, pdibbits, name, q);
}

/////////////////////////////////////////////////////////////////////

void JpegWriter::ctor(unsigned char * pdib, unsigned char * pdibbits, const char *name, int q)
{
	setQuality(q);
	outFileName = name;
	lpDib		= pdib;
	lpDibBits	= pdibbits;

	if(outFileName == NULL) 
		ReportAndThrowError("NULL output filename");

	if(lpDib == NULL)
		ReportAndThrowError("NULL pointer to lpDib");

	if(lpDibBits == NULL)
		ReportAndThrowError("NULL pointer to lpDibBits");

	if(lpDibBits <= lpDib)
		ReportAndThrowError("Bad pointer to lpDibBits");
}

/////////////////////////////////////////////////////////////////////

void JpegWriter::write()
{
	struct jpeg_error_mgr		jerr;
	cjpeg_source_ptr			src_mgr;
	struct jpeg_compress_struct	cinfo;
	
	jerr.showErrorsInMessageBox = 1;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	cinfo.in_color_space = JCS_RGB; /* arbitrary guess */


	jpeg_set_defaults(&cinfo);
	cinfo.client_data = lpDib; 
	src_mgr = jinit_read_dib(&cinfo);
	src_mgr->input_file = (FILE *)lpDibBits;	// HACK-O-RAMA!!!!!
	FILE*	output_file;

	if((output_file = fopen(outFileName, WRITE_BINARY)) == NULL) 
	{
		ReportAndThrowError("Can't open output file for writing.");
	}

	/* Read the header to obtain file size & colorspace. */
	(*src_mgr->start_input) (&cinfo, src_mgr);

	/* Now that we know input colorspace, fix colorspace-dependent defaults */
	jpeg_default_colorspace(&cinfo);

	/* set the quality */
	jpeg_set_quality(&cinfo, quality, FALSE);

	/* Specify data destination for compression */
	jpeg_stdio_dest(&cinfo, output_file);

	/* Start compressor */
	jpeg_start_compress(&cinfo, TRUE);

	/* Process data */
	JDIMENSION num_scanlines;

	while (cinfo.next_scanline < cinfo.image_height) 
	{
		num_scanlines = (*src_mgr->get_pixel_rows) (&cinfo, src_mgr);
	(void) jpeg_write_scanlines(&cinfo, src_mgr->buffer, num_scanlines);
	}

	/* Finish compression and release memory */
	(*src_mgr->finish_input) (&cinfo, src_mgr);
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	/* Close files, if we opened them */
	fclose(output_file);

}
/////////////////////////////////////////////////////////////////////

void JpegWriter::setQuality(int q)
{
	if(q > 1 && q < 100)
		quality = q;
}

/////////////////////////////////////////////////////////////////////

void JpegWriter::cleanup()
{

}

//////////////////////////////////////////////////////////////////////////////////////



void JpegWriter::ReportAndThrowError(const char* msg)
{
	Win32ErrorMessage(msg);
	Win32TraceMessage(msg);
	Win32Throw();
}
