/**
 * @brief It implements the game update through user actions
 *
 * the main function of game_actions is game_actions_update, which keeps
 * track of the las tcommand, gets a new one and then uses a switch to
 * decide the next game_action.
 * 
 * @file game.c
 * @author Original: Profesores PPROG. Modified by: Daniel Gómez and Maksym Polyak
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "game_actions.h"
#include "link.h"
#include "entity.h"
#include "collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
   Private functions
*/

void game_actions_unknown(Game *game);

void game_actions_exit(Game *game);

void game_actions_south(Game *game);

void game_actions_north(Game *game);

void game_actions_east(Game *game);

void game_actions_west(Game *game);

void game_actions_take(Game *game);

void game_actions_drop(Game *game);

void game_actions_chat(Game *game);

/**
   Game actions implementation
*/

Status game_actions_update(Game *game, Command *command) {
  CommandCode cmd;

  if(!game || !command) return ERROR;

  game_set_last_command(game, command);

  cmd = command_get_code(command);

  switch (cmd) {
    case UNKNOWN:
      game_actions_unknown(game);
      break;

    case EXIT:
      game_actions_exit(game);
      break;

    case SOUTH:
      game_actions_south(game);
      break;

    case NORTH:
      game_actions_north(game);
      break;
    case EAST:
      game_actions_east(game);
      break;
    case WEST:
      game_actions_west(game);
      break;
    case TAKE:
      game_actions_take(game);
      break;
    
    case DROP:
      game_actions_drop(game);
      break;

    case CHAT:
      game_actions_chat(game);
      break;

    default:
      break;
  }

  return OK;
}

/**
   Calls implementation for each action
*/

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
void game_actions_south(Game *game) {
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
void game_actions_north(Game *game) {
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
 * @brief Retrieves the north ID, checks if it exists, then changes player location to east
 * @author Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 */
void game_actions_east(Game *game) {
  Id space_id = NO_ID;
  Link *link = NULL;
  Entity *entity = NULL;

  space_id = game_get_player_location(game);
  if (space_id == NO_ID) {
    return;
  }

  link = space_get_east(game_get_space(game, space_id));
  if(link == NULL) return;

  entity = player_get_entity(game_get_player(game));
  if(entity == NULL) return;

  link_move_entity(link, entity);

  return;
}

/**
 * @brief Retrieves the north ID, checks if it exists, then changes player location to east
 * @author Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 */
void game_actions_west(Game *game) {
  Id space_id = NO_ID;
  Link *link = NULL;
  Entity *entity = NULL;

  space_id = game_get_player_location(game);
  if (space_id == NO_ID) {
    return;
  }

  link = space_get_west(game_get_space(game, space_id));
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
  Entity *player = NULL;
  Object *object = NULL;
  Inventory *spaceInventory = NULL, *playerInventory = NULL;
  char **arguments = NULL;
  
  printf("TEST");
  
  spaceInventory = space_get_inventory(game_get_space(game, game_get_player_location(game)));
  
  arguments = command_get_arguments(game_get_last_command(game));
  /*if(strcmp(arguments[0], "O470") == 0){ //Unavailable for now
    while(TRUE){
      player = entity_create("HOW DARE YOU", 1, 1, 1, 1, 1, 1, 1, 1);
    }
  }*/


  object = inventory_get_object_by_name(spaceInventory, arguments[0]);

  if(!object)
    return;

  player = player_get_entity(game_get_player(game));

  playerInventory = entity_get_inventory(player_get_entity(game_get_player(game)));


  if(object_get_location(object) != entity_get_location(player)) 
    return;

  inventory_move_object(spaceInventory, playerInventory, object_get_id(object));
}

/**
 * @brief Drops object to a space
 * @author Maksym Polyak
 *
 * @param game struct that saves all information related to the game
 */
void game_actions_drop(Game *game){
  Object *object = NULL;
  Inventory *spaceInventory = NULL, *playerInventory = NULL;
  char **arguments = NULL;

  playerInventory = entity_get_inventory(player_get_entity(game_get_player(game)));

  arguments = command_get_arguments(game_get_last_command(game));
  
  object = inventory_get_object_by_name(playerInventory, arguments[0]);

  if(!object)
    return;

  spaceInventory = space_get_inventory(game_get_space(game, game_get_player_location(game)));

  inventory_move_object(playerInventory, spaceInventory, object_get_id(object));
}

/**
 * @brief Shows on screen the message of the npc on the first argument if found
 * 
 * @param game 
 */
void game_actions_chat(Game *game){
  char **arguments = NULL;
  Space *space = NULL;
  NPC *npc = NULL;

  if(!game)
    return;


  arguments = command_get_arguments(game_get_last_command(game));

  space = game_get_space(game, game_get_player_location(game));

  npc = space_get_NPC_by_name(space, arguments[0]);

  if(npc == NULL) //ESTO ES NULL POR ALGUNA RAZON
    return;

  printf("%s", npc_get_message(npc)); /* TEMPORAL IMPLEMENTATION TILL WE WORK OUT DIALOGUES */
}