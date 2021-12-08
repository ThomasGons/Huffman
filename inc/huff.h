#ifndef __HUFFMANCODE_H__
#define __HUFFMANCODE_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>

            /* CONSTANTS AND MACROS */

#define CHAR_MAX 256

// get the fields value of an "over tree"
#define DATA_SUM(a, b) (Data) {'\0', a.occur + b.occur, 0, 0}
// retrieve the value of the p-th bit of a bit sequence
#define BIT_EXTRACTION(n, p) (1 & ((n) >> (p)))
// give the best prefix unit for data size
#define UNIT_PREFIX(n) ((n) >= 1000) ? \
                       ((n) >= 1000000) ? \
                       ((n) >= 1000000000) ? \
                       (Prefix) {(n) / 1000000000, "gb"} : \
                       (Prefix) {(n) / 1000000, "mb"} : \
                       (Prefix) {(n) / 1000, "kb"} : \
                       (Prefix) {(n), "bits"}


// important error codes

#define BAD_ARGUMENTS -1
#define FILE_DOES_NOT_EXIST -2
#define TREE_ALLOCATION_FAILED -3
#define QUEUE_ALLOCATION_FAILED -4

            /* STRUCTURES */ 

typedef enum bool{
    false, true
}bool;

typedef struct Data{
    unsigned char value;
    unsigned occur;
    unsigned encoding;
    uint8_t size_encoding;
}Data;

typedef struct TreeElm{
    Data data;
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
void compression(char*);
void decompression(char*);
PriorityQueue findCharFile(char*, Data*, uint8_t*);
Tree buildHuffmanTree(PriorityQueue);
void getCharEncoding(Tree, Data*, unsigned, uint8_t);
void makeCompressFile(Data*, char*, uint8_t*);
void decoampression(char*);
void getDictionary(FILE*, Data*, uint8_t);
void makeDecompressFile(FILE*, Tree, char*);

// misc.c
void help();
Data *insertionSort(Data*);
PriorityQueue push(PriorityQueue, Tree);
PriorityQueue pull(PriorityQueue);
void removeExt(char*);

// tree.c
Tree createTree(Data);
Tree createOverTree(Tree, Tree);
void freeTree(Tree);
void vDisplay(Tree, int);


#endif
