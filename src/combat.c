/**
 * @file combat.c
 * @author Maksym Polyak
 * @brief This module implements all the functionality from combat.h
 * @version 0.1
 * @date 2025-03-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "combat.h"
#include <stdio.h>
#include <stdlib.h>


struct _Combat{
    PlayerStats player_stats;               /*!< Stats of the player*/
    Stats allies_stats[NPC_MAX_ALLIES];     /*!< Stats of the allies*/
    int allies_count;                       /*!< Number of allies*/
    Stats enemies_stats[NPC_MAX_ENEMIES];   /*!< Stats of the enemies*/
    int enemies_count;                      /*!< Number of enemies*/

    bool is_player_turn;                    /*!< Determines who starts, true --> player party, false --> enemy party*/
    Space *space;                           /*!< Space where the combat is located*/

    bool endCombat;
};


/*
    * PRIVATE FUNCTIONS
*/

/**
 * @brief Copies the entity stats to a new stats struct for the combat
 * @author Maksym Polyak
 * 
 * @param entity 
 * @param stats
 * @return Status
 */
Status combat_copy_entity_stats(Entity *entity, Stats *stats);

/**
 * @brief Copies the player stats and other information to the combat struct
 * @author Maksym Polyak
 * 
 * @param player 
 * @param stats 
 * @return Status 
 */
Status combat_copy_player(Player *player, PlayerStats *stats);

void combat_finalize(Combat *combat){
    int i;
    combat->endCombat = true;

    entity_set_health(combat->player_stats.entity ,combat->player_stats.stats.health);
    for (i = 0; i < combat->allies_count; i++)
    {
        entity_set_health(combat->allies_stats[i].entity ,combat->allies_stats[i].stats.health);
    }
    for (i = 0; i < combat->enemies_count; i++)
    {
        entity_set_health(combat->enemies_stats[i].entity ,combat->enemies_stats[i].stats.health);
    }

}

Status combat_copy_entity_stats(Entity *entity, Stats *stats){

    if(!entity || !stats)    return ERROR;

    stats->stats.health = entity_get_health(entity);
    stats->stats.baseDamage = entity_get_baseDamage(entity);
    stats->stats.strength = entity_get_strength(entity);
    stats->stats.defense = entity_get_defense(entity);
    stats->stats.magicLevel = entity_get_magicLevel(entity);

    stats->entity = entity;

    return OK;
}

Status combat_copy_player(Player *player, PlayerStats *stats){
    Entity *entity = NULL;

    if(!player || !stats)
        return ERROR;

    entity = player_get_entity(player);
    
    stats->stats.health = entity_get_health(entity);
    stats->stats.baseDamage = entity_get_baseDamage(entity);
    stats->stats.strength = entity_get_strength(entity);
    stats->stats.defense = entity_get_defense(entity);
    stats->stats.magicLevel = entity_get_magicLevel(entity);

    stats->entity = entity;

    return OK;
}

Status combat_player_turn(Combat *combat){

    if(!combat)
        return ERROR;

    return OK;
}

Status combat_ally_turn(Combat *combat, int index){

    if(!combat)
        return ERROR;

    return OK;
}

Status combat_enemy_turn(Combat *combat, int index){

    if(!combat)
        return ERROR;

    return OK;
}


/*
    * PUBLIC FUNCTIONS
*/

Combat *combat_initialize(Space *space, Player *player, CommandCode code){
    Combat *combat = NULL;
    NPC *npc = NULL;
    int i, npc_count = 0, npc_allies = 0, npc_enemies = 0;

    if(!space || !player)
        return NULL;

    
    combat = (Combat *)calloc(1,sizeof(Combat));
    if(!combat) return NULL;
    /*Saves the player stats on the combat struct*/
    combat_copy_player(player, &(combat->player_stats));
    

    npc_count = space_get_npc_count(space);

    /*Saves the first four enemies and the first three allies on the combat struct*/
    for(i = 0; i < npc_count; i++){
        npc = space_get_NPC_at(space, i);
        if(npc_get_status(npc) == ENEMY && npc_enemies <= 4){
            if(entity_get_health(npc_get_entity(npc)) <= 0) continue;

            combat_copy_entity_stats(npc_get_entity(npc), &(combat->enemies_stats[npc_enemies]));
            npc_enemies++;
        }
        if(npc_get_status(npc) == ALLY && npc_allies <= 3){
            if(entity_get_health(npc_get_entity(npc)) <= 0) continue;
            
            combat_copy_entity_stats(npc_get_entity(npc), &(combat->allies_stats[npc_allies]));
            npc_allies++;
        }
    }
    
    combat->enemies_count = npc_enemies;
    combat->allies_count = npc_allies;
    
    if(combat->enemies_count == 0){
        free(combat);
        return NULL;
    }
    
    /*Initializes turns and the space where the combat is located*/
    
    if(code == ATTACK){
        combat->is_player_turn = true;
    } else {
        combat->is_player_turn = false;
    }
    
    combat->space = space;
    combat->endCombat = false;
    

    return combat;
}

void combat_free(Combat *combat){
    if(!combat)
        return;

    free(combat);
}

bool combat_get_isFinished(Combat *combat){
    if(!combat) return true;
    return combat->endCombat;
}

Status combat_update(Combat *combat, Command *last_cmd){
    int random;
    int deadEnemies = 0;
    
    if(!combat || !last_cmd)
        return ERROR;

    random = rand() % (100 - 0 + 1);

    if(random > 60){
        for (int i = 0; i < combat->enemies_count; i++)
        {
            combat->enemies_stats[i].stats.health -= 1;
            if(combat->enemies_stats[i].stats.health <= 0){
                entity_set_health(combat->enemies_stats[i].entity, 0);
                deadEnemies++;
            }
        } 
    }
    else{
        combat->player_stats.stats.health -= 1;
    }

    if(combat->player_stats.stats.health <= 0){
        combat_finalize(combat);
    }
    if(deadEnemies == combat->enemies_count){
        combat_finalize(combat);
    }
    return OK;
}

Status combat_runaway(Combat *combat){
    if(!combat) return ERROR;
    combat_finalize(combat);
    return OK;
}


int combat_get_enemies_count(Combat *combat){
    if(!combat) return 0;
    return combat->enemies_count;
}

int combat_get_allies_count(Combat *combat){
    if(!combat) return 0;
    return combat->allies_count;
}

Stats *combat_get_enemies_stats(Combat *combat){
    if(!combat) return NULL;
    return combat->enemies_stats;
}

Stats *combat_get_allies_stats(Combat *combat){
    if(!combat) return NULL;
    return combat->allies_stats;
}

PlayerStats *combat_get_player_stats(Combat *combat){
    if(!combat) return NULL;
    return &(combat->player_stats);
}
