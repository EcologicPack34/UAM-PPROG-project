/**
 * @brief It defines the game reader public interface
 *
 * Defines how data is read by the .dat and saves it on the game struct.
 * 
 * @file game_reader.c
 * @author Daniel Gómez Rodríguez
 * @version 0
 * @date 8-02-2025
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_reader.h"
#include "debug_printing.h"
#include "game.h"
#include "space.h"

/*
* Declaration of private functions
*/

/**
 * @brief Reads the filename to save all space and their links
 * @author Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @return OK if everything goes well or ERROR if there was some error
 */
Status game_reader_load_spaces(Game *game, char *filename);

/**
 * @brief Reads the filename to save all links between spaces
 * @author Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @return OK if everything goes well or ERROR if there was some error
 */
Status game_reader_load_links(Game *game, char *filename);

/**
 * @brief Reads the filename to save all objects
 * @author Maksym Polyak
 * 
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @return OK if everything goes well or ERROR if there was some error
 */
Status game_reader_load_objects(Game *game, char *filename);


/*
* Public functions implementation
*/
Status game_reader_create_from_file(Game **game, char *filename){
  Entity *player;
  Object *object;
  if (game_create(game) == ERROR){
    debug_log(LOG_ERROR, "Error creating game at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }

  if(game_reader_load_links(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading links at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if (game_reader_load_spaces(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading spaces at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if (game_reader_load_objects(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading objects at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }

  /* The player is located in the first space */
  player = player_get_entity(game_get_player(*game));
  entity_set_location(player, game_get_space_id_at(*game, 0));

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
  bool locked = false, adjacent = false;
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
      adjacent = atol(toks);
      toks = strtok(NULL, "|");
      locked = atol(toks);
      toks = strtok(NULL, "|");
      unlockingObject = atol(toks);


      debug_log(PRINT,"Read Link: #l:%ld|%ld|%ld|%ld|%ld|%ld", id, space1, space2, adjacent,locked, unlockingObject);

      link = link_create(id,space1, space2, adjacent,locked, unlockingObject);
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

      debug_log(PRINT,"Read Space: #s:%ld|%s|%ld|%ld|%ld|%ld", id, name, north, east, south, west);

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

Status game_reader_load_objects(Game *game, char *filename){
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  long objectid, objectlocation;
  InventoryType objectlocationtype;
  Object *object = NULL;

  Status status = OK;

  if (!filename) {
    debug_log(LOG_ERROR, "Missing file name at: game_reader_load_objects(Game*, char*) in game_reader.c");
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_objects(Game*, char*) in game_reader.c");
    return ERROR;
  }


  /*Gets each line of the data file, uses strtok to shred it and 
  saves each location ID on static memory*/
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#o:", line, 3) == 0) {
      toks = strtok(line + 3, "|");
      objectid = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      objectlocation = atol(toks);
      toks = strtok(NULL, "|");
      objectlocationtype = (InventoryType)(toks);

      debug_log(PRINT,"Read Object: #o:%ld|%s|%ld|%ld", objectid, name, objectlocation, objectlocationtype);

      /*Creates a object with object_create then saves it on the game with game_add_space*/
      object = object_create(objectid, name, objectlocation, objectlocationtype);
      if (object != NULL) {
        game_add_object(game, object);
        switch(objectlocationtype){
          case UNKNOWN_INVENTORY: return ERROR;
          case PLAYER_INVENTORY:
            inventory_add_object(entity_get_inventory(player_get_entity(game_get_player(game))), object);
            break;
          case NPC_INVENTORY:
            /* NON IMPLEMENTEDinventory_add_object()*/
            break;
          case SPACE_INVENTORY:
            inventory_add_object(space_get_inventory(game_get_space(game, objectlocation)), object);
            break;
        }
      }
    }
  }

  if (ferror(file)) {
    status = ERROR;
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_objects(Game*, char*) in game_reader.c");
  }

  fclose(file);

  return status;
}