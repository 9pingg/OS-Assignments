#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#include <stdlib.h>

#define NO_OF_PHILOSOPHERS 5
#define THINKING_STATE 2

int state_phil[NO_OF_PHILOSOPHERS];
int phil[NO_OF_PHILOSOPHERS] = { 0, 1, 2, 3, 4 };

sem_t mutex_take_fork;
sem_t mutex_put_fork;

sem_t S[NO_OF_PHILOSOPHERS];
sem_t bowl;

#define HUNGRY_STATE 1
#define EATING_STATE 0
#define LEFT (p_index + 4) % NO_OF_PHILOSOPHERS
#define RIGHT (p_index + 1) % NO_OF_PHILOSOPHERS

void try_to_take_fork(int p_index)
{
	if (state_phil[p_index] == HUNGRY_STATE && state_phil[LEFT] != EATING_STATE && state_phil[RIGHT] != EATING_STATE) {
        sem_wait(&bowl);
        int a;
        sem_getvalue(&bowl, &a);
        printf("Philosopher %d takes a bowl (left: %d) \n", p_index + 1, a);
		state_phil[p_index] = EATING_STATE;
		sleep(2);
		printf("Philosopher %d takes fork %d and %d\n",p_index + 1, LEFT + 1, p_index + 1);
		printf("Philosopher %d is Eating\n", p_index + 1);
		sem_post(&S[p_index]);
	}
}

void take_fork(int p_index)
{
	sem_wait(&mutex_take_fork);
	state_phil[p_index] = HUNGRY_STATE;
	printf("Philosopher %d is Hungry\n", p_index + 1);
	try_to_take_fork(p_index);
	sem_post(&mutex_take_fork);
	sem_wait(&S[p_index]);
	sleep(1);
}

void put_fork(int p_index)
{
	sem_wait(&mutex_put_fork);
    sem_post(&bowl);
	state_phil[p_index] = THINKING_STATE;
	printf("Philosopher %d putting fork %d and %d down\n",p_index + 1, LEFT + 1, p_index + 1);
	printf("Philosopher %d is in thinking state\n", p_index + 1);
	try_to_take_fork(LEFT);
	try_to_take_fork(RIGHT);
	sem_post(&mutex_put_fork);
}

void* philosopher(void* num)
{
	while (1) {
		int* i = num;
		sleep(1);
		take_fork(*i);
		sleep(0);
		put_fork(*i);
	}
}

int main()
{
	int i;
	pthread_t thread_id[NO_OF_PHILOSOPHERS];
	sem_init(&mutex_take_fork, 0, 1);
    sem_init(&mutex_put_fork, 0, 1);
    sem_init(&bowl, 0, 4);
	for (i = 0; i < NO_OF_PHILOSOPHERS; i++){
		sem_init(&S[i], 0, 0);
	}
	for (i = 0; i < NO_OF_PHILOSOPHERS; i++) {
		pthread_create(&thread_id[i], NULL,philosopher, &phil[i]);
		printf("Philosopher %d is in thinking state\n", i + 1);
	}

	for (i = 0; i < NO_OF_PHILOSOPHERS; i++)
		pthread_join(thread_id[i], NULL);
}
// If both the forks are taken before, than taking the bowls then no deadlock occurs, but if bowls are taken first
// then deadlock can occur.
// here bowls are taken first.