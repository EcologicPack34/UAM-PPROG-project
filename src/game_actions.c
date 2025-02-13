/**
 * @brief It implements the game update through user actions
 *
 * the main function of game_actions is game_actions_update, which keeps
 * track of the las tcommand, gets a new one and then uses a switch to
 * decide the next game_action.
 * 
 * @file game.c
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "game_actions.h"
#include "link.h"
#include "entity.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
   Private functions
*/

#pragma region PRIVATE

void game_actions_unknown(Game *game);

void game_actions_exit(Game *game);

void game_actions_next(Game *game);

void game_actions_back(Game *game);

void game_actions_take(Game *game);

void game_actions_drop(Game *game);

#pragma endregion

/**
   Game actions implementation
*/

Status game_actions_update(Game *game, Command *command) {
  CommandCode cmd;

  game_set_last_command(game, command);

  cmd = command_get_code(command);

  switch (cmd) {
    case UNKNOWN:
      game_actions_unknown(game);
      break;

    case EXIT:
      game_actions_exit(game);
      break;

    case NEXT:
      game_actions_next(game);
      break;

    case BACK:
      game_actions_back(game);
      break;

    case TAKE:
      game_actions_take(game);
      break;
    
    case DROP:
      game_actions_drop(game);
      break;

    default:
      break;
  }

  return OK;
}

/**
   Calls implementation for each action
*/

#pragma region GAME_ACTIONS_FUNCTIONS

/**
 * @brief No functionality.
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 */
void game_actions_unknown(Game *game) {}

/**
 * @brief No functionality
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 */
void game_actions_exit(Game *game) {}

/**
 * @brief Retrieves the south ID, checks if it exists, then changes player location to south
 * @author Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 */
void game_actions_next(Game *game) {
  Id space_id = NO_ID;
  Link *link = NULL;
  Entity *entity = NULL;

  space_id = game_get_player_location(game);
  if (space_id == NO_ID) {
    return;
  }

  link = space_get_south(game_get_space(game, space_id));
  if(link == NULL) return;

  entity = player_get_entity(game_get_player(game));
  if(entity == NULL) return;

  link_move_entity(link, entity);

  return;
}

/**
 * @brief Retrieves the north ID, checks if it exists, then changes player location to north
 * @author Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 */
void game_actions_back(Game *game) {
  Id space_id = NO_ID;
  Link *link = NULL;
  Entity *entity = NULL;

  space_id = game_get_player_location(game);
  if (space_id == NO_ID) {
    return;
  }

  link = space_get_north(game_get_space(game, space_id));
  if(link == NULL) return;

  entity = player_get_entity(game_get_player(game));
  if(entity == NULL) return;

  link_move_entity(link, entity);

  return;
}

/**
 * @brief Checks if the object is located on the same space as the player, then leaves it in the player
 * inventory and sets the object_location to INSIDE_INVENTORY;
 * @author Maksym Polyak
 *
 * @param game struct that saves all information related to the game
 */
void game_actions_take(Game *game){
  Entity *player;
  Object *object;

  player = player_get_entity(game_get_player(game));
  if(game_get_object_location(game) != entity_get_location(player))
    return;

  object = game_get_object(game);
  inventory_add_object(entity_get_inventory(player), object);
  object_set_location(object, -2);

  game_set_object_location(game, INSIDE_INVENTORY);
}

void game_actions_drop(Game *game){
  Entity *player;
  Object *object;
  Id id;

  if(game_get_object_location(game) != INSIDE_INVENTORY)
    return;
  
  
  
  id = game_get_player_location(game);
  game_set_object_location(game, id);

  player = player_get_entity(game_get_player(game));
  object = game_get_object(game);
  object_set_location(object, id);
  inventory_remove_object(entity_get_inventory(player),object);
}

#pragma endregion