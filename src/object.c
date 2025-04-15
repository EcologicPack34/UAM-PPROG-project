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

#define OBJECT_MAX_DATA_SIZE 200 /*!< Max data size of an object*/

/**
 * @brief ADT that stores all the information of an object
 */
struct _Object {
  Id id;                    /*!< Id number of the object, it must be unique */
  char name[WORD_SIZE];     /*!< Name of the object */
  Id location;              /*!< Id with the location of the object */
  InventoryType type;       /*!< Inventory type where the object is located */

  bool is_consumable;       /*!< Determines if object is removed after use*/
  bool is_equipped;         /*!< Determines if the object is equipped or not*/

  Ability *object_effect;   /*!< Ability with the effect of the object --> Assigned on skill read*/

  char *data;               /*!< Data with the effects of the object and where it can be equipped*/
  char descr[WORD_SIZE];    /*!< Description of the object*/
};

/**
   Game interface implementation
*/


/*Object public functions*/
Object *object_create(Id id, char *name, char* data, char *description, bool is_consumable, Id location, InventoryType type){
    Object *object = NULL;

    if((id <= UNDEFINED_ID) || !name || !data || !description || (location <= UNDEFINED_ID)) return NULL;

    if(!(object = (Object *)calloc(1,sizeof(Object))))
        return NULL;

    object->data = (char *)malloc(OBJECT_MAX_DATA_SIZE * sizeof(char) + 1);
    if(!object->data){
        free(object);
        return NULL;
    }
    strcpy(object->data, data);

    object->object_effect = NULL;
    object->is_consumable = is_consumable;
    object->is_equipped = false;
    
    object->id = id;
    object->location = location;
    object->type = type;
    strcpy(object->name,name);
    strcpy(object->descr, description);
    
    return object;
}

void object_destroy(void *object){
    Object *e = NULL;
    
    if(!object)
        return;

    e = (Object *)object;

    if(e->data)
        free(e->data);
    free(e);
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
    if(!object || (id<=UNDEFINED_ID))
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
    if(!object || (id<=UNDEFINED_ID))
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

Status object_set_is_equipped(Object *object, bool value){
    if(!object) return ERROR;

    object->is_equipped = value;

    return OK;
}

/*Object GETTERS*/

Id object_get_id(Object *object){
    if(!object)
        return NO_ID;

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

bool object_get_is_consumable(Object *object){
    if(!object)
        return false;
        
    return object->is_consumable;
}

Ability *object_get_object_effect(Object *object){
    if(!object) return NULL;

    return object->object_effect;
}

char *object_get_data(Object *object){
    if(!object) return NULL;

    return object->data;
}

bool object_get_is_equipped(Object *object){
    if(!object) return false;

    return object->is_equipped;
}

void object_print(void *object){
    
    printf("\n\n-------------\n\n");

    printf("=> Object:  \n");
    

    printf("=> Object id: %d\n", (int)object_get_id((Object *)object));
    printf("=> Object name: %s\n", object_get_name((Object *)object));
}

Status object_add_object_effect(Object *object, Ability *ability){
    if(!object || !ability) return ERROR;

    if(object->object_effect != NULL)
        return ERROR;

    object->object_effect = ability;

    return OK;
}