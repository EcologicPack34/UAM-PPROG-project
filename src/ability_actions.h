/**
 * @file ability_actions.h
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-03-25
 * 
 * @copyright Copyright (c) 2025
 * 
*/

#ifndef SKILLS_ACTIONS_H
#define SKILLS_ACTIONS_H

#include "ability_manager.h"
#include "game.h"
#include "types.h"

/**
 * @brief Uses the ability on the queue of the ability manager on the game
 * @author Maksym Polyak
 * 
 * @param game 
 * @return Status 
 */
Status ability_action_use_ability(Game *game);

#endif