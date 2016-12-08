#include "HuffMan.h"

typedef struct File{
  FILE * fileToRead;
  FILE * fileToWrite;
  int charIndex, bitIndex;
  char * charactersToWrite;
  HuffNode * root;
}File;

int encodeCharacter(File * file);

int writeCharacters(File * file);

int decodeCharacter(File * file);
