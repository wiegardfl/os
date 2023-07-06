#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#define MIN_THINK 1000
#define MAX_THINK 2000000
#define NUM_PHIL 5
enum { THINKING, HUNGRY, STARVING, EATING } status[NUM_PHIL];
sem_t phil[NUM_PHIL], mutex;
pthread_t thread[NUM_PHIL];

/* Return the elapsed time in ms between 2 timevals*/
double elapsed(struct timeval start, struct timeval stop){
    return (stop.tv_sec - start.tv_sec) * 1000.0 + (stop.tv_usec - start.tv_usec) / 1000.0;
}

/* Sleep for a random amount of time between MIN_THINK and (roughly) MAX_THINK */
double sleepRand() {
    usleep(rand() % MAX_THINK + MIN_THINK);
}

void think(int id) {
    sleepRand();
}

void eat(int id) {
    sleepRand();
    printf("Philosopher %d finished EATING and started THINKING again\n", id);
}

int left(int i) {
    return (i+NUM_PHIL-1) % NUM_PHIL;
}

int right(int i) {
    return (i+1) % NUM_PHIL;
}

void test_fork_available(int id) {   
    if ((status[id] == HUNGRY && status[left(id)] < STARVING && status[right(id)] < STARVING)
    || status[id] == STARVING) {
        status[id] = EATING;
        sem_post(&phil[id]);
    }
    else if (status[id] == HUNGRY) {
        int starving_id = -1;
        if (status[left(id)] == STARVING) {
            starving_id = left(id);
        }
        else if (status[right(id)] == STARVING){
            starving_id = right(id);
        }

        if (starving_id >= 0) {
            printf("Philosopher %d is HUNGRY but yields to STARVING philosopher %d\n", id, starving_id);
        }
        else printf("Philosopher %d is HUNGRY but cannot start EATING\n", id);
    }
}

void prevent_starvation(int id) {
    if (status[id] == HUNGRY && (status[left(id)] == EATING || status[right(id)] == EATING)){
        status[id] = STARVING;
        printf("Philosopher %d is STARVING\n", id);
    }
}

void grab(int id) {    
    sem_wait(&mutex);
        status[id] = HUNGRY;
        test_fork_available(id);
    sem_post(&mutex);

    struct timeval start, stop;
    /* Start the timer */
    gettimeofday(&start, NULL);
    /* Wait for both forks to be available */
    sem_wait(&phil[id]);
    /* Stop the timer */
    gettimeofday(&stop, NULL);
    /* Calculate the elapsed time and print it */
    printf("Philosopher %d began EATING after being HUNGRY for %0.f ms\n", id, elapsed(start, stop));
}

void drop(int id) {
    sem_wait(&mutex);
        status[id] = THINKING;
        test_fork_available(left(id));
        test_fork_available(right(id));
        prevent_starvation(left(id));
        prevent_starvation(right(id));
    sem_post(&mutex);
}

/* Passing id as offset to void* NULL*/
void* philosopher(void* arg) {
    int id = arg - NULL;
    printf("Philosopher %d took a seat and started THINKING\n", id);
    while(1) {
        think(id);
        grab(id);
        eat(id);
        drop(id);
    }
}

int main(int argc, char const *argv[])
{
     /* Initialize the random number generator 
     * (Note: This is not a safe way to initialize the RNG!)
     */
    srand((unsigned int) time(NULL));

    /* Initialize the mutex semaphore*/
    sem_init(&mutex, 0, 1);

    /* Initialize the philosopher semaphores with 0 because sem_post needs to be called before sem_wait 
     * Create a pthread for each philosopher and pass their id as offset to void* NULL
     */
    for (size_t i = 0; i < NUM_PHIL; i++)
    {
        sem_init(&phil[i], 0, 0);
        pthread_create(&thread[i], NULL, philosopher, NULL + i);
    }    

    /* Wait for the threads to end to keep the process alive. (The threads here aren't self-terminating)*/
    for (size_t i = 0; i < NUM_PHIL; i++){
        pthread_join(thread[i], NULL);
    }  

    return 0;
}
