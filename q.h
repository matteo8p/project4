#include <stdlib.h>
#include <unistd.h>
#include "tcb.h"

typedef struct queue {
	struct TCB_t *headPointer; // Pointer to 1st Element in Queue
} queue;

void initQueue(struct queue*);	
void addQueue(struct queue*, struct TCB_t*);	
void rotateQ(struct queue*);	
struct TCB_t* delQueue(struct queue*);	
struct TCB_t* newItem();	

void initQueue(struct queue *head) {
	head->headPointer = NULL;
}

void addQueue(struct queue *head, struct TCB_t *item) {
   TCB_t *pointer = head->headPointer; 

   if(pointer == NULL)                                //If head is empty (NULL)   
   {
      head->headPointer = item;                                   //set head to item. Point to itself. 
      head->headPointer->next = head->headPointer; 
      head->headPointer->prev = head->headPointer;                       
   }else                                              //If head node exists 
   {
      pointer = head->headPointer->prev;                        //Pointer is the last element in queue 
      item->next = head->headPointer;  
      item->prev = pointer; 
      pointer->next = item; 
      head->headPointer->prev = item;                     
   }
}

void rotQueue(struct queue *head) {
	addQueue(head, delQueue(head));
}

struct TCB_t* delQueue(struct queue *head) {
   TCB_t *delq = head->headPointer; 
   if(head->headPointer == NULL || head->headPointer->next == head->headPointer)
   {
      head->headPointer = NULL; 
      return delq; 
   }else
   {
      TCB_t* lastNode = head->headPointer->prev; 
      head->headPointer= head->headPointer->next; 
      lastNode->next = head->headPointer; 
      head->headPointer->prev = lastNode; 
   }
   return delq; 
}

struct TCB_t* newItem() {
	struct TCB_t *item = (struct TCB_t*) malloc(sizeof(struct TCB_t));
	return item;
}