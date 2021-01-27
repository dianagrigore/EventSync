#include "EventSyncLib.h"
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
int  EventOpen(bool* flag, pthread_mutex_t  mtx, pthread_cond_t condition)
{
    if(pthread_mutex_init(&mtx, NULL))
    {
        perror("Eroare initializare mutex");
        return errno;
    }
    if(pthread_cond_init(&condition, NULL))
    {
        perror("Eroare initializare condition");
    }
    *flag = true;
    printf("Event opened\n");
    return 0;
}

void  EventSignal(bool* flag, pthread_mutex_t mtx, pthread_cond_t condition)
{

    pthread_mutex_lock(&mtx);

    if(*flag)
       abort(); //eroare daca e inchis?
  
    *flag = true;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mtx);
    printf("Event was signaled\n");
}


void EventWait(bool* flag, pthread_mutex_t mtx, pthread_cond_t condition)
{
    pthread_mutex_lock(&mtx);
    while(!*flag) //not if to avoid spurious wakeups
       pthread_cond_wait(&condition,&mtx);
    *flag = false;
    pthread_mutex_unlock(&mtx);
    printf("Waiting\n");
}

void EventClose(bool* flag, pthread_mutex_t mtx, pthread_cond_t condition)
{
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&condition);
    *flag = false;
    printf("Close\n");
}


