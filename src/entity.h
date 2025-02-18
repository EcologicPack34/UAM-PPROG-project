/**
 * @brief It defines the entity module interface
 * 
 * An entity is the base ADT which stores essential information about Non-Playables Characters,
 * players and everything that moves. For now, each entity is defined by their type (EntityType),
 * a name, a unique id between entities, a location and an own inventory.
 * 
 * Right now the only entity that exists is the player, but in further iterations, NPCs functionality
 * will be added and this module will change, in the future it will store stats and other information
 * like health, defense, magic... To implement the combat module.
 *
 * @file entity.h
 * @author Maksym Polyak
 * @version 0
 * @date 06-02-2025
 * @copyright GNU Public License
 */

#ifndef ENTITY_H
#define ENTITY_H

#include "inventory.h"

typedef enum {UNKNOWN_ENTITY, PLAYER, NPC} EntityType;

typedef struct _Entity Entity;

/*
 * Entity public implementation
*/

/**
 * @brief Creates a entity
 * @author Maksym Polyak
 *
 * @param name name of the entity
 * @param identity id of the entity
 * @param idlocation id of the location of the entity
 * @param inventoryType type of inventory of the entity
 * @return Entity pointer if everything goes well or NULL if there was a mistake
 */
Entity *entity_create(char *name, Id identity, Id idlocation, InventoryType inventoryType);

/**
 * @brief Frees a entity
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 */
void entity_destroy(Entity *entity);

/*Entity SETTERS*/

/**
 * @brief Sets a entity name with the name argument
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 * @param name name of the entity
 * @return OK if everything went well or ERROR if there was a mistake
 */
Status entity_set_name(Entity *entity, char *name);

/**
 * @brief Sets a entity location on the map
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 * @param id id where the entity is going to be placed
 * @return OK if everything went well or ERROR if there was a mistake
 */
Status entity_set_location(Entity *entity, Id id);

/**
 * @brief Sets an entity id
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 * @param id unique id of the entity
 * @return OK if everything went well or ERROR if there was a mistake
 */
Status entity_set_id(Entity *entity, Id id);

/**
 * @brief Sets an entity entityType
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 * @param entityType type of the entity
 * @return OK if everything went well or ERROR if there was a mistake
 */
Status entity_set_entityType(Entity *entity, EntityType entityType);

/*Entity GETTERS*/

/**
 * @brief Gets the name of the entity
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 * @return entity's name or NULL if there was a mistake
 */
char *entity_get_name(Entity *entity);

/**
 * @brief Gets the Id of the entity's location space
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 * @return entity's location Id or -1 if there was a mistake
 */
Id entity_get_location(Entity *entity);

/**
 * @brief Gets the inventory pointer from a entity
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 * @return inventory pointer or NULL if there was a mistake
 */
Inventory *entity_get_inventory(Entity *entity);

/**
 * @brief Gets the entity id
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 * @return entity id or NULL if there was a mistake
 */
Id entity_get_id(Entity *entity);

/**
 * @brief Gets an entity entityType
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 * @param entityType type of the entity
 * @return OK if everything went well or ERROR if there was a mistake
 */
EntityType entity_get_entityType(Entity *entity);

/*Entity PRINTERS*/

/**
 * @brief NON IMPLEMENTED - Prints the entity status
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 */
/*
void entity_print_status(Entity *entity);
*/

/**
 * @brief NON IMPLEMENTED - Prints the entity inventory
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 */
/*
void entity_print_inventory(Entity *entity);
*/

#endif