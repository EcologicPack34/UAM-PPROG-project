/**
 * @brief It implements the game structure
 *
 * Has functions related to the game struct, which interact with almost every module of the game
 * 
 * @file game.c
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "game.h"
#include "debug_printing.h"
#include "collection.h"
#include "npc.h"
#include "vector2.h"
#include "queue.h"
#include "combat.h"
#include "message.h"
#include "game_reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLLECTION_INITIAL_SIZE 10  /*!< Collection initial size*/

/**
 * @brief Struct with all the information of the game, main bridge of main functionalities for the game to work
 */
struct _Game {
  /*Player related*/
  Player *active_player;              /*!< Contains all the information related to the player */
  Player *players[MAX_PLAYERS];       /*!< Contains all the players available on the game*/
  int active_player_index;            /*!< Index of the players array with the active_player*/
  int n_players;                      /*!< Number of players on the players array*/
  TurnValidation is_turn_valid;       /*!< Checks if the turn evaluated was valid or not*/

  /*Collections*/
  Collection *npcs;                   /*!< Contains all the information related to the NPCs*/
  Collection *objects;                /*!< Contains all the information related to the object */

  /*Space related*/
  Space *spaces[MAX_SPACES];          /*!< Array with all the spaces of the map */
  int n_spaces;                       /*!< int with the number of spaces on *spaces */
  Link *links[MAX_LINKS];             /*!< Array with all the links in the map*/
  int n_links;                        /*!< Number of links on the links array*/

  /*Others*/
  EventManager *event_manager;        /*!< Struct containing the info about the events that can happen*/
  Queue *screenLog;                   /*!< Queue containing a list of messages to print on screen*/
  bool godmode;                       /*!< bool that determines if god mode is activated*/

  /*Combat*/
  Combat *combat;                     /*!< Combat struct, only != NULL if a player is in combat*/
  AbilityManager *ability_manager;    /*!< Struct containing the info about the ability and their cooldown*/
  
  /*Game state related*/
  GameState current_state;            /*!< Enum storing the current game state*/
  Command *last_cmd;                  /*!< string with the last command */
  bool finished;                      /*!< bool that determines if the game has finished*/
};

/**
 * @brief Gets the link in a certain position of the game links array
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @param index index where the link is located
 * @return Link* or NULL if error
 */
Link *game_get_link_at(Game *game, long index){
  if(!game) return NULL;

  if(index < 0 || index >= game_get_n_links(game))
    return NULL;

  return game->links[index];
}

/**
 * @brief Maps spatially a block of spaces starting at initSpace
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @param initSpace initial space so start mapping
 * @param block number of the block that is mapped
 * @return Status 
 */
Status game_map_space_block(Game *game, Space *initSpace ,int block);

/**
   Game interface implementation
*/

Status game_create(Game **game) {
  int i;
  *game = (Game*)calloc(1,sizeof(Game));
  if(!(*game)) return ERROR;

  for (i = 0; i < MAX_SPACES; i++) {
    (*game)->spaces[i] = NULL;
  }

  (*game)->n_spaces = 0;
  (*game)->active_player = NULL; /*Player creation is controlled by game_reader*/
  (*game)->n_players = 0;
  (*game)->objects = collection_create(COLLECTION_INITIAL_SIZE, false, true, object_isEqual, object_print);
  if(!((*game)->objects)){
    debug_log(LOG_ERROR,"Error initializing collection of objects");
    return ERROR;
  } 
  
  (*game)->npcs = collection_create(COLLECTION_INITIAL_SIZE, false, true, npc_cmp, npc_print); /*TEMPORAL PRINT*/
  if(!((*game)->npcs)){
    debug_log(LOG_ERROR,"Error initializing collection of npcs");
    return ERROR;
  } 
  
  (*game)->last_cmd = command_create();
  if(!((*game)->last_cmd)){
    debug_log(LOG_ERROR,"Error creating command");
    return ERROR;
  }
  
  (*game)->godmode = false;
  (*game)->finished = false;
  (*game)->n_links = 0;
  (*game)->current_state = DEFAULT;
  (*game)->is_turn_valid = VALID;

  (*game)->event_manager = event_manager_create();
  if(!((*game)->event_manager)){
    debug_log(LOG_ERROR,"Error creating event manager");
    return ERROR;
  } 

  (*game)->ability_manager = ability_manager_create();
  if(!((*game)->ability_manager)){
    debug_log(LOG_ERROR,"Error creating ability manager");
    return ERROR;
  } 

  (*game)->screenLog = queue_create();
  if(!((*game)->screenLog)){
    debug_log(LOG_ERROR,"Error creating event screenLog");
    return ERROR;
  } 

  (*game)->combat = NULL;

  return OK;
}


Status game_destroy(Game *game) {
  int i = 0;
  int count;

  /*Destroys all spaces*/
  for (i = 0; i < game->n_spaces; i++) {
    space_destroy(game->spaces[i]);
  }

  for(i = 0; i < game->n_players; i++){
    player_destroy(game->players[i]);
  }
  
  collection_free_elements(game_get_objects(game), object_destroy);
  collection_destroy(game_get_objects(game));

  if(collection_free_elements(game_get_npcs(game), npc_destroy) == ERROR){
    printf("Error liberando colleccion de npcs");
  }
  collection_destroy(game_get_npcs(game));

  command_destroy(game->last_cmd);
  event_manager_destroy(game->event_manager);
  
  ability_manager_destroy(game->ability_manager);
  queue_destroy(game->screenLog);

  if(game->combat){
    combat_free(game->combat);
  }

  /*Destroys all links */
  count = game_get_n_links(game);

  for (i = 0; i < count; i++)
  {
    free(game_get_link_at(game,i));
  }

  free(game);
  return OK;
}

/*----------GETTERS----------*/

Link *game_get_link_by_id(Game *game, Id id){
  Link *link = NULL;
  
  if(!game || id == NO_ID)
    return NULL;

  for (int i = 0; i < game->n_links; i++)
  {
    link = game_get_link_at(game, i);
    if(link_get_id(link) == id){
      return link;
    }
  }

  return NULL;
}

Space *game_get_space(Game *game, Id id) {
  int i = 0;

  if (id == NO_ID) {
    debug_log(LOG_WARNING, "No Id assigned at: game_get_space(Game*, Id) in game.c");
    return NULL;
  }

  for (i = 0; i < game->n_spaces; i++) {
    if (id == space_get_id(game->spaces[i])) {
      return game->spaces[i];
    }
  }

  return NULL;
}

Space *game_get_space_at(Game *game, int ix){
  if(!game || (ix<0)) return NULL;
  if(ix >= game_get_n_spaces(game)) return NULL;

  return game->spaces[ix];
}

Player* game_get_player(Game *game){
  return game->active_player; 
}

int game_get_n_players(Game *game){
  return game->n_players;
}

Player* game_get_player_at(Game *game, int index){
  if(!game) return NULL;
  if(index < 0 || index >= game->n_players) return NULL;

  return game->players[index];
}
Frame_color game_get_player_color(Game *game){
  if(!game) return BLUE;
  return BLUE + game->active_player_index % MAX_PLAYERS;
}

Id game_get_player_location(Game *game){
  return entity_get_location(player_get_entity(game_get_player(game)));
}

Command* game_get_last_command(Game *game){
 return game->last_cmd; 
}

bool game_get_finished(Game *game){ 
  return game->finished;
}

Id game_get_space_id_at(Game *game, int position) {
  if (position < 0 || position >= game->n_spaces) {
    debug_log(LOG_WARNING, "No Id assigned at: game_get_space_id_at(Game*, int) in game.c");
    return NO_ID;
  }

  return space_get_id(game->spaces[position]);
}

Space *game_get_space_by_position(Game *game, Vector2 pos, int block){
  int i;
  Space *space = NULL;
  
  if(!game) return NULL;

  for (i = 0; i < game->n_spaces; i++)
  {
    space = game->spaces[i];
    if(vector2_isEqual(&pos, space_get_position(space)) == 0 && space_get_map_block(space) == block){
      return space;
    }
  }
  return NULL;
}

int game_get_n_spaces(Game *game) {
  if(!game) return -1;
  return game->n_spaces;
}

long game_get_n_links(Game *game){
  if(!game) return -1;
  return game->n_links;
}

GameState game_get_state(Game *game){
  if(!game) return ERROR_STATE;
  return game->current_state;
}

Collection *game_get_objects(Game *game){
  if(!game) return NULL;
  return game->objects;
}

EventManager *game_get_event_manager(Game *game){
  if(!game) return NULL;
  return game->event_manager;
}

Collection *game_get_npcs(Game *game){
  if(!game) return NULL;
  return game->npcs;
}

NPC *game_get_npc_by_id(Game *game, Id id){
  Collection *all_npcs=NULL;
  long i,size;

  if(!game || (id == NO_ID)) return NULL;

  if((all_npcs = game_get_npcs(game)) == NULL) return NULL;

  size = collection_length(all_npcs);

  for(i=0; i<size; i++){
    if(entity_get_id(npc_get_entity((NPC *)collection_get_element_at(all_npcs, i))) == id)
      return (NPC *)collection_get_element_at(all_npcs, i);
  }

  return NULL;
}

Player *game_get_player_by_id(Game *game, Id id){
  long i;

  if(!game || (id == NO_ID)) return NULL;

  for(i=0; i<MAX_PLAYERS; i++){
    if(entity_get_id(player_get_entity(game->players[i])) == id)
      return (game->players[i]);
  }

  return NULL;
}

bool game_get_god_mode(Game *game){
  if(!game) return false;

  return game->godmode;
}

TurnValidation game_get_is_turn_valid(Game *game){
  if(!game) return false;

  return game->is_turn_valid;
}

/*-----------SETTERS-----------*/

Status game_set_last_command(Game *game, Command *command) {
  game->last_cmd = command;

  return OK;
}

Status game_set_finished(Game *game, bool finished) {
  game->finished = finished;

  return OK;
}

Status game_set_player_location(Game *game, Id id){
  entity_set_location((Entity *)game_get_player(game), id);

  return OK;
}

Status game_set_state(Game *game, GameState state){
  if(!game) return ERROR;
  game->current_state = state;
  return OK;
}

Status game_set_godmode(Game *game, bool value){
  if(!game) return ERROR;
  game->godmode = value;
  return OK;
}

Status game_set_is_turn_valid(Game *game, TurnValidation value){
  if(!game) return ERROR;
  game->is_turn_valid = value;
  return OK;
}

/*
* Other functions that are not getters or setters
*/

Status game_spatial_map(Game *game){
  int i;
  int currentBlock = 0;

  if(!game) return ERROR;

  for (i = 0; i < game->n_spaces; i++)
  {
    if(!space_get_isMapped(game->spaces[i])){
      if(game_map_space_block(game, game->spaces[i], currentBlock) == ERROR){
        return ERROR;
      }
      currentBlock++;
    }
  }
  return OK;
}

Status game_map_space_block(Game *game, Space *initSpace ,int block){
  Queue *queue = NULL;
  Queue *queueAux = NULL;
  bool failed = false;

  Link *link = NULL;
  Id auxId = NO_ID;

  Vector2 n = {0 ,1};
  Vector2 e = {1,0};
  Vector2 s = {0,-1};
  Vector2 w = {-1,0};
  Vector2 nw = {-1,1};
  Vector2 ne = {1,1};
  Vector2 sw = {-1,-1};
  Vector2 se = {1,-1};
  Vector2 auxVector = {0,0};

  Space *auxSpace = NULL;

  int i;

  /*Struct local to this block scope, used to store needed info easily*/
  struct SpaceInfo{
    Space *space;
    Vector2 pos;
  };

  struct SpaceInfo *info = NULL;
  struct SpaceInfo *infoAux = NULL;

  if(!initSpace) return ERROR;


  /*Creates queue*/
  queue = queue_create();
  queueAux = queue_create();

  info = calloc(1, sizeof(struct SpaceInfo));
  if(!info){
    queue_destroy(queue);
    return ERROR;
  }

  info->pos.x = 0;
  info->pos.y = 0;
  info->space = initSpace;

  /*Assigns intial values to space 0 in the array of spaces*/
  space_set_isMapped(info->space,true);
  space_set_map_block(info->space, block);
  space_set_position(info->space, info->pos.x, info->pos.y);
  queue_push(queue, (void *)info);
  queue_push(queueAux, (void *)(initSpace));

  /*Cycles through queue adding neighbour spaces and mapping them*/
  while(!queue_isEmpty(queue) && !failed){
    info = (struct SpaceInfo *)queue_pop(queue);

    for (i = 0; i < 4; i++)
    {
      switch (i)
      {
        case 0:
          link = space_get_north(info->space);
          auxVector = n;
          break;
        case 1:
          link = space_get_east(info->space);
          auxVector = e;
          break;
        case 2:
          link = space_get_west(info->space);
          auxVector = w;
          break;
        case 3:
          link = space_get_south(info->space);
          auxVector = s;
          break;
        default:
          break;
      }
      if(link){
        auxId = link_get_oposite_space(link, space_get_id(info->space));
  
        if(!link_is_adjacent(link)){
          continue;
        }
        /*Gets memory for auxiliary info for queue*/
        infoAux = calloc(1, sizeof(struct SpaceInfo));
        if (!infoAux){
          /*If failed stops the loop*/
          failed = true;
          break;
        }
        /*Sets the new position*/
        infoAux->pos = info->pos;
        vector2_add(&(infoAux->pos), auxVector);
        infoAux->space = game_get_space(game, auxId);


        /*Checks if spaces has already beign processed, if not mark it, set position and add to queue*/
        if(!space_get_isMapped(infoAux->space)){
          space_set_isMapped(infoAux->space,true);
          space_set_map_block(infoAux->space, block);
          space_set_position(infoAux->space, infoAux->pos.x, infoAux->pos.y);
          queue_push(queue, (void *)infoAux);
          queue_push(queueAux, (void *)(infoAux->space));
        }else{
          /*If spaces already mapped it frees memory*/
          free(infoAux);
          infoAux = NULL;
        }
      }
    }
    /*Frees the element we poped from the queue as is no longer needed*/
    link = NULL;
    free(info);
  }

  /*If something failed in the above process, frees all the memory in the queue*/
  if(failed){
    while(queue_isEmpty(queue) == false){
      free(queue_pop(queue));
    }
    queue_destroy(queue);
    return ERROR;
  }
  queue_destroy(queue);
  
  /*Sets NW, NE, SW and SE spaces withing the Space struct as they are not considered above*/
  while(!queue_isEmpty(queueAux)){
    auxSpace = (Space *)queue_pop(queueAux);
    if(!space_get_isMapped(auxSpace)) continue;
    /*North*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, n);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector, block), N);
    /*North West*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, nw);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector, block), NW);
    /*North East*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, ne);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector, block), NE);
    /*South West*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, sw);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector, block), SW);
    /*South East*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, se);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector, block), SE);
    /*South*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, s);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector, block), S);
    /*East*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, e);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector, block), E);
    /*West*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, w);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector, block), W);
  }

  queue_destroy(queueAux);
  return OK;
}


Status game_add_space(Game *game, Space *space) {
  if(game == NULL) return ERROR;

  if ((space == NULL) || (game_get_n_spaces(game)>= MAX_SPACES)) {
    debug_log(LOG_ERROR, "Null space reference or MAX number of spaces reached at: game_add_space(Game*, Space*) in game.c");
    return ERROR;
  }

  game->spaces[game->n_spaces] = space;
  game->n_spaces++;

  debug_log(PRINT, "Game Added space: Count:%d, north: %d, east:%d, south:%d, west:%d", game_get_n_spaces(game), link_get_id(space_get_north(space)) 
            ,link_get_id(space_get_east(space)), link_get_id(space_get_south(space)), link_get_id(space_get_west(space)));
  return OK;

  return OK;
}

Status game_add_link(Game *game, Link *link){

  if ((link == NULL) || (game_get_n_links(game) >= MAX_LINKS)) {
    debug_log(LOG_ERROR, "Null space reference or MAX number of spaces reached at: game_add_space(Game*, Space*) in game.c");
    return ERROR;
  }

  game->links[(game->n_links)++] = link;
  debug_log(PRINT, "Game Added link: Count:%d, Id:%d ,Space1:%d, Space2:%d", game_get_n_links(game), link_get_id(link), link_get_space1(link), link_get_space2(link));
  return OK;
}

Status game_add_object(Game *game, Object *object){

  if(!object || !game)
    return ERROR;

  if(collection_add(game_get_objects(game), object) == ERROR)
    return ERROR;

  debug_log(PRINT,"Game Added Object: ID: %ld, name: %s, objectlocation: %ld, inventoryType: %d", object_get_id(object), object_get_name(object), object_get_location(object), object_get_type(object) - UNKNOWN_INVENTORY);
  return OK;
}

Status game_add_player(Game *game, Player *player){
  if(!game || !player || game->n_players >= MAX_PLAYERS)
    return ERROR;

  game->players[game->n_players] = player;
  game->n_players += 1;

  return OK;
}

Status game_add_event(Game *game, Event * event){
  if(!game || !event) return ERROR;
  return event_manager_add_event(game->event_manager, event);
}

Status game_add_npc(Game *game, NPC *npc){
  Entity *ent = NULL;
  Id locationid;
  
  if(!game || !npc) return ERROR;
  
  if(collection_add(game->npcs, npc) == ERROR)
    return ERROR;
    
  ent = npc_get_entity(npc);
  locationid = entity_get_location(ent);


  if(space_add_NPC(game_get_space(game, locationid), npc) == ERROR){
    return ERROR;
  }

  debug_log(PRINT,"Game Added NPC: ID: %ld, name: %s, objectlocation: %ld", entity_get_id(ent), entity_get_name(ent), entity_get_location(ent));
  return OK;
}

Status game_add_log_message(Game *game, MessageType type,char *message){
  Message *log;
  
  if(!game || !message) return ERROR;

  log = message_new(type, message);
  if(!log) return ERROR;

  if(queue_push(game->screenLog, (void *)log) == ERROR){
    free(log);
    return ERROR;
  }

  return OK;
}

Status game_get_log_message(Game *game, char *str){
  Message *log = NULL;
  if(!game) return ERROR;

  log = (Message *)queue_pop(game->screenLog);
  if(!log) return ERROR;
  
  message_get_str(log, str);

  free(log);

  return OK;
}

bool game_log_hasMessage(Game *game){
  if(!game) return false;
  return !queue_isEmpty(game->screenLog);
}

Status game_combat_start(Game *game){
  if(!game) return ERROR;

  game->combat = combat_initialize(game_get_space(game, game_get_player_location(game)), game->active_player, command_get_code(game->last_cmd));
  if(!game->combat) return ERROR;
  //AQUÍ METERLO!!!!

  game_reader_load_attacks(game);

  game->current_state = COMBAT;
  return OK;
}

Status game_combat_end(Game *game){
  if(!game) return ERROR;

  combat_free(game->combat);
  game->current_state = DEFAULT;
  game->combat = NULL;
  return OK;
}

Combat *game_get_combat(Game *game){
  if(!game) return NULL;
  return game->combat;
}

int game_switch_player(Game *game, int player){
  int i;
  bool alivePlayers = false;
  Entity *playerEnt = NULL;

  if(!game || player >= game->n_players) return ERROR;

  if(player < 0){
    /*Sets player to next*/
    /*cycles through players checking if they are dead, if so, skip to next*/
    for (i = 0; i < game->n_players; i++)
    {
      game->active_player_index = (game->active_player_index + 1) % game->n_players;
      playerEnt = player_get_entity(game->players[game->active_player_index]);

      if(entity_is_dead(playerEnt) == false){
        alivePlayers = true;
        break;
      }

      //game->active_player_index = (game->active_player_index + 1) % game->n_players;
    }

    if(!alivePlayers){
      game_add_log_message(game, MESSAGE_ERROR, "Couldn't find a player which is alive");
      game_set_finished(game, 1);
      return -2;/*Case if every player is dead*/
    } 
  } 
  else{
    playerEnt = player_get_entity(game->players[player]) ;
    if(entity_is_dead(playerEnt) == true){
      game_add_log_message(game, MESSAGE_ERROR, "Couldn't switch player because it isn't alive");
      return -1;
    }
    game->active_player_index = player;
  } 
  
  game->active_player = game->players[game->active_player_index];
  command_set_player_data(game->last_cmd, player_get_cmdData(game->active_player));

  return 0;
}

AbilityManager *game_get_ability_manager(Game *game){
  if(!game) return NULL;
  return game->ability_manager;
}

/*
Player *game_get_player_by_id(Game *game, Id id){
  int i;
  Player *player = NULL;
  
  if(!game) return NULL;

  for(i = 0; i < game->n_players; i++){
    player = game->players[i];
    if(entity_get_id(player_get_entity(player)) == id){
      return player;
    }
  }

  return NULL;
}*/

NPC *game_get_NPC_by_id(Game *game, Id id){
  int i, size;
  NPC *npc = NULL;
  Collection *collection = NULL;
  
  if(!game) return NULL;

  collection = game_get_npcs(game);
  size = collection_length(collection);
  for(i = 0; i < size; i++){
    npc = (NPC *)collection_get_element_at(collection, i);
    if(entity_get_id(npc_get_entity(npc)) == id){
      return npc;
    }
  }

  return NULL;
}

Object *game_get_object_by_id(Game *game, Id objectid){
  Object *object = NULL;
  Collection *collection = NULL;
  int i, size;
  
  if(!game) return NULL;

  collection = game_get_objects(game);
  size = collection_length(collection);
  for(i = 0; i < size; i++){
    object = collection_get_element_at(collection, i);
    if(object_get_id(object) == objectid)
      return object;
  }

  return NULL;
}

Status game_add_ability(Game *game, Ability *ability){
  Player *player = NULL;
  NPC *npc = NULL;
  Entity *entity = NULL;
  Object *object = NULL;

  Id entityid;
  
  if(!game || !ability) return ERROR;

  entityid = ability_get_entityid(ability);

  if(ability_manager_add_ability_to_cd(game_get_ability_manager(game), ability) == ERROR) return ERROR;

  if(ability_get_is_player_ability(ability) == true){
    player = game_get_player_by_id(game, entityid);
    if(!player)
      return ERROR;

    entity = player_get_entity(player);
    return entity_add_ability(entity, ability);
  } else if(ability_get_is_object_use(ability) == false) {
    npc = game_get_NPC_by_id(game, entityid);
    if(!npc)
      return ERROR;

    entity = npc_get_entity(npc);
    return entity_add_ability(entity, ability);
  } else {
    object = game_get_object_by_id(game, entityid);

    return object_add_object_effect(object, ability);
  }

  return ERROR;
}

#define RANDOM_WALK_ITERATIONS 3
#define RANDOM_WALK_STEPS 5
#define RANDOM_WALK_STEP_DIR 1

Status game_generate_procedural(){
  typedef enum {NO_SPACE = 0, MARKED}SpaceStatus;
  
  struct SpaceInfo{
    Space *current;
    Space *origin;
  };

  SpaceStatus map[MAX_PROCEDURAL_SIZE][MAX_PROCEDURAL_SIZE] = {{NO_SPACE,NO_SPACE}};
  Space *spaces[MAX_PROCEDURAL_SIZE][MAX_PROCEDURAL_SIZE] = {NULL};

  Collection *links = NULL;

  Queue *spaceQ = NULL;

  int x, y;
  int dirX, dirY;
  int stepCount = 0;

  int i,j;

  int spaceCount = 0;
  Space *space = NULL;
  Link *link = NULL;


  x = MAX_PROCEDURAL_SIZE/2;
  y = x;
  map[x][y] = MARKED;

  
  for (i = 0; i < RANDOM_WALK_ITERATIONS; i++)
  {
    stepCount = 0;
    for (j = 0; j < RANDOM_WALK_STEPS; j++)
    {
      if(x < 0 || x >= MAX_PROCEDURAL_SIZE || y < 0 || y >= MAX_PROCEDURAL_SIZE){
        break;
      }

      if(stepCount >= RANDOM_WALK_STEP_DIR){
        stepCount = 0;
      }
      
      if(stepCount == 0){
        dirX = rand()%3 - 2;
        if(dirX == 0){
          dirY = rand()%3 -2;
          if(dirY == 0){
            dirY++;
          }
        }else{
          dirY = 0;
        }
      }
      x += dirX;
      y += dirY;

      map[x][y] = MARKED;

      stepCount++;
    }
    x = MAX_PROCEDURAL_SIZE/2;
    y = x;
  }
  
  spaceCount = 1;
  for (x = 0; i < MAX_PROCEDURAL_SIZE; i++)
  {
    for (y = 0; i < MAX_PROCEDURAL_SIZE; i++)
    {
      if(map[x][y] == MARKED){
        space = space_create(spaceCount++);
        if(!space){
          return ERROR;
        }
        space_set_position(space, x, y);
      }
    }
  }
  


}


