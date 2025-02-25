/**
 * @brief It implements the object module
 * 
 * An object ADT serves as the foundation to the item managment for the player,
 * right now each object just saves its name, location and unique id, but it will
 * be useful in next iterations to add special effects to objects, conditions and more.
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

/**
 * @brief Object
 *
 * This struct stores all the information of an object
 */
struct _Object {
  Id id;                    /*!< Id number of the object, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the object */
  Id location;              /*!< Id with the location of the object */
  InventoryType type;       /*!< InventoryType where the object is located */
};

/**
   Game interface implementation
*/


/*Object public functions*/

Object *object_create(Id id, char *name, InventoryType type){
    Object *object = NULL;

    if(!(object = (Object *)calloc(1,sizeof(Object))))
        return NULL;
    
    object->id = id;
    strcpy(object->name,name);
    object->type = type;
    
    return object;
}

void object_destroy(Object *object){
    if(!object)
        return;

    free(object);
    object = NULL;
}

int object_isEqual(void *object1, void *object2){
    if(!object1 || !object2)
        return -1;

    if(object_get_id((Object *)object1) == NO_ID || object_get_id((Object *)object2) == NO_ID)
        return -1;

    if((object_get_id((Object *)object1) == object_get_id((Object *)object2)))
        return 1;
    
    return 0;
}

/*Object SETTERS*/

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

Status object_set_location(Object* object, Id id){
    if(!object)
        return ERROR;

    object->location = id;

    return OK;
}

Status object_set_type(Object *object, InventoryType type){
    if(!object)
        return ERROR;
    
    object->type = type;

    return OK;
}

/*Object GETTERS*/

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

Id object_get_location(Object* object){
    if(!object)
        return -1;

    return object->location;
}

InventoryType object_get_type(Object *object){
    if(!object)
        return NULL;

    return object->type;
}

void object_print(void *object){
    
    printf("\n\n-------------\n\n");

    printf("=> Object:  \n");
    

    printf("=> Object id: %d\n", (int)object_get_id((Object *)object));
    printf("=> Object name: %s\n", object_get_name((Object *)object));
}
