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
#include "combat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
   Private functions
*/

Status game_actions_unknown(Game *game);

Status game_actions_exit(Game *game);

Status game_actions_move(Game *game);

Status game_actions_take(Game *game);

Status game_actions_drop(Game *game);

Status game_actions_chat(Game *game);

Status game_actions_attack(Game *game);

Status game_actions_runaway(Game *game);

/**
   Game actions implementation
*/

Status game_actions_update(Game *game, Command *command) {
  CommandCode cmd;
  Status status = ERROR;

  if(!game || !command) return ERROR;

  game_set_last_command(game, command);

  cmd = command_get_code(command);

  if(game_get_state(game) == DEFAULT){
    switch (cmd) {
      case UNKNOWN:
        status = game_actions_unknown(game);
        break;
  
      case EXIT:
        status = game_actions_exit(game);
        break;
  
      case NORTH:
      case EAST:
      case WEST:
      case SOUTH:
        status = game_actions_move(game);
        break;
      case TAKE:
        status = game_actions_take(game);
        break;
      
      case DROP:
        status = game_actions_drop(game);
        break;
  
      case CHAT:
        status = game_actions_chat(game);
        break;
      
      case ATTACK:
        status = game_actions_attack(game);
        break;

      default:
        break;
    }
  }else if(game_get_state(game) == COMBAT){
    switch (cmd) {
      case UNKNOWN:
        status = game_actions_unknown(game);
        break;
  
      case EXIT:
        status = game_actions_exit(game);
        break;
      
      case ATTACK:
        status = game_actions_attack(game);
        break;
      case RUN_AWAY:
        status = game_actions_runaway(game);
        break;

      default:
        break;
    }
  }
  

  

  command_set_status(command, status);

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
Status game_actions_unknown(Game *game) { return OK;}

/**
 * @brief No functionality
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 */
Status game_actions_exit(Game *game) { return OK;}

/**
 * @brief Retrieves the south ID, checks if it exists, then changes player location to south
 * @author Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 */
Status game_actions_move(Game *game) {
  Id space_id = NO_ID;
  Link *link = NULL;
  Entity *entity = NULL;

  space_id = game_get_player_location(game);
  if (space_id == NO_ID) {
    return ERROR;
  }

  switch(command_get_code(game_get_last_command(game))){
    case NORTH:
      link = space_get_north(game_get_space(game, space_id));
      if(link == NULL) return ERROR;
      break;
    case WEST:
      link = space_get_west(game_get_space(game, space_id));
      if(link == NULL) return ERROR;
      break;
    case EAST:
      link = space_get_east(game_get_space(game, space_id));
      if(link == NULL) return ERROR;
      break;
    case SOUTH:
      link = space_get_south(game_get_space(game, space_id));
      if(link == NULL) return ERROR;
      break;
    default:
      break;
  }
    

  entity = player_get_entity(game_get_player(game));
  if(entity == NULL) return ERROR;

  link_move_entity(link, entity);

  return OK;
}

/**
 * @brief Checks if the object is located on the same space as the player, then leaves it in the player
 * inventory and sets the object_location to INSIDE_INVENTORY;
 * @author Maksym Polyak
 *
 * @param game struct that saves all information related to the game
 */
Status game_actions_take(Game *game){
  Entity *player = NULL;
  Object *object = NULL;
  Inventory *spaceInventory = NULL, *playerInventory = NULL;
  char **arguments = NULL;
  Command *cmd = NULL;

  cmd = game_get_last_command(game);

  if(command_get_arguments_count(cmd) != 1) return ERROR;

  spaceInventory = space_get_inventory(game_get_space(game, game_get_player_location(game)));
  
  arguments = command_get_arguments(cmd);
  if(strcmp(arguments[0], "O470") == 0){
    printf("%c[2J", 27);
    printf("Fatal error. Object O470 is not supported\n");
    abort();
  }


  object = inventory_get_object_by_name(spaceInventory, arguments[0]);

  if(!object)
    return ERROR;

  player = player_get_entity(game_get_player(game));

  playerInventory = entity_get_inventory(player_get_entity(game_get_player(game)));


  if(object_get_location(object) != entity_get_location(player)) 
    return ERROR;

  inventory_move_object(spaceInventory, playerInventory, object_get_id(object));
  return OK;
}

/**
 * @brief Drops object to a space
 * @author Maksym Polyak
 *
 * @param game struct that saves all information related to the game
 */
Status game_actions_drop(Game *game){
  Object *object = NULL;
  Inventory *spaceInventory = NULL, *playerInventory = NULL;
  char **arguments = NULL;
  Command *cmd = NULL;

  cmd = game_get_last_command(game);

  if(command_get_arguments_count(cmd) != 1) return ERROR;

  playerInventory = entity_get_inventory(player_get_entity(game_get_player(game)));

  arguments = command_get_arguments(cmd);
  
  object = inventory_get_object_by_name(playerInventory, arguments[0]);

  if(!object)
    return ERROR;

  spaceInventory = space_get_inventory(game_get_space(game, game_get_player_location(game)));

  inventory_move_object(playerInventory, spaceInventory, object_get_id(object));
  return OK;
}

/**
 * @brief Shows on screen the message of the npc on the first argument if found
 * @author Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_chat(Game *game){
  char **arguments = NULL;
  Space *space = NULL;
  NPC *npc = NULL;

  if(!game)
    return ERROR;


  arguments = command_get_arguments(game_get_last_command(game));

  space = game_get_space(game, game_get_player_location(game));

  npc = space_get_NPC_by_name(space, arguments[0]);

  if(npc == NULL)
    return ERROR;

  if(entity_get_health(npc_get_entity(npc)) <= 0)
    return ERROR;

  game_add_log_message(game, MESSAGE_NPC,npc_get_message(npc));

  return OK;
}

/**
 * @brief if the game state is DEFAULT, it triggers an attack on the NPC enemies located in the space
 * if they are not found, it doesn't do anything.
 * 
 * If the game state is COMBAT, attacks all the enemies with a probability set of 60 % to hurt enemies,
 * 40 % of the player taking damage
 * 
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_attack(Game *game){
  if(!game) return ERROR;

  Combat *combat = NULL;
  bool initCombat = false;

  combat = game_get_combat(game);
  if(!combat){
    game_combat_start(game);
    initCombat = true;
  } 
  combat = game_get_combat(game);
  if(!combat) return ERROR;
  
  if(!initCombat)
    combat_update(combat, game_get_last_command(game));

  return OK;
}

/**
 * @brief Only works in combat game state, this allows for the player to escape from a combat.
 * Right now it doesn't have a chance to fail.

 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_runaway(Game *game){
  if(!game) return ERROR;

  Combat *combat = NULL;

  combat = game_get_combat(game);
  if(!combat) return ERROR;

  combat_runaway(combat);

  return OK;
}