#include "../inc/huff.h"

int main(int argc, char **argv){
    options(argc, argv);
    return EXIT_SUCCESS;
}

void options(int argc, char **argv){
    if (argc == 1){
        fprintf(stderr, "A file is needed to start the execution.\
        \nTry './exe -h' or './exe --help' for more information.\n");
        exit(-1);
    }
    for (uint8_t opt = 1; opt < argc - 1; opt++){
        if (!strcmp(argv[opt], "-c"))
            compression(argv[argc - 1]);
        else if (!strcmp(argv[opt], "-d"))
            decompression(argv[argc - 1]);
        else if (!strcmp(argv[opt], "-h") || strcmp(argv[opt], "--help"))
            help();
        else
            fprintf(stderr, "Bad option.\nTry './exe -h' or './exe --help' for more information.\n");
    }
}

void compression(char* file){
    PtrQ charFile = findCharFile(file);
}

void decompression(char* file){
    ;
}


PtrQ findCharFile(char* file){
    Data allChar[CHAR_MAX] = {{0}, {0}};
    for (int i = 0; i < CHAR_MAX; i++)
        allChar[i].value = (char) i;
    int8_t c;
    FILE *f = fopen(file, "r");
    while ((c = getc(f)) != EOF)
        allChar[c].occur++;
    fclose(f);
    Data* tmp = insertionSort(allChar);
    PtrQ occursNode = NULL;
    for (int i = 0; i < CHAR_MAX; i++, tmp++)
        if (tmp->occur) occursNode = enqueue(occursNode, createTree(tmp->occur, tmp->value));

    return occursNode;
}




HTree buildHuffmanTree(PtrQ allChar){
    PtrQ subT = NULL; 
    if (allChar->next)
        subT = enqueue(subT, createSubHTree(allChar->pTree, allChar->next->pTree));
    else
        return allChar->pTree;
    allChar = dequeue(allChar); allChar = dequeue(allChar);
    while (allChar){
        if (allChar->next && allChar->pTree->occur + allChar->next->pTree->occur < subT->pTree->occur){
            subT = enqueue(subT, createSubHTree(allChar->pTree, allChar->next->pTree));
            allChar = dequeue(allChar);
        }
        else
            enqueue(subT, createSubHTree(subT->pTree, allChar->pTree));
        allChar = dequeue(allChar);
    }
    while (subT->next)
        subT = enqueue(subT, createSubHTree(subT->pTree, subT->next->pTree));   
    return subT->pTree;
}
/*
Encoding* getCharEncoding(HTree allChar, Encoding* allCharE){
     DFS    recursive    left (0) : int code << 1    right (1) : (int code << 1)+1
    ;
}
*/