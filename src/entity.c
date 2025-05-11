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


/**
 * @brief This struct stores all the information of an entity
 */
struct _Entity {
    EntityType entityType;    /*!< Type of the entity */
    char name[WORD_SIZE + 1]; /*!< Name of the entity */
    Id id;                    /*!< Unique id of the entity */
    Id location;              /*!< Id of the space where the entity is located*/
    Inventory *inventory;     /*!< entity inventory */

    Ability **ability;        /*!< Abilities of the entity*/
    int n_ability;            /*!< Number of ability the entity has*/

    char gdesc[ENTITY_GRAPHIC_LENGTH + 1]; /*!< Graphic description of the entity*/

    EntityStats stats;       /*!< Entity combat stats */
};

/*
    * Private functions
*/

/**
 * @brief Removes an ability from an entity
 * @author Maksym Polyak;
 * 
 * @param entity entity struct
 * @param ability to be removed
 * 
 * @return Status
 */
Status entity_remove_ability(Entity *entity, Ability *ability);

/**
 * @brief Gets an ability from an entity by its name
 * @author Maksym Polyak
 * 
 * @param entity entity struct
 * @param name name of the ability
 * @return Ability* or NULL if error
 */
Ability *entity_get_ability_by_name(Entity *entity, char *name);

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

/*
 * Entity public implementation
*/

Entity *entity_create(char *name, Id identity, Id idlocation, InventoryType inventoryType){
    Entity *entity = NULL;

    if(name == NULL){
        debug_log(LOG_ERROR, "Unasigned string when creating entity");
        return NULL;
    }

    if(idlocation <= UNDEFINED_ID){
        debug_log(LOG_ERROR, "Invalid idlocation when creating entity");
        return NULL;
    }

    entity = (Entity *)calloc(1,sizeof(Entity));
    if(entity == NULL){
    debug_log(LOG_ERROR, "Error allocating memory for entity");
        return NULL;
    }

    entity->inventory = inventory_create(inventoryType, identity);
    if((entity->inventory) == NULL){
        free(entity);
        return NULL;
    }

    /*Abilities are not initialized on the entity creation, they are added later*/
    entity->ability = (Ability **)calloc(MAX_SKILLS_ENTITY, sizeof(Ability *));
    if((entity->ability) == NULL){
        inventory_destroy(entity->inventory);
        free(entity);
        return NULL;
    }
    entity->n_ability = 0;

    if(inventoryType == PLAYER_INVENTORY){
        entity->entityType = PLAYER_TYPE;
    } else if(inventoryType == NPC_INVENTORY){
        entity->entityType = NPC_TYPE;
    }

    entity_set_graphic_description(entity, "ERR");

    entity_set_id(entity, identity);
    entity_set_name(entity, name);
    entity_set_location(entity, idlocation);
    
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
    if(entity->stats.health > maxhealth){
        entity->stats.health = maxhealth;
    }
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

Status entity_set_stats(Entity *ent, double maxhealth, double health, double baseDamage, int strength, int defense, int magicLevel){
    if(!ent) return ERROR;
    return entity_stats_set_all(&ent->stats, maxhealth, health, baseDamage, strength, defense, magicLevel);
}

Status entity_stats_set_all(EntityStats *es, double maxhealth, double health, double baseDamage, int strength, int defense, int magicLevel){
    if(!es) return ERROR;
    
    es->maxhealth = maxhealth;
    es->health = health;
    es->baseDamage = baseDamage;
    es->strength = strength;
    es->defense = defense;
    es->magicLevel = magicLevel;
    
    return OK;
}

/*Entity GETTERS*/

bool entity_stats_is_dead(EntityStats *stats){
    if(!stats) return true;
    return stats->health <= 0;
}

bool entity_is_dead(Entity *entity){
    if(!entity) return true;
    return entity->stats.health <= 0;
}

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
    
    if(!entity || !ability || entity->n_ability >= MAX_SKILLS_ENTITY || entity->n_ability < 0) 
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

Ability *entity_get_ability_at(Entity *entity, int index){
    if(!entity || index < 0 || index > entity->n_ability)
        return NULL;

    return entity->ability[index];
}

char *entity_get_ability_name_at(Entity *entity, int index){
    if(!entity)
        return NULL;

    return ability_get_name(entity->ability[index]);
}

int entity_get_n_abilities(Entity *entity){
    if(!entity)
        return -1;

    return entity->n_ability;
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

/*
    * Entity general functions
*/

int entity_compare(void *ent1, void *ent2){
    Entity *entity1 = NULL, *entity2 = NULL;
    
    if(!ent1 || !ent2) return 0;

    entity1 = (Entity *)ent1;
    entity2 = (Entity *)ent2;

    if(entity1->entityType != entity2->entityType){
        return strcmp(entity1->name, entity2->name);
    }

    return entity1->id - entity2->id;
}