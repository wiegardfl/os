#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define size 10000000
int *A,*B,*C; 
#define reps 100
#define threads 4
pthread_t thread[threads];
int threadNum[threads];

void* add1stHalf(void* argv) {
    for (size_t i = 0; i < size / 2; i++){
        C[i] = A[i] + B[i];
    }
    puts("Thread 1 terminated");
    pthread_exit(EXIT_SUCCESS);
    return NULL;
}

void* add2ndHalf(void* argv) {
    for (size_t i = size / 2; i < size; i++){
        C[i] = A[i] + B[i];
    }
    puts("Thread 2 terminated");
    pthread_exit(EXIT_SUCCESS);
    return NULL;
}

void* addNthPartition(void* arg){
    int N =  *(int*)arg;
    for (size_t rep = 0; rep < reps; rep++)
    {
        for (size_t i = size * N / threads; i < size * (N+1) / threads; i++){
        C[i] = A[i] + B[i];
        }
    }    
    
    // printf("Thread %d terminated\n", N);
    pthread_exit(EXIT_SUCCESS);
    return NULL;
}

int main(void)
{
    A = (int*)malloc(size*sizeof(int));
    B = (int*)malloc(size*sizeof(int));
    C = (int*)malloc(size*sizeof(int));

    for (size_t i = 0; i < size; i++)
    {
        A[i] = i;
        B[i] = size - i;
    }

    int ret;

    for (int i = 0; i < threads; i++){
        threadNum[i] = i;
        ret = pthread_create(&thread[i], NULL, addNthPartition, (void*)(threadNum+i));
    }

    for (int i = 0; i < threads; i++){
        pthread_join(thread[i], NULL);
    }    

    return 0;
}
