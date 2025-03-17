/**
 * @brief It defines the entity module interface
 * 
 * An entity is the base ADT which stores essential information about Non-Playables Characters,
 * players and everything that moves. For now, each entity is defined by their type (EntityType),
 * a name, a unique id between entities, a location and an own inventory.
 * 
 * Entity also saves the stats for each entity. The stats struct has not been made an ADT for the
 * reasons specified on the README.txt
 * 
 * Each entity saves its own graphic description for the graphic_engine
 *
 * @file entity.h
 * @author Maksym Polyak
 * @version 0
 * @date 06-02-2025
 * @copyright GNU Public License
 */

#ifndef ENTITY_H
#define ENTITY_H

#define ENTITY_GRAPHIC_LENGTH 3

#include "inventory.h"

typedef enum {UNKNOWN_ENTITY, PLAYER_TYPE, NPC_TYPE} EntityType;

typedef struct _Entity Entity;

/**
 * Struct that stores combat information about an entity
 */
typedef struct{
    double maxhealth;          /*!< Max health of the entity*/
    double health;             /*!< Health of the entity */
    double baseDamage;         /*!< Base damage of the entity */

    int strength;             /*!< Strength stat of the entity */
    int defense;              /*!< Defense stat of the entity */
    int magicLevel;           /*!< magicLevel stat of the entity */
}Entity_Stats;

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
 * @param maxhealth max health of the entity
 * @param health health of the entity
 * @param baseDamage base damage of the entity
 * @param strength strength of the entity
 * @param defense defense of the entity
 * @param magicLevel magic level of the entity
 * @return Entity pointer if everything goes well or NULL if there was a mistake
 */
Entity *entity_create(char *name, Id identity, Id idlocation, InventoryType inventoryType, double maxhealth, double health, double baseDamage, int strength, int defense, int magicLevel);

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

/**
 * @brief Sets an entity max health
 * @author Maksym Polyak
 * 
 * @param entity 
 * @param maxhealth 
 * @return Status 
 */
Status entity_set_max_health(Entity *entity, double maxhealth);

/**
 * @brief Sets the entity health
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @param health health to be set
 * @return Status 
 */
Status entity_set_health(Entity *entity, double health);

/**
 * @brief Sets the base damage
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @param baseDamage base damage to set
 * @return Status 
 */
Status entity_set_baseDamage(Entity *entity, double baseDamage);

/**
 * @brief Sets the strength stat
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @param strength strength stat to set
 * @return Status 
 */
Status entity_set_strength(Entity *entity, int strength);

/**
 * @brief Sets the defense stat
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @param defense defense stat to set
 * @return Status 
 */
Status entity_set_defense(Entity *entity, int defense);

/**
 * @brief Sets the magic level stat
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @param magicLevel magic level stat to set
 * @return Status 
 */
Status entity_set_magicLevel(Entity *entity, int magicLevel);

/**
 * @brief Sets the graphic descption of the entity to a given string
 * @author Daniel Gómez
 * 
 * @param entity 
 * @param gdesc 
 * @return Status 
 */
Status entity_set_graphic_description(Entity *entity, char *gdesc);

/*Entity GETTERS*/

/**
 * @brief Gets the graphic descption of the entity
 * @author Daniel Gómez
 * 
 * @param entity 
 * @param gdesc 
 * @return Reference to description 
 */
char *entity_get_graphic_description(Entity *entity);

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

/**
 * @brief Gets the entity max health
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @return health if well or -1 if wrong
 */
double entity_get_max_health(Entity *entity);

/**
 * @brief Gets the entity health
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @return health if well or -1 if wrong
 */
double entity_get_health(Entity *entity);


/**
 * @brief Gets the entity base damage
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @return baseDamage if well or -1 if wrong
 */
double entity_get_baseDamage(Entity *entity);

/**
 * @brief Gets the strength stat of the entity
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @return strength stat if well or -1 if wrong
 */
int entity_get_strength(Entity *entity);

/**
 * @brief Gets the defense stat of the entity
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @return defense stat if well or -1 if wrong
 */
int entity_get_defense(Entity *entity);

/**
 * @brief Gets the magic Level stat of the entity
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @return magicLevel stat if well or -1 if wrong
 */
int entity_get_magicLevel(Entity *entity);

#endif