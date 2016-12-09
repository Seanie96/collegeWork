#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Codec.h"

int encodeCharacter(File * file) {
  printf("poo\n");
  FILE * fileWrite = file->fileToWrite;
  FILE * fileRead = file->fileToRead;
  char c = fgetc(fileRead);
  if(!feof(fileRead)) {
    printf("char:%c\n", c);
    char * code = findCode(file->root, c);
    printf("code:%s\n", code);
    char * charactersToWrite = file->charactersToWrite;

    int charIndex = file->charIndex;
    printf("charIndex:%d\n", charIndex);
    int offSet = file->bitIndex;
    int index = 0;
    while(code[index] != '\0') {
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
    file->charIndex = charIndex;
    file->bitIndex = offSet;
    printf("index:%d\n", charIndex);
    return 1;
  } else  {
    return -1;
  }
}

int writeCharacters(File * file) {
  FILE * fileWrite = file->fileToWrite;
  if(fputs(file->charactersToWrite, file->fileToWrite) == 0) {
    return -1;
  } else  {
    return 1;
  }
}

int decodeCharacter(File * file) {
  return 1;
}
