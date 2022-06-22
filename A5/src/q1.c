#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define NO_OF_PHILOSOPHERS 5
#define LEFT_PHILOSOPHER (p_index + 4) % NO_OF_PHILOSOPHERS
#define RIGHT_PHILOSOPHER (p_index + 1) % NO_OF_PHILOSOPHERS

int state_phil[NO_OF_PHILOSOPHERS];
int phil_index[NO_OF_PHILOSOPHERS];

sem_t mutex_take_fork;
sem_t S[NO_OF_PHILOSOPHERS];

#define THINKING_STATE 2
#define HUNGRY_STATE 1
#define EATING_STATE 0

void try_to_take_fork(int p_index)
{
	if (state_phil[p_index] == HUNGRY_STATE) {
		
		if(state_phil[LEFT_PHILOSOPHER] != EATING_STATE && state_phil[RIGHT_PHILOSOPHER] != EATING_STATE){
			state_phil[p_index] = EATING_STATE;
			sleep(2);
			printf("Philosopher %d takes fork %d and %d\n",p_index + 1, LEFT_PHILOSOPHER + 1, p_index + 1);
			printf("Philosopher %d is in Eating state\n", p_index + 1);
			sem_post(&S[p_index]);
		}
	}
}
void take_fork(int p_index)
{
	sem_wait(&mutex_take_fork);
	state_phil[p_index] = HUNGRY_STATE;
	printf("Philosopher %d is in Hungry state\n", p_index + 1);
	try_to_take_fork(p_index);
	sem_post(&mutex_take_fork);
	sem_wait(&S[p_index]);
	sleep(1);
}

void put_fork(int p_index)
{
	sem_wait(&mutex_take_fork);
	state_phil[p_index] = THINKING_STATE;
	printf("Philosopher %d putting fork %d and %d down\n",p_index + 1, LEFT_PHILOSOPHER + 1, p_index + 1);
	printf("Philosopher %d is in Thinking state\n", p_index + 1);
	try_to_take_fork(LEFT_PHILOSOPHER);
	try_to_take_fork(RIGHT_PHILOSOPHER);
	sem_post(&mutex_take_fork);
}

void* philosopher(void* arg)
{
	while (1) {
		int* i_ptr = (int *)arg;
		sleep(1);
		take_fork(*i_ptr);
		sleep(0);
		put_fork(*i_ptr);
	}
}

int main(void){
	int i;
	pthread_t thread_id[NO_OF_PHILOSOPHERS];
	sem_init(&mutex_take_fork, 0, 1);
	for (i = 0; i < NO_OF_PHILOSOPHERS; i++){
		sem_init(&S[i], 0, 0);
		phil_index[i] = i;
	}
	for (i = 0; i < NO_OF_PHILOSOPHERS; i++) {
		pthread_create(&thread_id[i], NULL, philosopher, &phil_index[i]);
		printf("Philosopher %d is in Thinking state\n", i + 1);
	}

	for (i = 0; i < NO_OF_PHILOSOPHERS; i++)

		pthread_join(thread_id[i], NULL);
}
