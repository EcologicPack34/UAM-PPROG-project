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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLLECTION_INITIAL_SIZE 10  /*!< Collection initial size*/


struct _Game {
  /*Entity related*/
  Player *player;              /*!< Contains all the information related to the player */
  Collection *objects;         /*!< Contains all the information related to the object */
  Collection *npcs;            /*!< Contains all the information related to the NPCs*/

  /*Space related*/
  Space *spaces[MAX_SPACES];   /*!< Array with all the spaces of the map */
  int n_spaces;                /*!< int with the number of spaces on *spaces */
  Link *links[MAX_LINKS];     /*!< Array with all the links in the map*/
  int n_links;

  /*Others*/
  EventManager *event_manager;

  GameState current_state;     /*!< Enum storing the current game state*/
  Command *last_cmd;           /*!< string with the last command */
  bool finished;               /*!< bool that determines if the game has finished*/
};

/**
 * @brief Gets the link in a certain position of the game links array
 * @author Daniel Gómez
 * 
 * @param game 
 * @param index 
 * @return Link* 
 */
Link *game_get_link_at(Game *game, long index){
  if(!game) return NULL;

  if(index < 0 || index >= game_get_n_links(game))
    return NULL;

  return game->links[index];
}



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
  (*game)->player = NULL; /*Player creation is controlled by game_reader*/
  (*game)->objects = collection_create(COLLECTION_INITIAL_SIZE, false, true, object_isEqual, object_print);
  if(!((*game)->objects)) debug_log(LOG_ERROR,"Error initializing collection of objects");
  (*game)->npcs = collection_create(COLLECTION_INITIAL_SIZE, false, true, npc_cmp, npc_print); /*TEMPORAL PRINT*/
  if(!((*game)->npcs)) debug_log(LOG_ERROR,"Error initializing collection of npcs");
  (*game)->last_cmd = command_create();
  if(!((*game)->last_cmd)) debug_log(LOG_ERROR,"Error creating command");
  (*game)->finished = false;
  (*game)->n_links = 0;
  (*game)->current_state = DEFAULT;

  (*game)->event_manager = event_manager_create();
  if(!((*game)->event_manager)) debug_log(LOG_ERROR,"Error creating event manager");

  return OK;
}


Status game_destroy(Game *game) {
  int i = 0;
  int linkCount;

  /*Destroys all spaces*/
  for (i = 0; i < game->n_spaces; i++) {
    space_destroy(game->spaces[i]);
  }

  player_destroy(game_get_player(game));
  
  collection_free_elements(game_get_objects(game), object_destroy);
  collection_destroy(game_get_objects(game));

  if(collection_free_elements(game_get_npcs(game), npc_destroy) == ERROR){
    printf("Error liberando colleccion de npcs");
  }
  collection_destroy(game_get_npcs(game));

  command_destroy(game->last_cmd);
  event_manager_destroy(game->event_manager);

  /*Destroys all links */
  linkCount = game_get_n_links(game);

  for (i = 0; i < linkCount; i++)
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

Player* game_get_player(Game *game) {return game->player; }

Id game_get_player_location(Game *game) {return entity_get_location(player_get_entity(game_get_player(game)));}

Command* game_get_last_command(Game *game) { return game->last_cmd; }

bool game_get_finished(Game *game) { return game->finished; }

Id game_get_space_id_at(Game *game, int position) {
  if (position < 0 || position >= game->n_spaces) {
    debug_log(LOG_WARNING, "No Id assigned at: game_get_space_id_at(Game*, int) in game.c");
    return NO_ID;
  }

  return space_get_id(game->spaces[position]);
}

Space *game_get_space_by_position(Game *game, Vector2 pos){
  int i;
  Space *space = NULL;
  
  if(!game) return NULL;

  for (i = 0; i < game->n_spaces; i++)
  {
    space = game->spaces[i];
    if(vector2_isEqual(&pos, space_get_position(space)) == 0){
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

/*
* Other functions that are not getters or setters
*/

Status game_spatial_map(Game *game){
  Queue *queue = NULL;
  bool failed = false;

  Link *north = NULL, *east = NULL, *south = NULL, *west = NULL;
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

  if(!game) return ERROR;


  /*Creates queue*/
  queue = queue_create();

  info = calloc(1, sizeof(struct SpaceInfo));
  if(!info){
    queue_destroy(queue);
    return ERROR;
  }

  info->pos.x = 0;
  info->pos.y = 0;
  info->space = game->spaces[0];

  /*Assigns intial values to space 0 in the array of spaces*/
  space_set_isMapped(info->space,true);
  space_set_position(info->space, info->pos.x, info->pos.y);
  queue_push(queue, (void *)info);

  /*Cycles through queue adding neighbour spaces and mapping them*/
  while(!queue_isEmpty(queue)){
    info = (struct SpaceInfo *)queue_pop(queue);

    north = space_get_north(info->space);
    east = space_get_east(info->space);
    south = space_get_south(info->space);
    west = space_get_west(info->space);

    /*Checks if a connexion exist for each direction*/
    if(north){
      auxId = link_get_oposite_space(north, space_get_id(info->space));
      
      if(link_is_adjacent(north)){
        /*Gets memory for auxiliary info for queue*/
        infoAux = calloc(1, sizeof(struct SpaceInfo));
        if(!infoAux){
          /*If failed stops the loop*/
          failed = true;
          break;
        }

        /*Sets the new position*/
        infoAux->pos = info->pos;
        vector2_add(&(infoAux->pos), n);
        infoAux->space = game_get_space(game, auxId);
        
        /*Checks if spaces has already beign processed, if not mark it, set position and add to queue*/
        if(!space_get_isMapped(infoAux->space)){
          space_set_isMapped(infoAux->space,true);
          space_set_position(infoAux->space, infoAux->pos.x, infoAux->pos.y);
          queue_push(queue, (void *)infoAux);
        }else{
          /*If spaces already mapped it frees memory*/
          free(infoAux);
        }

      }
    }
    /*Comments from the above block scope applies to the next 3 blocks*/
    if(east){
      auxId = link_get_oposite_space(east, space_get_id(info->space));
      
      if(link_is_adjacent(east)){
        infoAux = calloc(1, sizeof(struct SpaceInfo));
        if(!infoAux){
          failed = true;
          break;
        }

        infoAux->pos = info->pos;
        vector2_add(&(infoAux->pos), e);
        infoAux->space = game_get_space(game, auxId);

        
        if(!space_get_isMapped(infoAux->space)){
          space_set_isMapped(infoAux->space,true);
          space_set_position(infoAux->space, infoAux->pos.x, infoAux->pos.y);
          queue_push(queue, (void *)infoAux);
        }else{
          free(infoAux);
        }
      }
    }
    if(south){
      auxId = link_get_oposite_space(south, space_get_id(info->space));
      
      if(link_is_adjacent(south)){
        infoAux = calloc(1, sizeof(struct SpaceInfo));
        if(!infoAux){
          failed = true;
          break;
        }

        infoAux->pos = info->pos;
        vector2_add(&(infoAux->pos), s);
        infoAux->space = game_get_space(game, auxId);

        
        if(!space_get_isMapped(infoAux->space)){
          space_set_isMapped(infoAux->space,true);
          space_set_position(infoAux->space, infoAux->pos.x, infoAux->pos.y);
          queue_push(queue, (void *)infoAux);
        }else{
          free(infoAux);
        }
      }
    }
    if(west){
      auxId = link_get_oposite_space(west, space_get_id(info->space));
      
      if(link_is_adjacent(west)){
        infoAux = calloc(1, sizeof(struct SpaceInfo));
        if(!infoAux){
          failed = true;
          break;
        }

        infoAux->pos = info->pos;
        vector2_add(&(infoAux->pos), w);
        infoAux->space = game_get_space(game, auxId);

        
        if(!space_get_isMapped(infoAux->space)){
          space_set_isMapped(infoAux->space,true);
          space_set_position(infoAux->space, infoAux->pos.x, infoAux->pos.y);
          queue_push(queue, (void *)infoAux);
        }else{
          free(infoAux);
        }
      }
    }

    /*Frees the element we poped from the queue as is no longer needed*/
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
  for (i = 0; i < game->n_spaces; i++)
  {
    auxSpace = game->spaces[i];
    if(!space_get_isMapped(auxSpace)) continue;
    /*North*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, n);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector), N);
    /*North West*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, nw);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector), NW);
    /*North East*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, ne);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector), NE);
    /*South West*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, sw);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector), SW);
    /*South East*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, se);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector), SE);
    /*South*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, s);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector), S);
    /*East*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, e);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector), E);
    /*West*/
    vector2_copy(&auxVector, space_get_position(auxSpace));
    vector2_add(&auxVector, w);
    space_set_neighbour(auxSpace, game_get_space_by_position(game, auxVector), W);
  }
  
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

  debug_log(PRINT,"Game Added Object: ID: %ld, name: %s, objectlocation: %ld, inventoryType: %d", object_get_id(object), object_get_name(object), object_get_location(object), (int)object_get_type(object));
  return OK;
}

Status game_add_player(Game *game, Player *player){
  if(!game || !player)
    return ERROR;

  game->player = player;

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