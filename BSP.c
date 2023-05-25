#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "BSP.h"
#include "Wall.h"
#include "List.h"

// constructor
BSPTree* BSP_Create(ListPtr data, void* (*GetSubtreeRoot)(ListPtr data), ListPtr (*GetSubtrees)(void* parent, ListPtr remaining), int (*DataCompare)(void* data1, void* data2), void (*DataPrinter)(void* data), void (*FreeData)(void *data)){
	BSPTree *tree = (BSPTree*)malloc(sizeof(BSPTree));
    tree->GetSubtreeRoot = GetSubtreeRoot;
    tree->GetSubtrees = GetSubtrees;
    tree->DataCompare = DataCompare;
    tree->DataPrinter = DataPrinter;
    tree->FreeData = FreeData;

    tree->root = BuildSubtree(tree, data, NULL);

    return tree;
}

TreeNode* TreeNode_Create(void* data){ // constructor
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->data = data;

    return node;
}

// base case is parent == NULL
// contains 2 steps from animation:
// 1. Select middle segment using the function GetSubtreeRoot (SelectPartitionRoot)
// 2. Partition elements based on the root element using the function GetSubtrees (GetPartitions)
TreeNode* BuildSubtree(BSPTree* tree, ListPtr remaining, TreeNode* parent) {
    if(list_length(remaining) == 0) {
        return NULL;
    }

    segment* partition = (segment*)tree->GetSubtreeRoot(remaining); // calls SelectPartitionRoot and sets partition to floor(numwalls/2)
    TreeNode* root = TreeNode_Create(partition);
    root->parent = parent;
    ListPtr left = tree->GetSubtrees(root->data, remaining); // left list and right list (remaining) are now split
    root->left = BuildSubtree(tree, left, root->left);
    root->right = BuildSubtree(tree, remaining, root->right);
    return root;
}

void BSP_print(BSPTree* tree){
    ListPtr printer = BSP_GetTraversal(tree);
    list_print(printer);
}	

// gets the preorder traversal of the tree (use PreOrderTraverse)
// for the list created in BSP_GetTraversal, we call list_create with tree functions as parameters? so like list_create(tree->DataCompare) Or is there something else i gotta do to the function pointers before passing it into list_create?
ListPtr BSP_GetTraversal(BSPTree* tree){
    ListPtr list = list_create(tree->DataCompare, tree->DataPrinter, tree->FreeData);
    PreOrderTraverse(list, tree->root);
    return list;
}

void PreOrderTraverse(ListPtr list, TreeNode* node){ // recursive function, implement steps in slide 32
    if(node != NULL) {
        // add current node to list, which will contain all nodes traversed through at the end of recursion
        list_append(list, node->data);
        // recurse into left child
        PreOrderTraverse(list, node->left);
        PreOrderTraverse(list, node->right);
    }
    else {
        return;
    }
}


void DestroyTree(BSPTree* tree, bool freeData) {
    ListPtr deletion = BSP_GetTraversal(tree);
    if(freeData) {
        list_destroy(deletion, true);
    }
    else {
        list_destroy(deletion, false);
    }
}