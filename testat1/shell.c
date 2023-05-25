#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>

char *user, *name;
char cwd[PATH_MAX];
size_t line_size = MAX_INPUT;
char* line;

// function intended as signal handler
static void exit_hint(int signo) {
    puts("\nEnter exit to exit this program!");
}

int main()
{
    // Set above function as signal handler for the SIGINT signal
    if (signal(SIGINT, exit_hint) == SIG_ERR) {
        perror("An error occurred while setting a signal handler.");
        return EXIT_FAILURE;
    }

    // Get user and host name for prompt
    user = getenv("USER");
    name = getenv("NAME");

    // Get the current working directory and return if error
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
       perror("getcwd() error");
       return 1;
    }
    
    // Allocate memory for command buffer
    line = (char*) malloc(sizeof(char)*line_size);

    while (1) {
        // Print prompt
        printf("%s@%s:%s>", user, name, cwd);

        // Copy pointer to line
        char * temp = line;

        // Get a line from standard input
        int n = getline(&temp, &line_size, stdin);

        //Remove terminating CRLF delimiter
        temp[n-1] = '\0';

        // Get the first word, delimited by a space
        char* word = strtok(temp, " ");

        // If the word is NULL, do nothing
        if (!word) continue;

        // Process the first word
        if (strcmp(word, "exit") == 0){ // if first word is "exit", exit the program
            // free memory
            free(line);
            return 0;
        }
        else if (strcmp(word, "cd") == 0) { // if first word is "cd", change directory using c library function
            // get next word
            word = strtok(NULL, " ");

            // if second argument exists and is not ~ then process it normally
            if (word && strcmp(word, "~") != 0 ){ 
                if (strtok(NULL, " ")){ // Check if there are additional arguments
                    // output error message
                    puts("cd: too many arguments");
                }
                else if (chdir(word) == 0) { // try to change directory and , if successful, set cwd to the new directory
                    getcwd(cwd, sizeof(cwd));
                }
                else { //output error if directory could not be changed
                    perror(word);
                }
            }
            else { 
                // change to home directory
                chdir(getenv("HOME"));
                getcwd(cwd, sizeof(cwd));
            }
        }
        else {
            // Otherwise execute a program in child process
            int pid = fork();

            // If fork failed, just exit
            if (pid < 0) exit(EXIT_FAILURE);

            //  Child  process calls programm
            if (pid == 0) {
                // Construct argument vector 
                char* argv[n/2+2];

                int i = 0;   
                // Terminate loop if no more word is read from command buffer
                while (word)
                {
                    // Write current word in i-th position of argument vector and increment i
                    argv[i++] = word;
                    
                    // Read next word
                    word = strtok(NULL, " ");
                }
                // set terminating NULL pointer for argument vector
                argv[i] = NULL;

                // Execute the program using argument vector
                if (execvp(argv[0], argv) < 0) {
                    // if given program cannot be executed, return error and exit
                    printf("%s: command not found\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
            }
            // parent process waits for child process to end
            if (pid > 0){
                int ret;
                wait(&ret);
            }
        }
    }    
    return 0;
}
