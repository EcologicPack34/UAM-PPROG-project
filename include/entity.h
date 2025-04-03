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

#define ENTITY_GRAPHIC_LENGTH 3 /*!< Maximum length of an entity graphic description (without \n)*/

#include "inventory.h"
#include "ability_manager.h"
#include "types.h"

#define N_STATS 7 /*!< Number of stats implemented + NO_STAT*/

/**
 * @brief enum to identify where the entity is located
 */
typedef enum {UNKNOWN_ENTITY, PLAYER_TYPE, NPC_TYPE} EntityType;

/**
 * @brief enum with all the stats implemented on the game
 */
typedef enum {NO_STAT, MAX_HEALTH, HEALTH, BASE_DAMAGE, STRENGTH, DEFENSE, MAGIC_LEVEL} StatCode;

/**
 * @brief Entity ADT with all the common information to an entity(Players and NPCs common information types)
 */
typedef struct _Entity Entity;

/**
 * Struct that stores combat information about an entity
 */
typedef struct{
    double maxhealth;           /*!< Max health of the entity*/
    double health;              /*!< Health of the entity */
    double baseDamage;          /*!< Base damage of the entity */

    int strength;               /*!< Strength stat of the entity */
    int defense;                /*!< Defense stat of the entity */
    int magicLevel;             /*!< magicLevel stat of the entity */
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

/**
 * @brief Sets an entity max health
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @param maxhealth max health to be set
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
 * @param entity contains all the information related to the entity
 * @param gdesc graphic description to set
 * @return Status 
 */
Status entity_set_graphic_description(Entity *entity, char *gdesc);

/**
 * @brief This function sets the stats values of an Entity;
 * 
 * @param ent contains all the information related to the entity
 * @param maxhealth 
 * @param health 
 * @param baseDamage 
 * @param strength 
 * @param defense 
 * @param magicLevel 
 * @return Status 
 */
Status entity_set_stats(Entity *ent, double maxhealth, double health, double baseDamage, int strength, int defense, int magicLevel);


/**
 * @brief This function sets the values of an EntityStats
 * @author Aaron Charameli Mair
 * 
 * @param es a pointer to the Entity_Stats to modify
 * @param maxhealth 
 * @param health 
 * @param baseDamage 
 * @param strength 
 * @param defense 
 * @param magicLevel 
 * @return Status 
 */
Status entity_stats_set_all(Entity_Stats *es, double maxhealth, double health, double baseDamage, int strength, int defense, int magicLevel);

/*Entity GETTERS*/

/**
 * @brief Gets if an entity is dead or not
 * @author Daniel Gómez
 * 
 * @param entity contains all the information related to the entity
 * @return true if entity is dead, also true if there was an error
 * @return false if entity is alive
 */
bool entity_is_dead(Entity *entity);

/**
 * @brief Gets the graphic descption of the entity
 * @author Daniel Gómez
 * 
 * @param entity contains all the information related to the entity
 * @return Reference to description or NULL if error
 */
char *entity_get_graphic_description(Entity *entity);

/**
 * @brief Gets the name of the entity
 * @author Maksym Polyak
 *
 * @param entity contains all the information related to the entity
 * @return entity name or NULL if there was a mistake
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
 * @return EntityType or UNKNOWN_ENTITY if error
 */
EntityType entity_get_entityType(Entity *entity);

/**
 * @brief Adds a ability to the entity if it is not full
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @param ability ability to add
 * @return Status 
 */
Status entity_add_ability(Entity *entity, Ability *ability);

/**
 * @brief Gets the ability at the index of the entity
 * @author Maksym Polyak
 * 
 * @param entity contains all the information related to the entity
 * @param index index of the ability
 * @return Ability* or NULL if error or not found
 */
Ability *entity_get_ability_at(Entity *entity, int index);

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