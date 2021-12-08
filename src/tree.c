#include "../inc/huff.h"

Tree createTree(Data dict){
    Tree new = malloc(sizeof *new);
    if (!new){
        fprintf(stderr, "The tree couldn't be created, allocation failed");
        exit(TREE_ALLOCATION_FAILED);
    }
    *new = (TreeElm) {.data = dict, .left = NULL, .right = NULL};
    return new;
}

Tree createOverTree(Tree pt1, Tree pt2){
    // Allow to make a tree with two given trees "as branches" by respecting BST rule
    Tree overTree = createTree(DATA_SUM(pt1->data, pt2->data)); 
    if (pt1->data.occur > pt2->data.occur){
        overTree->right = pt1; overTree->left = pt2;
    }
    else{
        overTree->right = pt2; overTree->left = pt1;
    }
    return overTree; 
}

void freeTree(Tree pt){
    // post-order traversal
    if (pt){
        freeTree(pt->left);
        freeTree(pt->right); 
        free(pt);
    }
}