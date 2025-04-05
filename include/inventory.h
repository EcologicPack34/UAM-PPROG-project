/**
 * @brief It defines the inventory module interface
 *
 * The inventory is defined by a location id, which is associated
 * to its location, if the inventorytype is the one of a player, then
 * the id corresponds to the one of an entity of type player, same will 
 * happen with NPCs and spaces.
 * 
 * Functions related to inventory can add, remove and move objects between two inventories.
 * 
 * @file inventory.h
 * @author Maksym Polyak
 * @version 0
 * @date 10-02-2025
 * @copyright GNU Public License
*/

#ifndef INVENTORY_H
#define INVENTORY_H
 
#include "types.h"
#include "object.h"
#include "collection.h"
 
#include <stdbool.h>

#define INVENTORY_PLAYER_MAX_SIZE 10    /*!< Maximum inventory space for a player*/
#define INVENTORY_ENTITY_MAX_SIZE 10    /*!< Maximum inventory space for an entity*/
#define INVENTORY_SPACE_MAX_SIZE 50     /*!< Maximum inventory space for a space*/
 
/**
 * @brief ADT with all the information of the objects related to an entity, or space
 */
typedef struct _Inventory Inventory;

/*Game public interface functions*/

/**
 * @brief Creates an inventory with its type and locationid
 * @author Maksym Polyak
 *
 * @param type contains the inventory type of an inventory
 * @param locationid id with the id location of the inventory
 * @return inventory initialized without objects or NULL if error
 */
Inventory *inventory_create(InventoryType type, Id locationid);

/**
 * @brief Frees an inventory but not its objects (Objects are controled in game->objects)
 * @author Maksym Polyak
 *
 * @param inventory contains the inventory information
 */
void inventory_destroy(Inventory *inventory);

/*Object GETTERS*/

/**
 * @brief Gets an object from an inventory by its id
 * @author Maksym Polyak
 *
 * @param inventory contains the inventory information
 * @param objectid id of the object
 * @return object pointer if it went well or NULL if there was a mistake (NULL if the object isn't in the inventory)
 */
Object *inventory_get_object_by_id(Inventory *inventory, Id objectid);

/**
 * @brief Gets the object on the object array at the position index
 * 
 * @param inventory struct with all the information related to the inventory
 * @param index index with the position of the object
 * @return Object* if well or NULL if error
 */
Object *inventory_get_object_at(Inventory *inventory, long index);

/**
 * @brief Gets an object from an inventory by its name
 * 
 * @param inventory contains the inventory information
 * @param objectname string with the name of the object
 * @return Object* if found or NULL if not found;
 */
Object *inventory_get_object_by_name(Inventory *inventory, char *objectname);

/**
 * @brief Gets the collection of an inventory
 * @author Maksym Polyak
 *
 * @param inventory inventory where the objects are located
 * @return Collection* or NULL if error
 */
Collection *inventory_get_collection(Inventory *inventory);

/**
 * @brief Gets the inventory object list as a string from the inventory
 * @author Maksym Polyak
 *
 * @param inventory inventory where the objects are located
 * @param objectlist string where the object list is going to be located
 * @param mode mode of the function
 * @param length number of obects listed
 * @return OK if well or ERROR if error
 */
Status inventory_get_object_list(Inventory *inventory, char *objectlist, int mode, int length);

/**
 * @brief Gets a str without \n with the id of the object and its name
 * @author Maksym Polyak
 *
 * @param inventory inventory struct
 * @param objectdescr where the object description is saved
 * @param index index from the object on the inventory
 * @return Status 
 */
Status inventory_get_object_str_at(Inventory *inventory, char *objectdescr, int index);

/**
 * @brief Gets the inventory number of objects
 * @author Maksym Polyak
 *
 * @param inventory inventory where the objects are located
 * @return long if well or -1 if error
 */
long inventory_get_size(Inventory *inventory);

/**
 * @brief Gets an object by its id on an inventory
 * @author Maksym Polyak
 *
 * @param inventory inventory where the objects are located
 * @param objectid id of the object
 * @return Object* if well or NULL if ERROR;
 */
Object *inventory_get_object(Inventory *inventory, Id objectid);

/*Inventory set functions*/

/**
 * @brief Adds an object to an inventory
 * @author Maksym Polyak
 *
 * @param inventory contains the inventory information
 * @param object object to be added to the inventory
 * @return OK if the object has been added(OK if the objet was in the inventory) or ERROR if there was a mistake
 */
Status inventory_add_object(Inventory *inventory, Object *object);

/**
 * @brief Removes an object from an inventory (Does not free the object)
 * @author Maksym Polyak
 *
 * @param inventory contains the inventory information
 * @param object object to be removed from the inventory
 * @return OK if the object has been removed (OK if the object wasn't in the inventory) or ERROR if there was a mistake 
 */
Status inventory_remove_object(Inventory *inventory, Object *object);

/**
 * @brief Moves an object from inventoryOUT to inventoryIN by the id of the object
 * @author Maksym Polyak
 *
 * @param inventoryOUT inventory from where the object is taken
 * @param inventoryIN inventory where the object is going to be located
 * @param objectid id of the object in inventoryOUT to move
 * @return OK if the object was moved or ERROR if there was a mistake (ERROR also if the object to move was not in inventoryOUT)
 */
Status inventory_move_object(Inventory *inventoryOUT, Inventory *inventoryIN, Id objectid);

/**
 * @brief Checks if an object is in the inventory by id
 * @author Maksym Polyak
 *
 * @param inventory contains the inventory information
 * @param objectid id of the object
 * @return 1 if it is in the inventory or 0 if not(ERROR also is set as 0)
 */
bool inventory_contains_object(Inventory *inventory, Id objectid);

/**
 * @brief Returns the number of objects with such name, and the objects -- TO BE IMPLEMENTED
 * @author Sofía Calvo
 *
 * @param inventory contains the inventory information
 * @param num id of the object
 * @return 1 if it is in the inventory or 0 if not(ERROR also is set as 0)
 */
//Object *inventory_get_object_and_num_by_name(Inventory *inv, int *num, char *name)

#endif