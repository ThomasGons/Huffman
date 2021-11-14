#ifndef __HUFFMANCODE_H__
#define __HUFFMANCODE_H__

#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>

#define CHAR_MAX 128


typedef struct Data{
    size_t occur;
    char value;
}Data;

typedef struct Encoding{
    int binaryC, lengthBc; 
    char value;
}Encoding;

typedef struct Tree{
    size_t occur;
    char value;
    struct Tree *left, *right;
}Tree, *HTree;


typedef struct Queue{
    HTree pTree;
    struct Queue *next;
}Queue, *PtrQ;


PtrQ findCharFile();
Data* insertionSort(Data*);
int leavesTree(HTree);
HTree createTree(size_t, char);
HTree createSubHTree(HTree, HTree);
PtrQ enqueue(PtrQ, HTree);
PtrQ dequeue(PtrQ);
HTree buildHuffmanTree(PtrQ);
Encoding* getCharEncoding(HTree, Encoding*);
void vDisplay(HTree, int);

#endif
