#ifndef TTREE_H
#define TTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "TNList.h" //for TNode and TNList

typedef struct ttree TTree;

struct ttree {
    uint size; //total number of nodes in the tree
    struct tnode* root; //pointer to root node of the tree
};

/****************************************
 * METHODS PROTOTYPES
 ***************************************/ 

TTree* initTTree(); // initialize a Taxonomic tree
TNList* getChildren(TNode* parent); // return pointer to the list of children 
TNode* getParent(TNode* child); // return pointer to parent of child
TNode* getChild(TNList* children,  uint index); // get a child pointer from its parent list of children
TNode* searchNode(TTree* taxonomy,  char* data); // search the tree for node containing data
TNode* preOrderSearch(TNode* root, char* key); // utilized preorder to search a tree for a key value
void destroyTTree(TTree* to_del); // clear memory of Taxonomic tree
void addChild(TNode* parent,  char* child); // add child to parent's children list
void insertNode(TTree* tree, TNode* node, TNode* parent); // inserts a new node into tree
void printTTree(TTree* taxonomy); // print the tree using preorder
void buildTTree(TTree* taxonomy, TNList* data_list); // insert a new node in Tree
void insertChild(char* child_data, TNode* parent); // insert child into its parent list
void preOrder(TNode* root, uint* counter); //run preorder on the list to either count or print
void recDestroy(TNode* root); //recursively destroy all nodes in tree

/*******************************************
 * TTree Methods
 ******************************************/

void recDestroy(TNode* root) {
    /*
    *   recursively destroying nodes in postorder from root
    */ 
    for(uint i = 0; i < root->children->size; ++i) 
        recDestroy(getChild(root->children, i));

    if(root == NULL) return;
    destroyTNode(root);   
}

void preOrder(TNode* root, uint* counter) {
    /*
    * Traverse the subtree in preorder from the root
    * if counter is NULL, preOrder is in printing mode
    * else preOrder is in counting mode 
    * to count all node in subtree from root
    */
    if(root == NULL) return;
     
    if(counter != NULL) (*counter)++; //preOrder is in counting mode
    else printf("%s ", root->data); //preOrder is in printing mode

    for(uint i = 0; i < root->children->size; ++i)
        preOrder(getChild(root->children, i), counter);
}

TNode* preOrderSearch(TNode* root, char* key) {
    /*
    * search the tree for a value.
    */ 
    if(!strcmp(root->data, key)) return root;

    for(uint i = 0; i < root->children->size; ++i) {
        TNode* tmp = preOrderSearch(getChild(root->children, i), key);
        if(tmp != NULL) return tmp;
        //else if(!strcmp(tmp->data, key)) return tmp;
        //else return tmp;
    }
    return NULL;
}

TNode* searchNode(TTree* taxonomy, char* data) {
    /*
    * Search the tree for data node and return the pointer to the data node
    */ 
    return preOrderSearch(taxonomy->root, data);
}

void buildTTree(TTree* taxonomy, TNList* data_list) {

    /*
    * build a tree with the data_list
    * parent data is getAt(data_list, 0)
    * children data is getAt(data_list, 1 to data_list->size -1)
    */ 
    if (taxonomy->root == NULL) {

        taxonomy->root = initTNode(getAt(data_list, 0), NULL, initTNList());
        ++taxonomy->size;
        for(uint i = 1; i < data_list->size; ++i) { //adding children
            insertChild(getAt(data_list, i), taxonomy->root);
            ++taxonomy->size;
        }
    } 
    else { //inserting non -root position

        TNode* parent = searchNode(taxonomy, getAt(data_list, 0));
        for(uint i = 1; i < data_list->size; ++i) { //adding children
            insertChild(getAt(data_list, i), parent);
            ++taxonomy->size;
        }
    }
    
    destroyTNList(data_list); //clear space created by datalist
}


TNode* getParent(TNode* child) {
    /*
    * Return a pointer to the parent node
    */ 
   return child->parent;
}

TNList* getChildren(TNode* parent) {
    /*
    * Return a pointer to the children list of a node
    */ 
   return parent->children;
}

TNode* getChild(TNList* children,  uint index) {
    /*
    * Returns a pointer to a specific child in the list of Children
    */
    return (traverse(children, index));
}

void addChild(TNode* parent,  char* child_data) {
    /*
    * Adds a child node to parent list of children
    */
    insertChild(child_data, parent);
}

void printTTree(TTree* taxonomy) {
    /*
    * calles preOrder to print the tree
    */ 
    preOrder(taxonomy->root, NULL);
    printf("\n");
}

TTree* initTTree() {
    /*
        Initialize a Taxonomic tree and return its pointer
    */
   TTree* new_tree = (TTree*)malloc(sizeof(TTree));
   new_tree->root = NULL;
   return new_tree;

}

void destroyTTree(TTree* to_del) {
    /*
    * Traverses the tree to delete all its nodes Clears the memory of the tree
    */
    recDestroy(to_del->root);
    free(to_del);
}

void insertChild(char* child_data, TNode* parent) {
    /*
    Insert node at right position
    */  
    TNode* child = initTNode(child_data, parent, initTNList());

    if(parent->children == NULL) {
        printf("Cannot insert in null list");
        return;
    }

    if(parent->children->size == 0) { //case 1: empty children
        parent->children->head = child;
        child->next = NULL;
    } 
    else 
    if(strcmp(parent->children->head->data, child->data) > 0) { //case 2: front _insertion
        child->next = parent->children->head;
        parent->children->head = child;
    } 
    else { //case 3: Non empty children with insert at middle
        TNode* prev = parent->children->head;
        while(prev->next != NULL && (strcmp(prev->next->data, child->data) < 0)) prev = prev->next;
        child->next = prev->next;
        prev->next = child;
    }

    ++(parent->children->size);
}


#endif //TTREE_H