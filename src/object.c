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

/**
 * @brief Object
 *
 * This struct stores all the information of an object
 */
struct _Object {
  Id id;                    /*!< Id number of the object, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the object */
  Id location;              /*!< Id with the location of the object */
};

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

Status object_set_location(Object* object, Id id){
    if(!object)
        return ERROR;

    object->location = id;

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

Id object_get_location(Object* object){
    if(!object)
        return -1;

    return object->location;
}

#pragma endregion

void object_print(Object *object){
    
    printf("\n\n-------------\n\n");

    printf("=> Object:  \n");
    

    printf("=> Object id: %d\n", (int)object_get_id(object));
    printf("=> Object name: %s\n", object_get_name(object));
}

#pragma endregion