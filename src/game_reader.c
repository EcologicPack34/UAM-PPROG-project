#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug_printing.h"
#include "game.h"
#include "space.h"

/*
* Declaration of private functions
*/
/**
 * @brief Reads the filename to save all space and their contiguous spaces
 *
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @return OK if everything goes well or ERROR if there was some error
 */
Status game_reader_load_spaces(Game *game, char *filename);

Status game_reader_load_links(Game *game, char *filename);


/*
* Public functions implementation
*/
Status game_reader_create_from_file(Game *game, char *filename){
  Entity *player;
  Object *object;
  if (game_create(game) == ERROR){
    debug_log(LOG_ERROR, "Error creating game at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }

  if(game_reader_load_links(game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading links at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if (game_reader_load_spaces(game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading spaces at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }

  /* The player and the object are located in the first space */
  player = (Entity*)game_get_player(game);
  entity_set_location(player, game_get_space_id_at(game, 0));

  object = game_get_object(game);
  object_set_location(object, game_get_space_id_at(game, 0));

  return OK;
}

/*
* Private functions implementation
*/

Status game_reader_load_links(Game *game, char *filename){
  FILE *file = NULL;

  char line[WORD_SIZE] = "";
  char *toks = NULL;

  Id id = NO_ID, space1 = NO_ID, space2 = NO_ID, unlockingObject = NO_ID;
  bool locked = false;
  Link *link = NULL;
  Status status = OK;

  if (!filename) {
    debug_log(LOG_ERROR, "Missing file name at: game_reader_load_links(Game*, char*) in game_reader.c");
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_links(Game*, char*) in game_reader.c");
    return ERROR;
  }

  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#l:", line, 3) == 0) {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      space1 = atol(toks);
      toks = strtok(NULL, "|");
      space2 = atol(toks);
      toks = strtok(NULL, "|");
      locked = atol(toks);
      toks = strtok(NULL, "|");
      unlockingObject = atol(toks);


      debug_log(DEBUG,"Read Link: #l:%ld|%ld|%ld|%ld|%ld", id, space1, space2, locked, unlockingObject);

      link = link_create(id,space1, space2, locked, unlockingObject);
      if(link != NULL){
        game_add_link(game, link);
      }
    }
  }

  if (ferror(file)) {
    status = ERROR;
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_spaces(Game*, char*) in game_reader.c");
  }

  fclose(file);

  return status;
}


Status game_reader_load_spaces(Game *game, char *filename) {
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, north = NO_ID, east = NO_ID, south = NO_ID, west = NO_ID;
  Space *space = NULL;
  Status status = OK;

  if (!filename) {
    debug_log(LOG_ERROR, "Missing file name at: game_reader_load_spaces(Game*, char*) in game_reader.c");
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_spaces(Game*, char*) in game_reader.c");
    return ERROR;
  }


  /*Gets each line of the data file, uses strtok to shred it and 
  saves each location ID on static memory*/
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#s:", line, 3) == 0) {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      north = atol(toks);
      toks = strtok(NULL, "|");
      east = atol(toks);
      toks = strtok(NULL, "|");
      south = atol(toks);
      toks = strtok(NULL, "|");
      west = atol(toks);

      debug_log(DEBUG,"Read Space: #s:%ld|%s|%ld|%ld|%ld|%ld", id, name, north, east, south, west);

      /*Creates a space with space_create, and sets the ID on that space
      then saves it on the game with game_add_space*/
      space = space_create(id);
      if (space != NULL) {
        space_set_name(space, name);
        space_set_north(space, game_get_link_by_id(game, north));
        space_set_east(space, game_get_link_by_id(game, east));
        space_set_south(space, game_get_link_by_id(game, south));
        space_set_west(space, game_get_link_by_id(game, west));

        game_add_space(game, space);
      }
    }
  }

  if (ferror(file)) {
    status = ERROR;
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_spaces(Game*, char*) in game_reader.c");
  }

  fclose(file);

  return status;
}