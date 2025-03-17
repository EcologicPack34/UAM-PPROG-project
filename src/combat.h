/**
 * @file combat.h
 * @author Maksym Polyak && Daniel Gómez
 * @brief The combat module takes care of the game state COMBAT, checks stats of all
 * entities involved and then creates all the logic of the combat.
 * 
 * Every time a combat starts, the combat struct is created and takes all the information
 * from the space and the player.
 * 
 * Combat is based on turns, player turn and enemies turn, player can choose to run away
 * or attack in their turn.
 * 
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

/*Struct of the combat module to save the stats of an NPC, public for the graphic_engine*/
typedef struct {
    Entity_Stats stats;             /*!< Copy of the stats in order to buff/debuff and not modify the original stats*/
    Entity *entity;                 /*!< Entity from which the stats are saved*/
}Stats;

/*Struct of the combat module to save the stats of the player, public for the graphic_engine*/
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

/**
 * @brief Finalizes the combat and returns the game state to DEFAULT
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat 
 * @return Status 
 */
Status combat_runaway(Combat *combat);

/**
 * @brief Gets the number of enemies on the combat
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat 
 * @return int or 0 if error;
 */
int combat_get_enemies_count(Combat *combat);

/**
 * @brief Gets the number of allies on the combat - NON IMPLEMENTED THE FUNCTIONALITY
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat 
 * @return int or 0 if error;
 */
int combat_get_allies_count(Combat *combat);

/**
 * @brief Gets the stats pointer of an enemy
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat 
 * @return int or 0 if error;
 */
Stats *combat_get_enemies_stats(Combat *combat);

/**
 * @brief Gets the stats pointer of an ally - NON IMPLEMENTED THE FUNCTIONALITY
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat 
 * @return int or 0 if error;
 */
Stats *combat_get_allies_stats(Combat *combat);

/**
 * @brief Gets the PlayerStats pointer of player
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat 
 * @return int or 0 if error;
 */
PlayerStats *combat_get_player_stats(Combat *combat);

/**
 * @brief Gets the bool with the information of the combat end
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat 
 * @return int or 0 if error;
 */
bool combat_get_isFinished(Combat *combat);


#endif