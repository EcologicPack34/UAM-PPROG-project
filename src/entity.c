/**
 * @brief It implements the entity module
 * 
 * The entity module is created but further functions will be added, it is a simple ADT
 * to control the common parts of characters in game.
 *
 * @file entity.c
 * @author Maksym Polyak
 * @version 0
 * @date 06-02-2025
 * @copyright GNU Public License
 */

#include "entity.h"
#include "debug_printing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_NAME ""

/**
 * @brief Entity
 * @author Maksym Polyak
 *
 * This struct stores all the information of an entity
 */
struct _Entity {
    EntityType entityType;    /*!< Type of the entity */
    char name[WORD_SIZE + 1]; /*!< Name of the entity */
    Id id;                    /*!< Unique id of the entity */
    Id location;              /*!< Id of the space where the entity is located*/
    Inventory *inventory;     /*!< entity inventory */

    Entity_Stats stats;       /*!< Entity combat stats */
};



/*
 * Entity public implementation
*/

Entity *entity_create(char *name, Id id, Id location, InventoryType inventoryType, double health, double baseDamage, int strength, int defense, int magicLevel){
    Entity *entity = NULL;

    if(name == NULL){
        debug_log(LOG_ERROR, "Unasigned string when creating entity");
        return NULL;
    }

    entity = (Entity *)calloc(1,sizeof(Entity));
    if(entity == NULL){
    debug_log(LOG_ERROR, "Error allocating memory for entity");
        return NULL;
    }

    entity->inventory = inventory_create(inventoryType, id);
    if((entity->inventory) == NULL){
        free(entity);
        return NULL;
    }


    entity_set_id(entity, id);
    entity_set_name(entity, name);
    entity_set_location(entity, location);
    
    entity_set_health(entity, health);
    entity_set_baseDamage(entity, baseDamage);
    entity_set_strength(entity, strength);
    entity_set_defense(entity, defense);
    entity_set_magicLevel(entity, magicLevel);

    return entity;
}

void entity_destroy(Entity *entity){
    if(!entity)
        return;
    
    inventory_destroy(entity->inventory);
    free(entity);
    entity = NULL;
}

/*Entity SETTERS*/

Status entity_set_name(Entity *entity, char *name){
    if(!entity)
        return ERROR;
    
    strcpy(entity->name, name);
    return OK;
}

Status entity_set_location(Entity *entity, Id id){
    if(!entity)
        return ERROR;
    
    entity->location = id;
    return OK;
}

Status entity_set_id(Entity *entity, Id id){
    if(!entity)
        return ERROR;
    
    entity->id = id;
    return OK;
}

Status entity_set_entityType(Entity *entity, EntityType entityType){
    if(!entity)
        return ERROR;
    
    entity->entityType = entityType;
    return OK;
}

Status entity_set_health(Entity *entity, double health){
    if(!entity)
        return ERROR;

    entity->stats.health = health;

    return OK;
}

Status entity_set_baseDamage(Entity *entity, double baseDamage){
    if(!entity)
        return ERROR;
    
    entity->stats.baseDamage = baseDamage;
    return OK;
}

Status entity_set_strength(Entity *entity, int strength){
    if(!entity)
        return ERROR;

    entity->stats.strength = strength;

    return OK;
}

Status entity_set_defense(Entity *entity, int defense){
    if(!entity)
        return ERROR;

    entity->stats.defense = defense;
    return OK;
}

Status entity_set_magicLevel(Entity *entity, int magicLevel){
    if(!entity)
        return ERROR;

    
    entity->stats.magicLevel = magicLevel;

    return OK;
}

/*Entity GETTERS*/

char *entity_get_name(Entity *entity){
    if(!entity)
        return NULL;
    
    return entity->name;
}

Id entity_get_location(Entity *entity){
    if(!entity)
        return NO_ID;
    
    return entity->location;
}

Inventory *entity_get_inventory(Entity *entity){
    if(!entity)
        return NULL;
    
    return entity->inventory;
}

Id entity_get_id(Entity *entity){
    if(!entity)
        return NO_ID;
    
    return entity->id;
}

EntityType entity_get_entityType(Entity *entity){
    if(!entity)
        return UNKNOWN_ENTITY;

    return entity->entityType;
}

double entity_get_health(Entity *entity){
    if(!entity)
        return -1;

    return entity->stats.health;
}

double entity_get_baseDamage(Entity *entity){
    if(!entity)
        return -1;
    
    return entity->stats.baseDamage;
}

int entity_get_strength(Entity *entity){
    if(!entity)
        return -1;

    return entity->stats.strength;
}

int entity_get_defense(Entity *entity){
    if(!entity)
        return -1;

    return entity->stats.defense;
}

int entity_get_magicLevel(Entity *entity){
    if(!entity)
        return -1;

    
    return entity->stats.magicLevel;
}

