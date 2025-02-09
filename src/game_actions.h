/**
 * @brief It defines the game update interface
 *
 * @file game.h
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef GAME_ACTIONS_H
#define GAME_ACTIONS_H

#include "command.h"
#include "game.h"
#include "types.h"
#include "debug_printing.h"
#include "link.h"
#include "entity.h"

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
