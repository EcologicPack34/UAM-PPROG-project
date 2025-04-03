/**
 * @file event_actions.h
 * @author Daniel Gómez
 * @brief       Complementary module to event_manager, its purpose is to avoid cross-references
 * @version 0.1
 * @date 2025-03-01
 * 
 * @copyright GNU Public License
 * 
 */

#ifndef EVENT_ACTIONS_H
#define EVENT_ACTIONS_H

#include "game.h"

/**
 * @brief Triggers all the events in the event manager stored in game
 * @author Daniel Gómez
 * 
 * @param game 
 */
void event_actions_trigger_events(Game *game);

#endif