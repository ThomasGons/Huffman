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
            compression(argv[argc - 1]);
        else if (!strcmp(argv[opt], "-d"))
            decompression(argv[argc - 1]);
        else if (!strcmp(argv[opt], "-h") || !strcmp(argv[opt], "--help"))
            help();
        else
            printf("Bad option.\nTry './bin/exe -h <file>' or './bin/exe --help <file>' for more information.\n");
    }
    if (opt == 1){  // just file name
        compression(argv[argc - 1]);
        decompression(concat(argv[argc - 1], ".hfm"));
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
    HTree huffTree = buildHuffmanTree(charFile);
    vDisplay(huffTree, 0);
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
    if (!allChar->next)
        return allChar->pTree;
    PtrQ subT = NULL;
    subT= enqueue(subT, createSubHTree(allChar->pTree, allChar->next->pTree));
    allChar = dequeue(allChar); allChar = dequeue(allChar);
    while (allChar){
        if (subT->next && subT->pTree->occur <= allChar->pTree->occur && subT->next->pTree->occur <= allChar->pTree->occur){
            subT = enqueue(subT, createSubHTree(subT->pTree, subT->next->pTree));
            subT = dequeue(subT); subT = dequeue(subT);
        }
        else if (allChar->next && allChar->pTree->occur <= subT->pTree->occur && allChar->next->pTree->occur <= allChar->pTree->occur){
            subT = enqueue(subT, createSubHTree(allChar->pTree, allChar->next->pTree));
            allChar = dequeue(allChar); allChar = dequeue(allChar);
        }
        else{
            subT = enqueue(subT, createSubHTree(subT->pTree, allChar->pTree));
            allChar = dequeue(allChar); subT = dequeue(subT);
        }
    }
    while (subT->next){
        subT = enqueue(subT, createSubHTree(subT->pTree, subT->next->pTree));
        subT = dequeue(subT); subT = dequeue(subT);
    }
    return subT->pTree;
}
/*
Encoding* getCharEncoding(HTree allChar, Encoding* allCharE){
     DFS    recursive    left (0) : int code << 1    right (1) : (int code << 1)+1
    ;
}
*/