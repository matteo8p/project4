#include <stdio.h>
#include <stdbool.h>
#include "sem.h"

void reader(); 
void writer(); 

struct semaphore *mutex; 
struct semaphore *wrt; 
int rc;

int R;                                        //Number of readers 
int W;                                        //Number of writers
int global_i = 0;                                       //Global variable i 

int main()
{
    mutex = (struct semaphore *)malloc(sizeof(struct semaphore)); 
    wrt = (struct semaphore *)malloc(sizeof(struct semaphore)); 

    initSem(mutex, 1); 
    initSem(wrt, 1); 
    rc = 0;  

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
    for(int i = 0; i < 2; i++)
    {
        P(mutex); 
        rc++; 
        if(rc == 1)
        {
            P(wrt); 
        }
        V(mutex); 

        //Read the gloabl variable i 

            if(i == 0)                              //Read first time 
            {
                printf("\n This is the %dth reader reading value i = %d for the first time", id, global_i); 
            }else                                   //Read second time 
            {
                printf("\n This is the %dth reader reading value i = %d for the second time", id, global_i); 
            }

        P(mutex); 
        rc--; 
        if(rc == 0)
        {
            V(wrt); 
        }
        V(mutex); 
    }
}

void writer(int id)
{
    P(wrt);

    printf("\n This is the %d th writer writing value i = %d \n", id, id);
    global_i = id; 
    printf("\n This is the %d th writer verifying value i = %d \n", id, global_i); 

    V(wrt); 
}

