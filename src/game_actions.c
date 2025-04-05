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
#include "ability_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LVL 999999      /*<!stats "max lvl" (to be added on godmode)*/

/**
   Private functions
*/

/**
 * @brief Action for Unkown command
 * @author Profesores PPROG
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_unknown(Game *game);

/**
 * @brief Action for Unkown command
 * @author Profesores PPROG
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_exit(Game *game);

/**
 * @brief Action for Unkown command
 * @author Maksym Polyak y Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_move(Game *game);

/**
 * @brief Action for taking object
 * @author Maksym Polyak
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_take(Game *game);

/**
 * @brief Action for droping object
 * @author Maksym Polyak
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_drop(Game *game);

/**
 * @brief Action for chating with npcs
 * @author Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_chat(Game *game);

/**
 * @brief Action for combat
 * @author Maksym Polyak y Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_attack(Game *game);

/**
 * @brief Action for escaping combat
 * @author Maksym Polyak y Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_runaway(Game *game);

/**
 * @brief Action for switching player command
 * @author Maksym Polyak and Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_switch(Game *game);

/**
 * @brief Action for using ability
 * @author Maksym Polyak
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_use_ability(Game *game);

/**
 * @brief Action for using object if posible
 * @author Maksym Polyak
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_object_use(Game *game);

/**
 * @brief Action for Help command
 * @author Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_help(Game *game);


/**
 * @brief Action for Searching command
 * @author Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_search(Game *game);

/**
 * @brief Equips a piece of equipment if possible and
 * removes it from the inventory
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_equip(Game *game);

/**
 * @brief Activates god mode
 * @author Aaron Charameli Mair
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_god_mode(Game *game);



/**
 * @brief Unequips a piece of equipment and returns
 * it to the player inventory
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_unequip(Game *game);

/**
 * @brief Action to inspect an object
 * @author Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_inspect(Game *game);

/**
   Game actions implementation
*/

Status game_actions_update(Game *game, Command *command) {
  CommandCode cmd;
  Status status = ERROR;
  char str[WORD_SIZE] = "";
  int i = 0;
  Player *pl = NULL;
  Entity *player = NULL;

  if(!game || !command) return ERROR;
  
  if(command_get_code(command) != SWITCH 
  || (command_get_code(command) == SWITCH && strncmp("list", command_get_arguments(command)[0], 5) == 0) ){
    command_update_player_data(command);
  }
  
  game_set_last_command(game, command);

  cmd = command_get_code(command);
  
  if(cmd == GM){
    game_set_godmode(game, TRUE);
    game_actions_god_mode(game);

    if(game_get_state(game) == COMBAT)
      combat_update(game_get_combat(game), game_get_last_command(game));

    return command_set_status(command, OK);
  }

  if(!command_current_type_valid_by_state(game_get_last_command(game), game_get_state(game))){
    command_set_status(game_get_last_command(game), ERROR);
    debug_log(LOG_WARNING, "Introduced command was not valid for current game state (state: %d)", game_get_state(game) - ERROR_STATE);
    return ERROR;
  }

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
    case MOVE:
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
      pl = game_get_player(game);
      player = player_get_entity(pl);
      if (entity_get_health(player) == 0)
      {
        do
        { 
          pl = game_get_player(game);
          player = player_get_entity(pl);
          game_switch_player(game, i);
          i++;

        } while (entity_get_health(player) == 0); 
      }
      break;
    case RUN_AWAY:
      status = game_actions_runaway(game);
      break;
    case SWITCH:
      status = game_actions_switch(game);
      break;
    case HELP:
      status = game_actions_help(game);
      break;
    case ABILITY:
      status = game_actions_use_ability(game);
      break;
    case OBJECT_USE:
      status = game_actions_object_use(game);
      break;
    case SEARCH:
      status = game_actions_search(game);
      break;
    case EQUIP:
      status = game_actions_equip(game);
      break;
    case UNEQUIP:
      status = game_actions_unequip(game);
      break;
    case INSPECT:
      status = game_actions_inspect(game);
      break;
    default:
      break;
  }

  command_set_status(command, status);

  command_get_as_string(game_get_last_command(game), str);

  player = player_get_entity(game_get_player(game));

  if(player){
    debug_log(PRINT,"Executed command: %s; by player %d:%s",str , entity_get_id(player), entity_get_name(player));
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
Status game_actions_unknown(Game *game){ 
  return OK;
}

/**
 * @brief No functionality
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 */
Status game_actions_exit(Game *game){ 
  return OK;
}

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
  CommandCode code = NO_CMD;
  Command *cmd = NULL;
  NPC **followers = NULL;
  Space *actual_space = NULL, *next_space = NULL;
  int i;

  space_id = game_get_player_location(game);
  if (space_id == NO_ID) {
    return ERROR;
  }

  cmd = game_get_last_command(game);

  if(command_get_code(cmd) == MOVE){
    if(command_get_arguments_count(cmd) != 1){
      game_add_log_message(game, MESSAGE_ERROR, "Invalid number of arguments for Move command. Use 'help move' for more info");
      return ERROR;
    }
    code = command_get_code_from_str(command_get_arguments(cmd)[0]);
  }else{
    code = command_get_code(cmd);
  }

  actual_space = game_get_space(game, space_id);

  switch(code){
    case NORTH:
      link = space_get_north(actual_space);
      if(link == NULL) return ERROR;
      break;
    case WEST:
      link = space_get_west(actual_space);
      if(link == NULL) return ERROR;
      break;
    case EAST:
      link = space_get_east(actual_space);
      if(link == NULL) return ERROR;
      break;
    case SOUTH:
      link = space_get_south(actual_space);
      if(link == NULL) return ERROR;
      break;
    default:
      game_add_log_message(game, MESSAGE_ERROR, "Invalid direction for move command. Use 'help move' for more info");
      break;
  }
    

  entity = player_get_entity(game_get_player(game));
  if(entity == NULL) return ERROR;

  link_move_entity(link, entity);

  followers = player_get_followers(game_get_player(game));
  if(!followers) return OK;

  next_space = game_get_space(game, link_get_oposite_space(link, space_id));

  for(i = 0; i < NPC_MAX_FOLLOWERS; i++){
    if(followers[i] != NULL){
      entity = npc_get_entity(followers[i]);
      space_move_NPC(actual_space, next_space, followers[i]);
    }
  }

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

  if(npc_get_is_follower(npc) == true && npc_get_status(npc) == NEUTRAL){
    npc_set_status(npc, ALLY);
    player_add_follower(game_get_player(game), npc);
  }else if(npc_get_is_follower(npc) == true && npc_get_status(npc) == ALLY){
    npc_set_status(npc, NEUTRAL);
    player_remove_follower_by_name(game_get_player(game), entity_get_name(npc_get_entity(npc)));
  }

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

Status game_actions_switch(Game *game){
  char **arguments = NULL;
  int player;
  Command *cmd = NULL;

  char str[WORD_SIZE] = "";
  char strAux[WORD_SIZE] = "";
  int i, n_players;
  Entity *playerEnt = NULL;

  if(!game)
    return ERROR;

  cmd = game_get_last_command(game);
  
  if(command_get_arguments_count(cmd) > 1){
    game_add_log_message(game, ERROR, "Invalid number of arguments for switch command. Use 'help switch' for more info");
    return ERROR;
  } 

  arguments = command_get_arguments(cmd);

  if(strcmp(arguments[0], "list") == 0){
    n_players = game_get_n_players(game);
    for (i = 0; i < n_players; i++)
    {
      playerEnt = player_get_entity(game_get_player_at(game, i));
      strcat(str, "\n");
      sprintf(strAux, "%d. ID:%ld NAME: %s", i + 1, entity_get_id(playerEnt), entity_get_name(playerEnt));
      strcat(str, strAux);
    }
    return game_add_log_message(game, MESSAGE_PLAYER_LIST, str);
  }

  if(command_get_arguments_count(cmd) == 0) player = -1;
  else player = atoi(arguments[0]) - 1;

  return game_switch_player(game, player);
}


Status game_actions_help(Game *game){
  
  Command *cmd = NULL;
  char **args = NULL;
  int argc = 0;
  char str[WORD_SIZE] = "";
  
  CommandCode code = NO_CMD;
  
  if(!game) return ERROR;

  cmd = game_get_last_command(game);

  args = command_get_arguments(cmd);
  argc = command_get_arguments_count(cmd);

  if(argc == 0){
    command_get_list(cmd, str, ERROR_STATE, true);

  }else if(argc == 1){
    code = command_get_code_from_str(args[0]);
    command_get_info(cmd, code, str);

  }else{
    game_add_log_message(game, MESSAGE_ERROR, "Invalid number of arguments for command help");
    return ERROR;
  }

  return game_add_log_message(game, MESSAGE_HELP ,str);
}
/**
 * @brief Uses the ability received as argument
 * @author Maksym Polyak
 * 
 * @param game 
 * @return Status 
 */
Status game_actions_use_ability(Game *game){
  char **arguments = NULL;
  int n_arg;
  int index;
  Entity *entity = NULL;
  Ability *ability = NULL;
  
  if(!game) return ERROR;

  n_arg = command_get_arguments_count(game_get_last_command(game));
  if(n_arg != 1)
    return ERROR;

  arguments = command_get_arguments(game_get_last_command(game));

  index = atoi(arguments[0]);
  if(n_arg < 0 || n_arg >= MAX_SKILLS_ENTITY)
    return ERROR;

  entity = player_get_entity(game_get_player(game));

  ability = entity_get_ability_at(entity, index);
  if(!ability)
    return ERROR;

  if(ability_manager_use_ability(game_get_ability_manager(game), ability) == ERROR)
    return ERROR;

  return OK;
}

Status game_actions_object_use(Game *game){
  Command *comm = NULL;
  Object *object = NULL;
  Entity *entity = NULL;
  
  if(!game) return ERROR;

  comm = game_get_last_command(game);

  if(command_get_arguments_count(comm) != 1)
    return ERROR;

  entity = player_get_entity(game_get_player(game));
  object = inventory_get_object_by_name(entity_get_inventory(entity), command_get_arguments(comm)[0]);

  if(object_get_is_consumable(object) == true){
    inventory_remove_object(entity_get_inventory(entity), object);
  }

  return ability_manager_use_ability(game_get_ability_manager(game), object_get_object_effect(object));
}

Status game_actions_search(Game *game){
  Space *space = NULL;
  
  if(!game) return ERROR;

  space = game_get_space(game, game_get_player_location(game));
  if(space_get_isDiscovered(space)) return ERROR;
  
  space_set_discovered(space, true);
  return OK;
}

Status game_actions_god_mode(Game *game){
  Player *pl=NULL;

  if(!game) return ERROR;

  if(game_get_god_mode(game) == false)
    return ERROR;
  
  pl = game_get_player(game);

  return player_set_stats(pl, MAX_LVL, MAX_LVL, MAX_LVL, MAX_LVL, MAX_LVL, MAX_LVL);
}

Status game_actions_equip(Game *game){
  Object *object = NULL;
  Player *player = NULL;
  Inventory *inventory = NULL;
  Command *comm = NULL;
  
  if(!game) return ERROR;

  comm = game_get_last_command(game);

  if(command_get_arguments_count(comm) != 1) return ERROR;

  player = game_get_player(game);
  inventory = entity_get_inventory(player_get_entity(player));
  object = inventory_get_object_by_name(inventory, command_get_arguments(comm)[0]);
  if(!object) return ERROR;

  return player_equip_piece(player, object);
}


Status game_actions_unequip(Game *game){
    Player *player = NULL;
    Command *comm = NULL;
    
    if(!game) return ERROR;
  
    comm = game_get_last_command(game);
  
    if(command_get_arguments_count(comm) != 1) return ERROR;
  
    player = game_get_player(game);
    
    if(player_unequip_piece(player, command_get_arguments(comm)[0]) == ERROR) return ERROR;

    return OK;
}

Status game_actions_inspect(Game *game){
  Inventory *playerInv = NULL;
  Object *obj;

  Command *cmd = NULL;

  if(!game) return ERROR;

  cmd = game_get_last_command(game);

  if(command_get_arguments_count(cmd) != 1) return ERROR;

  playerInv = entity_get_inventory(player_get_entity(game_get_player(game)));
  obj = inventory_get_object_by_name(playerInv, command_get_arguments(cmd)[0]);

  if(!obj) return ERROR;

  return game_add_log_message(game, MESSAGE_INSPECT, object_get_descr(obj));
}