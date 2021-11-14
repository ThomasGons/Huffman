#include "../inc/huff.h"


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