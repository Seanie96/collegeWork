#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Codec.h"

void encode(char * trainingFileTxt, char * inputFileTxt, char * outputFileTxt);
void decode(char * trainingFileTxt, char * inputFileTxt, char * outputFileTxt);

int main(int argc, char ** argv) {
  if(strcmp(argv[1], "encode") == 0) {
    encode(argv[2], argv[3], argv[4]);
  } else if(strcmp(argv[1], "decode") == 0) {
    decode(argv[2], argv[3], argv[4]);
  } else  {
    printf("you can either decode or encode a file....\n");
  }
  return 0;
}

void encode(char * trainingFileTxt, char * inputFileTxt, char * outputFileTxt) {
  FILE * trainingFile, * inputFile, * outputFile;
  char c;
  char * charArray = malloc(sizeof(char) * 2000000);      // number of characters in training file
  int index = 0;

  trainingFile = fopen(trainingFileTxt, "r");
  c = fgetc(trainingFile);	// attempt to read a byte
  while( !feof(trainingFile) ) {
    charArray[index] = c;
    index++;
    c = fgetc(trainingFile);
  }
  fclose(trainingFile);

  char * charArrayInput = malloc(sizeof(char) * 20000);      // number of characters in training file
  int counter = 0;
  inputFile = fopen(inputFileTxt, "r");
  c = fgetc(inputFile);	// attempt to read a byte

  while( !feof(inputFile) ) {
    charArrayInput[counter] = c;
    counter++;
    c = fgetc(inputFile);
  }
  fclose(inputFile);

  File * file = malloc(sizeof(File));
  file->fileToRead = inputFile;
  file->fileToWrite = outputFile;
  file->charactersToWrite = malloc(sizeof(char) * counter);       // max size of characters of the file will be counter.
  Encode * encode = malloc(sizeof(Encode));
  encode->bitIndex = 0;
  encode->charIndex = 0;
  file->encode = encode;

  char * encodingsTxt = "encodings.txt";
  file->encodings = fopen(encodingsTxt, "w");

  HuffNode * root = make_huffman_tree(charArray, index, 256);
  int * code = malloc(sizeof(int) * 100);                 // greatest possible number of bits to represent a letter.
  printFrequency(file->encodings, root, code, 0);
  fclose(file->encodings);

  int result = 1;

  file->fileToRead = fopen(inputFileTxt, "r");
  while(result == 1) {
    file->encodings = fopen(encodingsTxt, "r");
    result = encodeCharacter(file);
    fclose(file->encodings);
  }
  fclose(file->fileToRead);

  printf("encoded version: %s\n", file->charactersToWrite);



  file->fileToWrite = fopen(outputFileTxt, "w");
  if(writeCharacters(file) == -1) {
    printf("Writing to file did not work!\n");
  } else  {
    printf("writing to file now finished!\n");
  }
  fclose(file->fileToWrite);

  printf("Finished encoding\n");

}

void decode(char * trainingFileTxt, char * inputFileTxt, char * outputFileTxt) {
  FILE * trainingFile, * inputFile, * outputFile;
  char c;
  char * charArray = malloc(sizeof(char) * 2000000);      // number of characters in training file
  int index = 0;

  trainingFile = fopen(trainingFileTxt, "r");
  c = fgetc(trainingFile);	// attempt to read a byte
  while( !feof(trainingFile) ) {
    charArray[index] = c;
    index++;
    c = fgetc(trainingFile);
  }
  fclose(trainingFile);

  char * charArrayInput = malloc(sizeof(char) * 20000);      // number of characters in training file
  int counter = 0;
  inputFile = fopen(inputFileTxt, "r");
  c = fgetc(inputFile);	// attempt to read a byte

  while( !feof(inputFile) ) {
    charArrayInput[counter] = c;
    counter++;
    c = fgetc(inputFile);
  }
  fclose(inputFile);


}
