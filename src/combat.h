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
void combat_end(Combat *combat);

/**
 * @brief Updates the combat turns
 * @author Maksym Polyak
 * 
 * @param combat 
 * @param last_cmd 
 * @return Status 
 */
Status combat_update(Combat *combat, Command *last_cmd);


#endif