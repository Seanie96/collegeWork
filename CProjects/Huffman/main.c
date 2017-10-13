
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

  char * charArray = malloc(sizeof(char) * 2000000);                            // Max length of the character array. 2 million Bytes.
  int index = 0;
  c = fgetc(file);	                                                             // attempt to read a byte
  while( !feof(file) ) {
    charArray[index] = c;
    index++;
    c = fgetc(file);
  }
  fclose(file);

  HuffNode * root = make_huffman_tree(charArray, index, 256);                   // 256 is the range of characters.
  int * code = malloc(sizeof(int) * 100);

  int * counter = malloc(sizeof(int));
  char ** table = malloc(sizeof(char *) * 256);
  saveEncodings(root, code, 0, table, counter);

  /*
    Print the table with the encodings of eacch character.
  */

  for(int i = 0; i < 256; i++) {
    printf("%s", table[i]);
  }

  free(root);

  return 0;
}
