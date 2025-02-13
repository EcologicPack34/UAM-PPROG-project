/**
 * @brief It implements the inventory module
 * 
 * Defines the internal functionality of the inventory system,
 * works with the object ADT to keep track of items, but also has
 * an InventoryType for further iterations, were NPCs will be added.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Inventory
 *
 * This struct stores all the information of the player inventory
 */
struct _Inventory {
    Object *Array[INVENTORY_MAX_SIZE];  /*!< Array of pointers that point to the objects in the inventory (NOT ORGANIZED) */
    int objectcount;                    /*!< Total number of objects on the inventory */
    InventoryType inventoryType;        /*!< Inventory type of the inventory */
    Id id;                              /*!< Id with the inventory location */

};


/*
 * Private functions
*/

#pragma region PRIVATE

#pragma region PRIVATE_DECLARATION

#pragma region PRIVATE_GETTERS

/**
 * @brief Gets the object array
 * 
 * @param inventory struct with all the information related to the inventory
 * @return Object* if well or NULL if ERROR
 */
Object **inventory_get_object_array(Inventory *inventory);

/**
 * @brief Gets the number of objects in the inventory
 * 
 * @param inventory struct with all the information related to the inventory
 * @return object count (int) if well or -1 if there was an error
 */
int inventory_get_object_count(Inventory *inventory);

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
 * @brief Gets the object on the object array at the position index
 * 
 * @param inventory struct with all the information related to the inventory
 * @param index index with the position of the object
 * @return Object* if well or NULL if error
 */
Object *inventory_get_object_at(Inventory *inventory, int index);

/**
 * @brief Gets the object index in the inventory object array
 * 
 * @param inventory struct with all the information related to the inventory
 * @param object object to look for
 * @return index(int) if well or -1 if not found(But returns the index if the object is NULL)
 */
int inventory_get_object_index(Inventory *inventory, Object *object);

#pragma endregion

#pragma region PRIVATE_SETTERS

/**
 * @brief Sets the number of objects in the inventory
 * 
 * @param inventory struct with all the information related to the inventory
 * @param object_count number of objects to set in the inventory
 * @return OK if well or ERROR if error
 */
Status inventory_set_object_count(Inventory *inventory, int object_count);

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

#pragma endregion

#pragma endregion

#pragma region PRIVATE_IMPLEMENTATION

#pragma region PRIVATE_GETTERS

Object **inventory_get_object_array(Inventory *inventory){
    if(!inventory)
        return NULL;
    
    return inventory->Array;
}

int inventory_get_object_count(Inventory *inventory){
    if(!inventory)
        return -1;
    
    return inventory->objectcount;
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

Object *inventory_get_object_at(Inventory *inventory, int index){
    if(!inventory)
        return NULL;

    return (inventory->Array)[index];
}

int inventory_get_object_index(Inventory *inventory, Object *object){
    int i;
    
    if(!inventory || !object)
        return -1;
    
    for(i = 0; i < INVENTORY_MAX_SIZE; i++){
        if(inventory_get_object_at(inventory, i) == object)
            return i;
    }

    /*If object was not found returns error as -1*/
    return -1;
}

#pragma endregion

#pragma region PRIVATE_SETTERS

Status inventory_set_object_count(Inventory *inventory, int object_count){
    if(!inventory)
        return ERROR;
    
    inventory->objectcount = object_count;

    return OK;
}



Status inventory_set_object_at(Inventory *inventory, Object *object, int index){
    if(!inventory)
        return ERROR;
    
    (inventory->Array)[index] = object;

    return OK;
}

#pragma endregion

#pragma endregion

#pragma endregion

/*
 * Game public interface functions
*/

#pragma region INVENTORY

Inventory *inventory_create(InventoryType type, Id locationid){
    Inventory *inventory = NULL;

    /*Reserved with calloc to set to NULL the objects and the other variables*/
    inventory = calloc(1, sizeof(Inventory));
    if(!inventory){
        debug_log(ERROR, "inventory_create dynamic memory error at inventory: inventoryType: %d locationid: %d", inventory->inventoryType, inventory->id);
        return NULL;
    }

    inventory->inventoryType = type;
    inventory->id = locationid;

    return inventory;
}

void inventory_destroy(Inventory *inventory){free(inventory);}

Object *inventory_get_object_by_id(Inventory *inventory, Id objectid){
    Object *object = NULL;
    int i;
    
    if(!inventory)
        return NULL;
    
    /*Searches by id in all the positions of the inventory array*/
    for(i = 0; i < INVENTORY_MAX_SIZE; i++){
        object = inventory_get_object_at(inventory, i);
        if(object_get_id(object) == objectid)
            return object;
    }

    return NULL;
}

#pragma region DERIVED_FUNCTIONS

bool inventory_contains_object(Inventory *inventory, Id objectid){
    if(!inventory)
        return 0;
    
        /*Inventory_get_object_by_id returns NULL if it has not found the object*/
    if(inventory_get_object_by_id(inventory, objectid) != NULL)
        return 1;
    
    return 0;
}

Status inventory_add_object(Inventory *inventory, Object *object){
    int i;
    bool finished;
    Object *tempobject = NULL;
    
    if(!inventory){
        debug_log(ERROR, "inventory is NULL in inventory_add_object");
        return ERROR;
    }

    if(!object){
        debug_log(ERROR, "object is NULL in inventory_add_object");
        return ERROR;
    }

    /*Checks if the object is in the inventory*/
    if(inventory_contains_object(inventory, object_get_id(object)) == 0)
        return OK;

    for(i = 0, finished = 0; (i < INVENTORY_MAX_SIZE) && (finished == 0); i++){
        if((tempobject = inventory_get_object_at(inventory, i)) != NULL){
            inventory_set_object_at(inventory, object, i);
            finished = 1;
        }
    }

    inventory_set_object_count(inventory, inventory_get_object_count(inventory) + 1);

    debug_log(DEBUG, "Object %ld has been added to the inventory of type:%d and id:%ld", object_get_id(object), (int)inventory_get_type(inventory), inventory_get_location_id(inventory));
    return OK;
}

Status inventory_remove_object(Inventory *inventory, Object *object){
    int i;
    
    if(!inventory){
        debug_log(ERROR, "inventory is NULL in inventory_add_remove");
        return ERROR;
    }

    if(!object){
        debug_log(ERROR, "object is NULL in inventory_add_remove");
        return ERROR;
    }
    
    for(i = 0; i < INVENTORY_MAX_SIZE; i++){
        if(inventory_get_object_at(inventory, i) == object){
            inventory_set_object_at(inventory, NULL, i);
            inventory_set_object_count(inventory, inventory_get_object_count(inventory) - 1);
            return OK;
        }
    }
    
    /*If not found it doesnt have to be removed*/
    debug_log(DEBUG, "Object %ld has been removed from the inventory of type:%d and id:%ld", object_get_id(object), (int)inventory_get_type(inventory), inventory_get_location_id(inventory));
    return OK;
}

Status inventory_move_object(Inventory *inventoryOUT, Inventory *inventoryIN, Id objectid){
    Object *object = NULL;
    
    if(!inventoryOUT){
        debug_log(ERROR, "inventoryOUT is NULL in inventory_move_object");
        return ERROR;
    }

    if(!inventoryIN){
        debug_log(ERROR, "inventoryIN is NULL in inventory_move_object");
        return ERROR;
    }

    /*Checks if the object is in inventoryOUT, if not, there is not object to move*/
    object = inventory_get_object_by_id(inventoryOUT, objectid);
    if(object == NULL)
        return ERROR;
    
    inventory_remove_object(inventoryOUT, object);

    inventory_add_object(inventoryIN, object);

    debug_log(DEBUG, "Object %ld has been moved to the inventory of type:%d and id:%ld to the inventory of type:%d and id:%ld", object_get_id(object), (int)inventory_get_type(inventoryOUT), inventory_get_location_id(inventoryOUT), (int)inventory_get_type(inventoryIN), inventory_get_location_id(inventoryIN));
    return OK;
}

Status inventory_get_object_list(Inventory *inventory, char *objectlist){
    int num, i;
    Object *tempobject = NULL;
    
    if(!inventory || !objectlist)
        return ERROR;

    for(i = 0, num = 1; i < INVENTORY_MAX_SIZE; i++){
        if((tempobject = inventory_get_object_at(inventory, i)) != NULL){
            sprintf(objectlist, "%d. Id: %ld | Name: %s | LocationId: %ld\n", num++, object_get_id(tempobject), object_get_name(tempobject), object_get_location(tempobject));
        }
    }

    return OK;
}

#pragma endregion

#pragma endregion