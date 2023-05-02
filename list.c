#include "list.h"
#include <stdio.h>
#include <stdlib.h>

list* createNode (int n){
    list* l = malloc(sizeof(list));
    l->data = n;
    l->next = NULL;
    return l;
}

list* append (list * old, int n){
    if (old == NULL){
        return createNode(n);
    }
    else {
        if (old->next == NULL){
            old->next = createNode(n);
        }
        else append(old->next, n);
        return old;
    }
}

list* delete (list * list, int n){
    if (list == NULL){

    }
    else if (list->data == n){
        list = list->next;
    }
    else if (list->next != NULL){
        if (list->next->data == n){
            list->next = list->next->next;
        }
        else delete(list->next, n);
    }
    return list;
}

list* find (list * list, int n){
    if (list == NULL){
        return NULL;
    }
    else if (list->data == n){
        return list;
    }
    else return find(list->next, n);
}

void output(list* list){
    if (list != NULL){
        printf("%d ", list->data);
    }
    if (list->next){
        output(list->next);
    }
}

int main(int argc, char** argv){
    list* root = createNode(0);

    for (int i = 1; i < 10; i++){
        append(root, i);
    }

    root = delete(root, 5);
    root = delete(root, 0);

    output(root);

    root = find(root, 7);

    output(root);

    printf("\n");
    return 0;
}