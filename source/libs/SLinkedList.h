//Copyright to Josias Moukpe, 2019

#ifndef SLINKEDLIST_H
#define SLINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STR_SIZE 256

typedef unsigned int uint;

typedef struct node {
    /*
    * Node for single linked list structure
    */
    char data[MAX_STR_SIZE]; //data contained in node
    //char* data;
    struct node* next; //pointer to next node
} Node;

typedef struct {
    /*
    * Single Linked List struct
    */ 
   uint size; //overall size of Single Linked List
   Node* head; //pointer to first element in the list

} SLList;


//Methods to operate on list
SLList* _initList(void); //initialize data member of the list
void _destroy(SLList* List); //_destroy a list and free all pointers
Node* _traverse(SLList* List, const uint index);  //return the pointer to the node previous to the node at index 
void _insert(SLList* List,const int index,const char* data); //_insert a node with payload data at position index
char* _suppress(SLList* List,const int index);  //deletes a node at position index
void _pushfront(SLList* List,const char* data); //_insert node at the front of the list
void _pushback(SLList* List,const char* data); //_insert node at the end of the list
char* _popfront(SLList* List);  //delete node at the back of the list
char* _popback(SLList* List);  //delete node at the front of the list
void _printlist(SLList* List); //print all elements in the list
char* _getAt(SLList* List, const int index); //get the data at index 
SLList* parseWords(char* line); //parse words in line into nodes in list 


char* _getAt(SLList* List, const int index) {
    return (_traverse(List, index)->data);
}

SLList* _initList(void) {
    /*
    *   initialize size to 0 and head to NULL
    */
   SLList* List = (SLList*)malloc(sizeof(SLList));
   List->size = 0;
   List->head = NULL;
   return List;
}

void _destroy(SLList* List) {
    while(List->size != 0) _popback(List);
    free(List);
}

void _printlist(SLList* List) {
    /*
    * print all elements in the list
    */
   if(List == NULL || List->size == 0 ) {
       printf("empty list\n");
       return;
   }
   
   printf("[");
   Node* tmp = List->head;

   if(List->size != 0) {
       while(tmp != NULL) {
           printf(" %s,", tmp->data);
           tmp = tmp->next;
       }
   }
   printf(" ]\n");
}

Node* _traverse(SLList* List, const uint index) {
    /*
    * give an index N, _traverse() _traverse the list until N and return pointer to N-1
    * O(n)
    */
   
    //case 1: empty list
    if(List->size == 0) {
        printf("empty list!\n"); 
        return NULL;
    }

    //case 2: index is not present
    if(index < 0 || index >= List->size) {
        printf("Invalid index!\n");
        return NULL;
    }

    //case 3: index present
    Node* tmp = List->head; 
    for(unsigned int i = 0; i < index; ++i) {//traversing till index - 1 
        //printf("tmp value: %d for i = %d\n ", tmp->data, i);    
        tmp = tmp->next; //moving to the next node
    }
    //printf("about to return\n");
    return tmp;
}

void _insert(SLList* List, int index,const char* data) {
    /*
    * _insert a node with data at index
    * O(n)
    */

    Node* new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->data,data);

    
    if(List->size == 0) { //case 1: empty list
        List->head = new_node;
        new_node->next = NULL;
    } 
    else if(index == 0) { //case 2: front _insertion
        new_node->next = List->head;
        List->head = new_node;
    } 
    else { //case 3: Non empty list with _insert at middle
        Node* prev = _traverse(List, index);
        new_node->next = prev->next;
        prev->next = new_node;
    }

    ++List->size;
}


void _pushfront(SLList* List,const char* data) {
    /*
    * _insert node at the front of the list
    */
   _insert(List, 0, data);
} 

void _pushback(SLList* List,const char* data) {
    /*
    * _insert node at the end of the list
    */
   if(List->size == 0) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        strcpy(new_node->data, data);
        new_node->next = NULL;
        List->head = new_node;
        ++List->size;
   }
   else if(List->size == 1) { //edge case for single node list
        Node* new_node = (Node*)malloc(sizeof(Node));
        strcpy(new_node->data, data);
        new_node->next = List->head->next;
        List->head->next = new_node;
        ++List->size;

    } else {
        _insert(List, List->size - 1, data);
    }
}  


char* _suppress(SLList* List,const int index) {
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

    Node* to_del;
    char* tmp = (char*)malloc(MAX_STR_SIZE*sizeof(char));
    
    if(index == 0) { //case 3: deleting head
       to_del = List->head;
       List->head = (List->head)->next;

    } else {  //case 4: deleting any node that is not head
        Node* prev = _traverse(List, index - 1);
        to_del = prev->next;
        prev->next = to_del->next;

    }

    strcpy(tmp, to_del->data);
    free(to_del);
    --List->size;
    return tmp;
}

char* _popfront(SLList* List) {
    /*
    * delete node at the back of the list
    */
    return _suppress(List,0);
}  
char* _popback(SLList* List) {
    /*
    * delete node at the front of the list
    */
    return _suppress(List, List->size - 1);
}   

SLList* parseWords(char* line) {
    /*
    * Parses the input line for relevant commands
    */ 
   /*
    SLList* tmp = _initList(); //command created with be used by then freed by parseCmd()

    char* word_token;
    char* delim = " \n";

    //get the first token 
    word_token = strtok(line, delim);

    //walk through other tokens 
    while(word_token != NULL) {
        //add new words to end of the list
        _pushback(tmp, word_token); 
        word_token = strtok(NULL, delim);
    }

    return tmp; //return pointer to list containing commands
    */
   SLList* tmp = _initList(); //command created with be used by then freed by parseCmd()

    char* word_token;
    char* delim = " \n";

    //get the first token 
    word_token = strtok(line, delim);

    //walk through other tokens 
    while(word_token != NULL) {
        //add new words to end of the list
        _pushback(tmp, word_token); 
        word_token = strtok(NULL, delim);
    }

    return tmp; //return pointer to list containing commands
} 

#endif //SLINKEDLIST_H