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
 * @brief Uses the evaluated ability of the ability manager
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @return Status 
 */
Status ability_action_use_ability(Game *game);

/**
 * @brief Updates the cooldowns of the abilities in the queue
 * or removes them if they are available
 * @author Maksym Polyak
 * 
 * @param game 
 * @return Status 
 */
Status ability_actions_manage_cooldowns(Game *game);

#endif