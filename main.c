#include "EventSyncLib.h"
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

struct event{
	bool flag;
	pthread_mutex_t mtx;
	pthread_cond_t condition;
};


sem_t full, empty;

struct event* mutex;

void* consumer(void* p)
{
	int* num = (int*) p;
       do{
		sem_wait(&full);
		EventWait(&mutex -> flag, mutex -> mtx, mutex->condition);
		if(*num > 0)
		{
			printf("Remove  1\n");
			(*num)--;	
		}

		if(*num == 0)
		{

			EventSignal(&mutex ->flag, mutex -> mtx, mutex->condition);
			sem_post(&empty);
			return 0;

		}
		EventSignal(&mutex->flag, mutex->mtx, mutex->condition);
		sem_post(&empty);
	}while(true);


}

void* producer(void * p)
{
	int* num = (int*) p;
	do
{
	sem_wait(&empty);
	EventWait(&mutex->flag, mutex->mtx, mutex->condition);

	if(*num > 0)
	{
		printf("Add  1\n");
		(*num)--;
	}
	if(*num == 0)
	{

		EventSignal(&mutex->flag, mutex->mtx, mutex->condition);
		sem_post(&full);



		return 0;
	}
	EventSignal(&mutex->flag, mutex->mtx, mutex->condition);

sem_post(&full);

}while(true);
}

int main(int argc, char*argv[])
{
	int size, Producers, Consumers;
	scanf("%d", &size);
	scanf("%d", &Producers);
	scanf("%d", &Consumers);
	int produce[Producers], consume[Consumers];
	int ps = 0; int cs = 0;
	for(int i = 0; i < Producers; i++)
{
	scanf("%d", &produce[i]);
	ps = ps + produce[i];
}
	for(int i = 0; i < Consumers; i++)
{
	scanf("%d", &consume[i]);
	cs = cs + consume[i];
}
	if(cs > ps)
	{
		printf("Consum mai mult decat produc.\n");
	}
	if(ps-cs > size)
	{
		printf("Buffer prea mic\n");
	}


	 mutex = (struct event*)malloc(sizeof(struct event));
	EventOpen(&mutex -> flag, mutex -> mtx, mutex -> condition);
	sem_init(&full, 0,size);
	sem_init(&empty, 0, 0);
	pthread_t producid[Producers];
	pthread_t consumeid[Consumers];



	for(int i = 0; i < Producers; i++)
		{
			int* arg = (int*)malloc(sizeof(int));
			*arg = produce[i];
			if(pthread_create(&producid[i], NULL, producer, (void*)(arg)))
			{
				perror(NULL);
				return errno;
			}
		}
	for(int i = 0; i < Consumers; i++)
		{
			int* arg = (int*)malloc(sizeof(int));
			*arg = consume[i];
			if(pthread_create(&consumeid[i], NULL, consumer,(void*) (arg)))
			{
				perror(NULL);
				return errno;
			}
			}
	
	for(int i = 0; i < Producers; i++)
		{
			if(pthread_join(producid[i],NULL))
			{
				perror(NULL);
				return errno;
			}
		}
	for(int i = 0; i < Consumers; i++)
		{
			if(pthread_join(consumeid[i], NULL))
			{
				perror(NULL);

				return errno;
			}
		}
		
	EventClose(&mutex ->flag, mutex -> mtx, mutex -> condition);
	return 0;
	
}
