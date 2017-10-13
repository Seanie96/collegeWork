typedef struct HuffNode {
  char character;
  int frequency;
  struct HuffNode * left;
  struct HuffNode * right;
}HuffNode;

HuffNode * make_huffman_tree(char * string, int size, int range);

void fillFreqsWithOnes(HuffNode * tree, int size);

void sort(HuffNode * level, int range);

int getRidOfNodes(HuffNode * level, int range, HuffNode * newLevel);

int make_compoundNode(HuffNode * level, int size);

void saveEncodings(HuffNode * node, int * code, int depth, char ** table, int * index);
