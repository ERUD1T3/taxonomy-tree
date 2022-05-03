//Copyright to Josias Moukpe, 2019

#ifndef TNODELIST_H
#define TNODELIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 64

typedef unsigned int uint;

typedef struct tnode {
    char data[SIZE];
    struct tnode* parent;
    struct tnode* children[SIZE];
} TNode;



typedef struct {
    /*
    * Single Linked List struct
    */ 
   int size; //overall size of Single Linked List
   TNode* head; //pointer to first element in the list

} TNList;


//Methods to operate on list
TNList* initList(void); //initialize data member of the list
void _destroy(TNList* List); //_destroy a list and free all pointers
TNode* _traverse(TNList* List, const uint index);  //return the pointer to the tnode previous to the tnode at index 
void _insert(TNList* List,const int index,const char* data); //_insert a tnode with payload data at position index
char* _suppress(TNList* List,const int index);  //deletes a tnode at position index
void _pushfront(TNList* List,const char* data); //_insert tnode at the front of the list
void _pushback(TNList* List,const char* data); //_insert tnode at the end of the list
char* _popfront(TNList* List);  //delete tnode at the back of the list
char* _popback(TNList* List);  //delete tnode at the front of the list
void _printlist(TNList* List); //print all elements in the list
char* _getAt(TNList* List, const int index); //get the data at index 
char* itoa(int num, char* str, int base); //converts from integer to string
void reverse(char str[], int length); //utility function for itoa


char* _getAt(TNList* List, const int index) {
    return (_traverse(List, index)->data);
}

TNList* initList(void) {
    /*
    *   initialize size to 0 and head to NULL
    */
   TNList* List = (TNList*)malloc(sizeof(TNList));
   List->size = 0;
   List->head = NULL;
   return List;
}

void _destroy(TNList* List) {
    while(List->size != 0) _popback(List);
    free(List);
}

void _printlist(TNList* List) {
    /*
    * print all elements in the list
    */
   if(List == NULL || List->size == 0 ) {
       printf("empty list\n");
       return;
   }
   printf("[");
   TNode* tmp = List->head;

   if(List->size != 0) {
       while(tmp != NULL) {
           printf(" %s,", tmp->data);
           tmp = tmp->next;
       }
   }
   printf(" ]\n");
}

TNode* _traverse(TNList* List, const uint index) {
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
    TNode* tmp = List->head; 
    for(unsigned int i = 0; i < index; ++i) {//traversing till index - 1 
        //printf("tmp value: %d for i = %d\n ", tmp->data, i);    
        tmp = tmp->next; //moving to the next tnode
    }
    //printf("about to return\n");
    return tmp;
}

void _insert(TNList* List, int index,const char* data) {
    /*
    * _insert a tnode with data at index
    * O(n)
    */

    TNode* new_node = (TNode*)malloc(sizeof(TNode));
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
        TNode* prev = _traverse(List, index);
        new_node->next = prev->next;
        prev->next = new_node;
    }

    ++List->size;
}


void _pushfront(TNList* List,const char* data) {
    /*
    * _insert tnode at the front of the list
    */
   _insert(List, 0, data);
} 

void _pushback(TNList* List,const char* data) {
    /*
    * _insert tnode at the end of the list
    */
   if(List->size == 0) {
        TNode* new_node = (TNode*)malloc(sizeof(TNode));
        strcpy(new_node->data, data);
        new_node->next = NULL;
        List->head = new_node;
        ++List->size;
   }
   else if(List->size == 1) { //edge case for single tnode list
        TNode* new_node = (TNode*)malloc(sizeof(TNode));
        strcpy(new_node->data, data);
        new_node->next = List->head->next;
        List->head->next = new_node;
        ++List->size;

    } else {
        _insert(List, List->size - 1, data);
    }
}  


char* _suppress(TNList* List,const int index) {
    /*
    * _suppress() deletes tnode at index
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
    char* tmp = (char*)malloc(MAX_STR_SIZE*sizeof(char));
    
    if(index == 0) { //case 3: deleting head
       to_del = List->head;
       List->head = (List->head)->next;

    } else {  //case 4: deleting any tnode that is not head
        TNode* prev = _traverse(List, index - 1);
        to_del = prev->next;
        prev->next = to_del->next;

    }

    strcpy(tmp, to_del->data);
    free(to_del);
    --List->size;
    return tmp;
}

char* _popfront(TNList* List) {
    /*
    * delete tnode at the back of the list
    */
    return _suppress(List,0);
}  
char* _popback(TNList* List) {
    /*
    * delete tnode at the front of the list
    */
    return _suppress(List, List->size - 1);
}  

char* itoa(int num, char* str, int base) { 
    int i = 0; 
    bool isNegative = false; 
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return str; 
    } 
  
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10) 
    { 
        isNegative = true; 
        num = -num; 
    } 
  
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    // If number is negative, append '-' 
    if (isNegative) 
        str[i++] = '-'; 
  
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    reverse(str, i); 
  
    return str; 
} 

void reverse(char str[], int length) { 

    int start = 0; 
    int end = length -1; 
    while (start < end) 
    { 
        char tmp = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = tmp;
        start++; 
        end--; 
    } 
} 

#endif //TNODELIST_H