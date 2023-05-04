typedef struct node {
    int data;
    struct node* next;
}  node;

typedef struct {
    node* head;
    node** tail;
} nodeList;

void push  (nodeList* , int);
node* pop (nodeList *); 