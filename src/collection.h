/**
 * @file collection.h
 * @author Daniel Gómez
 * @brief 
 *          ADT that aims to implement a generic list of elements.
 *          It contains the behaviour of Set module asked for I2
 * 
 *          THE COLLECTION DOES NOT FREE THE MEMORY OF THE ELEMENTS WHEN REMOVED OR DESTROYED
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
 * @param compare_elements Method to compare the elements: return 0 if equal, < 0 if smaller and > 0 if greater
 * @param print_element Method to print elements
 * @return Collection*; NULL if something went wrong
 */
Collection *collection_create(unsigned long initialSize, bool fixedLength, bool uniqueElements, int (*compare_elements)(void*, void*), void (*print_element)(void *));

/**
 * @brief Frees the memory of a collection
 * @author Daniel Gómez
 * 
 * @param collection 
 */
void collection_destroy(Collection *collection);

/*------------SETTERS------------*/

/**
 * @brief Adds an element to the collection if posible
 * @author Daniel Gómez
 * 
 * @param collection 
 * @param element 
 * @return Status Ok if everything was done correctly, ERROR if something failed
 */
Status collection_add(Collection *collection, void *element);

/**
 * @brief Removes an element from the collection
 * @author Daniel Gómez
 * 
 * @param collection 
 * @param element 
 * @return Status Ok if removed, ERROR if null reference or element not found
 */
Status collection_remove(Collection *collection, void *element);

/**
 * @brief Removes the element at a given index from the collection
 * @author Daniel Gómez
 * 
 * @param collection 
 * @param index 
 * @return Status 
 */
Status collection_remove_at(Collection *collection, unsigned long index);

/*------------GETTERS------------*/

/**
 * @brief Gets the element at a given index
 * 
 * @param collection 
 * @param index 
 * @return NULL if something went wrong, pointer to element if correct
 */
void *collection_get_element_at(Collection *collection, unsigned long index);

/**
 * @brief Checks if the collection contains a certain element
 * @author Daniel Gómez
 * 
 * @param collection 
 * @param element 
 * @return -1 if not contained
 * @return the index if contained
 */
int collection_contains(Collection *collection, void *element);

/**
 * @brief Returns the number of elements in the collection, not to be confused with allocated size
 * @author Daniel Gómez
 * 
 * @param collection 
 * @return long : -1 if null reference, length if correct
 */
long collection_length(Collection *collection);

/*----------OTHERS----------*/

/**
 * @brief Frees all the elements of the collection using the given fucntion
 * 
 * @param collection 
 * @param free 
 * @return Status : ERROR if null references, else OK
 */
Status collection_free_elements(Collection *collection, void (*free_element)(void *));