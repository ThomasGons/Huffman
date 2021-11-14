#include "../inc/huff.h"

int leavesTree(HTree pt){
    if (!pt->left && !pt->right) return 1;
    else return 0;
    return leavesTree(pt->left) + leavesTree(pt->right);
}

HTree createTree(size_t n, char c){
    HTree new = malloc(sizeof *new);
    if (!new){
        printf("The tree couldn't be created, allocation failed");
        exit(-2);
    }
    *new = (Tree) {.occur = n, .value = c, .left = NULL, .right = NULL};
    return new;
}

HTree createSubHTree(HTree t_1, HTree t_2){
    HTree sht = createTree(t_1->occur + t_2->occur, '0');  // perhaps a bad idea
    if (t_1->occur > t_2->occur){
        sht->right = t_1; sht->left = t_2;
    }
    else{
        sht->right = t_2; sht->left = t_1;
    }
    return sht; 
}


void vDisplay(HTree pt, int dpt){
    if (pt){
        printf("\x1b[38;2;%d;0;0m \u01c0", 255 - dpt * 13); // if the depth times DEPTHMAX does not exceed 255
        for (int i = 0; i < dpt; i++)
            printf("\u2014");
        printf(" %ld\n ", pt->occur); printf("\x1b[1m");
        // preorder traversal
        vDisplay(pt->left, dpt + 1);
        vDisplay(pt->right, dpt + 1);
    }
}