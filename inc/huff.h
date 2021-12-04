#ifndef __HUFFMANCODE_H__
#define __HUFFMANCODE_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>

            /* CONSTANTS AND MACROS */

#define CHAR_MAX 256

#define UNIT_PREFIX(n) ((n) >= 1000) ? \
                       ((n) >= 1000000) ? \
                       ((n) >= 1000000000) ? \
                       (Prefix) {(n) / 1000000000, "gb"} : \
                       (Prefix) {(n) / 1000000, "mb"} : \
                       (Prefix) {(n) / 1000, "kb"} : \
                       (Prefix) {(n), "bits"}


#define BAD_ARGUMENTS -1
#define TREE_ALLOCATION_FAILED -2 
#define QUEUE_ALLOCATION_FAILED -3

            /* STRUCTURES */ 

typedef enum bool{
    false, true
}bool;

typedef struct Data{
    size_t occur;
    unsigned char value;
    unsigned encoding;
    uint8_t size_encoding;
}Data;

typedef struct TreeElm{
    size_t occur;
    unsigned char value;
    struct TreeElm *left, *right;
}TreeElm, *Tree;

typedef struct PriorityQueueElm{
    Tree pt;
    struct PriorityQueueElm *next;
}PriorityQueueElm, *PriorityQueue;


typedef struct Prefix{
    float n;
    char* prf;
}Prefix;

            /* PROTOTYPES */

// huff.c
char *removeExt(char*);
void compression(char*);
void decompression(char*);
PriorityQueue findCharFile(char*, Data*, uint8_t*);
Tree buildHuffmanTree(PriorityQueue);
void getCharEncoding(Tree, Data*, unsigned, uint8_t);
void makeCompressFile(Data*, char*, uint8_t*);
void decoampression(char*);
void getDictionary(FILE*, Data*, uint8_t);
void makeDecompressFile(FILE*, Data*, char*, uint8_t);

// misc.c
void help();
Data *insertionSort(Data*);
PriorityQueue push(PriorityQueue, Tree);
PriorityQueue pull(PriorityQueue);
char *unitPrefix(float*);

// tree.c
int leavesTree(Tree);
Tree createTree(size_t, char);
Tree createOverTree(Tree, Tree);
void freeTree(Tree);
void vDisplay(Tree, int);


#endif
