/**
 * @file combat.h
 * @author Maksym Polyak
 * @brief The combat module takes care of the game state FIGHT, checks stats of all
 * entities involved and then creates all the logic of the combat
 * @version 0.1
 * @date 2025-03-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef COMBAT_H
#define COMBAT_H

#include "collection.h"
#include "debug_printing.h"
#include "queue.h"
#include "entity.h"
#include "npc.h"
#include "space.h"
#include "player.h"
#include "command.h"

typedef struct {
    Entity_Stats stats;             /*!< Copy of the stats in order to buff/debuff and not modify the original stats*/
    Entity *entity;                 /*!< Entity from which the stats are saved*/
}Stats;

typedef struct {
    Entity_Stats stats;             /*!< Copy of the stats in order to buff/debuff and not modify the original stats*/
    Entity *entity;                 /*!< Entity from which the stats are saved*/
}PlayerStats;                       /*Needs to be different for further implementation of equipment and more*/

typedef struct _Combat Combat;

/**
 * @brief Initializes the combat struct
 * @author Maksym Polyak
 * 
 * @param space 
 * @param player 
 * @param code last cmd 
 * @return Combat* or NULL if ERROR
 */
Combat *combat_initialize(Space *space, Player *player, CommandCode code);

/**
 * @brief Ends the combat and frees all the memory related to it
 * @author Maksym Polyak
 * 
 * @param combat 
 */
void combat_free(Combat *combat);

/**
 * @brief Updates the combat turns
 * @author Maksym Polyak
 * 
 * @param combat 
 * @param last_cmd 
 * @return Status 
 */
Status combat_update(Combat *combat, Command *last_cmd);

Status combat_runaway(Combat *combat);


int combat_get_enemies_count(Combat *combat);

int combat_get_allies_count(Combat *combat);

Stats *combat_get_enemies_stats(Combat *combat);

Stats *combat_get_allies_stats(Combat *combat);

PlayerStats *combat_get_player_stats(Combat *combat);

bool combat_get_isFinished(Combat *combat);


#endif