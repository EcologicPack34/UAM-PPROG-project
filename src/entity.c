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
#include "ability_manager.h"

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

    Ability **ability; /*!< Abilitys of the entity*/
    int n_ability;   /*!< Number of ability the entity has*/

    char gdesc[ENTITY_GRAPHIC_LENGTH + 1];

    Entity_Stats stats;       /*!< Entity combat stats */
};



/*
 * Entity public implementation
*/

Entity *entity_create(char *name, Id id, Id location, InventoryType inventoryType){
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

    /*Abilitys are not initialized on the entity creation, they are added later*/
    entity->ability = (Ability **)calloc(MAX_SKILLS_ENTITY, sizeof(Ability *));
    if((entity->ability) == NULL){
        inventory_destroy(entity->inventory);
        free(entity);
        return NULL;
    }
    entity->n_ability = 0;

    entity_set_graphic_description(entity, "ERR");
    entity_set_entityType(entity, UNKNOWN_ENTITY);




    entity_set_id(entity, id);
    entity_set_name(entity, name);
    entity_set_location(entity, location);
    
    entity_set_max_health(entity, 1);
    entity_set_health(entity, 1);
    
    entity_set_baseDamage(entity, 1);
    entity_set_strength(entity, 1);
    entity_set_defense(entity, 1);
    entity_set_magicLevel(entity, 1);

    return entity;
}

void entity_destroy(Entity *entity){
    if(!entity)
        return;
    
    free(entity->ability);
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

Status entity_set_max_health(Entity *entity, double maxhealth){
    if(!entity)
        return ERROR;
    
    entity->stats.maxhealth = maxhealth;
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

Status entity_set_graphic_description(Entity *entity, char *gdesc){
    if(!entity || !gdesc) return ERROR;
    strncpy(entity->gdesc, gdesc, ENTITY_GRAPHIC_LENGTH);
    return OK;
}

/*Entity GETTERS*/

char *entity_get_graphic_description(Entity *entity){
    if(!entity) return NULL;
    return entity->gdesc;
}

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

Status entity_add_ability(Entity *entity, Ability *ability){
    int i;
    
    if(!entity || !ability || entity->n_ability > MAX_SKILLS_ENTITY || entity->n_ability < 0) 
        return ERROR;

    for(i = 0; i < MAX_SKILLS_ENTITY; i++){
        if(entity->ability[i] == NULL){
            entity->ability[i] = ability;
            entity->n_ability++;
            return OK;
        }
    }

    return OK;
}

Status entity_remove_ability(Entity *entity, Ability *ability){
    int i;
    
    if(!entity || !ability || entity->n_ability == 0) 
        return ERROR;

    for(i = 0; i < entity->n_ability; i++){
        if(entity->ability[i] == ability){
            entity->ability[i] = NULL;
            entity->n_ability--;
            return OK;
        }
    }

    return OK;
}

Ability *entity_get_ability_at(Entity *entity, int index){
    if(!entity)
        return NULL;

    return entity->ability[index];
}

Ability *entity_get_ability_by_name(Entity *entity, char *name){
    int i;
    
    if(!entity || !name)
        return NULL;

    for(i = 0; i < entity->n_ability; i++){
        if(strcpy(ability_get_name(entity->ability[i]), name) == 0)
            return entity->ability[i];
    }

    return NULL;
}

double entity_get_max_health(Entity *entity){
    if(!entity)
        return -1;
    
    return entity->stats.maxhealth;
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

