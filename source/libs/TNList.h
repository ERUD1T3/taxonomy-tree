#ifndef TNLIST_H
#define TNLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 32 //size of data stored in each node

typedef unsigned int uint; //unsigned integer alias
typedef struct tnode TNode; // node alias
typedef struct tnlist TNList; // list alias

struct tnode {
    char data[SIZE]; // data carried by node
    struct tnode *parent; // pointer to parent node
    struct tnode *next; // pointer to next
    struct tnlist* children; // list of children
};

struct tnlist {
    uint size; // size of the list
    struct tnode* head; // pointer to parent
};


/****************************************
 * METHODS PROTOTYPES
 ***************************************/ 

// NODE METHODS
TNode* initTNode(char* data, TNode* parent, TNList* children);
void destroyTNode(TNode* to_del); // clear memory of Taxonomic node

//LIST METHODS
TNList* initTNList(); // initialize a list of TNodes
TNList* parseWords(char* line); //parse the input into TNList
TNode* traverse(TNList* List,  uint index);  //return the pointer to the node previous to the node at index 
void destroyTNList(TNList* to_del); //clear memory of Taxonomic node
void insert(TNList* List, uint index, char* data); //_insert a node with payload data at position index
void pushfront(TNList* List, char* data); //_insert node at the front of the list
void pushback(TNList* List, char* data); //_insert node at the end of the list
void printlist(TNList* List); //print all elements in the list
char* suppress(TNList* List, uint index);  //deletes a node at position index
char* popfront(TNList* List);  //delete node at the back of the list
char* popback(TNList* List);  //delete node at the front of the list
char* getAt(TNList* List,  uint index); //get the data at index 


/****************************************
 * TNode Methods
 ***************************************/ 

TNode* initTNode(char* data, TNode* parent, TNList* children) {
    /*
    * Creates a new node for the tree
    */ 
    TNode* new_node = (TNode*)malloc(sizeof(TNode));
    strcpy(new_node->data, data);
    new_node->parent = parent;
    new_node->children = children;
    new_node->next = NULL;
    return new_node;
}

void destroyTNode(TNode* to_del) {
    /*
    * Destroy to_del node 
    */ 
   free(to_del);
}


/*******************************************
 * TNList Methods
 ******************************************/

char* getAt(TNList* List,  uint index) {
    return (traverse(List, index)->data);
}

TNList* initTNList(void) {
    /*
    *   initialize size to 0 and head to NULL
    */
   TNList* List = (TNList*)malloc(sizeof(TNList));
   List->size = 0;
   List->head = NULL;
   return List;
}

void destroyTNList(TNList* List) {
    while(List->size != 0) popback(List);
    free(List);
}

void printlist(TNList* List) {
    /*
    * print all elements in the list
    */
    if(List == NULL || List->size == 0 ) {
        printf("empty list\n");
        return;
    }

    TNode* tmp = List->head;

    if(List->size != 0) {
        while(tmp != NULL) {
            printf("%s ", tmp->data);
            tmp = tmp->next;
        }
    }
}

TNode* traverse(TNList* List,  uint index) {
    /*
    * give an index N, _traverse() _traverse the list until N and return pointer to N-1
    * O(n)
    */

    // case 1: empty list
    if(List->size == 0) {
        printf("empty list!\n"); 
        return NULL;
    }
    // case 2: index is not present
    if(index < 0 || index >= List->size) {
        printf("Invalid index!\n");
        return NULL;
    }

    // case 3: index present
    TNode* tmp = List->head; 
    for(uint i = 0; i < index; ++i) tmp = tmp->next; 

    return tmp;
}

void insert(TNList* List,  uint index, char* data) {
    /*
    * _insert a node with data at index
    * O(n)
    */

    TNode* new_node = (TNode*)malloc(sizeof(TNode));
    strcpy(new_node->data,data);

    
    if(List->size == 0) { //case 1: empty list
        List->head = new_node;
        new_node->next = NULL;
    } 
    else 
    if(index == 0) { //case 2: front _insertion
        new_node->next = List->head;
        List->head = new_node;
    } 
    else { //case 3: Non empty list with _insert at middle
        TNode* prev = traverse(List, index);
        new_node->next = prev->next;
        prev->next = new_node;
    }

    ++List->size;
}

void pushfront(TNList* List, char* data) {
    /*
    * _insert node at the front of the list
    */
   insert(List, 0, data);
} 

void pushback(TNList* List, char* data) {
    /*
    * _insert node at the end of the list
    */
    if(List->size == 0) {
        TNode* new_node = (TNode*)malloc(sizeof(TNode));
        strcpy(new_node->data, data);
        new_node->next = NULL;
        List->head = new_node;
        ++List->size;
    }
    else 
    if(List->size == 1) { //edge case for single node list
        TNode* new_node = (TNode*)malloc(sizeof(TNode));
        strcpy(new_node->data, data);
        new_node->next = List->head->next;
        List->head->next = new_node;
        ++List->size;

    } else insert(List, List->size - 1, data);
}  

char* suppress(TNList* List, uint index) {
    /*
    * _suppress() deletes node at index
    * O(n)
    */ 

    //case 1: empty list
    if(List->size == 0) {
        printf("empty list!\n");
        return NULL;
    }
    //case 2: invalid index
    if(index < 0 || index >= List->size) {
        printf("invalid index");
        return NULL;
    }

    TNode* to_del;
    char* tmp = (char*)malloc(SIZE*sizeof(char));
    
    if(index == 0) { //case 3: deleting head
       to_del = List->head;
       List->head = (List->head)->next;

    } 
    else {  //case 4: deleting any node that is not head
        TNode* prev = traverse(List, index - 1);
        to_del = prev->next;
        prev->next = to_del->next;

    }

    strcpy(tmp, to_del->data);
    free(to_del);
    --List->size;
    return tmp;
}

char* popfront(TNList* List) {
    /*
    * delete node at the back of the list
    */
    return suppress(List,0);
}  

char* popback(TNList* List) {
    /*
    * delete node at the front of the list
    */
    return suppress(List, List->size - 1);
}  

TNList* parseWords(char* line) {
    /*
    * Parses the input line for relevant commands
    */ 
   TNList* tmp = initTNList(); //command created with be used by then freed by parseCmd()

    char* word_token;
    char* delim = " \r\n";

    //get the first token 
    word_token = strtok(line, delim);

    //walk through other tokens 
    while(word_token != NULL) {
        //add new words to end of the list
        pushback(tmp, word_token); 
        word_token = strtok(NULL, delim);
    }
    return tmp; //return pointer to list containing commands
} 


#endif //TNLIST_H