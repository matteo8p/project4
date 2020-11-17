#include "threads.h"

typedef struct semaphore {
	int value;	
	struct queue *semQ;	
} semaphore;

void initSem(semaphore*, int);
void P(semaphore*, int id);
void V(semaphore*);

void initSem(semaphore *sem, int value) {
	sem->semQ = (struct queue*) malloc(sizeof(struct queue));
	initQueue(sem->semQ);
	sem->value = value;
}

void P(semaphore *sem, int id) 
{
	while(1)
	{
		if(sem->value == 0)
		{
			if(id > 0)
			{
				printf("\n Producer %d is waiting \n", id);
			}else
			{
				printf("\n Consumer %d is waiting \n", -id);
			}
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