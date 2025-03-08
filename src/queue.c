#include <stdlib.h>
#include <stdio.h>

typedef struct{
    void *element;
    void *nextNode;
}_Node;

#include "queue.h"


struct _Queue{
    _Node *firstNode;
    _Node *lastNode;
    long size;
};

Queue *queue_create(){
    Queue *queue = NULL;

    queue = calloc(1, sizeof(Queue));
    if(!queue) return NULL;

    return queue;
}

void queue_destroy(Queue *q){
    int i;
    _Node *currentNode = NULL;
    _Node *nextNode = NULL;
    
    if(q){
        if(q->size > 0){
            currentNode = q->firstNode;
            for (i = 0; i < q->size; i++)
            {
                nextNode = (_Node*) currentNode->nextNode;
                free(currentNode);
                currentNode = nextNode;
            }
        }
        free(q);
    }
}

Status queue_push(Queue *q, void *element){
    _Node *node = NULL;

    if(!q || !element) return ERROR;

    node = (_Node*)calloc(1,sizeof(_Node));
    if(!node) return ERROR;

    node->element = element;

    if((q->size) == 0){ 
        q->firstNode = node;
        q->lastNode = node;
    }else{
        q->lastNode->nextNode = (void *)node;
        q->lastNode = node;
    }

    (q->size)++;

    return OK;
}

void *queue_pop(Queue *q){
    _Node *node = NULL;
    void *element = NULL;
    
    if(!q) return NULL;
    
    if((q->size) <= 0) return NULL;
    
    node = q->firstNode;
    
    if(q->size > 1) q->firstNode = (_Node *)node->nextNode;
    else{
        q->firstNode = NULL;
        q->lastNode = NULL;
    }
    
    element = node->element;
    (q->size)--;

    free(node);

    return element;
}

void *queue_front(Queue *q){
    if(!q) return NULL;
    return q->firstNode->element;
}

void *queue_rear(Queue *q){
    if(!q) return NULL;
    return q->lastNode->element;
}

long queue_size(Queue *q){
    if(!q) return -1;
    return q->size;
}

bool queue_isEmpty(Queue *q){
    if(!q) return true;

    return !(q->size);
}