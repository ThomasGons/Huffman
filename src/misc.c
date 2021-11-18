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

Data* insertionSort(Data *array){
    Data tmp = {0, 0};
    for (int i = 1, test = 0, j; i < CHAR_MAX; i++){
        test++;
        j = i;
        while (j > 0 && array[j - 1].occur > array[j].occur){
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
    *elm = (Queue) {.pTree = t, .next = NULL};
    if (!ptr) return elm;
    PtrQ tmp = ptr;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = elm;
    return ptr;
}

PtrQ dequeue(PtrQ ptr){
    if (!ptr || !ptr->next) return NULL;
    PtrQ tmp = ptr->next;
    ptr->next = NULL;
    free(ptr);
    return tmp; 
}


//Process data
void Tree_Process_Data(HTree tree, int level)
{
    for (int i = 0; i < level; i++)
        printf(i == level-1 ? "└──" : "   "); //|\u2014\u2014
    printf("%ld: %c\n", tree->occur, tree->value);
}

//Preorder depth-first search (DFS) and process data.
void DFS_Preorder(HTree root, int level)
{
    if (root) {
        Tree_Process_Data(root, level);
        DFS_Preorder(root->left, level+1);
        DFS_Preorder(root->right, level+1);
    }
}