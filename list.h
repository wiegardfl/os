typedef struct list {
    int data;
    struct list *next;
} list;

list* append (list *, int);
list* delete (list *, int);
list* find (list *, int);