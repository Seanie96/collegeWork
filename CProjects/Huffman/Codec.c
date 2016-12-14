#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Codec.h"

int findEncoding(File * file, int size) {
  char ** table = file->table;
  FILE * fileRead = file->fileToRead;
  char c = fgetc(fileRead);
  if(!feof(fileRead)) {
    char * code = findCode(c, table, size);
    if(code[0] == '-') {
      return -1;
    } else  {
      printf("%s", code);
      //printf("char:%c", c);
      addEncoding(file, code);
      return 1;
    }
  } else  {
    char character = EOF;
    char * code = findCode(character, table, size);
    //printf("%s", code);
    //printf("char:%c", character);
    addEncoding(file, code);
    return -1;
  }
}

void addEncoding(File * file, char * code) {
  char * charactersToWrite = file->charactersToWrite;
  Encode * encode = file->encode;
  int charIndex = encode->charIndex;
  int offSet = encode->bitIndex;
  //printf("encoding:%s\n", code);

  int index = 0;
  while(code[index] != '\n') {
    printf("code: %s\n", code);
    printf("encodedVersion: %c\n", charactersToWrite[charIndex]);
    char chartmp = charactersToWrite[charIndex];
    int zeroOrOne;
    if(code[index] == '0') {
      zeroOrOne = 0;
    } else  {
      zeroOrOne = 1;
    }
    zeroOrOne = zeroOrOne << offSet;
    chartmp = chartmp | zeroOrOne;
    charactersToWrite[charIndex] = chartmp;
    if(++offSet == 8) {
      charIndex++;
      offSet = 0;
    }
    index++;
  }

  printf("In addEncoding, character:%c\n", charactersToWrite[0]);
  encode->charIndex = charIndex;
  encode->bitIndex = offSet;
}

int writeCharacters(File * file, int size) {
  FILE * fileWrite = file->fileToWrite;
  char * characters = file->charactersToWrite;
  //int keepGoing = 1, index = 0;
  for(int i = 0; i < size; i++) {
    printf("char: %c\n", characters[i]);
    fputc(characters[i], fileWrite);
  }
  return 1;
}

void getBits(File * file, int size) {
  char ** table = file->table;
  FILE * fileRead = file->fileToRead;
  Decode * decode = file->decode;
  int bitCharIndex = 0;
  char * bits = decode->bitArray;       // 8 bits per char
  int index = 0;
  for(int i = 0; i < size; i++) {
    char c = fgetc(fileRead);
    //printf("char:%c\n", c);
    if(!feof(fileRead)) {
      for(int i = 7; i >= 0; i--) {
        int bit = !!((c << i) & 0x80);
        int index = (i - 7) * -1;
        bits[index + (bitCharIndex * 8)] = 0x30 + bit;
        //printf("%d", bit);
        index++;
      }
      //printf("\n");
      bitCharIndex++;
    }
    //bits[bitCharIndex * 8] = '\0';                      that might need to be there!
  }
  decode->size = (bitCharIndex * 8);
}

void decodCharactersAndPrint(File * file) {
  Decode * decode = file->decode;
  FILE * outputFile = file->fileToWrite;
  char ** table = file->table;
  char * bits = decode->bitArray;
  //printf("bits:%s................................................\n", decode->bitArray);
  char * code;
  int size;
  int keepGoing = 1, startingIndex = 0, character = 0, endingIndex = 1;
  while(keepGoing == 1) {
    int characterFound = 0;
    char result = NULL;
    while(characterFound == 0) {
      printf("startingIndex:%d, endingIndex:%d\n", startingIndex, endingIndex);
      size = endingIndex - startingIndex;
      code = malloc(sizeof(char) * (size + 1));
      memset(code, 0, size + 1);
      strncpy(code, &bits[startingIndex], size);
      //code[size + 1] = '\n';
      //printf("code:%s\n", code);
      result = findCharacter(code, table, size);
      if(result != NULL) {
        characterFound = 1;
        if(result == EOF) {
          keepGoing = 0;
        } else  {
          //printf("about to print!!");
          fputc(result, outputFile);
        }
      }
      endingIndex++;
      //printf("encoding:%s\n", code);
      free(code);
    }
    //printf("character:%c\n", result);
    startingIndex = endingIndex - 1;
  }
}

char findCharacter(char * code, char ** table, int size) {
  //printf("codeRecieved:%s, size:%d\n", code, size);
  char * codeInTable;
  for(int i = 0; i < 256; i++) {
    codeInTable = malloc(sizeof(char) * 100);
    memset(codeInTable, 0, 100);
    int index = 2;
    char tmp = table[i][index++];
    while(tmp != '\n') {
      tmp = table[i][index++];
    }
    strncpy(codeInTable, &table[i][2], (index - 3));
    if(strcmp(codeInTable, code) == 0) {
      //free(codeInTable);
      //printf("found match!\n");
      //printf("codeFound:%s\n", code);
      return table[i][0];
    }
    free(codeInTable);
  }
  //printf("no match!!\n");
  char c = NULL;
  return c;
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
      printf("character: %c, code:%s", character ,code);
      code[counter - 1] = '\n';
      return code;
    }
  }
  char * row = malloc(sizeof(char));
  row[0] = '-';
  return row;
}

void putEncodingsToFile(File * file, int size) {
  FILE * encodings = file->encodings;
  char ** table = file->table;
  for(int i = 0; i < size; i++) {
    fputs(table[i], encodings);
  }
}
