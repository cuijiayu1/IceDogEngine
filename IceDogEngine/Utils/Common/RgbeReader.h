#pragma once

#include <string>
#include <stdio.h>

/* flags indicating which fields in an rgbe_header_info are valid */
#define RGBE_VALID_PROGRAMTYPE 0x01
#define RGBE_VALID_GAMMA       0x02
#define RGBE_VALID_EXPOSURE    0x04

/* return codes for rgbe routines */
#define RGBE_RETURN_SUCCESS 0
#define RGBE_RETURN_FAILURE -1

namespace IceDogUtils
{
	struct rgbe_header_info 
	{
		int valid;            /* indicate which fields are valid */
		char programtype[16]; /* listed at beginning of file to identify it
							  * after "#?".  defaults to "RGBE" */
		float gamma;          /* image has already been gamma corrected with
							  * given gamma.  defaults to 1.0 (no correction) */
		float exposure;       /* a value of 1.0 in an image corresponds to
							  * <exposure> watts/steradian/m^2.
							  * defaults to 1.0 */
	};

	enum rgbe_error_codes {
		rgbe_read_error,
		rgbe_write_error,
		rgbe_format_error,
		rgbe_memory_error,
	};


	class RgbeImage
	{
	public:
		int c_width;
		int c_height;
		float* r_data;

		void Release();
	};

	class RgbeReader
	{
	public:
		static RgbeImage ReadRgbeFile(std::string url);
	private:
		/* read or write headers */
		/* you may set rgbe_header_info to null if you want to */
		static int RGBE_WriteHeader(FILE *fp, int width, int height, rgbe_header_info *info);
		static int RGBE_ReadHeader(FILE *fp, int *width, int *height, rgbe_header_info *info);

		/* read or write pixels */
		/* can read or write pixels in chunks of any size including single pixels*/
		static int RGBE_WritePixels(FILE *fp, float *data, int numpixels);
		static int RGBE_ReadPixels(FILE *fp, float *data, int numpixels);

		/* read or write run length encoded files */
		/* must be called to read or write whole scanline */
		static int RGBE_WritePixels_RLE(FILE *fp, float *data, int scanline_width,
			int num_scanlines);
		static int RGBE_ReadPixels_RLE(FILE *fp, float *data, int scanline_width,
			int num_scanlines);
		static int rgbe_error(int rgbe_error_code, char *msg);
		static void float2rgbe(unsigned char rgbe[4], float red, float green, float blue);
		static void rgbe2float(float *red, float *green, float *blue, unsigned char rgbe[4]);
		static int RGBE_WriteBytes_RLE(FILE *fp, unsigned char *data, int numbytes);
	};
}

