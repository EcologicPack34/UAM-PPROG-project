/**
 * @file skills_actions.h
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

#include "skills_manager.h"
#include "game.h"
#include "types.h"

/**
 * @brief Uses the skills on the queue of the skill manager on the game
 * @author Maksym Polyak
 * 
 * @param game 
 * @return Status 
 */
Status skill_action_use_skills(Game *game);

#endif