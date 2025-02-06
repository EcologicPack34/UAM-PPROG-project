/**
 * @brief It defines the object module interface
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

typedef struct _Inventory Inventory;

/*Object public functions*/
#pragma region OBJECTS

/**
 * @brief It creates an object and returns its pointer
 * @author Maksym Polyak
 *
 * @param id id of the object that is created
 * @param name name of the object
 * @return a new object, initialized or NULL if there was a mistake
 */
Object *object_create(Id id, char *name);

/**
 * @brief It destroys an object
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 */
void object_destroy(Object *object);

/**
 * @brief Compares two objects and returns if their Id is equal or not
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 * @return 1 if the two objects are equal, 0 if they are not equal or -1 if there was a mistake
 */
int object_isEqual(Object *object1, Object *object2);

/*Object SETTERS*/
#pragma region SETTERS

/**
 * @brief Sets the object id to the one received as an argument
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 * @param id id of the object
 * @return OK if everything went fine or ERROR if there was a mistake
 */
Status object_set_id(Object* object, Id id);

/**
 * @brief Sets the object name to the one received as an argument
 * @author Maksym Polyak
 *
 * @param object contains the information of an object
 * @param name name of the object
 * @return OK if everything went fine or ERROR if there was a mistake
 */
Status object_set_name(Object* object, char* name);

#pragma endregion

/*Object GETTERS*/
#pragma region GETTERS

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

#pragma endregion

#pragma endregion

/*Inventory public functions*/
#pragma region INVENTORY

/**
 * @brief Creates an inventory
 * @author Maksym Polyak
 *
 * @return inventory pointer if everything went fine or NULL if there was a mistake
 */
Inventory *inventory_create();

/**
 * @brief Frees an inventory
 * @author Maksym Polyak
 *
 * @param inventory stores the information of an inventory
 */
void inventory_destroy(Inventory *inventory);

/*Inventory interactions with objects*/
#pragma region INVENTORY_OBJECT_INTERACTION

/**
 * @brief Adds an object to the inventory
 * @author Maksym Polyak
 *
 * @param inventory contains the information of an inventory
 * @param object contains the information of an object
 * @return OK if everything went fine or ERROR if there was a mistake
 */
Status inventory_add_object(Inventory *inventory, Object *object);


/**
 * @brief Removes an object from the inventory
 * @author Maksym Polyak
 *
 * @param inventory contains the information of an inventory
 * @param object contains the information of an object
 * @return OK if everything went fine or ERROR if there was a mistake
 */
Status inventory_remove_object(Inventory *inventory, Object *object);

/**
 * @brief Moves an object from an inventory to another (NOT COPYING)
 * @author Maksym Polyak
 *
 * @param inventoryOUT inventory where the object is taken
 * @param inventoryIN inventory where the object is received
 * @param object contains the information of an object
 * @return OK if everything went fine or NULL if there was a mistake
 */
Status inventory_object_move(Inventory *inventoryOUT, Inventory *inventoryIN, Object *object);

/**
 * @brief Checks if an object is in a invetory by its id
 * 
 * @param inventory 
 * @param object 
 * @return The index of the object if found, -1 if not
 */
int inventory_contains_object(Inventory *inventory, Id object);

#pragma endregion

/**
 * @brief NON IMPLEMENTED - Prints on screen all the objects
 * @author Maksym Polyak
 *
 * @param inventory stores the information of an inventory
 */
/*
void inventory_print(Inventory *inventory){}
*/

#pragma endregion

#endif