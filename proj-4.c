#include <stdio.h>
#include <stdbool.h>
#include "sem.h"

void reader(); 
void writer(); 

// struct semaphore *mutex; 
struct semaphore *wrt; 
struct semaphore *rsem; 
struct semaphore *wsem; 
int rwc; int wwc; int rc; int wc; 

int R;                                        //Number of readers 
int W;                                        //Number of writers
int global_i = 0;                                       //Global variable i 

int main()
{
    wrt = (struct semaphore *)malloc(sizeof(struct semaphore)); 
    rsem = (struct semaphore *)malloc(sizeof(struct semaphore)); 
    wsem = (struct semaphore *)malloc(sizeof(struct semaphore)); 

    runQ = (struct queue*)malloc(sizeof(struct queue)); 

    initSem(wrt, 1); 
    initSem(rsem, 0); 
    initSem(wsem, 1); 

    rwc = wwc = rc = wc = 0; 

    scanf("%d,%d",&R,&W);                       //Scan for number of readers and writers 

    for(int i = 0; i < R + W; i++)              //Loop R + W times to scan for reader and writer IDs. 
    {
        int id; 
        scanf("%d", &id);

        if(id > 0)                              //Readers are positive 
        {
            // printf("Start Reader");
            startThread(reader, id); 
        }else                                   //Writers are negative 
        {
            // printf("Start writer"); 
            startThread(writer, id); 
        }
    }
    run();                                      //Init threads 
    return 0; 
}

void reader(int id)
{
    if(wwc > 0 || wc > 0)
    {
        rwc++; 
        P(rsem);
        rwc--; 
    }
    rc++; 

    printf("\n This is the %d th reader reading value i = %d for the first time \n", id, global_i); 
    yield(); 
    printf("\n This is the %d th reader reading value i = %d for the second time \n", id, global_i); 

    rc--;
    if(rc == 0 && wwc > 0)
    {
        V(wsem); 
    }
    
    TCB_t *tcb = delQueue(runQ); 
    if(runQ->headPointer == NULL) exit(0); 
    swapcontext(&(tcb->context), &(runQ->headPointer->context)); 
}

void writer(int id)
{
    if(rc > 0 || wc > 0)
    {
        wwc++; 
        P(wsem); 
        wwc--; 
    }
    wc++; 
    id = -id;                                                                //Convert negative id to positive id
    global_i = id; 

    printf("\n This is the %d th writer writing value i = %d \n", id, id);
    yield();
    printf("\n This is the %d th writer verifying value i = %d \n", id, global_i); 

    wc--; 

    if(rwc > 0)
    {
        V(rsem); 
    }

    TCB_t *tcb = delQueue(runQ); 
    if(runQ->headPointer == NULL) exit(0); 
    swapcontext(&(tcb->context), &(runQ->headPointer->context)); 
}

