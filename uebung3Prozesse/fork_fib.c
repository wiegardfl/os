/* Exercise 3.2a: fork()
 *
 * Compile: gcc -o fork fork.c
 * Execute: ./fork
 *
 * Sven Reissmann (sven.reissmann@rz.hs-fulda.de)
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

#define CHILDS 5
#define FIB_LOOPS 5
#define FIB_ARG 32

u_int32_t f(u_int32_t n) {
    if (n <= 1) return 1;
    else return f(n-1) + f(n-2);
}

int main (void) {
    int pid, ret;

    /* Fork
     */
    int i = 0;

    do {
        pid = fork();
    } while (pid > 0 && ++i < CHILDS);

    /* If fork failed, just exit
     */
    if (pid == -1)
        exit (EXIT_FAILURE);

    /* This is the child (return of fork == 0)
     * (say hello and exit)
     */
    if (pid == 0) {
        for (size_t i = 0; i < FIB_LOOPS; i++)
        {
            f(FIB_ARG);
            printf("Kind %6d Durchlauf %lu\n", getpid(), i + 1);
        }        
        exit (EXIT_SUCCESS);
    }

    /* This is the parent (return of fork > 0)
     * (say hello, wait for child, and exit)
     */
    if (pid > 0) {
        printf("Parent (PID = %6d, PPID = %6d)\n", getpid (), getppid ());

        /* Calling wait() blocks the parent until the child exited
         */
        for (size_t i = 0; i < CHILDS; i++)
        {
            wait(&ret);
        }
        
        printf ("All children exited with %d\n", ret);

        exit (EXIT_SUCCESS);
    }
}
