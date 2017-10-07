#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HuffMan.h"

/*
  Makes a HuffMan tree. Returns a pointer to the root node.
  size is passed in, indicating the size of the character Array.
  Range is the range of characters (0 - 255).
*/
HuffNode * make_huffman_tree(char * string, int size, int range) {
  HuffNode * level;
  level = (HuffNode *) malloc(sizeof(HuffNode) * range);

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
      level[1].frequency++;
    }
  }

  fillFreqsWithOnes(level, range);                  // fill 0 frequencies with 1.
  sort(level, range);                               // sort the array.
  size = range;

  while(size != 1) {
    size = make_compoundNode(level, size);          // size is thee size of the specific depth of the tree returned.
    sort(level, size);                              // sort.
  }

  HuffNode * tree = (HuffNode *)malloc(sizeof(HuffNode));
  tree[0] = level[0];
  return tree;                                      // returns the root node.
}

/*
  Fill's all the character nodes with frequency zero, to frequency one.
*/
void fillFreqsWithOnes(HuffNode * tree, int size) {
  for(int i = 0; i < size; i++) {
    if(tree[i].frequency == 0) {
      tree[i].character = (char)i;
      tree[i].frequency = 1;
    }
  }
}

/*
  sorts the level of the tree, from smallest to highest.
*/
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

/*
  Make a compound node between either 2 character nodes, a character node and a compound Node, or 2 compound nodes.
  return the new size of the level/array.
*/
int make_compoundNode(HuffNode * level, int size) {
  HuffNode node1 = level[0], node2 = level[1], compound;
  compound.frequency = node1.frequency + node2.frequency;
  HuffNode * node1ptr = (HuffNode *)malloc(sizeof(HuffNode)), * node2ptr = (HuffNode *)malloc(sizeof(HuffNode));
  node1ptr[0] = node1;
  node2ptr[0] = node2;
  compound.left = node1ptr;
  compound.right = node2ptr;
  level[0] = compound;
  HuffNode tmp = level[size - 1];
  level[1] = tmp;
  return --size;
}


/*
  Recurrsivly calculate the encodings of each node, then when a node has neither a left or right node,
  use fputc(File * file) to print the encoding to the file, along with the character of that node.
*/
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
    table[*index] = (char *)malloc(sizeof(char) * (depth + 3));
    table[*index][0] = node->character;
    table[*index][1] = ',';
    for(int i = 2; (i-2) < depth; i++) {
      table[*index][i] = (0x30 + code[i - 2]);
    }
    table[*index][depth + 2] = '\n';                                                          // attach \n to the end of the row, so that the program know the end of the enocing.
    code[depth - 1] = (int)NULL;
    *index = *index + 1;
  }
}
