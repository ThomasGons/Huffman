#include "../inc/huff.h"

void help(){
    // An execution help displayable with './bin/exe -h' or with a file but the file will be not proccesed
    printf("\n\t\t\t\x1b[1mHUFFMAN CODE \x1b[0m\n\n\n\
    This code allows you to compress and to decompress all text file by using the famous huffman code.\n\
    By default, without option, the program will compress and decompress your file.\n\n\
    \x1b[1mUSAGE\x1b[0m\n\n\
    \t./EXECUTABLE_PATH [OPTION] [FILE]\n\n\
    \x1b[1mLISTING OPTIONS\x1b[0m\n\n\
    \t\x1b[1m-c\x1b[0m\tCompress the text file.\n\
    \t\x1b[1m-d\x1b[0m\tDecompress the compressed file.\n\
    \t\x1b[1m-dr\x1b[0m\tDecompress the compressed file and delete the compressed.\n\
    \t\x1b[1m-h\x1b[0m\tOutputs this help message.\n\
    \t\x1b[1m--help\x1b[0m\tSame as '-h'.\n\n\n\
    \x1b[3mWarn: This message cancel any file processing.\x1b[0m\n\n");
}

PriorityQueue push(PriorityQueue ptr, Tree pt){
    PriorityQueue elm = malloc(sizeof *elm);
    if (!elm){
        fprintf(stderr, "The node couldn't be created, allocation failed");
        exit(QUEUE_ALLOCATION_FAILED);
    }
    *elm = (PriorityQueueElm) {.pt = pt, .next = NULL};
    // If there is not element, the one that has just been created will be the first element
    if (!ptr) return elm;
    PriorityQueue tmp = ptr;
    // We move to the next node until the value is less than that of the node
    while (tmp->next && tmp->next->pt->data.occur < pt->data.occur)
        tmp = tmp->next;
    // Put the created element at the good place by linking to the previous and the next
    PriorityQueue tmpB = tmp->next;
    tmp->next = elm;
    elm->next = tmpB;
    return ptr;
}

PriorityQueue pull(PriorityQueue ptr){
    // Place the second element of the queue at the head and free the first
    if (!ptr) return NULL;
    PriorityQueue tmp = ptr->next;
    free(ptr);
    ptr = NULL;
    return tmp; 
}

void removeExt(char* file){
    /* Permanent removal of the extension
    Remove the dot if it exists and replace it with null char. */
    char *dot = strchr(file, '.');
    if (dot) dot[0] = '\0';
}
