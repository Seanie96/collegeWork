#include <pthread.h>
#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <time.h>
#include "math.h"


unsigned char *raw_image = NULL;

int width;
int height;
int bytes_per_pixel = 3;
int color_space = JCS_RGB;
int num_threads;
int pixels_per_thread;

void * computeGreyscale(void * threadid);

  /*
    Decompression
  */



int read_jpeg( char *filename ) {
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];

  FILE *infile = fopen( filename, "rb" );
  unsigned long location = 0;
  int i = 0;

  if ( !infile )
  {
  printf("Error opening jpeg file %s\n!", filename );
  return -1;
  }
  cinfo.err = jpeg_std_error( &jerr );
  jpeg_create_decompress( &cinfo );
  jpeg_stdio_src( &cinfo, infile );
  jpeg_read_header( &cinfo, TRUE );

  jpeg_start_decompress( &cinfo );

  width = cinfo.image_width;
  height = cinfo.image_height;

  raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
  row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );

  while( cinfo.output_scanline < cinfo.image_height ) {
    jpeg_read_scanlines( &cinfo, row_pointer, 1 );

    for( i=0; i<cinfo.image_width*cinfo.num_components;i++) {
      raw_image[location++] = row_pointer[0][i];
    }
  }

  /*
  for(int i = 0; i < (3 * (cinfo.image_width) * (cinfo.image_height)) - 2; i++) {
    printf("[%d,%d,%d] ", raw_image[i], raw_image[i + 1], raw_image[i + 2]);
  }
  */

  printf("\nNumber Of RGB pixels:%lu\n", location);

  jpeg_finish_decompress( &cinfo );
  jpeg_destroy_decompress( &cinfo );
  free( row_pointer[0] );
  fclose( infile );
  return 1;
}

  /*
    Compression
  */

int write_jpeg( char *filename ) {
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  JSAMPROW row_pointer[1];
  FILE *outfile = fopen( filename, "wb" );

  if ( !outfile ) {
    printf("Error opening output jpeg file %s\n!", filename );
    return -1;
  }
  cinfo.err = jpeg_std_error( &jerr );
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = bytes_per_pixel;
  cinfo.in_color_space = color_space;
  jpeg_set_defaults( &cinfo );
  cinfo.num_components = 3;
  cinfo.dct_method = JDCT_FLOAT;
  jpeg_set_quality(&cinfo, 30, TRUE);
  jpeg_start_compress( &cinfo, TRUE );

	/*
	    PThread stuff....
	*/

  double result = (cinfo.image_height * cinfo.image_width * bytes_per_pixel) / num_threads;
  pixels_per_thread = (int)floor(result);

  int id[num_threads];
  pthread_t threads[num_threads - 1];
  int rc,t;

  for (t = 0; t < num_threads; t++) {
      id[t] = t;
      rc = pthread_create(&threads[t], NULL, computeGreyscale, &id[t]);
      if (rc) {
          printf("ERROR return code from pthread_create(): %d\n", rc);
          exit(-1);
      }
  }

  // wait for threads to exit
  for(t = 0; t < num_threads; t++) {
    pthread_join( threads[t], NULL);
  }


  while( cinfo.next_scanline < cinfo.image_height ) {
    row_pointer[0] = &raw_image[ cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
    jpeg_write_scanlines( &cinfo, row_pointer, 1 );
  }

  jpeg_finish_compress( &cinfo );
  jpeg_destroy_compress( &cinfo );
  fclose( outfile );
  return 1;
}

void * computeGreyscale(void * threadid) {
	int row = *(int *)threadid;
	for(int i = (pixels_per_thread * row); (i - 2) < ((row + 1) * pixels_per_thread); i += 3) {
		int max = 0;
		if(raw_image[i] > max) {
			max = raw_image[i];
		}
		if(raw_image[i + 1] > max) {
			max = raw_image[i + 1];
		}
		if(raw_image[i + 2] > max) {
			max = raw_image[i + 2];
		}

		max = max / 2;

		raw_image[i] = max;
		raw_image[i + 1] = max;
		raw_image[i + 2] = max;
	}
}

int main(int argc, char **argv) {
  char *infilename = argv[1], *outfilename = argv[2];

  struct timespec tstart={0,0}, tend={0,0};
  clock_gettime(CLOCK_MONOTONIC, &tstart);

  num_threads = atoi(argv[3]);

  if( read_jpeg( infilename ) > 0 ) {
    if( write_jpeg( outfilename ) < 0 ) {
      return -1;
    }

    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("%d, %d\n", height, width);
    printf("%d by %d, %d pixels, %d bit RGB.\n Conversion to greyscale.\n %s Threads in use.\n", width, height, (width * height), bytes_per_pixel, argv[3]);
		printf("%.5f seconds picture processing time.\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
  } else {
    return -1;
  }
  return 0;
}
