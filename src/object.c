/**
 * @brief It implements the object module
 *
 * @file object.c
 * @author Maksym Polyak
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */

#include "object.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INVENTORY_MAX_SIZE 10

/**
 * @brief Object
 *
 * This struct stores all the information of an object
 */
struct _Object {
  Id id;              /*!< Id number of the object, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the object */
};

/**
 * @brief Inventory
 *
 * This struct stores all the information of the player inventory
 */
struct _Inventory {
    Object **Array;  /*!< Array of pointers that point to the objects in the inventory */
    int objectnum;   /*!< Total number of objects on the inventory */
};

/**
   Private functions
*/
#pragma region PRIVATE_DECLARATION
/**
 * @brief Returns the object from the inventory in the index i
 * @author Maksym Polyak
 *
 * @param inventory contains the information of an inventory
 * @param index index of the object
 * @return Object pointer if everything went fine or NULL if there was a mistake
 */
Object *inventory_get_object_from_index(Inventory *inventory, int index);

/**
 * @brief Searchs the index of an object in a inventory
 * @author Maksym Polyak
 *
 * @param inventory contains the information of an inventory
 * @param object contains the information of an object
 * @return index if everything went fine or -1 if object not found
 */
int inventory_object_search(Inventory *inventory, Object *object);

/**
 * @brief Reorganizes the inventory objects so there are no NULL pointers between objects (from left to right)
 * @author Maksym Polyak
 *
 * @param inventory contains the information of an inventory
 * @return OK if everything went fine or ERROR if there was a mistake
 */
Status inventory_rearrange_objects(Inventory *inventory);

/**
 * @brief Sets the object received as argument on the index received to the inventory
 * @author Maksym Polyak
 *
 * @param inventory contains the information of an inventory
 * @param object object to be inserted on the inventory
 * @param index index of the object
 * @return OK if everything went fine or NULL if there was a mistake
 */
Status inventory_set_object_on_index(Inventory *inventory, Object *object, int index);

/**
 * @brief Returns the array of objects from the inventory
 * @author Maksym Polyak
 *
 * @param inventory contains the information of an inventory
 * @return Object pointer if everything went fine or NULL if there was a mistake
 */
Object **inventory_get_array(Inventory *inventory);

/**
 * @brief Sets the total number of objects stored in the inventory
 * @author Maksym Polyak
 *
 * @param inventory contains the information of an inventory
 * @param num number of objects stored in the inventory
 * @return OK if everything went fine or ERROR if there was a mistake
 */
Status inventory_set_object_count(Inventory *inventory, int num);

/**
 * @brief Gets the total number of objects stored in the inventory
 * @author Maksym Polyak
 *
 * @param inventory contains the information of an inventory
 * @param num number of objects stored in the inventory
 * @return num if everything went fine or -1 if there was a mistake
 */
int inventory_get_object_count(Inventory *inventory);

#pragma endregion

#pragma region PRIVATE_IMPLEMENTATION

Object *inventory_get_object_from_index(Inventory *inventory, int index){
    if(!inventory || index >= INVENTORY_MAX_SIZE || index < 0)
        return NULL;
    
    return inventory->Array[index];
}

int inventory_object_search(Inventory *inventory, Object *object){
    int i;

    for(i = 0; i < INVENTORY_MAX_SIZE; i++){
        if(object_isEqual(object, inventory_get_object_from_index(inventory, i)))
            return i;
    }
    
    return -1;
}

Status inventory_rearrange_objects(Inventory *inventory){
    /*!< numnull = Number of NULL on the inventory | indexnull = index of the numnull*/
    int i, j, indexnull[INVENTORY_MAX_SIZE], numnull;
    bool found;
    Object *object1 = NULL;

    if(!inventory)
        return ERROR;

    /*Sets to 0 elements of indexnull in order to determine null index in next for loop*/
    for(i = 0; i < INVENTORY_MAX_SIZE; i++){
        indexnull[i] = 0;
    }
    
    /*Search for null objects in inventory*/
    for(i = 0, numnull = 0; i < INVENTORY_MAX_SIZE; i++){
        if(inventory_get_object_from_index(inventory, i) == NULL){
            numnull++;
            indexnull[i] = 1;
        }
    }

    /*If there are not nulls, function ends*/
    if(numnull == 0)
        return OK;
    
    /*Searches in indexnull if the index i is NULL, then replaces it with the nearest non NULL pointer that is greater than i*/
    for(i = 0; (i < INVENTORY_MAX_SIZE); i++){
        if(indexnull[i] == 1){
            /*Finds the nearest non NULL pointer from the next index (i + 1), if it doesn't find it, function ends*/
            for(j = i + 1, found = false; (j < INVENTORY_MAX_SIZE) && (found == false); j++){
                object1 = inventory_get_object_from_index(inventory,j);
                if(object1 != NULL){
                    if(inventory_set_object_on_index(inventory, object1, i) == ERROR)
                        return ERROR;
                    
                    inventory_set_object_on_index(inventory,NULL, j);
                    found = true;
                /*If it reaches the end, finishes function*/
                } else if(j == INVENTORY_MAX_SIZE - 1){
                    return OK;
                }
            }
        }
    }

    return OK;
}

Status inventory_set_object_on_index(Inventory *inventory, Object *object, int index){
    /*The comprobation does not check object because it can be set to NULL*/
    if(!inventory || index >= INVENTORY_MAX_SIZE || index < 0)
        return ERROR;
    
    inventory->Array[index] = object;
    
    return OK;
}

Object **inventory_get_array(Inventory *inventory){
    if(!inventory)
        return NULL;
    
    return inventory->Array;
}

Status inventory_set_object_count(Inventory *inventory, int count){
    if(!inventory)
        return ERROR;
    
    inventory->objectnum = count;

    return OK;
}

int inventory_get_object_count(Inventory *inventory){
    if(!inventory)
        return -1;
    
    return inventory->objectnum;
}

#pragma endregion





/**
   Game interface implementation
*/


/*Object public functions*/
#pragma region OBJECT

Object *object_create(Id id, char *name){
    Object *object = NULL;

    if(!(object = (Object *)calloc(1,sizeof(Object))))
        return NULL;
    
    object->id = id;
    strcpy(object->name,name);
    
    return object;
}

void object_destroy(Object *object){
    if(!object)
        return;

    free(object);
    object = NULL;
}

int object_isEqual(Object *object1, Object *object2){
    if(!object1 || !object2)
        return -1;

    if(object_get_id(object1) == NO_ID || object_get_id(object2) == NO_ID)
        return -1;

    if((object_get_id(object1) == object_get_id(object2)))
        return 1;
    
    return 0;
}

/*Object SETTERS*/
#pragma region SETTERS

Status object_set_id(Object* object, Id id){
    if(!object)
        return ERROR;

    object->id = id;

    return OK;
}

Status object_set_name(Object* object, char* name){
    if(!object || !name)
        return ERROR;

    strcpy(object->name,name);

    return OK;
}

#pragma endregion

/*Object GETTERS*/
#pragma region GETTERS

Id object_get_id(Object *object){
    if(!object)
        return ERROR;

    return object->id;
}

char *object_get_name(Object *object){
    if(!object)
        return ERROR;

    return object->name;
}

#pragma endregion

#pragma endregion

/*Inventory public functions*/
#pragma region INVENTORY

Inventory *inventory_create(){
    Inventory *inventory = NULL;

    if(!(inventory = (Inventory *)malloc(sizeof(Inventory))))
        return NULL;
    
    /*Creates memory for the pointers to the objects*/
    if(!(inventory->Array = (Object **)calloc(INVENTORY_MAX_SIZE, sizeof(Object)))){
        free(inventory);
        return NULL;
    }

    inventory->objectnum = 0;
    
    return inventory;
}

void inventory_destroy(Inventory *inventory){
    int i;
    
    if(!inventory)
        return;

    /*Frees all the objects inside an inventory*/
    for(i = inventory->objectnum; i >= 0; i--){
        object_destroy(inventory->Array[i]);
    }

    free(inventory);
    inventory = NULL;
}

/*Inventory interactions with objects*/
#pragma region INVENTORY_OBJECT_INTERACTION
Status inventory_add_object(Inventory *inventory, Object *object){
    if(!inventory || !object)
        return ERROR;
    
    if(inventory->objectnum >= INVENTORY_MAX_SIZE)
        return ERROR;

    /*Increases the objectnum and then replaces the object on the new spot*/
    inventory_set_object_count(inventory, inventory_get_object_count(inventory) + 1);
    inventory_set_object_on_index(inventory, object, inventory_get_object_count(inventory));

    return OK;
}

Status inventory_remove_object(Inventory *inventory, Object *object){
    int index;
    
    if(!inventory || !object)
        return ERROR;

    /*Searches the index of the object in the inventory*/
    if((index = inventory_object_search(inventory, object)) == -1){
        return ERROR;
    }

    /*Sets to NULL the object in the inventory and destroys the object*/
    inventory_set_object_on_index(inventory, NULL, index);
    object_destroy(object);

    /*Reduces the objectnum on inventory*/
    inventory_set_object_count(inventory, inventory_get_object_count(inventory) - 1);

    if(inventory_rearrange_objects(inventory) == ERROR)
        return ERROR;

    return OK;
}

Status inventory_object_move(Inventory *inventoryOUT, Inventory *inventoryIN, Object *object){
    int index;
    
    if(!inventoryOUT || !inventoryIN || !object)
        return ERROR;

    /*Checks if the inventoryIN is full*/
    if(inventory_get_object_count(inventoryIN) == INVENTORY_MAX_SIZE)
        return ERROR;
    
    
    /*Searchs the object index to delete it if it exists, then sets it to NULL*/
    index = inventory_object_search(inventoryOUT, object);
    if(index == -1)
        return ERROR;

    inventory_set_object_on_index(inventoryOUT, NULL, index);
    inventory_set_object_count(inventoryOUT, inventory_get_object_count(inventoryOUT) - 1);

    if(inventory_rearrange_objects(inventoryOUT) == ERROR)
        return ERROR;

    /*Adds the object to the other inventory(that should actually be rearranged)*/
    inventory_set_object_count(inventoryIN, inventory_get_object_count(inventoryIN) + 1);

    return OK;
}

int inventory_contains_object(Inventory *inventory, Id object){

    Object *obj = NULL;

    for (int i = 0; i < INVENTORY_MAX_SIZE; i++)
    {
        obj = inventory_get_object_from_index(inventory,i);
        if(obj == NULL)
            continue;
        
        if(object_get_id(obj) == object)
            return i;
    }
    return -1;
}

#pragma endregion

#pragma endregion