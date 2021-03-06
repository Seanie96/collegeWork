typedef struct Encode{
  int charIndex, bitIndex;
}Encode;

typedef struct Decode{
  char * bitArray;
}Decode;

typedef struct File{
  FILE * fileToRead;
  FILE * fileToWrite;
  FILE * encodings;
  char * charactersToWrite;
  char ** table;
  Encode * encode;
  Decode * decode;
}File;

int findEncoding(File * file, int size);

void addEncoding(File * file, char * code);

void getBits(File * file, int size);

void decodCharactersAndPrint(File * file);

void writeCharacters(File * file, int size);

char * findCode(char character, char ** table, int size);

char findCharacter(char * code, char ** table);

void putEncodingsToFile(File * file, int size);
