#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <string.h>

#define MAX_THINK 2000000
#define NUM_PHIL 5
enum {THINKING, EATING, HUNGRY} status[NUM_PHIL];
sem_t phil[NUM_PHIL], mutex;
pthread_t thread[NUM_PHIL];

double elapsed(struct timeval start, struct timeval stop){
    return (stop.tv_sec - start.tv_sec) * 1000.0 + (stop.tv_usec - start.tv_usec) / 1000.0;
}

void printStatus(){
    int s, accu = 9;
    for (size_t i = 0; i < NUM_PHIL; i++)
    {
        accu = 10*accu + status[i];
    }
    printf("%6d\n", accu);
}

double sleepRand() {
    struct timeval start, stop;
    /* Start the timer */
    gettimeofday(&start, NULL);
    /* Sleep for random amount of microseconds */
    usleep(rand() % MAX_THINK + 1001);
    /* Stop the timer */
    gettimeofday(&stop, NULL);
    /* Calculate the elapsed time and print it */
    return elapsed(start, stop);
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

void test_fork_available(int i) {   
    if (status[i] == HUNGRY && status[left(i)] != EATING && status[right(i)] != EATING) {
        status[i] = EATING;
        sem_post(&phil[i]);
    }
    else {
        printf("Philosopher %d is HUNGRY but cannot start EATING\n", i);
    }
}

void grab(int i) {    
    struct timeval start, stop;
    sem_wait(&mutex);
        status[i] = HUNGRY;
        test_fork_available(i);
    sem_post(&mutex);

    /* Start the timer */
    gettimeofday(&start, NULL);
    /* Wait for both forks to be available */
    sem_wait(&phil[i]);
    /* Stop the timer */
    gettimeofday(&stop, NULL);
    /* Calculate the elapsed time and print it */
    printf("Philosopher %d began EATING after being HUNGRY for %0.f ms\n", i, elapsed(start, stop));
}

void drop(int i) {
    sem_wait(&mutex);
        status[i] = THINKING;
        test_fork_available(left(i));
        test_fork_available(right(i));
    sem_post(&mutex);
}

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

    sem_init(&mutex, 0, 1);

    for (size_t i = 0; i < NUM_PHIL; i++)
    {
        sem_init(&phil[i], 0, 0);
        pthread_create(&thread[i], NULL, philosopher, NULL + i);
    }    

    // while (1) {
    //     usleep(MAX_THINK / 10);
    //     printStatus();
    // }

    for (size_t i = 0; i < NUM_PHIL; i++){
        pthread_join(thread[i], NULL);
    }  

    return 0;
}
