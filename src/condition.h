/**
 * @brief It defines the condition module interface
 *
 * @file condition.h
 * @author Maksym Polyak
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
*/

#ifndef CONDITION_H
#define CONDITION_H

#include "object.h"

typedef struct _Conditions Conditions;

/**
 * @brief It creates a conditions struct
 * @author Maksym Polyak
 *
 * @return a conditions struct initialized or NULL if there was a mistake
 */
Conditions *conditions_create();

/**
 * @brief Frees a condition struct
 * @author Maksym Polyak
 *
 * @param conditions struct with all the information about conditions
 */
void conditions_destroy(Conditions *conditions);

/**
 * @brief Adds an objet as a condition (location)
 * @author Maksym Polyak
 *
 * @param conditions struct with the conditions
 * @param num num of objects conditioned
 * @return OK if well or ERROR if there was a mistake
 */
Status conditions_add_object_conditioned(Conditions *conditions, Object *object);

/**
 * @brief It gets the number of objects conditioned
 * @author Maksym Polyak
 *
 * @param conditions struct with the conditions
 * @param num num of objects conditioned
 * @return n_objects_conditioned or -1 if there was a mistake
 */
int condition_get_n_objects_conditioned(Conditions *conditions);

/**
 * @brief Gets the object pointer of objects conditioned
 * @author Maksym Polyak
 *
 * @param conditions struct with the conditions
 * @return object pointer or NULL if there was a mistake
 */
Object **condition_get_objects_conditioned(Conditions *conditions);

/**
 * @brief Gets the object pointer of objects conditioned at index
 * @author Maksym Polyak
 *
 * @param conditions struct with the conditions
 * @param index index received
 * @return object pointer or NULL if there was a mistake
 */
Object *condition_get_objects_conditioned_at(Conditions *conditions, int index);

/**
 * @brief Chyecks if the finish conditions have been completed
 * @author Maksym Polyak
 *
 * @param conditions struct with the conditions
 * @return bool 1 if finished 0 if not
 */
bool conditions_check(Conditions *conditions);

#endif