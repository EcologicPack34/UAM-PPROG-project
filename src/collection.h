/**
 * @file collection.h
 * @author Daniel Gómez
 * @brief 
 *          ADT that aims to implement a generic list of elements.
 *          It contains the behaviout of Set module asked for I2
 * @version 0.1
 * @date 2025-02-20
 * 
 * @copyright GNU Public license
 */

#include "types.h"
#include <stdbool.h>

/**
 * @brief Data type containing the info about the list
 */
typedef struct _Collection Collection;

/**
 * @brief Initializes a collection with the given parameters
 * @author Daniel Gómez
 * 
 * @param initialSize Initial allocated size for the list
 * @param fixedLength Whether the collection can be resized or not
 * @param uniqueElements Whether the collection can have repeated elements or not
 * @param compare_elements Method to compare the elements
 * @param print_element Method to print elements
 * @return Collection*; NULL if something went wrong
 */
Collection *collection_create(unsigned long initialSize, bool fixedLength, bool uniqueElements, int (*compare_elements)(void* e1, void* e2), void (*print_element)(void *element));

/**
 * @brief Frees the memory of a collection
 * @author Daniel Gómez
 * 
 * @param collection 
 */
void collection_destroy(Collection *collection);

/*------------SETTERS------------*/

Status collection_add(Collection *collection, void *element);

Status collection_remove(Collection *collection, void *element);

bool collection_contains(Collection *collection, void *element);

/*------------GETTERS------------*/

long collection_length(Collection *);
