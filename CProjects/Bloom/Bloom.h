//
// Created by root on 23/11/16.
//

#ifndef BLOOM_BLOOM_H
#define BLOOM_BLOOM_H

#endif //BLOOM_BLOOM_H

#include "String.h"

typedef struct Bloom{
    int top;
    Bitset * set;
    char ** strings;
    int size;
}Bloom;

// create a new, empty Bloom filter of 'size' items
Bloom * bloom_new(int size);

// check to see if a string is in the set
int bloom_lookup(Bloom * this, char * item);

// add a string to the set
// has no effect if the item is already in the set
void bloom_add(Bloom * this, char * item);

// place the union of src1 and src2 into dest
void bloom_union(Bloom * dest, Bloom * src1,
                 Bloom * src2);

// place the intersection of src1 and src2 into dest
void bloom_intersect(Bloom * dest, Bloom * src1,
                 Bloom * src2);

// print Strings
void printStrings(Bloom * bloom);
