#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define NO_OF_PHILOSOPHERS 5
#define THINKING_STATE 2
#define HUNGRY_STATE 1
#define EATING_STATE 0
int state_phil[NO_OF_PHILOSOPHERS];
int state_fork[NO_OF_PHILOSOPHERS];
int phil[NO_OF_PHILOSOPHERS] = { 0, 1, 2, 3, 4 };
sem_t bowl;
sem_t mutex_take_fork;//1
sem_t mutex_put_fork;//2
sem_t S[NO_OF_PHILOSOPHERS];
#define LEFT_PHILOSOPHER (p_index + 4) % NO_OF_PHILOSOPHERS
#define RIGHT_PHILOSOPHER (p_index + 1) % NO_OF_PHILOSOPHERS
#define FREE 0
#define USE 1

int try_to_take_fork(int p_index)
{
	if (state_phil[p_index] == HUNGRY_STATE){
        if(state_fork[p_index] == FREE && state_fork[LEFT_PHILOSOPHER] == FREE){
            sem_wait(&bowl);
            int a;
            sem_getvalue(&bowl, &a);
            printf("Philosopher %d takes a bowl (left: %d) \n", p_index + 1, a);
            int t = rand() % 2;
            int x;
            if(t == 0){
                x = p_index;
            }
            else{
                x = LEFT_PHILOSOPHER;
            }
            state_phil[x] = EATING_STATE;
            state_fork[x] = USE;
            sleep(2);
            printf("Philosopher %d takes fork %d\n",p_index + 1, x + 1);
            printf("Philosopher %d is Eating\n", p_index + 1);
            sem_post(&S[p_index]);
            return x;
        }
        else if(state_fork[p_index] == FREE){
            sem_wait(&bowl);
            state_phil[p_index] = EATING_STATE;
            state_fork[p_index] = USE;
            sleep(2);
            printf("Philosopher %d takes fork %d\n",p_index + 1,  p_index + 1);
            printf("Philosopher %d is Eating\n", p_index + 1);
            sem_post(&S[p_index]);
            return p_index;
        } 
        else if (state_fork[LEFT_PHILOSOPHER] == FREE) {
            sem_wait(&bowl);
            state_phil[p_index] = EATING_STATE;
            state_fork[p_index] = USE;
            sleep(2);
            printf("Philosopher %d takes fork %d \n",p_index + 1, LEFT_PHILOSOPHER + 1);
            printf("Philosopher %d is Eating\n", p_index + 1);
            sem_post(&S[p_index]);
            return LEFT_PHILOSOPHER;
	    }
    }

}

int take_fork(int p_index)
{
	sem_wait(&mutex_take_fork);
	state_phil[p_index] = HUNGRY_STATE;
	printf("Philosopher %d is Hungry\n", p_index + 1);
    sleep(1);
	int res = try_to_take_fork(p_index);
	sem_post(&mutex_take_fork);
	sem_wait(&S[p_index]);
	sleep(1);
    return res;

}

void put_fork(int p_index, int x)
{
	sem_wait(&mutex_put_fork);
    sem_post(&bowl);
	state_phil[p_index] = THINKING_STATE;
    state_fork[x] = FREE;
	printf("Philosopher %d is thinking\n", p_index + 1);
    try_to_take_fork(x);
	//try_to_take_fork(LEFT_PHILOSOPHER);
	//try_to_take_fork(RIGHT_PHILOSOPHER);
	sem_post(&mutex_put_fork);
}

void* philosopher(void* arg)
{
	while (1) {
		int* i_ptr = (int *)arg;
		sleep(1);
		int result = take_fork(*i_ptr);
		sleep(0);
		put_fork(*i_ptr, result);
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
        state_fork[i] = FREE;    
    }
	for (i = 0; i < NO_OF_PHILOSOPHERS; i++) {
		pthread_create(&thread_id[i], NULL,philosopher, &phil[i]);
		printf("Philosopher %d is thinking\n", i + 1);
	}

	for (i = 0; i < NO_OF_PHILOSOPHERS; i++){
		pthread_join(thread_id[i], NULL);
    }
}
