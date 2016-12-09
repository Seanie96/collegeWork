#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Codec.h"

void encode(FILE * trainingFile, FILE * inputFile, FILE * outputFile);
void decode(FILE * trainingFile, FILE * inputFile, FILE * outputFile);

int main(int argc, char ** argv) {
  FILE * trainingFile, * inputFile, * outputFile;
  trainingFile = fopen(argv[2], "r");       // training file.
  assert( trainingFile != NULL );

  inputFile = fopen(argv[3], "r");
  assert( inputFile != NULL );

  outputFile = fopen(argv[4], "w");
  int result = strcmp(argv[1], "encode");
  if(result == 0) {
    encode(trainingFile, inputFile, outputFile);
  } else if(strcmp(argv[0], "decode") == 0) {
    decode(trainingFile, inputFile, outputFile);
  } else  {
    printf("you can either decode or encode a file....\n");
  }
  return 0;
}

void encode(FILE * trainingFile, FILE * inputFile, FILE * outputFile) {
  char c;
  char * charArray = malloc(sizeof(char) * 2000000);      // number of characters in training file
  int index = 0;
  c = fgetc(trainingFile);	// attempt to read a byte
  while( !feof(trainingFile) ) {
    charArray[index] = c;
    index++;
    c = fgetc(trainingFile);
  }
  fclose(trainingFile);

  char * charArrayInput = malloc(sizeof(char) * 20000);      // number of characters in training file
  int counter = 0;
  c = fgetc(inputFile);	// attempt to read a byte

  while( !feof(inputFile) ) {
    charArrayInput[counter] = c;
    counter++;
    c = fgetc(inputFile);
  }
  fclose(inputFile);
  printf("poo\n");
  File * file = malloc(sizeof(File));
  file->fileToRead = inputFile;
  file->fileToWrite = outputFile;
  file->charactersToWrite = malloc(sizeof(char) * counter);       // max size of characters of the file will be counter.
  file->charIndex = 0, file->bitIndex = 0;

  FILE * encodings;
  encodings = fopen("encodings.txt", "w+");
  file->encodings = encodings;

  HuffNode * root = make_huffman_tree(charArray, index, 256);
  int * code = malloc(sizeof(int) * 100);                 // greatest possible number of bits to represent a letter.
  printFrequency(encodings, root, code, 0);

  int result = 1;
  printf("poo\n");
  while(result == 1) {
    result = encodeCharacter(file);
  }

  if(writeCharacters(file) == -1) {
    printf("Writing to file did not work!\n");
  }

  printf("Finished encoding\n");

}

void decode(FILE * trainingFile, FILE * inputFile, FILE * outputFile) {

}
