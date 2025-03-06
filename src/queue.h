/**
 * @file queue.h
 * @author Daniel Gómez
 * @brief Queue implementation with a linked list
 * @version 0.1
 * @date 2025-03-06
 * 
 * @copyright GNU Public License
 */

#include "types.h"
#include <stdbool.h>

typedef struct _Queue Queue;

Queue *queue_create();

void queue_destroy(Queue *q);

Status queue_push(Queue *q, void *element);

void *queue_pop(Queue *q);

long queue_size(Queue *q);

bool queue_isEmpty(Queue *q);