//
// Created by root on 23/11/16.
//

#ifndef BLOOM_STRING_H
#define BLOOM_STRING_H

#endif //BLOOM_STRING_H

#include "BitVector.h"

// add int attribute values from the hash pointer to the bitset.
void add_Stringtoset(char * word, Bitset * set);

// checks to see if checkVal exists in values. Return 1 if it exists in the Vector set. Else 0.
int stringExistsInSet(char * word, Bitset * set);

// returns the 1st hash values associated with a String.
int getHashVal1(char * string);

// returns the 2nd hash values associated with a String.
int getHashVal2(char * string);
