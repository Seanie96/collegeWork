#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HuffMan.h"

HuffNode * make_huffman_tree(char * string, int size, int range) {
  HuffNode ** level;
  level = malloc(sizeof(HuffNode *) * range);                       // 10 possible levels!!

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
  compound->character = -2;                                       // make it a negative character so that the compound nodes can't be recognised as a leaf node.
  level[0] = compound;
  level[1] = NULL;
  return --size;
}

void saveEncodings(HuffNode * node, int * code, int depth, char ** table, int * index) {
  if(node->left != NULL) {
    code[depth] = 0;
    depth++;
    saveEncodings(node->left, code, depth, table, index);
    depth--;
  }
  if(node->right != NULL) {
    code[depth] = 1;
    depth++;
    saveEncodings(node->right, code, depth, table, index);
    depth--;
  }
  if(node->left == NULL && node->right == NULL) {
    table[*index] = malloc(sizeof(char) * (depth + 3));          // possible size of encdoing...
    table[*index][0] = node->character;
    table[*index][1] = ',';
    for(int i = 2; (i-2) < depth; i++) {
      table[*index][i] = (0x30 + code[i - 2]);
    }
    table[*index][depth + 2] = '\n';
    code[depth - 1] = NULL;
    *index = *index + 1;
  }
}
