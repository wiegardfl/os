#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <spawn.h>
#include <sys/wait.h>

pid_t pid;
int ret;
extern char** environ;

int main(int argc, char const *argv[])
{
    if (argc < 2){
        puts("No program to execute has been given");
    }
    else {
        puts(argv[0]);
        char* argv2[argc];

        memcpy(argv2, &argv[1], sizeof(char*) * (argc - 1));
        argv2[argc-1] = NULL;

        execvp(argv[1], argv2);

        posix_spawnp(&pid, argv[1], NULL, NULL, argv2, environ);
        
        // waitpid(pid, &ret, 0);
        wait(&ret);

        printf("Program %s terminated with exit code %d", argv[1], ret);
    }
    return 0;
}