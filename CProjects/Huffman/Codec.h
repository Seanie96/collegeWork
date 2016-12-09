#include "HuffMan.h"

typedef struct File{
  FILE * fileToRead;
  FILE * fileToWrite;
  FILE * encodings;
  int charIndex, bitIndex;
  char * charactersToWrite;
}File;

int encodeCharacter(File * file);

int writeCharacters(File * file);

int decodeCharacter(File * file);
