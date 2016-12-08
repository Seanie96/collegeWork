//
// Created by root on 09/11/16.
//

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <errno.h>
#include "Bloom.h"

#define SIZE 90000
#define MAX_STRING 256

void bitVector();
void bloom(char * argv[]);
int tokenise_ops(char buffer[], int startingIndex, char charResult[]);

int main(int argc, char * argv[]) {
    if(strcmp(argv[1], "bitvector") == 0) {
        bitVector();
    }   else if(strcmp(argv[1], "bloom") == 0) {
        bloom(argv);
    }   else    {
        printf("that option does not exist!");
    }

    return 0;
}

void bitVector() {

    char line1[MAX_STRING];
    memset(line1, 0, MAX_STRING - 1);
    fgets(line1, MAX_STRING, stdin);

    char line2[MAX_STRING];
    memset(line2, 0, MAX_STRING - 1);
    fgets(line2, MAX_STRING, stdin);

    Bitset *set1 = bitset_new(MAX_STRING);

    int index = 0;
    int characterInDec = (int) line1[index];
    while (characterInDec != '\n') {
        bitset_add(set1, characterInDec);
        characterInDec = line1[++index];
    }

    Bitset *set2;
    set2 = bitset_new(MAX_STRING);

    index = 0;
    characterInDec = (int) line2[index];
    while (characterInDec != '\n') {
        bitset_add(set2, characterInDec);
        characterInDec = line2[++index];
    }

    for (int i = 0; i < MAX_STRING; i++) {

        int returns = bitset_lookup(set1, i);

        if (returns == 1) {
            char character = (char) i;
            printf("%c \n", character);
        }
    }

    printf("Second line...\n");

    for (int i = 0; i < MAX_STRING; i++) {

        int returns = bitset_lookup(set2, i);

        if (returns == 1) {
            char character = (char) i;
            printf("%c \n", character);
        }

    }

    printf("The Union set..... \n");

    Bitset *uni;
    uni = bitset_new(MAX_STRING);
    bitset_union(uni, set1, set2);

    for (int i = 0; i < MAX_STRING; i++) {

        int returns = bitset_lookup(uni, i);

        if (returns == 1) {
            char character = (char) i;
            printf("%c \n", character);
        }

    }


    printf("The Intersection set..... \n");

    Bitset *inter;
    inter = bitset_new(MAX_STRING);

    bitset_intersect(inter, set1, set2);


    for (int i = 0; i < MAX_STRING; i++) {

        int returns = bitset_lookup(inter, i);

        if (returns == 1) {
            char character = (char) i;
            printf("%c \n", character);
        }

    }

    free(set1);
    free(set2);
    free(uni);
    free(inter);
}

void bloom(char * argv[]) {

    const char * st1 = argv[2];
    const char * mode = "r";

    FILE * file;
    file = fopen(st1, mode);

    char ** strings1 = malloc(sizeof(char *) * SIZE);
    int index1 = 0;
    strings1[index1] = malloc(sizeof(char) * 100);           // max size of word = 100.

    while(fscanf(file, "%s ", strings1[index1]) != EOF) {
        printf("%s \n", strings1[index1++]);
        strings1[index1] = malloc(sizeof(char) * 100);       // max size of word = 100.
    }

    printf("size:%d \n", index1);

    Bloom * seen = bloom_new(index1);
    printf("%s........... \n", strings1[0]);
    printf("%s........... \n", strings1[1]);
    int keepGoing = 1;
    for(int i = 0; i < index1; i++) {
        bloom_add(seen, strings1[i]);
        /*if(strcmp(strings1[i], "Mary") == 0) {
          printf("hash1:%d \n", getHashVal1(strings1[1]));
          printf("hash2:%d \n", getHashVal2(strings1[1]));
          keepGoing = 0;
        }*/
        printf("%s \n", strings1[i]);
    }

    printf("\nThe following words are those that are in %s but also in %s \n \n", argv[3], argv[2]);

    st1 = argv[3];
    mode = "r";
    file = fopen(st1, mode);

    char ** strings2 = malloc(sizeof(char *) * SIZE);
    int index2 = 0;

    Bloom * written = bloom_new(seen->size);
    strings2[index2] = malloc(sizeof(char) * 100);

    while(fscanf(file, "%s", strings2[index2]) != EOF) {
        if(bloom_lookup(seen, strings2[index2]) == 1) {
          if(bloom_lookup(written, strings2[index2]) == 0) {
            printf("%s\n", strings2[index2]);
            bloom_add(written, strings2[index2++]);
          } else  {

          }
          strings2[index2] = malloc(sizeof(char) * 100);
        }
    }

    printf("size: %d", index2);
}
