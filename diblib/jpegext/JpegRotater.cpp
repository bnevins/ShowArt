// testrot.cpp : Defines the entry point for the console application.
//

#include "JpegRotater.h"
#include "bnutils.h"
extern "C"
{
#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "transupp.h"		/* Support routines for jpegtran */
#include "jversion.h"		/* for version message */
}

/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////

/*
int main(int argc, char **argv)
{
	JpegRotater jr("e:\\docs\\art\\side.jpg", "e:\\docs\\art\\fixed2.jpg", -2);
	jr.rotate();
	return 0;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////

static JXFORM_CODE calcCode(int numQuarterTurns)
{
	// + --> CW, - --> CCW

	switch(numQuarterTurns)
	{
		case -3:	// fall through
		case  1:	return JXFORM_ROT_90;
		case -2:	// fall through
		case  2:	return JXFORM_ROT_180;
		case -1:	// fall through
		case  3:	return JXFORM_ROT_270;
		default:	return JXFORM_NONE;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

JpegRotater::JpegRotater(const char* inName, const char* outName, int numTurns)
{
	infileName		= inName;
	outfileName		= outName;
	numQuarterTurns	= numTurns;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool JpegRotater::rotate()
{
	jpeg_transform_info				transformoption; /* image transformation options */
	struct jpeg_decompress_struct	srcinfo;
	struct jpeg_compress_struct		dstinfo;
	struct jpeg_error_mgr			jsrcerr;
	struct jpeg_error_mgr			jdsterr;


	jvirt_barray_ptr * src_coef_arrays;
	jvirt_barray_ptr * dst_coef_arrays;
	FILE*				infile;
	FILE*				outfile;

	JXFORM_CODE rotCode = calcCode(numQuarterTurns);

	if(rotCode == JXFORM_NONE)
		return false; // FIXME -- error message??


	transformoption.transform		= rotCode;
	transformoption.trim			= FALSE;
	transformoption.force_grayscale = FALSE;

	/* Initialize the JPEG decompression object with default error handling. */
	srcinfo.err = jpeg_std_error(&jsrcerr);
	jpeg_create_decompress(&srcinfo);

	/* Initialize the JPEG compression object with default error handling. */
	dstinfo.err = jpeg_std_error(&jdsterr);
	jpeg_create_compress(&dstinfo);

	srcinfo.mem->max_memory_to_use = dstinfo.mem->max_memory_to_use;

	/* Open the input file. */
	if((infile = fopen(infileName, READ_BINARY)) == NULL) 
		return false;

	/* Open the output file. */
	if((outfile = fopen(outfileName, WRITE_BINARY)) == NULL)
	{
		fclose(infile);
		return false;
	}

	/* Specify data source for decompression */
	jpeg_stdio_src(&srcinfo, infile);

	/* Enable saving of extra markers that we want to copy */
	jcopy_markers_setup(&srcinfo, JCOPYOPT_DEFAULT);

	/* Read file header */
	(void) jpeg_read_header(&srcinfo, TRUE);

	/* Any space needed by a transform option must be requested before
	* jpeg_read_coefficients so that memory allocation will be done right.
	*/
	jtransform_request_workspace(&srcinfo, &transformoption);

	/* Read source file as DCT coefficients */
	src_coef_arrays = jpeg_read_coefficients(&srcinfo);

	/* Initialize destination compression parameters from source values */
	jpeg_copy_critical_parameters(&srcinfo, &dstinfo);

	/* Adjust destination parameters if required by transform options;
	* also find out which set of coefficient arrays will hold the output.
	*/
	dst_coef_arrays = jtransform_adjust_parameters(&srcinfo, &dstinfo,
						 src_coef_arrays,
						 &transformoption);

	/* Specify data destination for compression */
	jpeg_stdio_dest(&dstinfo, outfile);

	/* Start compressor (note no image data is actually written here) */
	jpeg_write_coefficients(&dstinfo, dst_coef_arrays);

	/* Copy to the output file any extra markers that we want to preserve */
	jcopy_markers_execute(&srcinfo, &dstinfo, JCOPYOPT_DEFAULT);

	/* Execute image transformation, if any */
	jtransform_execute_transformation(&srcinfo, &dstinfo,
					src_coef_arrays,
					&transformoption);

	/* Finish compression and release memory */
	jpeg_finish_compress(&dstinfo);
	jpeg_destroy_compress(&dstinfo);
	(void) jpeg_finish_decompress(&srcinfo);
	jpeg_destroy_decompress(&srcinfo);

	/* make the times match the old file */

	copyFileTimes(_fileno(infile), _fileno(outfile));
	/* Close files, if we opened them */
	fclose(infile);
	fclose(outfile);

  /* All done. */
  return true;
}

