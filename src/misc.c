#include "../inc/huff.h"

void help(){
    printf("\n\t\t\t\x1b[1mHUFFMAN CODE \x1b[0m\n\n\n\
    This code allows you to compress and to decompress a text file by using the famous huffman code.\n\
    By default, without option, the program will compress and decompress your file.\n\n\
    \x1b[1mUSAGE\x1b[0m\n\n\
    \t./EXECUTABLE_PATH [OPTION] [FILE]\n\n\
    \x1b[1mLISTING OPTIONS\x1b[0m\n\n\
    \t\x1b[1m-c\x1b[0m\tCompress the text file.\n\
    \t\x1b[1m-d\x1b[0m\tDecompress the compressed file.\n\
    \t\x1b[1m-h\x1b[0m\tOutputs this help message.\n\
    \t\x1b[1m--help\x1b[0m\tSame as '-h'\n\n");
}

PriorityQueue push(PriorityQueue ptr, Tree pt){
    PriorityQueue elm = malloc(sizeof *elm);
    if (!elm){ 
        printf("The node couldn't be created, allocation failed");
        exit(QUEUE_ALLOCATION_FAILED);
    }
    *elm = (PriorityQueueElm) {.pt = pt, .next = NULL};
    if (!ptr) return elm;
    PriorityQueue tmp = ptr;
    while (tmp->next && tmp->next->pt->occur < pt->occur)
        tmp = tmp->next;
    
    PriorityQueue tmpB = tmp->next;
    tmp->next = elm;
    elm->next = tmpB;
    return ptr;
}

PriorityQueue pull(PriorityQueue ptr){
    if (!ptr || !ptr->next) return NULL;
    PriorityQueue tmp = ptr->next;
    //freeTree(ptr->pt);
    free(ptr);
    return tmp; 
}
