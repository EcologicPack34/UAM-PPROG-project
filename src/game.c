/**
 * @brief It implements the game structure
 *
 * @file game.c
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "debug_printing.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOMBRE_PLAYER "Heroe" /*!< Nombre del player */
#define PLAYER_BASE_ID 1      /*!< Id del player */
#define OBJECT_NAME "Grain"   /*!< Nombre del objeto principal */

#pragma region PRIVATE_DECLARATIONS

/**
 * @brief Gets the link in a certain position of the game links array
 * @author Daniel Gómez
 * 
 * @param game 
 * @param index 
 * @return Link* 
 */
Link *game_get_link_at(Game *game, long index);

#pragma endregion

#pragma region PRIVATE_IMPLEMENTATION

Link *game_get_link_at(Game *game, long index){
  if(!game) return NULL;

  if(index < 0 || index >= game_get_n_links(game))
    return NULL;

  return game->links[index];
}

#pragma endregion



/**
   Game interface implementation
*/

Status game_create(Game *game) {
  int i;

  for (i = 0; i < MAX_SPACES; i++) {
    game->spaces[i] = NULL;
  }

  game->n_spaces = 0;
  game->player = player_create(NOMBRE_PLAYER, (Id)PLAYER_BASE_ID, -1);
  /*Creates the object with the first id not taken by the spaces*/
  game->object = object_create(game_get_space_id_at(game, game_get_n_spaces(game) - 1) + 1, OBJECT_NAME);
  game->last_cmd = command_create();
  game->finished = false;

  return OK;
}


Status game_destroy(Game *game) {
  int i = 0;

  for (i = 0; i < game->n_spaces; i++) {
    space_destroy(game->spaces[i]);
  }

  player_destroy(game_get_player(game));
  object_destroy(game_get_object(game));

  command_destroy(game->last_cmd);

  for (int i = 0; i < game_get_n_links(game); i++)
  {
    free(game_get_link_at(game,i));
  }

  return OK;
}

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

#pragma region GETTERS

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

Id game_get_player_location(Game *game) {return entity_get_location((Entity *)game_get_player(game));}

Object* game_get_object(Game *game) {return game->object; }

Id game_get_object_location(Game *game) {return (object_get_location(game_get_object(game)));}

Command* game_get_last_command(Game *game) { return game->last_cmd; }

bool game_get_finished(Game *game) { return game->finished; }

Id game_get_space_id_at(Game *game, int position) {
  if (position < 0 || position >= game->n_spaces) {
    debug_log(LOG_WARNING, "No Id assigned at: game_get_space_id_at(Game*, int) in game.c");
    return NO_ID;
  }

  return space_get_id(game->spaces[position]);
}

int game_get_n_spaces(Game *game) {
  if(!game) return -1;
  return game->n_spaces;
}

long game_get_n_links(Game *game){
  if(!game) return -1;
  return game->n_links;
}

#pragma endregion

#pragma region SETTERS

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

Status game_set_object_location(Game *game, Id id){
  object_set_location(game_get_object(game), id);

  return OK;
}

/**
 * @brief Sets the object location
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param id id with the location of the object
 * @return OK if everything went well or ERROR if there was a mistake
 */
Status game_set_object_location(Game *game, Id id);

#pragma endregion

/*
* Other functions that are not getters or setters
*/

Status game_add_space(Game *game, Space *space) {
  if(game == NULL) return ERROR;

  if ((space == NULL) || (game_get_n_spaces(game)>= MAX_SPACES)) {
    debug_log(LOG_ERROR, "Null space reference or MAX number of spaces reached at: game_add_space(Game*, Space*) in game.c");
    return ERROR;
  }

  game->spaces[game->n_spaces] = space;
  game->n_spaces++;

  debug_log(DEBUG, "Game Added space: Count:%d, north: %d, east:%d, south:%d, west:%d", game_get_n_spaces(game), link_get_id(space_get_north(space)) 
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
  debug_log(DEBUG, "Game Added link: Count:%d, Id:%d ,Space1:%d, Space2:%d", game_get_n_links(game), link_get_id(link), link_get_space1(link), link_get_space2(link));
  return OK;
}

