#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Codec.h"

/*
  This method finds the encoding of the next character recieved by fgetc(FILE * pt). It
  then adds the encoding into a char array of o's and 1's. It returns 1 if the encoding was
  successfully found, else it will return -1.
*/
int findEncoding(File * file, int size) {
  char ** table = file->table;
  FILE * fileRead = file->fileToRead;
  char c = fgetc(fileRead);
  if(!feof(fileRead)) {
    char * code = findCode(c, table, size);
    if(code[0] == '-') {
      return -1;
    } else  {
      addEncoding(file, code);
      return 1;
    }
  } else  {
    char character = EOF;
    char * code = findCode(character, table, size);
    addEncoding(file, code);
    return -1;
  }
}

/*
  This method specifically adds the encoding to a character array, stored in the File ADT
*/
void addEncoding(File * file, char * code) {
  char * charactersToWrite = file->charactersToWrite;
  Encode * encode = file->encode;
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
    chartmp = chartmp | zeroOrOne;
    charactersToWrite[charIndex] = chartmp;
    if(++offSet == 8) {
      charIndex++;
      offSet = 0;
    }
    index++;
  }
  encode->charIndex = charIndex;
  encode->bitIndex = offSet;
}

/*
  Writes the encoded characters stored in the File->encoded, to the File->fileToWrite attribute.
*/
void writeCharacters(File * file, int size) {
  FILE * fileWrite = file->fileToWrite;
  char * characters = file->charactersToWrite;
  for(int i = 0; i < size; i++) {
    fputc(characters[i], fileWrite);
  }
}

/*
  Gets the 0's and 1's bits of every character in the file and stores them in the bitArray in File->decode
*/
void getBits(File * file, int size) {
  char ** table = file->table;
  FILE * fileRead = file->fileToRead;
  Decode * decode = file->decode;
  int bitCharIndex = 0;
  char * bits = decode->bitArray;       // 8 bits per char
  int index = 0;
  for(int i = 0; i < size; i++) {
    char c = fgetc(fileRead);
    if(!feof(fileRead)) {
      for(int i = 7; i >= 0; i--) {
        int bit = !!((c << i) & 0x80);
        int index = (i - 7) * -1;
        bits[index + (bitCharIndex * 8)] = 0x30 + bit;
        index++;
      }
      bitCharIndex++;
    }
  }
}

/*
  decodes the bitArray in File * file, and prints the decoded characters into the file->fileToWrite.
*/
void decodCharactersAndPrint(File * file) {
  Decode * decode = file->decode;
  FILE * outputFile = file->fileToWrite;
  char ** table = file->table;
  char * bits = decode->bitArray;
  char * code;
  int size;
  int keepGoing = 1, startingIndex = 0, character = 0, endingIndex = 1;
  while(keepGoing == 1) {
    int characterFound = 0;
    char result = NULL;
    while(characterFound == 0) {
      size = endingIndex - startingIndex;
      code = malloc(sizeof(char) * (size + 1));
      memset(code, 0, size + 1);
      strncpy(code, &bits[startingIndex], size);
      result = findCharacter(code, table);
      if(result != NULL) {
        characterFound = 1;
        if(result == EOF) {
          keepGoing = 0;
        } else  {
          fputc(result, outputFile);
        }
      }
      endingIndex++;
      free(code);
    }
    startingIndex = endingIndex - 1;
  }
}

/*
  Find the character associated with the given encoded series of bits, in the array of strings called table.
*/
char findCharacter(char * code, char ** table) {
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
      return table[i][0];
    }
    free(codeInTable);
  }
  char c = NULL;
  return c;
}

/*
  Finds the encoded sequence of bits associated with the passed in character, in the array of strings called table.
*/
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

/*
  Uses fputs(File * file), to print out the strings of character to encodings, to the file->encodings.
*/
void putEncodingsToFile(File * file, int size) {
  FILE * encodings = file->encodings;
  char ** table = file->table;
  for(int i = 0; i < size; i++) {
    fputs(table[i], encodings);
  }
}
