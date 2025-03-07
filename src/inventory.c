/**
 * @brief It implements the inventory module
 * 
 * Defines the internal functionality of the inventory system,
 * works with the object and collection ADT to keep track of items, but also has
 * an InventoryType in order to locate the inventories on the game struct easily.
 * 
 *
 * @file inventory.c
 * @author Maksym Polyak
 * @version 0
 * @date 10-02-2025
 * @copyright GNU Public License
*/

#include "inventory.h"
#include "debug_printing.h"
#include "collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Inventory
 *
 * This struct stores all the information of the player inventory
 */
struct _Inventory {
    Collection *objects;                /*!< Collection with unique items and fixed space */
    InventoryType inventoryType;        /*!< Inventory type of the inventory */
    Id id;                              /*!< Id with the inventory location */
};


/*
 * Private functions
*/

/**
 * @brief Gets the number of objects in the inventory
 * 
 * @param inventory struct with all the information related to the inventory
 * @return object count (int) if well or -1 if there was an error
 */
long inventory_get_object_count(Inventory *inventory);

/**
 * @brief Gets the inventory type
 * 
 * @param inventory struct with all the information related to the inventory
 * @return InventoryType if well or -1 if ERROR
 */
InventoryType inventory_get_type(Inventory *inventory);

/**
 * @brief Gets the id with the location of the inventory
 * 
 * @param inventory struct with all the information related to the inventory
 * @return Id if well or -1 if ERROR
 */
Id inventory_get_location_id(Inventory *inventory);

/**
 * @brief Gets the size of inventory a type has defined
 * 
 * @param type 
 * @return unsigned long if well or -1 if error
 */
long inventory_get_size_by_type(InventoryType type);

/**
 * @brief Sets an inventory on the position index of the inventory object array
 * NOTE object can be null
 * 
 * @param inventory struct with all the information related to the inventory
 * @param object object to be set at the index
 * @param index index with the position of the object
 * @return OK if well or ERROR if error
 */
Status inventory_set_object_at(Inventory *inventory, Object *object, int index);

long inventory_get_object_count(Inventory *inventory){
    if(!inventory)
        return -1;
    
    return collection_length(inventory->objects);
}

InventoryType inventory_get_type(Inventory *inventory){
    if(!inventory)
        return -1;
    
    return inventory->inventoryType;
}

Id inventory_get_location_id(Inventory *inventory){
    if(!inventory)
        return -1;
    
    return inventory->id;
}

Object *inventory_get_object_at(Inventory *inventory, long index){
    return collection_get_element_at(inventory_get_collection(inventory), index);
}

long inventory_get_size_by_type(InventoryType type){
    long size;
    
    switch(type){
        case UNKNOWN_INVENTORY:
            debug_log(LOG_ERROR, "InventoryType is UNKNOWN_INVENTORY error at inventory_get_size_by_type");
            return -1;
        case PLAYER_INVENTORY:
            size = INVENTORY_PLAYER_MAX_SIZE; break;
        case NPC_INVENTORY:
            size = INVENTORY_ENTITY_MAX_SIZE; break;
        case SPACE_INVENTORY:
            size = INVENTORY_SPACE_MAX_SIZE; break;
    }  

    return size;
}

Status inventory_set_object_at(Inventory *inventory, Object *object, int index){
    if(!inventory)
        return ERROR;
        
    collection_add(inventory_get_collection(inventory), (void *)object);

    return OK;
}

/*
 * Game public interface functions
*/

Inventory *inventory_create(InventoryType type, Id locationid){
    Inventory *inventory = NULL;
    long initialsize;

    /*Reserved with calloc to set to NULL the objects and the other variables*/
    inventory = calloc(1, sizeof(Inventory));
    if(!inventory){
        debug_log(LOG_ERROR, "inventory_create dynamic memory error at inventory: inventoryType: %d locationid: %d", inventory->inventoryType, inventory->id);
        return NULL;
    }

    initialsize = inventory_get_size_by_type(type);

    /*Creates a collections with unique objects and fixed length determined by initialsize*/
    inventory->objects = collection_create(initialsize, TRUE, TRUE, object_isEqual, object_print);
    if(!(inventory->objects)){
        debug_log(LOG_ERROR, "inventory_create on collection objects - dynamic memory error at inventory: inventoryType: %d locationid: %d", inventory->inventoryType, inventory->id);
        free(inventory);
        return NULL;
    }
    

    inventory->inventoryType = type;
    inventory->id = locationid;

    return inventory;
}

void inventory_destroy(Inventory *inventory){
    if(inventory){
        if(inventory->objects)
            collection_destroy(inventory->objects);
        
        free(inventory);
    }
}

Object *inventory_get_object_by_id(Inventory *inventory, Id objectid){
    Object *object = NULL;
    long i, size;
    
    if(!inventory)
        return NULL;

    size = (int)inventory_get_size(inventory);
    if(size <= 0) return NULL;
    /*Searches by id in all the positions of the inventory array*/
    for(i = 0; i < size; i++){
        object = inventory_get_object_at(inventory, i);
        if(object_get_id(object) == objectid)
            return object;
    }

    return NULL;
}

/*Inventory GETTERS*/

Object *inventory_get_object_by_name(Inventory *inventory, char *objectname){
    int i, size;
    Object *object = NULL;
    
    if(!inventory || !objectname)
        return NULL;

    size = (int)inventory_get_size(inventory);
    for(i = 0; i < size; i++){
        object = inventory_get_object_at(inventory, i);
        if(strcmp(objectname, object_get_name(object)) == 0)
            return object;
    }

    return NULL;
}

Collection *inventory_get_collection(Inventory *inventory){
    if(!inventory)
        return NULL;

    return inventory->objects;
}

Status inventory_get_object_list(Inventory *inventory, char *objectlist){
    int num, i, size;
    Object *tempobject = NULL;
    
    if(!inventory || !objectlist)
        return ERROR;

    size = (int)inventory_get_size(inventory);
    for(i = 0, num = 1; i < size; i++){
        if((tempobject = inventory_get_object_at(inventory, i)) != NULL){
            sprintf(objectlist, "%d. Id: %ld | Name: %s\n", num++, object_get_id(tempobject), object_get_name(tempobject));
        }
    }

    return OK;
}

Status inventory_get_object_str_at(Inventory *inventory, char *objectdescr, int index){
    Object *tempobject = NULL;
    
    if(!inventory || !objectdescr)
        return ERROR;

    tempobject = inventory_get_object_at(inventory, index);
    sprintf(objectdescr, "%d. Id: %ld | Name: %s", index + 1, object_get_id(tempobject), object_get_name(tempobject));

    return OK;
}

long inventory_get_size(Inventory *inventory){
    if(!inventory)
        return -1;

    return collection_length(inventory_get_collection(inventory));
}

Object *inventory_get_object(Inventory *inventory, Id objectid){
    Object *object = NULL;
    long size, i;
    
    if(!inventory || !objectid)
        return NULL;

    size = inventory_get_size(inventory);
    for(i = 0; i < size; i++){
        object = inventory_get_object_at(inventory, i);
        if(objectid == object_get_id(object)){
            return object;
        }
    }

    return NULL;
}

/*Inventory set functions*/

Status inventory_add_object(Inventory *inventory, Object *object){
    if(!inventory){
        debug_log(LOG_ERROR, "inventory is NULL in inventory_add_object");
        return ERROR;
    }

    if(!object){
        debug_log(LOG_ERROR, "object is NULL in inventory_add_object");
        return ERROR;
    }

    /*Checks if the object is in the inventory*/
    collection_add(inventory_get_collection(inventory), (void *)object);

    object_set_type(object, inventory_get_type(inventory));
    object_set_location(object, inventory_get_location_id(inventory));

    return OK;
}

Status inventory_remove_object(Inventory *inventory, Object *object){
    if(!inventory){
        debug_log(LOG_ERROR, "inventory is NULL in inventory_add_remove");
        return ERROR;
    }

    if(!object){
        debug_log(LOG_ERROR, "object is NULL in inventory_add_remove");
        return ERROR;
    }
    
    if(collection_remove(inventory_get_collection(inventory), (void *)object) == ERROR) return ERROR;
    
    /*If not found it doesnt have to be removed*/
    return OK;
}

Status inventory_move_object(Inventory *inventoryOUT, Inventory *inventoryIN, Id objectid){
    Object *object = NULL;
    
    if(!inventoryOUT){
        debug_log(LOG_ERROR, "inventoryOUT is NULL in inventory_move_object");
        return ERROR;
    }

    if(!inventoryIN){
        debug_log(LOG_ERROR, "inventoryIN is NULL in inventory_move_object");
        return ERROR;
    }

    /*Checks if the object is in inventoryOUT, if not, there is not object to move*/
    object = inventory_get_object_by_id(inventoryOUT, objectid);
    if(object == NULL)
        return ERROR;
    
    if(inventory_remove_object(inventoryOUT, object) == ERROR){
        debug_log(LOG_ERROR, "Object %ld has failed to move due to fail on inventory remove from the inventory of type:%d and id:%ld", object_get_id(object), (int)inventory_get_type(inventoryOUT), inventory_get_location_id(inventoryOUT));
        return ERROR;
    }

    if(inventory_add_object(inventoryIN, object) == ERROR){
        debug_log(LOG_ERROR, "Object %ld was lost, move has failed due to fail on inventory add from the inventory of type:%d and id:%ld", object_get_id(object), (int)inventory_get_type(inventoryIN), inventory_get_location_id(inventoryIN));
        return ERROR;
    }

    debug_log(PRINT, "Object %ld has been moved to the inventory of type:%d and id:%ld to the inventory of type:%d and id:%ld", object_get_id(object), (int)inventory_get_type(inventoryOUT), inventory_get_location_id(inventoryOUT), (int)inventory_get_type(inventoryIN), inventory_get_location_id(inventoryIN));
    return OK;
}

bool inventory_contains_object(Inventory *inventory, Id objectid){
    if(!inventory)
        return 0;
    
    /*Inventory_get_object_by_id returns NULL if it has not found the object*/
    if(inventory_get_object_by_id(inventory, objectid) != NULL)
        return 1;
    
    return 0;
}