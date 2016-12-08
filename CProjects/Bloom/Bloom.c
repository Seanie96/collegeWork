//
// Created by root on 23/11/16.
//

//
// Created by root on 10/11/16.
//

#include <stdlib.h>
#include <stdio.h>
#include "Bloom.h"

// create a new, empty Bloom filter of 'size' items
Bloom * bloom_new(int size){
    Bloom * this = malloc(sizeof(Bloom));
    this->set = bitset_new(size);
    char ** areaForStrings = malloc(sizeof(char *) * size);
    for(int i = 0; i < size; i++) {
      areaForStrings[i] = malloc(sizeof(char) * 100);        // char length...
    }
    this->strings = areaForStrings;
    this->size = size;
    this->top = 0;

    return this;
}

// check to see if a string is in the set
int bloom_lookup(Bloom * this, char * item){
    int size = this->size;
    Bitset * set = this->set;
    int result = stringExistsInSet(item, set);
    return result;
}

// add a string to the set
// has no effect if the item is already in the set
void bloom_add(Bloom * this, char * item){
    Bitset * set = this->set;
    char ** strings = this->strings;
    int top = this->top;
    strings[top] = item;
    top++;
    this->top = top;
    add_Stringtoset(item, set);
}

// place the union of src1 and src2 into dest
void bloom_union(Bloom * dest, Bloom * src1, Bloom * src2){
    Bitset * set1 = src1->set;
    Bitset * set2 = src2->set;
    Bitset * setdest = dest->set;

    bitset_union(setdest, set1, set2);
}

// place the intersection of src1 and src2 into dest
void bloom_intersect(Bloom * dest, Bloom * src1, Bloom * src2){
    Bitset * set1 = src1->set;
    Bitset * set2 = src2->set;
    Bitset * setdest = bitset_new(src1->size);

    bitset_intersect(setdest, set1, set2);
}
