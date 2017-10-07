//
// Created by root on 23/11/16.
//

#ifndef BLOOM_BITVECTOR_H
#define BLOOM_BITVECTOR_H

#endif //BLOOM_BITVECTOR_H

typedef struct bitset{
    int * areaOfSet;
    int size;
}Bitset;

// create a new, empty bit vector set of 'size' items
Bitset * bitset_new(int size);

// check to see if an item is in the set
// returns 1 if in the set, 0 if not, and -1 if 'item' is out of bounds
int bitset_lookup(Bitset * set, int item);

// add an item, with number 'item' to the set
// (returns 0 if item is out of bounds, 1 otherwise)
// has no effect if the item is already in the set
int bitset_add(Bitset * set, int item);

// remove an item with number 'item' from the set
// (returns 0 if item is out of bounds, 1 otherwise)
int bitset_remove(Bitset * set, int item);

// place the union of src1 and src2 into dest
void bitset_union(Bitset * dest, Bitset * src1,
                  Bitset * src2);

// place the intersection of src1 and src2 into dest
void bitset_intersect(Bitset * dest, Bitset * src1,
                      Bitset * src2);