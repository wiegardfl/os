/* Example: Synchronization problem
 *
 * For this example, let's assume we have a bank account. A global variable
 * 'balance' keeps track of the credit balance. The bank is not willing to
 * let our account balance to go negative. 
 *
 * Several threads try to spend money by chosing a random amount, checking
 * the balance, and subtracting the amount - if the account has funds!
 *
 * For comparison, first run this program several times with only one 
 * thread, and then try again several times with multiple threads (two 
 * threads are enough to finish with a negative balance eventually. More
 * threads will speed up the process).
 *
 * Compile: gcc -lpthread -o nosync nosync.c
 * Execute: ./nosync
 *
 * Sven Reissmann (sven.reissmann@rz.hs-fulda.de)
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

/* Configuration
 */
#define NUMBER_OF_THREADS 10
#define INITIAL_BALANCE 1000
#define MAX_PER_TRANSACTION 100


/* Global variable for keeping track of the accounts balance
 */
long int balance = INITIAL_BALANCE;
sem_t mutex;


/* The function executed by each thread
 */
void* tfunc (void* arg) {
    int spend;
    long int currentBalance;

    while (1) {
        /* Choose a random amount to spend
         */
        spend = rand() % MAX_PER_TRANSACTION + 1;


        sem_wait(&mutex);

        currentBalance = balance;

        /* If account has enough funds, spend the chosen amount
         */
        if (balance >= spend) {
            balance -= spend;

            sem_post(&mutex);

            printf("Thread %16ld: Balance = %6ld, Spend %4d, Result %6ld\n",
                    pthread_self(), currentBalance, spend, balance);
        /* Otherwise, exit the thread
         */
        } else {
            sem_post(&mutex);

            printf("Thread %16ld: Balance = %6ld, Spend %4d -> balance too low!\n",
                    pthread_self(), currentBalance, spend);

            pthread_exit ((void *) EXIT_SUCCESS);
        }
    }
}

int main (int argc, char **argv) {
    pthread_t threads[NUMBER_OF_THREADS];
    int ret[NUMBER_OF_THREADS];
    if(sem_init(&mutex, 0, 1) != 0){
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    /* Initialize the random number generator 
     * (Note: This is not a safe way to initialize the RNG!)
     */
    srand((unsigned int) time(NULL));

    /* Create NUMBER_OF_THREADS
     */
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        if ((ret[i] = pthread_create (&threads[i], NULL, &tfunc, NULL)) != 0) {
            perror ("pthread_join");
            exit (EXIT_FAILURE);
        }
    }

    /* Wait for the threads to end
     */
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        if ((ret[i] = pthread_join (threads[i], NULL)) != 0) {
            perror ("pthread_join");
            exit (EXIT_FAILURE);
        }
    }

    /* Print the final balance (should be > 0), and exit
     */
    printf ("All threads ended. Final balance: %ld\n", balance);

    /* Destroy semaphore */
    if(sem_destroy(&mutex) != 0){
        exit(EXIT_FAILURE);
    }

    exit (EXIT_SUCCESS);
}

