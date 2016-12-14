#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Codec.h"
#include "HuffMan.h"

void encode(char * trainingFileTxt, char * inputFileTxt, char * outputFileTxt);
void decode(char * trainingFileTxt, char * inputFileTxt, char * outputFileTxt);

int main(int argc, char ** argv) {
  printf("here\n");
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

  printf("here\n");

  trainingFile = fopen(trainingFileTxt, "r");
  c = fgetc(trainingFile);	// attempt to read a byte
  while( !feof(trainingFile) ) {
    charArray[index] = c;
    index++;
    c = fgetc(trainingFile);
  }
  fclose(trainingFile);

  printf("here\n");

  char * charArrayInput = malloc(sizeof(char) * 2000000);      // number of characters in training file
  int counter = 0;
  inputFile = fopen(inputFileTxt, "r");
  c = fgetc(inputFile);	// attempt to read a byte

  printf("here\n");

  while( !feof(inputFile) ) {
    charArrayInput[counter] = c;
    counter++;
    c = fgetc(inputFile);
  }
  fclose(inputFile);

  printf("here\n");

  File * file = malloc(sizeof(File));
  file->table = malloc(sizeof(char  *) * 256);
  file->fileToRead = inputFile;
  file->fileToWrite = outputFile;
  file->charactersToWrite = malloc(sizeof(char) * counter);       // max size of characters of the file will be counter.
  Encode * encode = malloc(sizeof(Encode));
  encode->bitIndex = 0;
  encode->charIndex = 0;
  file->encode = encode;

  memset(file->charactersToWrite, 0, counter);

  char * encodingsTxt = "encodings.txt";

  HuffNode * root = make_huffman_tree(charArray, index, 256);
  int * code = malloc(sizeof(int) * 100);                 // greatest possible number of bits to represent a letter.
  int * count = malloc(sizeof(int));
  saveEncodings(root, code, 0, file->table, count);

  file->encodings = fopen(encodingsTxt, "w");
  putEncodingsToFile(file, 256);
  fclose(file->encodings);

  int result = 1;
  for(int i = 0; i < 256; i++) {
    printf("%s", file->table[i]);
  }

  file->fileToRead = fopen(inputFileTxt, "r");
  //file->fileToWrite = fopen(outputFileTxt);
  while(result == 1) {
    file->encodings = fopen(encodingsTxt, "r");
    result = findEncoding(file, 256);
    fclose(file->encodings);
  }
  //fclose(file->fileToWrite);
  fclose(file->fileToRead);

  printf("encoded version: %s\n", file->charactersToWrite);

  file->fileToWrite = fopen(outputFileTxt, "w");
  int sizeOfCharIndex = file->encode->charIndex + file->encode->bitIndex;
  if(writeCharacters(file, sizeOfCharIndex) == -1) {
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
  char * charArray = malloc(sizeof(char) * 2000000);              // number of characters in training file
  int index = 0;

  trainingFile = fopen(trainingFileTxt, "r");
  c = fgetc(trainingFile);	                                      // attempt to read a byte
  while( !feof(trainingFile) ) {
    charArray[index] = c;
    index++;
    c = fgetc(trainingFile);
  }
  fclose(trainingFile);

  char * charArrayInput = malloc(sizeof(char) * 2000000);           // number of characters in training file
  int counter = 0;
  inputFile = fopen(inputFileTxt, "r");
  c = fgetc(inputFile);	                                            // attempt to read a byte

  while( !feof(inputFile) ) {
    charArrayInput[counter] = c;
    counter++;
    c = fgetc(inputFile);
  }
  fclose(inputFile);
  printf("\n");

  File * file = malloc(sizeof(File));
  file->table = malloc(sizeof(char  *) * 256);
  file->fileToRead = inputFile;
  file->fileToWrite = outputFile;
  file->charactersToWrite = malloc(sizeof(char) * (counter * 8));       // max size of characters of the file will be counter.
  Decode * decode = malloc(sizeof(Decode));
  decode->bitArray = malloc(sizeof(char) * (8 * counter));
  file->decode = decode;
  char * encodingsTxt = "encodings.txt";

  HuffNode * root = make_huffman_tree(charArray, index, 256);
  int * code = malloc(sizeof(int) * 100);                         // greatest possible number of bits to represent a letter.
  int * count = malloc(sizeof(int));
  saveEncodings(root, code, 0, file->table, count);

  file->encodings = fopen(encodingsTxt, "w");
  putEncodingsToFile(file, 256);
  fclose(file->encodings);

  file->fileToRead = fopen(inputFileTxt, "r");
  getBits(file, counter);
  fclose(file->fileToRead);

  free(charArray);

  file->fileToWrite = fopen(outputFileTxt, "w");
  decodCharactersAndPrint(file);
  fclose(file->fileToWrite);
}
