/**
 * @brief It implements the conditions module, to define the game status and when it finishes
 *
 * @file condition.c
 * @author Maksym Polyak
 * @version 0
 * @date 09-02-2025
 * @copyright GNU Public License
*/

#include "condition.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
struct _Conditions{
    Object *object_location_conditioned[MAX_OBJECTS];
    int n_objects_conditioned;
};

/*
 * Private functions
*/

/**
 * @brief It sets a number of objects conditioned by location
 * @author Maksym Polyak
 *
 * @param conditions struct with the conditions
 * @param num num of objects conditioned
 * @return OK or ERROR if there was a mistake
 */
Status condition_set_n_objects_conditioned(Conditions *conditions, int num){
    if(!conditions)
        return ERROR;
    
    conditions->n_objects_conditioned = num;

    return OK;
}

/**
 * @brief Sets the object on the index of conditioned objects
 * @author Maksym Polyak
 *
 * @param conditions struct with the conditions
 * @param object conditioned object
 * @param index index received
 * @return OK or ERROR if there was a mistake
 */
Status condition_set_objects_conditioned_at(Conditions *conditions, Object *object, int index){
    if(!conditions || !object)
        return ERROR;
    
    (conditions->object_location_conditioned)[index] = object;

    return OK;
}

/*
 * Game interface implementation
*/

Conditions *conditions_create(){
    Conditions *conditions = NULL;

    conditions = (Conditions *)malloc(sizeof(Conditions));
    if(!conditions)
        return NULL;

    condition_set_n_objects_conditioned(conditions, 0);

    return conditions;
}

void conditions_destroy(Conditions *conditions){
    free(conditions);
}

Status conditions_add_object_conditioned(Conditions *conditions, Object *object){
    int n_conditions;
    
    if(!conditions || !object)
        return ERROR;
    
    n_conditions = condition_get_n_objects_conditioned(conditions);
    condition_set_objects_conditioned_at(conditions, object, n_conditions);

    condition_set_n_objects_conditioned(conditions, n_conditions + 1);

    return OK;
}

int condition_get_n_objects_conditioned(Conditions *conditions){
    if(!conditions)
        return -1;
    
    return conditions->n_objects_conditioned;
}

Object **condition_get_objects_conditioned(Conditions *conditions){
    if(!conditions)
        return NULL;
    
    return conditions->object_location_conditioned;
}

Object *condition_get_objects_conditioned_at(Conditions *conditions, int index){
    if(!conditions)
        return NULL;
    
    return (conditions->object_location_conditioned)[index];
}

bool conditions_check(Conditions *conditions){
    int i, num_cond;
    Object *object = NULL;
    
    if(!conditions)
        return 0;

    num_cond = condition_get_n_objects_conditioned(conditions);
    for(i = 0; i < num_cond; i++){
        object = condition_get_objects_conditioned_at(conditions, i);
        if(object_get_condition_location(object) != object_get_location(object))
            return 0;
    }

    return 1;
}