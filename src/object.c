/**
 * @brief It implements the object module
 * 
 * An object ADT serves as the foundation to the item managment for the player,
 * right now each object just saves its name, location and unique id, but it will
 * be useful in next iterations to add special effects to objects, conditions and more.
 * 
 * If an object is inside an entity inventory, the location id of the object is the
 * same as the one the entity has
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
  char name[WORD_SIZE];     /*!< Name of the object */
  Id location;              /*!< Id with the location of the object */
  InventoryType type;       /*!< Inventory type where the object is located */

  char descr[WORD_SIZE];    /*!< Description of the object*/
};

/**
   Game interface implementation
*/


/*Object public functions*/
Object *object_create(Id id, char *name, char *description, Id location, InventoryType type){
    Object *object = NULL;

    if(!(object = (Object *)calloc(1,sizeof(Object))))
        return NULL;
    
    object->id = id;
    object->location = location;
    strcpy(object->name,name);
    strcpy(object->descr, description);
    
    return object;
}

void object_destroy(void *object){
    if(!object)
        return;

    free(object);
}

int object_isEqual(void *object1, void *object2){
    if(!object1 || !object2)
        return -1;

    if(object_get_id((Object *)object1) == NO_ID || object_get_id((Object *)object2) == NO_ID)
        return -1;

    if((object_get_id((Object *)object1) == object_get_id((Object *)object2)))
        return 0;
    
    return -1;
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

Status object_set_descr(Object *object, char *str){
    if(!object || !str)
        return ERROR;

    strcpy(object->descr, str);

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
        return 0;

    return object->type;
}

char *object_get_descr(Object *object){
    if(!object)
        return NULL;

    return object->descr;
}

void object_print(void *object){
    
    printf("\n\n-------------\n\n");

    printf("=> Object:  \n");
    

    printf("=> Object id: %d\n", (int)object_get_id((Object *)object));
    printf("=> Object name: %s\n", object_get_name((Object *)object));
}
