#include "../inc/huff.h"

int main(){
    PtrQ charFile = findCharFile();
    vDisplay(buildHuffmanTree(charFile), 0);
    return EXIT_SUCCESS;
}

PtrQ findCharFile(){
    Data allChar[CHAR_MAX] = {{0}, {0}};
    for (int i = 0; i < CHAR_MAX; i++)
        allChar[i].value = (char) i;
    int8_t c;
    while ((c = getc(stdin)) != EOF)
        allChar[c].occur++;
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

Encoding* getCharEncoding(HTree allChar, Encoding* allCharE){
    // DFS    recursive    left (0) : int code << 1    right (1) : (int code << 1)+1
    ;
}
