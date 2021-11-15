#include "../inc/huff.h"

int main(int argc, char **argv){
    if (argc == 1){
        fprintf(stderr, "A file is needed to start the execution.\
        \nTry './bin/exe -h <file>' or './bin/exe --help <file>' for more information.\n");
        exit(-1);
    }
    uint8_t opt;
    for (opt = 1; opt < argc - 1; opt++){
        if (!strcmp(argv[opt], "-c"))
            compression(argv[argc]);
        else if (!strcmp(argv[opt], "-d"))
            decompression(argv[argc]);
        else if (!strcmp(argv[opt], "-h") || !strcmp(argv[opt], "--help"))
            help();
        else
            printf("Bad option.\nTry './bin/exe -h <file>' or './bin/exe --help <file>' for more information.\n");
    }
    if (opt == 1){  // just file name
        compression(argv[argc]);
        decompression(concat(argv[argc], ".hff"));
    }
    return EXIT_SUCCESS;
}

char* concat(char* a, char* b){
    char *tmp = malloc(sizeof *tmp *(strlen(a) + strlen(b))), *tmpB = tmp;
    for (; *b != '\0'; (*a == '\0') ? b++ : a++, tmpB++)
        *tmpB = *a;
    for (; *b != '\0'; b++, tmpB++)
        *tmpB = *b;
    return tmp;   
}


void compression(char* file){
    /*
    Burrows-Wheeler => Move to front => Huffman
    */
    PtrQ charFile = findCharFile(file);
}

void decompression(char* file){
    /*
    Huffman => Move to front =>Burrows-Wheeler
    */
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