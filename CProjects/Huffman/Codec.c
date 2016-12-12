#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Codec.h"

int findEncoding(File * file, int size) {
  char ** table = file->table;
  FILE * fileRead = file->fileToRead;
  char c = fgetc(fileRead);
  if(!feof(fileRead)) {
    char * code = findCode(c, table, size);
    printf("char: %c, code:%s", c, code);
    if(code[0] == '-') {
      printf("character is not in set of encodings.\n");
      return -1;
    } else  {
      addEncoding(file, code);
      return 1;
    }
  } else  {
    char character = EOF;
    char * code = findCode(character, table, size);
    addEncoding(file, code);
    printf("char: %c, code:%s", character, code);
    printf("end of text file!\n");
    return -1;
  }
}

void addEncoding(File * file, char * code) {
  char * charactersToWrite = file->charactersToWrite;
  Encode * encode = file->encode;
  int charIndex = encode->charIndex;
  int offSet = encode->bitIndex;

  printf("charIndex:%d, bitIndex:%d\n", charIndex, offSet);
  printf("charEncodedBefore:%d\n", charactersToWrite[charIndex]);
  int index = 0;
  while(code[index] != '\n') {
    char chartmp = charactersToWrite[charIndex];
    int zeroOrOne;
    if(code[index] == '0') {
      zeroOrOne = 0;
    } else  {
      zeroOrOne = 1;
    }
    printf("zeroOrOne......before:%d\n", zeroOrOne);
    zeroOrOne = zeroOrOne << offSet;
    printf("zeroOrOne......After:%d\n", zeroOrOne);
    chartmp = chartmp | zeroOrOne;
    printf("charTmp......now:%d\n", chartmp);
    charactersToWrite[charIndex] = chartmp;
    if(++offSet == 8) {
      charIndex++;
      offSet = 0;
    }
    index++;
  }
  printf("charEncodedAfter:%d\n", charactersToWrite[charIndex]);
  encode->charIndex = charIndex;
  encode->bitIndex = offSet;
  printf("index:%d\n", charIndex);
}

int writeCharacters(File * file) {
  FILE * fileWrite = file->fileToWrite;
  char * characters = file->charactersToWrite;
  int keepGoing = 1, index = 0;
  while(keepGoing == 1) {
    fputc(characters[index++], fileWrite);
    printf("char:%d\n", characters[index]);
    if(characters[index] == '\0') {
      printf("null char reached!\n");
      keepGoing = 0;
    }
  }
  return 1;
}

int getBits(File * file, int size) {
  char ** table = file->table;
  FILE * fileRead = file->fileToRead;
  Decode * decode = file->decode;
  int bitCharIndex = decode->bitCharIndex;
  bitChar = 0;
  char * bits = decode->bitArray;       // 8 bits per char
  bits = malloc(sizeof(char) * (size * 8));

  for(int i = 0; i < size; i++) {
    char c = fgetc(fileRead);
    if(!feof(fileRead)) {
      for(int i = 7; i >= 0; i++) {
        int bit = 1;
        bit = bit << i;
        int bitReceived = bit & c;
        bitReceived = bitReceived >> i;
        int index = (i - 7) * -1;
        bits[index + (bitCharIndex * 8)] = 0x30 + bitReceived;
      }
      bitCharIndex++;
    }
  }
}

void decodedCharacter(File * file) {
  Decode * decode = file->decode;
  int bitCharIndex = decode->bitCharIndex;
  char * bits = decode->bitArray;
  char * characters = file->charactersToWrite;

  int keepGoing = 1;
  int startingIndex = 0;
  int character = 0;
  while(keepGoing == 1) {
    while()
  }
}

char * findCode(char character, char ** table, int size) {
  for(int i = 0; i < size; i++) {
    if(table[i][0] == character && table[i][1] == ',') {
      int counter = 2;
      char * code = malloc(sizeof(char) * 256);
      char nextChar = table[i][counter];
      code[counter - 2] = nextChar;
      counter++;
      while(nextChar != '\n') {
        nextChar = table[i][counter];
        code[counter - 2] = nextChar;
        counter++;
      }
      code[counter - 1] = '\n';
      return code;
    }
  }
  char * row = malloc(sizeof(char));
  row[0] = '-';
  return row;
}

int findCharacter(char * code, char ** table, int size) {
  int character = NULL;
  for(int i = 0; i < size; i++) {
    int keepGoing = 1, foundEncoding = 1, index = 2;
    while(keepGoing == 1) {
      if(table[i][index] == '\n') {
        keepGoing = 0;
      } else if(table[i][index] != code[index - 2]) {
        keepGoing = 0;
        foundEncoding = 0;
      }
      index++;
    }
    if(foundEncoding == 1) {
      return table[i][0];
    }
  }
  return character;
}

void putEncodingsToFile(File * file, int size) {
  FILE * encodings = file->encodings;
  char ** table = file->table;
  for(int i = 0; i < size; i++) {
    fputs(table[i], encodings);
  }
}
