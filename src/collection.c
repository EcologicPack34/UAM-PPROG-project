/**
 * @file collection.c
 * @author Daniel Gómez
 * @brief Implementation of collection.h
 * @version 0.1
 * @date 2025-02-20
 * 
 * @copyright GNU Public License
 * 
 */
#include "collection.h"
#include "debug_printing.h"

#include <stdio.h>
#include <stdlib.h>


struct _Collection{
    void **list;                /*<! Stores the elements of the collections */
    unsigned long length;       /*<! Stores the amount of elements in the collection*/
    unsigned long allocatedSize;/*<! Stores the amount of memory used by the collection*/

    bool fixedLength;           /*<! Can the collection increase its allocated size*/
    bool uniqueElements;        /*<! Can the collection have repeated elements*/

    int (*compare_elements)(void* e1, void* e2); /*Method to compare elements of the collection*/
    void (*print_element)(void *element);        /*Method to print an element of the collection*/
};

/*----------PRIVATE FUNCTIONS-----------*/

/**
 * @brief Adds a unique element to the collection
 * @author Daniel Gómez
 * 
 * @param collection
 * @param element 
 * @return Status 
 */
Status collection_add_unique(Collection *collection, void *element){
    /*We omit error control as it is done in collection_add()*/

    //Search in collection->list if the element is already stored using collection->compareElements

    //If found return OK and do nothing more

    //If it doesnt found it:
    // Check allocated size and then add the element

    return OK;
}

/**
 * @brief Adds a non unique element to the collection
 * @author Daniel Gómez
 * 
 * @param collection 
 * @param element 
 * @return Status 
 */
Status collection_add_non_unique(Collection *collection, void *element){
    /*We ommit error control as it is done in collection_add()*/
    void **auxp = NULL;

    bool isFull = false;

    /*Control over the size of the list*/
    if(collection->allocatedSize == collection->length) isFull = true;
    if(collection->fixedLength && isFull){
        debug_log(LOG_WARNING, "Couldn't add element to collection: fixed size collection has reached it's maximum size");
        return ERROR;
    }else if(isFull){
        auxp = collection->list;

        auxp = (void**)realloc(auxp, collection->allocatedSize * 2);
        if(!auxp){
            debug_log(LOG_ERROR,"Couldn't allocate more memory for Collection");
            return ERROR;
        }

        collection->list = auxp;
    }

    collection->list[(collection->length)++] = element;
    return OK;
}


/*----------PUBLIC FUNCTIONS----------*/

Collection *collection_create(unsigned long initialSize, bool fixedLength, bool uniqueElements, int (*compare_elements)(void* e1, void* e2), void (*print_element)(void *element)){
    Collection *collection = NULL;
    
    if(!compare_elements) return NULL;
    if(initialSize <= 0){
        debug_log(LOG_ERROR, "Error creating collection: collection size must be non-zero positive number");
        return NULL;
    }
    if(!print_element){
        debug_log(LOG_WARNING, "No print method assigned to a collection.");
    }

    collection = (Collection *)calloc(1, sizeof(Collection));
    if(!collection){
        debug_log(LOG_ERROR, "Error creating collection: Couldn't allocate memory for collection");
        return NULL;
    } 
        

    collection->list = (void**)malloc(initialSize * sizeof(void*));
    if(!(collection->list)){
        debug_log(LOG_ERROR, "Error creating collection: Couldn't allocate memory for list");
        free(collection);
        return NULL;
    }
    
    collection->length = 0;
    collection->allocatedSize = initialSize;

    collection->fixedLength = fixedLength;
    collection->uniqueElements = uniqueElements;

    collection->compare_elements = compare_elements;
    collection->print_element = print_element;
    
    return collection;
}

void collection_destroy(Collection * collection){
    if(collection){
        if(collection->list)
            free(collection->list);
        free(collection);
    }
}

/*-----------SETTERS----------*/

Status collection_add(Collection *collection, void *element){

}