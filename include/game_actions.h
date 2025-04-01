/**
 * @brief It defines the game update interface
 *
 * game_actions is the module that controls movement and takes or drops objects. 
 * In further iterations more commands will be implemented, for now, the only
 * functionality of game_actions is to move north and south, exit the game and
 * take or drop an object.
 * 
 * @file game.h
 * @author Original: Profesores PPROG. Modified By: Daniel Gómez and Maksym Polyak
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef GAME_ACTIONS_H
#define GAME_ACTIONS_H

#include "game.h"
#include "command.h"
#include "types.h"
#include "link.h"
#include "entity.h"
#include "inventory.h"

/**
 * @brief Updates the game action, checks the las command and uses game_actions functions
 * to select the next action
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to game
 * @param command struct that saves all information related to command
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status game_actions_update(Game *game, Command *cmd);

#endif
