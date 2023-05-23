#include "nodeList.h"
#include <stdio.h>
#include <stdlib.h>


void push(nodeList* list, int data) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->data = data;
    newNode->next = NULL;

    *list->tail = newNode;
    list->tail = &newNode->next;
}


node* pop(nodeList * list){
    node* pop = list->head;
    if (pop != NULL){
        list->head = pop->next;
    }

    return pop;
}


void output(nodeList* list){
    node* current = list->head;
    printf("list content: ");
    while (current != NULL){
        printf("%d ", current->data);
        current = current->next;
    }
    puts("\n");
}

int main(){
    nodeList* list = (nodeList*)malloc(sizeof(nodeList));
    list->head = NULL;
    list->tail = &list->head;

    output(list);

    for (int i = 0; i < 5; i++){
        push(list, i);
        output(list);
    }

    for (int i = 0;i < 6; i++){
        node* popped = pop(list);
        if (popped){
            printf("Popped %d\n", popped->data);
        }
        else printf("pop failed: list empty");
        output(list);
    }
    
    return 0;
}