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

typedef struct {
    Entity_Stats stats;             /*!< Copy of the stats in order to buff/debuff and not modify the original stats*/
    Entity *entity;                 /*!< Entity from which the stats are saved*/
}Stats;

typedef struct {
    Entity_Stats stats;             /*!< Copy of the stats in order to buff/debuff and not modify the original stats*/
    Entity *entity;                 /*!< Entity from which the stats are saved*/
}PlayerStats;                       /*Needs to be different for further implementation of equipment and more*/

struct _Combat{
    PlayerStats player_stats;               /*!< Stats of the player*/
    Stats allies_stats[NPC_MAX_ALLIES];     /*!< Stats of the allies*/
    int allies_count;                       /*!< Number of allies*/
    Stats enemies_stats[NPC_MAX_ENEMIES];   /*!< Stats of the enemies*/
    int enemies_count;                      /*!< Number of enemies*/

    bool is_player_turn;                    /*!< Determines who starts, true --> player party, false --> enemy party*/
    Space *space;                           /*!< Space where the combat is located*/
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

/**
 * @brief Determines the course of action for the player turn
 * 
 * @param combat 
 * @return Status 
 */
Status combat_player_turn(Combat *combat);

/**
 * @brief Determines the course of action for the ally turn in the position index
 * 
 * @param combat 
 * @param index
 * @return Status 
 */
Status combat_ally_turn(Combat *combat, int index);

/**
 * @brief Determines the course of action for the enemy turn in the position index
 * 
 * @param combat 
 * @param index
 * @return Status 
 */
Status combat_enemy_turn(Combat *combat, int index);

Status combat_copy_entity_stats(Entity *entity, Stats *stats){

    if(!entity || !stats)    return NULL;

    stats->stats.health = entity_get_health(entity);
    stats->stats.baseDamage = entity_get_baseDamage(entity);
    stats->stats.strength = entity_get_strength(entity);
    stats->stats.defense = entity_get_defense(entity);
    stats->stats.magicLevel = entity_get_magicLevel(entity);

    stats->entity = entity;

    return OK;
}

Status combat_copy_player(Player *player, PlayerStats *stats){
    Entity *ent = NULL;

    if(!player || !stats)
        return ERROR;

    combat_copy_entity_stats(player_get_entity(player), stats);

    return OK;
}

Status combat_player_turn(Combat *combat){

    if(!combat)
        return ERROR;
}

Status combat_ally_turn(Combat *combat, int index){

    if(!combat)
        return ERROR;

    
}

Status combat_enemy_turn(Combat *combat, int index){

    if(!combat)
        return ERROR;

    
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
    if(!combat)
        return NULL;

    /*Saves the player stats on the combat struct*/
    combat_copy_player(player, &(combat->player_stats));

    /*Saves the first four enemies and the first three allies on the combat struct*/
    for(i = 0; i < npc_count; i++){
        npc = space_get_NPC_at(space, i);
        if(npc_get_status(npc) == ENEMY && npc_enemies <= 4){
            combat_copy_entity_stats(npc_get_entity(npc), &(combat->enemies_stats[npc_enemies]));
            npc_enemies++;
        }
        if(npc_get_status(npc) == ALLY && npc_allies <= 3){
            combat_copy_entity_stats(npc_get_entity(npc), &(combat->allies_stats[npc_allies]));
            npc_allies++;
        }
    }

    /*Initializes turns and the space where the combat is located*/

    if(code == ATTACK){
        combat->is_player_turn = true;
    } else {
        combat->is_player_turn = false;
    }

    combat->space = space;

    return combat;
}

void combat_end(Combat *combat){
    if(!combat)
        return;

    free(combat);
}

Status combat_update(Combat *combat, Command *last_cmd){
    if(!combat || !last_cmd)
        return ERROR;

    
}