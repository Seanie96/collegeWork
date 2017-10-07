#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Codec.h"
#include "HuffMan.h"

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
  char * charArray = malloc(sizeof(char) * 2000000);             // max number of characters to be read in. 2 million bytes.
  int index = 0;

  trainingFile = fopen(trainingFileTxt, "r");
  c = fgetc(trainingFile);	                                     // attempt to read a byte
  while( !feof(trainingFile) ) {
    charArray[index] = c;
    index++;
    c = fgetc(trainingFile);
  }
  fclose(trainingFile);

  char * charArrayInput = malloc(sizeof(char) * 2000000);        // max number of characters to be read in. 2 million bytes.
  int counter = 0;
  inputFile = fopen(inputFileTxt, "r");
  c = fgetc(inputFile);	                                         // attempt to read a byte

  while( !feof(inputFile) ) {
    charArrayInput[counter] = c;
    counter++;
    c = fgetc(inputFile);
  }
  fclose(inputFile);

  File * file = malloc(sizeof(File));
  file->table = malloc(sizeof(char  *) * 256);                   // range of characters.
  file->fileToRead = inputFile;
  file->fileToWrite = outputFile;
  file->charactersToWrite = malloc(sizeof(char) * counter);      // max size of characters of the file will be counter.
  Encode * encode = malloc(sizeof(Encode));
  encode->bitIndex = 0;
  encode->charIndex = 0;
  file->encode = encode;

  memset(file->charactersToWrite, 0, counter);

  char * encodingsTxt = "encodings.txt";

  HuffNode * root = make_huffman_tree(charArray, index, 256);
  int * code = malloc(sizeof(int) * 100);                        // greatest possible number of bits to represent a letter.
  int * count = malloc(sizeof(int));
  saveEncodings(root, code, 0, file->table, count);

  file->encodings = fopen(encodingsTxt, "w");
  putEncodingsToFile(file, 256);                                // puts the encodings to encodings.txt
  fclose(file->encodings);

  int result = 1;

  file->fileToRead = fopen(inputFileTxt, "r");
  while(result == 1) {
    file->encodings = fopen(encodingsTxt, "r");
    result = findEncoding(file, 256);                           // finds the encodings for each character in the character array stored as an attribute in file.
    fclose(file->encodings);
  }
  fclose(file->fileToRead);

  file->fileToWrite = fopen(outputFileTxt, "w");
  int sizeOfCharIndex = file->encode->charIndex + file->encode->bitIndex;
  writeCharacters(file, sizeOfCharIndex);                       // write the encoded characters to the file->fileToWrite
  fclose(file->fileToWrite);

  printf("Finished encoding\n");

}

void decode(char * trainingFileTxt, char * inputFileTxt, char * outputFileTxt) {
  FILE * trainingFile, * inputFile, * outputFile;
  char c;
  char * charArray = malloc(sizeof(char) * 2000000);            // Max number of characters in training file. 2 million bytes.
  int index = 0;

  trainingFile = fopen(trainingFileTxt, "r");
  c = fgetc(trainingFile);	                                    // attempt to read a byte
  while( !feof(trainingFile) ) {
    charArray[index] = c;
    index++;
    c = fgetc(trainingFile);
  }
  fclose(trainingFile);

  char * charArrayInput = malloc(sizeof(char) * 2000000);               // Max number of characters in training file. 2 million bytes.
  int counter = 0;
  inputFile = fopen(inputFileTxt, "r");
  c = fgetc(inputFile);	                                                // attempt to read a byte

  while( !feof(inputFile) ) {
    charArrayInput[counter] = c;
    counter++;
    c = fgetc(inputFile);
  }
  fclose(inputFile);

  File * file = malloc(sizeof(File));
  file->table = malloc(sizeof(char  *) * 256);
  file->fileToRead = inputFile;
  file->fileToWrite = outputFile;
  file->charactersToWrite = malloc(sizeof(char) * (counter * 8));       // max size of characters of the file will be counter.
  Decode * decode = malloc(sizeof(Decode));
  decode->bitArray = malloc(sizeof(char) * (8 * counter));              // bit array will need to be at least 8 times the size of the number of characters read in from the input file.
  file->decode = decode;
  char * encodingsTxt = "encodings.txt";

  HuffNode * root = make_huffman_tree(charArray, index, 256);           // make HuffMan tree.
  int * code = malloc(sizeof(int) * 100);                               // greatest possible number of bits to represent a letter.
  int * count = malloc(sizeof(int));
  saveEncodings(root, code, 0, file->table, count);                     // save encodings to a 1d string array.

  file->encodings = fopen(encodingsTxt, "w");
  putEncodingsToFile(file, 256);                                        // write the encodings to the encodings.txt file
  fclose(file->encodings);

  file->fileToRead = fopen(inputFileTxt, "r");
  getBits(file, counter);                                               //  get the bits of each encoded character and store them in a character array, where 0 is '0' and 1 is '1'.
  fclose(file->fileToRead);

  free(charArray);

  file->fileToWrite = fopen(outputFileTxt, "w");
  decodCharactersAndPrint(file);                                        // decode the bitArray with the character representation of each bit ('0', '1'), and print the resulting characters to file->fileToWrite.
  fclose(file->fileToWrite);
  printf("finished decoding");
}
