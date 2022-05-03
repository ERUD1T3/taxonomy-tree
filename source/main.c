/*

  Author: Josias Moukpe
  Email: jmoukpe2016@my.fit.edu
  Course: cse2010
  Section: 14
  Description: Based on input from a file, the program builds a taxonomic tree
  in alphabetic order and run queries from a file on the taxonomic tree.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "TTree.h"  //for TTree and functions
#include "Querying.h" //for processQuery()

int main(int argc, char** argv)
{

  // pointer the input data and query files
  FILE* fdata = fopen(argv[1], "r");  // datafile name passed as first argument
  FILE* fquery = fopen(argv[2], "r"); // queryfile name passed as second argument


  // exits with failure if program fails to open data file
  if(fdata == NULL) {
    printf("\nError: failed to open data file.\n");
    exit(EXIT_FAILURE);
  }

  // exits with failure if program fails to open query file
  if(fquery == NULL) {
    printf("\nError: failed to open query file.\n");
    exit(EXIT_FAILURE);
  }

  size_t len = 0; // keeps track of the length of the lines
  char* input_line = NULL;  // the input line charracter array  
  TTree* taxonomy = initTTree(); // initializing a empty taxonomic tree

  // parseWords(input_line)
  while(getline(&input_line, &len, fdata) != EOF) 
    buildTTree(taxonomy, parseWords(input_line));
  
  // printTTree(taxonomy); // to print tree in preorder
  // printf("tree size: %d\n", taxonomy->size); //to print tree's size
  // printf("\n");
  
  // Reading query file and executing the queries
  while(getline(&input_line, &len, fquery) != EOF) {
    TNList* querylist = parseWords(input_line);
    printlist(querylist);
    processQuery(taxonomy, querylist);
    printf("\n");
  }

  // garbage collection and clean up closing the files
  destroyTTree(taxonomy);
  fclose(fdata);
  fclose(fquery);
  
  return EXIT_SUCCESS;
}


