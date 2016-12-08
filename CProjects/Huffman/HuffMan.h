typedef struct HuffNode {
  char character;
  int frequency;
  struct HuffNode * left;
  struct HuffNode * right;
  char * code;
}HuffNode;

HuffNode * make_huffman_tree(char * string, int size, int range);

void fillFreqsWithOnes(HuffNode ** tree, int size);

void sort(HuffNode ** level, int range);

int make_compoundNode(HuffNode ** level, int size);

void printFrequency(HuffNode * node, int * code, int depth);

char * findCode(HuffNode * node, char character);

char findCharacter(HuffNode * node, char * code, int index);
