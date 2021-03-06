#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "SSU_Sem.h"

#define NUM_THREADS 3
#define NUM_ITER 10

SSU_Sem lock[NUM_THREADS];

void *justprint(void *data)
{
	int thread_id = *((int *)data);
	
	for (int i = 0; i < NUM_ITER; i++) {
		SSU_Sem_down(&lock[thread_id]);
		printf("This is thread %d\n", thread_id);
		
		if (thread_id == NUM_THREADS - 1)
			SSU_Sem_up(&lock[0]);
		else
			SSU_Sem_up(&lock[thread_id + 1]);
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	pthread_t mythreads[NUM_THREADS];
	int mythread_id[NUM_THREADS];
	
	for (int i = 0; i < NUM_THREADS; i++) {
		SSU_Sem_init(&lock[i], 0);
	}
	SSU_Sem_up(&lock[0]);
	
  	for(int i = 0; i < NUM_THREADS; i++) {
		mythread_id[i] = i;
		pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
		usleep(1000);
	}
  
  	for(int i =0; i < NUM_THREADS; i++) {
		pthread_join(mythreads[i], NULL);
	}

	return 0;
}
