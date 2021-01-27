#ifndef EventSync_H
#define EventSync_H
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

struct Event{
    bool flag; //true = unlocked, false = locked
    pthread_mutex_t mtx;
    pthread_cond_t condition;
};

int EventOpen(bool* flag, pthread_mutex_t  mtx, pthread_cond_t  condition);
void EventSignal(bool* flag, pthread_mutex_t mtx, pthread_cond_t condition);
void EventWait(bool* flag, pthread_mutex_t  mtx, pthread_cond_t condition);
void EventClose(bool* flag, pthread_mutex_t  mtx, pthread_cond_t condition);

#endif //EventSync_H

