#include <unistd.h>
#include <stdio.h>
#include <string.h>

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
    }
    return 0;
}
