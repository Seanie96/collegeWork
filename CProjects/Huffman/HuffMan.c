#include <stdio.h>
#include <stdlib.h>
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
  level[0] = compound;
  level[1] = NULL;
  return --size;
}

void printFrequency(HuffNode * node, int * code, int depth) {
  if(node->left != NULL) {
    code[depth] = 0;
    depth++;
    printFrequency(node->left, code, depth);
    depth--;
  }
  if(node->right != NULL) {
    code[depth] = 1;
    depth++;
    printFrequency(node->right, code, depth);
    depth--;
  }
  if(node->left == NULL && node->right == NULL)  {
    node->code = malloc(sizeof(char) * (depth + 1));          // possible size of encdoing...
    for(int i = 0; i < depth; i++) {
      node->code[i] = (char)(0x30 + code[i]);
    }
    node->code[depth] = '\0';
    printf("character: %c, encoding:%s\n", node->character, node->code);
    code[depth - 1] = NULL;
  }
}

char * findCode(HuffNode * node, char character) {
  printf("in here!\n");
  if(node->character == character) {
    printf("char:%c\n", node->character);
    return node->code;
  } else  {
    if(node->left != NULL) {
      printf("in here1!\n");
      return findCode(node->left, character);
    }
    printf("in here!\n");
    if(node->right != NULL) {
      printf("in here2!\n");
      return findCode(node->right, character);
    }
  }
}

char findCharacter(HuffNode * node, char * code, int index) {
  if(code[index] == '0') {
    return findCharacter(node->left, code, ++index);
  } else if(code[index] == '1') {
    return findCharacter(node->right, code, ++index);
  } else {
    return node->character;
  }
}
