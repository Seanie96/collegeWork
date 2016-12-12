
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "HuffMan.h"

int main(int argc, char ** argv) {
  unsigned char c;
  FILE * file;

  if ( argc != 2 ) {
    fprintf(stderr, "Useage: huffman <filename>\n");
    exit(1);	    // exit with error code
  }

  file = fopen(argv[1], "r");
  assert( file != NULL );

  char * charArray = malloc(sizeof(char) * 2000000);
  int index = 0;
  c = fgetc(file);	// attempt to read a byte
  while( !feof(file) ) {
    charArray[index] = c;
    index++;
    c = fgetc(file);
  }
  fclose(file);
  HuffNode * root = make_huffman_tree(charArray, index, 256);
  int * code = malloc(sizeof(int) * 100);

  printEncodings(root, code, 0);
  return 0;
}
