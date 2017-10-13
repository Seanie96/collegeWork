#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HuffMan.h"

HuffNode * make_huffman_tree(char * string, int size, int range) {
  HuffNode * level;
  level = malloc(sizeof(HuffNode) * range);                               // 10 possible levels!!

  for(int i = 0; i < range; i++) {
    level[i].frequency = 0;
    level[i].left = NULL;
    level[i].right = NULL;
  }

  for(int i = 0; i < size; i++) {
    int character = (int)string[i];
    if(level[character].frequency == 0) {
      level[character].character = (char)character;
      level[character].frequency++;
    } else  {
      level[character].frequency++;
    }
  }

  printf("here!!6\n");
  sort(level, range);

  for(int i = 0; i < range; i++) {
    printf("i:%d ,freq:%d\n", i, level[i].frequency);
  }

  printf("here!!7\n");
  HuffNode * newLevel = malloc(sizeof(HuffNode) * range);
  printf("here!!8\n");
  int newSize = getRidOfNodes(level, range, newLevel);
  printf("here!!9\n");
  fillFreqsWithOnes(level, newSize);
  printf("size:%d\n", newSize);

  int i = 0;
  while(newSize != 1) {
    printf("size:%d\n", newSize);
    newSize = make_compoundNode(level, newSize);
    sort(level, newSize);
  }

  HuffNode * tree = malloc(sizeof(HuffNode));
  tree[0] = level[0];
  return tree;
}

void fillFreqsWithOnes(HuffNode * tree, int size) {
  for(int i = 0; i < size; i++) {
    if(tree[i].frequency == 0) {
      tree[i].character = (char)i;
      tree[i].frequency = 1;
    }
  }
}

int getRidOfNodes(HuffNode * level, int range, HuffNode * newLevel) {
  HuffNode largest;
  int insertion = 0;
  for(int i = 0; i < (range - 1); i++) {
    largest = level[i];
    int index = i + 1;
    while(largest.frequency == level[index].frequency && index < (range - 1)) {
      if(largest.character < level[index].character) {
        largest = level[index];
      }
      index++;
    }
    i = --index;
    newLevel[insertion].frequency = largest.frequency;
    newLevel[insertion++].character = largest.character;
  }
  return insertion;
}

void sort(HuffNode * level, int range) {
  for (int i = 0; i < range; ++i) {
    for (int j = i + 1; j < range; ++j) {
      if(level[i].frequency > level[j].frequency) {
        HuffNode tmp = level[i];
        level[i] = level[j];
        level[j] = tmp;
      }
    }
  }
}

int make_compoundNode(HuffNode * level, int size) {
  HuffNode node1 = level[0], node2 = level[1], compound;
  compound.frequency = node1.frequency + node2.frequency;
  HuffNode * node1ptr = malloc(sizeof(HuffNode)), * node2ptr = malloc(sizeof(HuffNode));
  node1ptr[0] = node1;
  node2ptr[0] = node2;
  compound.left = node1ptr;
  compound.right = node2ptr;
  compound.character = -2;                                       // make it a negative character so that the compound nodes can't be recognised as a leaf node.
  level[0] = compound;
  HuffNode tmp = level[size - 1];
  level[1] = tmp;
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
  free(node);
}
