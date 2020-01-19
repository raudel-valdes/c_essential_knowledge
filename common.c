#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>



typedef struct Node{
  char *word;
  int count;
  struct Node *next;
  struct Node *prev;
} Node;

typedef struct List{
  Node *head;
  Node *tail;
} List;

void insertNodeAtTail(List *, char **);
void printList(List *, int);
void sortList(List *);
void swapAdjNodes(List **, Node **, Node **);
void destroyList(List *);
// void insertNodeAtHead();

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Please include the two input text file names and the output file name at execution time! \n");
    exit(EXIT_FAILURE);
  }

  List firstFileList;
  List secondFileList;

  firstFileList.head = NULL;
  firstFileList.tail = NULL;

  secondFileList.head = NULL;
  secondFileList.tail = NULL;

  FILE *fPtr1;
  FILE *fPtr2;
  // FILE *fPtr3;
  char *scannedWord = NULL;
  
  fPtr1 = fopen(argv[1], "r");

  if (fPtr1 == NULL) {
    printf("The file %s was not found or could not be open. Please try again!", argv[1]);
    exit(EXIT_FAILURE);
  }

  fPtr2 = fopen(argv[2], "r");

  if (fPtr2 == NULL) {
    printf("The file %s was not found or could not be open. Please try again!", argv[2]);
    exit(EXIT_FAILURE);
  }

  while(fscanf(fPtr1,"%ms", &scannedWord) != EOF) {
    insertNodeAtTail(&firstFileList, &scannedWord);
  }

  while(fscanf(fPtr2,"%ms", &scannedWord) != EOF) {
    insertNodeAtTail(&secondFileList, &scannedWord);
  }
  
  sortList(&firstFileList);
  //printList(&firstFileList, 0);

  destroyList(&firstFileList);
  destroyList(&secondFileList);


  fclose(fPtr1);
  fclose(fPtr2);
  //fclose(fPtr3);

  return 0;
}

void insertNodeAtTail(List *firstFileList, char **scannedWord) {

  char * wordToInsert = strndup(*scannedWord, strlen(*scannedWord));
  Node *nextTailNode = malloc(sizeof(Node));

  nextTailNode->word = wordToInsert;
  nextTailNode->count = 1;

  Node *currentNode = firstFileList->head;
  // Node *currentHeadNode = firstFileList->head;
  Node *currentTailNode = firstFileList->tail;

  if (currentNode == NULL) {

    nextTailNode->prev = NULL;
    nextTailNode->next = NULL;

    firstFileList->head = nextTailNode;
    firstFileList->tail = nextTailNode;

  } else {
 
    while(currentNode != NULL) {
      
      if (strcmp(currentNode->word, wordToInsert) == 0) {

        currentNode->count++;

        free(nextTailNode->word);
        free(nextTailNode);

        break;
        
      } else 
        currentNode = currentNode->next;

      if (currentNode == NULL) {

        nextTailNode->prev = currentTailNode;
        nextTailNode->next = NULL;
        currentTailNode->next = nextTailNode;
        firstFileList->tail = nextTailNode;

      }

    }
  
  }

  free(*scannedWord);
}

void printList(List *list, int reverse) {

  Node *currentNode = NULL;

  if (!reverse) {
    currentNode = list->head;
    
    while (currentNode != NULL) {

      printf("WORD: %s \n COUNT: %d \n\n", currentNode->word, currentNode->count);

      currentNode = currentNode->next;
    }

  } else {
    
    currentNode = list->tail;
    
    while (currentNode != NULL) {

      printf("WORD: %s \n COUNT: %d \n\n", currentNode->word, currentNode->count);

      currentNode = currentNode->prev;
    }

  }

}

void swapAdjNodes(List **unsortedList, Node **nodeOne, Node **nodeTwo) {

  Node *tempNode = NULL;

  tempNode = (*nodeOne)->prev;

  if(tempNode != NULL) {
    tempNode->next = (*nodeTwo);
    (*nodeTwo)->prev = tempNode;
    (*nodeOne)->next = (*nodeTwo)->next;
    (*nodeTwo)->next = (*nodeOne);
  } else {
    (*nodeTwo)->prev = tempNode;
    (*nodeOne)->next = (*nodeTwo)->next;
    (*nodeTwo)->next = (*nodeOne);
    (*unsortedList)->head = (*nodeTwo);
  }

  tempNode = (*nodeOne)->next;

  if(tempNode != NULL) {
    tempNode->prev = (*nodeOne);
    (*nodeOne)->prev = (*nodeTwo);
  } else {
    (*nodeOne)->prev = (*nodeTwo);
    (*unsortedList)->tail = (*nodeOne);
  }

}

void sortList(List *unsortedList) {

  Node *marker = NULL;
  Node *markerPrev = NULL;
  Node *compareNode = NULL;
  Node *originalSwap = NULL;

  markerPrev = unsortedList->head;
  marker = unsortedList->head->next;  

  while(marker != NULL && markerPrev != NULL) {

    if (strcmp(markerPrev->word, marker->word) < 0) {
      marker = marker->next;
      markerPrev = markerPrev->next;

    } else { 

      swapAdjNodes(&unsortedList, &markerPrev, &marker);

      originalSwap = marker;
      marker = markerPrev->next;
      compareNode = originalSwap->prev;
      
      while(compareNode != NULL && originalSwap != NULL && (strcmp(compareNode->word, originalSwap->word) > 0)) {
       
        swapAdjNodes(&unsortedList, &compareNode, &originalSwap);
        compareNode = originalSwap->prev;
      }

      if (marker != NULL)
        markerPrev = marker->prev;
    }
  }

}

void destroyList(List *listToDestroy) {
  Node *nodeToDestroy = NULL;
  Node *tempNode = NULL;

  nodeToDestroy = listToDestroy->head;
    
    while (nodeToDestroy != NULL) {
      tempNode = nodeToDestroy->next;

      free(nodeToDestroy->word);
      free(nodeToDestroy);

      nodeToDestroy = tempNode;
    }
}