//
// Created by root on 10/11/16.
//

#include <stdlib.h>
#include <stdio.h>
#include "BitVector.h"



// create a new, empty bit vector set of 'size' items
Bitset * bitset_new(int size) {

    Bitset * bitSetArea = malloc(sizeof(Bitset));
    bitSetArea->size = size;
    bitSetArea->areaOfSet = calloc(size, sizeof(int)); // because there's 32 bits in each interger.
    return bitSetArea;

}

// item starts at 0, ends at (size - 1)
// check to see if an item is in the set
// returns 1 if in the set, 0 if not, and -1 if 'item' is out of bounds
int bitset_lookup(Bitset * set, int item) {

    //printf("in here 9! \n");
    int * array, size;
    array = set->areaOfSet;
    size = set->size;

    //printf("in here 8!\n");

    if(item < 0 || item > (size - 1)) {
        printf("out of bounds!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        return -1;
    }
    if(size != 0) {
        int offset = (item/32);
        int num = *(array + offset);

        int leftShiftBy = item % 32;
        num = num >> leftShiftBy;
        num = num & 0x01;

        //printf("in here5!\n");

        if(num == 1) {
            return 1;
        }
    }   else    {
        //printf("in here 7!\n");
        return -1;
    }
    return 0;

}

// item starts at 0, ends at (size - 1)
// add an item, with number 'item' to the set
// (returns 0 if item is out of bounds, 1 otherwise)
// has no effect if the item is already in the set
int bitset_add(Bitset * set, int item) {

    int * array;
    array = set->areaOfSet;

    int isItemThere = bitset_lookup(set, item);

    if(isItemThere == 0) {
        int offset = (item/32);
        //printf("in here!!\n");
        int rightShiftBy = item % 32;
        int ORnum = 1;
        ORnum = ORnum << rightShiftBy;
        //printf("in here!!!\n");
        int num = *(array + offset);
        *(array + offset) = num | ORnum;
        //printf("in here!!!\n");
    }   else if(isItemThere == -1) {
        //printf("in here!!!!\n");
        return 0;
    }
    return 1;

}

// remove an item with number 'item' from the set
// (returns 0 if item is out of bounds, 1 otherwise)
int bitset_remove(Bitset * set, int item) {

    int * array;
    array = set->areaOfSet;

    int isItemThere = bitset_lookup(set, item);
    if(isItemThere == 0) {
        int offset = (item/32);

        int rightShiftBy = item % 32;
        int ANDnum = 1;
        ANDnum = ANDnum << rightShiftBy;
        ANDnum = ~(ANDnum);

        *(array + offset) = *(array + offset) & ANDnum;
    }   else if(isItemThere == -1) {
        return 0;
    }

    return 1;

}

// place the union of src1 and src2 into dest
void bitset_union(Bitset * dest, Bitset * src1, Bitset * src2) {

    int size  = dest->size;
    int isSet1;
    int isSet2;
    for(int i = 0; i < size; i++) {



        isSet1 = bitset_lookup(src1, i);
        isSet2 = bitset_lookup(src2, i);
        if(isSet1 == 1 || isSet2 == 1) {
            bitset_add(dest, i);
        }
    }

}

// place the intersection of src1 and src2 into dest
void bitset_intersect(Bitset * dest, Bitset * src1, Bitset * src2) {

    int size1 = src1->size;
    int size2 = src2->size;
    int sizeDest = dest->size;

    int key = 0;

    int item1, item2;

    while(key < size1 && key < size2 && key < sizeDest) {

        item1 = bitset_lookup(src1, key);
        item2 = bitset_lookup(src2, key);

        if(item1 == 1 && item2 == 1) {
            bitset_add(dest, key);
        }

        key++;
    }

}
