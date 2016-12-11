#include "HuffMan.h"

typedef struct Encode{
  int charIndex, bitIndex;
}Encode;

typedef struct Decode{

}Decode;

typedef struct File{
  FILE * fileToRead;
  FILE * fileToWrite;
  FILE * encodings;
  char * charactersToWrite;
  Encode * encode;
  Decode * decode;
}File;

int encodeCharacter(File * file);

int writeCharacters(File * file);

int decodeCharacter(File * file);
