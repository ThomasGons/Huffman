#include "../inc/huff.h"


void help(){
    printf(
    "\n\tHUFFMAN CODE \x1b[0m\n\n\
    This code allows you to compress and to decompress a text file by using the famous huffman code.\n\
    By default, without option, the program will compress and decompress your file.\n\n\n\
    \t\x1b[1mUSAGE\x1b[0m\n\n\
    ./EXECUTABLE [OPTION] [FILE]\n\n\
    \t\x1b[1mLISTING OPTIONS\x1b[0m\n\n\
    \t-c\tCompress the text file.\n\
    \t-d\tDecompress the compressed file.\n\
    \t-h\tOutputs this help message.\n\
    \t--help\tSame as '-h'\n\n");
}

Data* insertionSort(Data *array){
    Data tmp = {0, 0};
    for (int i = 1, test = 0, j; i < CHAR_MAX; i++){
        test++;
        j = i;
        while (j > 0 && array[j - 1].occur < array[j].occur){
            test++;
            tmp = array[j];
            array[j] = array[j - 1];
            array[j - 1] = tmp;
            j--;
        }
    }
    return array;
}

PtrQ enqueue(PtrQ ptr, HTree t){
    PtrQ elm = malloc(sizeof * elm);
    if (!elm){ 
        printf("The node couldn't be created, allocation failed");
        exit(-3);
    }
    *elm = (Queue) {.pTree = t, .next = ptr};
    return elm;
}

PtrQ dequeue(PtrQ ptr){
    if (!ptr || !ptr->next) return NULL;
    PtrQ tmp = ptr, tmpB = NULL;
    while(tmp->next->next)
        tmp = tmp->next;
    tmpB = tmp->next;
    tmp->next = NULL;
    free(tmpB);
    return ptr; 
}