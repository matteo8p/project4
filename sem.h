#include "threads.h"

typedef struct semaphore {
	int value;	
	struct queue *semQ;	
} semaphore;

void initSem(semaphore*, int);
void P(semaphore*);
void V(semaphore*);

void initSem(semaphore *sem, int value) {
	sem->semQ = (struct queue*) malloc(sizeof(struct queue));
	initQueue(sem->semQ);
	sem->value = value;
}

void P(semaphore *sem) 
{
	while(1)
	{
		if(sem->value == 0)
		{
			struct TCB_t *tcb = delQueue(runQ);
			addQueue(sem->semQ, tcb);
			if(runQ->headPointer == NULL)
			{
				exit(0); 
			}
			swapcontext(&(sem->semQ->headPointer->prev->context), &(runQ->headPointer->context));
		}else
		{
			sem->value--; 
			return; 
		}
	}
}

void V(semaphore *sem) 
{
	sem->value++;
	if(sem->semQ->headPointer != NULL)
	{
		struct TCB_t *tcb = delQueue(sem->semQ);
		addQueue(runQ, tcb);
	}
	yield(); 
}