#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define size 100
int A[size], B[size], C[size];

void* add1stHalf(void* argv) {
    for (int i = 0; i < size / 2; i++){
        C[i] = A[i] + B[i];
    }
    pthread_exit(EXIT_SUCCESS);
    return NULL;
}

void* add2ndHalf(void* argv) {
    for (int i = size / 2; i < size; i++){
        C[i] = A[i] + B[i];
    }
    pthread_exit(EXIT_SUCCESS);
    return NULL;
}

int main(void)
{
    for (int i = 0; i < size; i++)
    {
        A[i] = i;
        B[i] = size - i;
    }

    pthread_t thread1, thread2;
    int ret1, ret2;

    ret1 = pthread_create(&thread1, NULL, &add1stHalf, NULL);
    ret2 = pthread_create(&thread2, NULL, &add2ndHalf, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
