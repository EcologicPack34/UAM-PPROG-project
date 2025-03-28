/**
 * @brief It defines the object module interface
 *
 * The ADT object represents the base for the inventory and item interaction in game.
 * Each object is defined by a unique id, a name and a location and is the ADT that
 * regulates how the items work in the inventory ADT.
 *
 * All items will be stored in their corresponding inventory, but in the next iteration,
 * object pointers will be organized also by an array on the collection module(set module),
 * the same way as it is implemented on the game struct but with an array.
 *
 * @file object.h
 * @author Maksym Polyak
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "types.h"

#include <stdbool.h>

typedef struct _Object Object;

/*Object public functions*/

/**
 * @brief It creates an object and returns its pointer
 * @author Maksym Polyak
 *
 * @param id id of the object that is created
 * @param name name of the object
 * @param data data with the effects of the object and if it can be weared
 * @param description description of the object
 * @param location id where the object is located
 * @param type type of inventory where the object is located
 * @return a new object, initialized or NULL if there was a mistake
 */
Object *object_create(Id id, char *name, char *data, char *description, Id location, InventoryType type);

/**
 * @brief It destroys an object
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 */
void object_destroy(void *object);

/**
 * @brief Compares two objects and returns if their Id is equal or not
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 * @return 1 if the two objects are not equal.
 * @return 0 if they are equal.
 * @return -1 if there was a mistake.
 */
int object_isEqual(void *object1, void *object2);

/*Object SETTERS*/

/**
 * @brief Sets the object id to the one received as an argument
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 * @param id id of the object
 * @return OK if everything went fine or ERROR if there was a mistake
 */
Status object_set_id(Object *object, Id id);

/**
 * @brief Sets the object name to the one received as an argument
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 * @param name name of the object
 * @return OK if everything went fine or ERROR if there was a mistake
 */
Status object_set_name(Object *object, char *name);

/**
 * @brief Sets the object location to the one received as an argument
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 * @param id id of the space where the object is going to be located
 * @return OK if everything went fine or ERROR if there was a mistake
 */
Status object_set_location(Object *object, Id id);

/**
 * @brief Sets the inventorytype inside the object struct
 * @author Maksym Polyak
 * 
 * @param object contains the information of an object
 * @param type type to set
 * @return Status 
 */
Status object_set_type(Object *object, InventoryType type);

/**
 * @brief Sets the description for an object, copies the content from str on the
 * object description information
 * @author Maksym Polyak
 * 
 * @param object 
 * @param str 
 * @return Status 
 */
Status object_set_descr(Object *object, char *str);

/*Object GETTERS*/

/**
 * @brief Gets the id of an object
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 * @return id of the object or -1 if there was a mistake
 */
Id object_get_id(Object *object);

/**
 * @brief Gets the object name
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 * @return name of the object or NULL if there was a mistake
 */
char *object_get_name(Object *object);

/**
 * @brief Gets the object location
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 * @return id with the space id where the object is located or -1 if there was a mistake
 */
Id object_get_location(Object *object);

/**
 * @brief Gets the inventorytype where the object is located
 * @autho Maksym Polyak
 * 
 * @param object object to be evaluated
 * @return InventoryType or 0 if error
 */
InventoryType object_get_type(Object *object);

/**
 * @brief Gets the object description pointer
 * @author Maksym Polyak
 * 
 * @param object 
 * @return char* or NULL if error
 */
char *object_get_descr(Object *object);

/**
 * @brief Prints on screen an object
 * @author Maksym Polyak
 *
 * @param inventory stores the information of an inventory
 */
void object_print(void *object);

#endif