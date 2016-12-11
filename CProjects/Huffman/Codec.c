#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Codec.h"

int encodeCharacter(File * file) {
  FILE * fileRead = file->fileToRead;
  FILE * encodings = file->encodings;
  Encode * encode = file->encode;
  char c = fgetc(fileRead);
  if(!feof(fileRead)) {
    char * code = findCode(encodings, c);
    if(code[0] == '-') {
      printf("here!\n");
      return -1;
    } else  {
      int count = 2;
      printf("here!\n");
      char character = code[count];
      printf("code:%s", code);
      char * charactersToWrite = file->charactersToWrite;
      int charIndex = encode->charIndex;
      int offSet = encode->bitIndex;
      int index = 0;
      while(code[index] != '\n') {
        char chartmp = charactersToWrite[charIndex];
        int zeroOrOne;
        if(code[index] == '0') {
          zeroOrOne = 0;
        } else  {
          zeroOrOne = 1;
        }
        zeroOrOne = zeroOrOne << offSet;
        chartmp = chartmp || zeroOrOne;
        charactersToWrite[charIndex] = chartmp;
        if(++offSet == 8) {
          charIndex++;
          offSet = 0;
        }
        index++;
      }
      if(character == '\n') {
        file->charactersToWrite[charIndex + 1] = '\0';
      }
      encode->charIndex = charIndex;
      encode->bitIndex = offSet;
      printf("index:%d\n", charIndex);
      return 1;
    }
  } else  {
    printf("end of text file!\n");
    return -1;
  }
}

int writeCharacters(File * file) {
  FILE * fileWrite = file->fileToWrite;
  char * characters = file->charactersToWrite;
  int keepGoing = 1, index = 0;
  while(keepGoing == 1) {
    fputc(characters[index++], fileWrite);
    if(characters[index] == '\0') {
      keepGoing = 0;
    }
  }
  return 1;
}

int decodeCharacter(File * file) {
  return 1;
}
