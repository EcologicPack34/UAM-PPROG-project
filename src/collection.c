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


/**
 * @brief Collection to manage a list of objects with special parametrs as fixed_length and UniqueElements
 */
struct _Collection{
    void **list;                            /*!< Stores the elements of the collections */
    long length;                            /*!< Stores the amount of elements in the collection*/
    long allocated_size;                    /*!< Stores the amount of memory used by the collection*/

    bool fixed_length;                      /*!< Can the collection increase its allocated size*/
    bool unique_elements;                   /*!< Can the collection have repeated elements*/

    P_elem_cmp compare_elements;            /*!< Method to compare elements of the collection: return 0 if equal, < 0 if smaller and > 0 if greater*/
    void (*print_element)(void *element);   /*!< Method to print an element of the collection*/
};

/*----------PRIVATE FUNCTIONS-----------*/

/**
 * @brief Adds a unique element to the collection
 * @author Daniel Gómez
 * 
 * @param collection collection to modifiy
 * @param element element to add checking if its unique
 * @return Status 
 */
Status collection_add_unique(Collection *collection, void *element);

/**
 * @brief Adds a non unique element to the collection
 * @author Daniel Gómez
 * 
 * @param collection collection to modifiy
 * @param element element to add without checking if its unique
 * @return Status 
 */
Status collection_add_non_unique(Collection *collection, void *element);

Status collection_add_unique(Collection *collection, void *element){
    /*We omit error control as it is done in collection_add()*/

    if(collection_contains(collection, element) != -1) return ERROR;
    
    return collection_add_non_unique(collection, element);
}

Status collection_add_non_unique(Collection *collection, void *element){
    /*We ommit error control as it is done in collection_add()*/
    void **auxp = NULL;

    bool isFull = false;

    /*Control over the size of the list*/
    if(collection->allocated_size == collection->length) isFull = true;
    if(collection->fixed_length && isFull){
        debug_log(LOG_WARNING, "Couldn't add element to collection: fixed size collection has reached it's maximum size");
        return ERROR;
    }else if(isFull){
        auxp = collection->list;

        auxp = (void**)realloc(auxp, collection->allocated_size * 2);
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
Collection *collection_create(long initialSize, bool fixed_length, bool unique_elements, P_elem_cmp compare_elements, void (*print_element)(void *)){
    Collection *collection = NULL;
    
    if(!compare_elements) return NULL;
    if(initialSize <= 0){
        debug_log(LOG_ERROR, "Error creating collection: collection size must be non-zero positive number");
        return NULL;
    }

    collection = (Collection *)calloc(1, sizeof(Collection));
    if(!collection){
        debug_log(LOG_ERROR, "Error creating collection: Couldn't allocate memory for collection");
        return NULL;
    } 
        

    collection->list = (void**)calloc(initialSize, sizeof(void*));
    if(collection->list == NULL){
        debug_log(LOG_ERROR, "Error creating collection: Couldn't allocate memory for list, list size: %ld", initialSize);
        free(collection);
        return NULL;
    }
    
    collection->length = 0;
    collection->allocated_size = initialSize;

    collection->fixed_length = fixed_length;
    collection->unique_elements = unique_elements;

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
    if(!collection || !element) return ERROR;
    
    if(collection->unique_elements){
        return collection_add_unique(collection, element);
    }

    return collection_add_non_unique(collection, element);
}

Status collection_remove(Collection *collection, void *element){
    int i;
    if(!collection || !element) return ERROR;

    /*Iterates through collection and moves every element*/
    for (i = 0; i < collection->length ; i++)
    {   
        /*Checks if element is contained*/
        if(collection->compare_elements(element, collection_get_element_at(collection, i)) == 0){
            collection_remove_at(collection, i);
            return OK;
        }
    }

    return ERROR;
}

Status collection_remove_at(Collection *collection, long index){
    int i;
    
    if(!collection) return ERROR;
    if(index > collection->length || index < 0) return ERROR;

    collection->list[index] = NULL;

    if(index != collection->length -1){
        for (i = index + 1; i < collection->length; i++)
        {
            collection->list[i -1] = collection->list[i];
        }
    }
    
    
    collection->list[collection->length -1] = NULL;
    (collection->length)--;

    return OK;
}

/*----------GETTERS----------*/

void *collection_get_element_at(Collection *collection, long index){
    if(!collection) return NULL;

    if((index > collection->length) || (index < 0)) return NULL;

    return collection->list[index];
}

int collection_contains(Collection *collection, void *element){
    int i;
    if(!collection || !element) return -1;

    for (i = 0; i < collection->length; i++)
    {
        if(collection->compare_elements(element, collection->list[i]) == 0){
            return i;
        }
    }
    return -1;
}

void *collection_find(Collection *collection, void *element){
    int index;
    if(!collection || !element) return NULL;

    index = collection_contains(collection, element);
    if(index == -1) return NULL;
    return collection_get_element_at(collection, index);
}

long collection_length(Collection *collection){
    if(!collection) return -1;

    return collection->length;
}

/*----------OTHERS----------*/

Status collection_free_elements(Collection *collection, void (*free_element)(void *)){
    int i;

    if(!collection || !free_element) return ERROR;
    if(!(collection->list)) return ERROR;

    if(collection->length == 0) return OK;

    for (i = 0; i < collection->length; i++)
    {
        free_element(collection->list[i]);
    }

    return OK;
}