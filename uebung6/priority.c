#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define NUMBER_OF_THREADS 6
#define NUMBER_OF_SEMAPHORES 8

sem_t sem_0,
    sem_1_2, sem_1_3,
    sem_2_4, sem_2_6,
    sem_3_4, sem_3_5,
    sem_5_6;
sem_t* sem[NUMBER_OF_SEMAPHORES] = { &sem_0, &sem_1_2, &sem_1_3, &sem_2_4, &sem_2_6, &sem_3_4, &sem_3_5, &sem_5_6 };

char* output;
pthread_mutex_t mutex;

void printThread() {
    printf("Thread %16ld done\n", pthread_self());    
}

void* P1() {
    puts("P1 waiting");
    // sem_wait(&sem_0);
    // pthread_mutex_lock(&mutex);
    // strcat(output, "P1 done\n");
    // pthread_mutex_unlock(&mutex);
    // sleep(1);
    sem_post(&sem_1_3);
    sem_post(&sem_1_2);
    // pthread_mutex_lock(&mutex);
    // strcat(output, "P1 signalled\n");
    // pthread_mutex_unlock(&mutex);
    puts("P1 signalled");
    exit(EXIT_SUCCESS);
}

void* P2() {
    puts("P2 waiting");
    sem_wait(&sem_1_2);
    puts("P2 done");
    sem_post(&sem_2_4);
    sem_post(&sem_2_6);
    exit(EXIT_SUCCESS);
}

void* P3() {
    puts("P3 waiting");
    sem_wait(&sem_1_3);
    puts("P3 done");
    sem_post(&sem_3_4);
    sem_post(&sem_3_5);
    exit(EXIT_SUCCESS);
}

void* P4() {
    puts("P4 waiting");
    sem_wait(&sem_2_4);
    sem_wait(&sem_3_4);
    puts("P4 done");
    exit(EXIT_SUCCESS);
}

void* P5() {
    puts("P5 waiting");
    sem_wait(&sem_3_5);
    puts("P5 done");
    sem_post(&sem_5_6);
    exit(EXIT_SUCCESS);
}

void* P6() {
    puts("P6 waiting");
    sem_wait(&sem_2_6);
    sem_wait(&sem_5_6);
    puts("P6 done");
    exit(EXIT_SUCCESS);
}

void* (*func[NUMBER_OF_THREADS]) (void*) = { P1, P2, P3, P4, P5, P6};

int main (int argc, char **argv) {
    pthread_t threads[NUMBER_OF_THREADS];
    int ret[NUMBER_OF_THREADS];

    output = (char*)malloc(1024);
    pthread_mutex_init(&mutex, NULL);

    // Init NUMBER_OF_SEMAPHORES
    for (size_t i = 0; i < NUMBER_OF_SEMAPHORES; i++)
    {
        if(sem_init(sem[i], 0, 0) != 0){
            perror("sem_init");
            exit(EXIT_FAILURE);
        }
    }
    pthread_create(&threads[0], NULL, P1, NULL);
    pthread_create(&threads[1], NULL, P2, NULL);
    pthread_create(&threads[2], NULL, P3, NULL);
    pthread_create(&threads[3], NULL, P4, NULL);
    pthread_create(&threads[4], NULL, P5, NULL);
    pthread_create(&threads[5], NULL, P6, NULL);
    
    /* Create NUMBER_OF_THREADS
     */
    // for (int i = 0; i < NUMBER_OF_THREADS; i++) {
    //     if ((ret[i] = pthread_create (&threads[i], NULL, func[i], NULL)) != 0) {
    //         perror("pthread_join");
    //         exit(EXIT_FAILURE);
    //     }
    // }

    // sem_post(&sem_0);

    /* Wait for the threads to end
     */
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(threads[i], NULL);         
    }

    puts(output);

    exit(EXIT_SUCCESS);
}
