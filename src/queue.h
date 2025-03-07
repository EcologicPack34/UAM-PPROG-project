/**
 * @file queue.h
 * @author Daniel Gómez
 * @brief Queue implementation with a linked list
 *          THE QUEUE DOESN'T MANAGE THE MEMORY OF THE ELEMENTS IT STORES
 * @version 0.1
 * @date 2025-03-06
 * 
 * @copyright GNU Public License
 */

#include "types.h"
#include <stdbool.h>

/**
 * @brief Data type containing all the information of the queue 
 * @author Daniel Gómez
 */
typedef struct _Queue Queue;

/**
 * @brief Initializes and allocates memory for a queue
 * @author Daniel Gómez
 * 
 * @return Pointer to allocated memory, NULL if error
 */
Queue *queue_create();

/**
 * @brief Frees all the memory used by queue
 *          DOESN'T FREE MEMORY USED BY ELEMENTS STORE IN QUEUE
 * @param q pointer to queue
 */
void queue_destroy(Queue *q);

Status queue_push(Queue *q, void *element);

void *queue_pop(Queue *q);

long queue_size(Queue *q);

bool queue_isEmpty(Queue *q);