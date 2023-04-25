#include "list.h"
#include <stdio.h>
#include <stdlib.h>

list* append (list * tail, int n){
    list* head = malloc(sizeof(list));
    head->data = n;
    head->next = tail;    
}

list* delete (list * list, int n){

}

list* find (list * list, int n){

}

void output(list* list){
    printf("%d ", list->data);
    if (list->next){
        output(list->next);
    }
}

int main(int argc, char** argv){
    list root = {1, NULL};
    append(&root, 2);
    output(&root);

    printf("\n");
    return 0;
}