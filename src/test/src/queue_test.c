#include <stdio.h>
#include <stdlib.h>

#include "queue.h"


int main(){
    Queue * q = NULL;

    q = queue_create();
    if(!q){
        printf("error creating queueu");
        return 0;
    }
    
    int *elem = NULL;
    for (size_t i = 0; i < 10; i++)
    {
        elem = malloc(sizeof(int));
        *elem = i;
        queue_push(q, elem);
        printf("Queue length = %ld\n", queue_size(q));
    }
    
    for (size_t i = 0; i < 5; i++)
    {
        elem = (int *)queue_pop(q);
        if(elem != NULL){
            printf("Queue size = %ld, elem num = %d\n", queue_size(q), *elem);
            free(elem);
        }
    }

    /*Al salir del programa quedaran 5 elementos sin liberar, pero los nodos del queue si se liberan, el problema es por parte de la gestion del ususario*/

    queue_destroy(q);
    return 0;
}