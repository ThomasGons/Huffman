#include "../inc/huff.h"

int leavesTree(Tree pt){
    if (!pt->left && !pt->right) return 1;
    else return 0;
    return leavesTree(pt->left) + leavesTree(pt->right);
}

Tree createTree(size_t n, char c){
    Tree new = malloc(sizeof *new);
    if (!new){
        printf("The tree couldn't be created, allocation failed");
        exit(TREE_ALLOCATION_FAILED);
    }
    *new = (TreeElm) {.occur = n, .value = c, .left = NULL, .right = NULL};
    return new;
}

Tree createOverTree(Tree pt1, Tree pt2){
    Tree overTree = createTree(pt1->occur + pt2->occur, '\0');  // perhaps a bad idea
    if (pt1->occur > pt2->occur){
        overTree->right = pt1; overTree->left = pt2;
    }
    else{
        overTree->right = pt2; overTree->left = pt1;
    }
    return overTree; 
}

void freeTree(Tree pt){
    if (pt){
        freeTree(pt->left);
        freeTree(pt->right); 
        free(pt);
    }
}

void vDisplay(Tree pt, int dpt){
    if (pt){
        printf("|");
        for (int i = 0; i < dpt; i++)
            printf("\u2014");
        printf(" %ld: %c\n", pt->occur, pt->value);
        // preorder traversal
        vDisplay(pt->left, dpt + 1);
        vDisplay(pt->right, dpt + 1);
    }
}