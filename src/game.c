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

/**
   Game interface implementation
*/

Status game_create(Game *game) {
  int i;

  for (i = 0; i < MAX_SPACES; i++) {
    game->spaces[i] = NULL;
  }

  game->n_spaces = 0;
  game->player_location = NO_ID;
  game->object_location = NO_ID;
  game->last_cmd = command_create();
  game->finished = false;

  return OK;
}


Status game_destroy(Game *game) {
  int i = 0;

  for (i = 0; i < game->n_spaces; i++) {
    space_destroy(game->spaces[i]);
  }

  command_destroy(game->last_cmd);

  return OK;
}

void game_print(Game *game) {
  int i = 0;

  printf("\n\n-------------\n\n");

  printf("=> Spaces: \n");
  for (i = 0; i < game->n_spaces; i++) {
    space_print(game->spaces[i]);
  }

  printf("=> Object location: %d\n", (int)game->object_location);
  printf("=> Player location: %d\n", (int)game->player_location);
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

Id game_get_player_location(Game *game) { return game->player_location; }

Id game_get_object_location(Game *game) { return game->object_location; }

Command* game_get_last_command(Game *game) { return game->last_cmd; }

bool game_get_finished(Game *game) { return game->finished; }

Id game_get_space_id_at(Game *game, int position) {
  if (position < 0 || position >= game->n_spaces) {
    debug_log(LOG_WARNING, "No Id assigned at: game_get_space_id_at(Game*, int) in game.c");
    return NO_ID;
  }

  return space_get_id(game->spaces[position]);
}

#pragma endregion

#pragma region SETTERS

Status game_set_player_location(Game *game, Id id) {
  if (id == NO_ID) {
    debug_log(LOG_WARNING, "No Id assigned at: game_set_player_location(Game*, Id) in game.c");
    return ERROR;
  }

  game->player_location = id;

  return OK;
}

Status game_set_object_location(Game *game, Id id) {

  if (id == NO_ID) {
        debug_log(LOG_WARNING, "No Id assigned at: game_set_object_location(Game*, Id) in game.c");
    return ERROR;
  }

  game->object_location = id;
  space_set_object(game_get_space(game, id), true);
  return OK;
}
Status game_set_last_command(Game *game, Command *command) {
  game->last_cmd = command;

  return OK;
}

Status game_set_finished(Game *game, bool finished) {
  game->finished = finished;

  return OK;
}

#pragma endregion

/*
* Other functions that are not getters or setters
*/

Status game_add_space(Game *game, Space *space) {
  if ((space == NULL) || (game->n_spaces >= MAX_SPACES)) {
    debug_log(LOG_ERROR, "Null space reference or MAX number of spaces reached at: game_add_space(Game*, Space*) in game.c");
    return ERROR;
  }

  game->spaces[game->n_spaces] = space;
  game->n_spaces++;

  return OK;
}



