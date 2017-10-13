//
// Created by root on 19/11/16.
//
// creates a new String.

#include <stdlib.h>
#include <stdio.h>
#include "String.h"


// add int attribute values from the hash pointer to the bitset.
void add_Stringtoset(char * word, Bitset * set) {

    if(stringExistsInSet(word, set) == 0) {
      int hash1 = getHashVal1(word);
      int hash2 = getHashVal2(word);
      //int hash3 = getHashVal3(word);
      //printf("hashval1: %d, hashval2: %d \n", hash1, hash2);

      bitset_add(set, hash1);
      bitset_add(set, hash2);
      //bitset_add(set, hash3);
      //printf("gets here!\n");
    } else  {
      //printf("String is already in set\n");
    }
}

int getHashVal1(char * string) {
  int i;
  int result = 0;

  for(int i = 0; string[i] != '\0'; i++) {
    result = (result<<1) ^ string[i];
  }

  result = result % 256;
  if(result < 0) {
    result *= -1;
  }

  return result;
}

int getHashVal2(char * string) {
  int c;
  int result = 5381;              // magic number.

  while(0 != (c = *string++)) {
    result = ((result<<5) + result) + c;
  }

  result = result % 256;
  if(result < 0) {
    result *= -1;
  }

  return result;
}

int stringExistsInSet(char * word, Bitset * set) {

    //printf("here!\n");
    int hash1 = getHashVal1(word);
    //printf("heyie \n");
    int hash2 = getHashVal2(word);

    //int hash3 = getHashVal3(word);

    //printf("now here!!!\n");

    int res1 = bitset_lookup(set, hash1);
    int res2 = bitset_lookup(set, hash2);
    //int res3 = bitset_lookup(set, hash3);

    if(res1 == 1 && res2 == 1) {
        return 1;
    }   else    {
        return 0;
    }
}
