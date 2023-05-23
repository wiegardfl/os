#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    printf("Bitte Text eingeben: ");

    size_t size = 64;
    char* str = (char*) malloc(sizeof(char)*size);

    int n = getline(&str, &size, stdin);

    char* word = strtok(str, " ");

    while (word){
        puts(word);
        word = strtok(NULL, " ");
    }
}