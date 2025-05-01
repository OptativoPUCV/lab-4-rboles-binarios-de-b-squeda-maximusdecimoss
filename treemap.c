#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}


TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap * map = (TreeMap *) malloc(sizeof(TreeMap));
    if (map == NULL) return NULL;

    map -> root = NULL;
    map -> current = NULL;
    map -> lower_than = lower_than;

    return map;

}


void insertTreeMap(TreeMap* tree, void* key, void* value) {
    if (searchTreeMap(tree, key)) return;

    TreeNode *parent = NULL, *current = tree->root;

    while (current) {
        parent = current;
        current = tree->lower_than(key, current->pair->key) ? current->left : current->right;
    }

    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    Pair* newPair = (Pair*)malloc(sizeof(Pair));

    newPair->key = key;
    newPair->value = value;

    newNode->pair = newPair;
    newNode->left = newNode->right = NULL;
    newNode->parent = parent;

    if (!parent) tree->root = newNode;
    else if (tree->lower_than(key, parent->pair->key)) parent->left = newNode;
    else parent->right = newNode;

    tree->current = newNode;
}


TreeNode * minimum(TreeNode * x) {
    if (x == NULL) return NULL;

    while (x->left != NULL) {
        x = x->left;
    }

    return x;
}



void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair* searchTreeMap(TreeMap* tree, void* key) {
    TreeNode* current = tree->root;

    while (current != NULL) {
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            current = current->right;
        } else {
            tree->current = current;  
            return current->pair;     
        }
    }

    return NULL;  
}



Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
