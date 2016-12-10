#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HuffMan.h"

HuffNode * make_huffman_tree(char * string, int size, int range) {
  HuffNode ** level;
  level = malloc(sizeof(HuffNode *) * range);   // 10 possible levels!!

  for(int i = 0; i < range; i++) {
    level[i] = malloc(sizeof(HuffNode));
    HuffNode * node = level[i];
    node->frequency = 0;
    node->left = NULL;
    node->right = NULL;
  }

  for(int i = 0; i < size; i++) {
    int character = (int)string[i];
    HuffNode * node = level[character];
    if(node->frequency == 0) {
      node->character = (char)character;
      node->frequency++;
    } else  {
      node->frequency++;
    }
  }

  fillFreqsWithOnes(level, range);
  sort(level, range);
  size = range;

  while(size != 1) {
    size = make_compoundNode(level, size);
    sort(level, (size + 1));
  }


  return level[0];
}

void fillFreqsWithOnes(HuffNode ** tree, int size) {
  for(int i = 0; i < size; i++) {
    HuffNode * node = tree[i];
    if(node->frequency == 0) {
      node->character = (char)i;
      node->frequency = 1;
    }
  }
}

void sort(HuffNode ** level, int range) {
  for (int i = 0; i < range; ++i) {
    for (int j = i + 1; j < range; ++j) {
      if(level[i] == NULL && level[j] != NULL) {
        level[i] = level[j];
        level[j] = NULL;
      } else if(level[j] == NULL) {
        // Do Nothing!
      } else if(level[i]->frequency > level[j]->frequency) {
        HuffNode * tmp = level[i];
        level[i] = level[j];
        level[j] = tmp;
      }
    }
  }
}

int make_compoundNode(HuffNode ** level, int size) {
  HuffNode * node1 = level[0], * node2 = level[1];
  HuffNode * compound = malloc(sizeof(HuffNode));
  compound->frequency = node1->frequency + node2->frequency;
  compound->left = node1;
  compound->right = node2;
  compound->character = -2;     // make it a negative character so that the compound nodes can't be recognised as a leaf node.
  level[0] = compound;
  level[1] = NULL;
  return --size;
}

void printFrequency(FILE * file, HuffNode * node, int * code, int depth) {
  if(node->character == -1) {
    printf("pooo......");
  }
  if(node->left != NULL) {
    code[depth] = 0;
    depth++;
    printFrequency(file, node->left, code, depth);
    depth--;
  }
  if(node->right != NULL) {
    code[depth] = 1;
    depth++;
    printFrequency(file, node->right, code, depth);
    depth--;
  }
  if(node->left == NULL && node->right == NULL) {
    char * codeChar = malloc(sizeof(char) * (depth));          // possible size of encdoing...
    for(int i = 0; i < depth; i++) {
      codeChar[i] = (0x30 + code[i]);
    }
    codeChar[depth] = '\n';
    char * line = malloc(sizeof(char) * 50); // max size of line;
    line[0] = node->character;
    strcat(line, ",");
    strcat(line, codeChar);
    fputs(line, file);
    code[depth - 1] = NULL;
  }
}

char * findCode(FILE * encoding, char character) {
  FILE * file = fopen("encodings.txt", "r");
  char * line = malloc(sizeof(char) * 50);
  printf("charNeeded:%d\n", character);
  while(fgets(line, 256, file) != NULL && line[0] != character) {
    printf("charFound:%d\n", line[0]);
    memset(line, 0, 50);
  }
  printf("charFound:%d\n", line[0]);
  if(line[0] == character) {
    return line;
  } else  {
    line[0] = '-';
    return line;
  }
}

char findCharacter(FILE * encoding, char * code, int index) {
  int foundEncoding = 0;
  char character = '\0';
  char * line = malloc(sizeof(char) * 256);
  fgets(line, 256, encoding);
  while(line != NULL && foundEncoding == 0) {
      int index = 2;
      int keepGoing = 1;
      while(foundEncoding == 0 && keepGoing == 1) {
        if(line[index] != code[index - 2]) {
          keepGoing = 0;
        }
        if(line[index] == '\n') {
          foundEncoding = 1;
          character = line[0];
        }
        index++;
      }
      if(keepGoing == 0) {
        fgets(line, 256, encoding);
      }
  }
  return character;
}
