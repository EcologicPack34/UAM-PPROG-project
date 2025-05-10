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

#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"
#include <stdbool.h>

/**
 * @brief Data type containing all the information of the queue 
 * @author Daniel Gómez
 */
typedef struct _Queue Queue;

/**
 * @brief Generic free function type
 */
typedef void (*P_free_ele)(void*);

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
 * @author Daniel Gómez
 * @param q pointer to queue
 */
void queue_destroy(Queue *q);

/**
 * @brief Tries to add an element to the queue
 * @author Daniel Gómez
 * 
 * @param q pointer to queue
 * @param element pointer to element
 * @return Ok if added, ERROR if not 
 */
Status queue_push(Queue *q, void *element);

/**
 * @brief Retrieves an element from the queue
 * @author Daniel Gómez
 * 
 * @param q pointer to queue
 * @return Pointer to element, NULL if something went wrong
 */
void *queue_pop(Queue *q);

/**
 * @brief Gets the first element of the queue without extracting it
 * @author Daniel Gómez
 * 
 * @param q queue struct
 * @return void* 
 */
void *queue_front(Queue *q);

/**
 * @brief Gets the last element of the queue without extracting it
 * @author Daniel Gómez
 * 
 * @param q queue struct
 * @return void* 
 */
void *queue_rear(Queue *q);

/**
 * @brief Returns the size of the queue
 * @author Daniel Gómez
 * 
 * @param q queue struct
 * @return size, -1 if wrong
 */
long queue_size(Queue *q);

/**
 * @brief Returns if the queue has elements in it or not
 * @author Daniel Gómez
 * 
 * @param q queue struct
 * @return true 
 * @return false 
 */
bool queue_isEmpty(Queue *q);

/**
 * @brief Frees the elements of a queue with the ele_free function
 * @author Maksym Polyak
 * 
 * @param q queue struct
 * @param ele_free generic free function
 * @return Status 
 */
Status queue_free_elements(Queue *q, P_free_ele ele_free);

#endif