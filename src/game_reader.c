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
#include <ctype.h>

#include "game_reader.h"
#include "debug_printing.h"
#include "game.h"
#include "space.h"
#include "event_manager.h"
#include "effect.h"
#include "attack.h"

/*
* Declaration of private functions
*/

/**
 * @brief Reads the filename to load all space and their links
 * @author Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @param fromSaveFile bool to indicate if the spaces are loading from a save file or a new file
 * @return OK if everything goes well or ERROR if there was some error
 */
Status game_reader_load_spaces(Game *game, char *filename, bool fromSaveFile);

/**
 * @brief Reads the filename to load all links between spaces
 * @author Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @return OK if everything goes well or ERROR if there was some error
 */
Status game_reader_load_links(Game *game, char *filename);

/**
 * @brief Reads the filename to load all objects
 * @author Maksym Polyak
 * 
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @return OK if everything goes well or ERROR if there was some error
 */
Status game_reader_load_objects(Game *game, char *filename);

/**
 * @brief Reads the filename to load the player information
 * @author Maksym Polyak
 * 
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @param fromSaveFile bool to indicate if the spaces are loading from a save file or a new file
 * @return OK if everything goes well or ERROR if there was some error
 */
Status game_reader_load_player(Game *game, char *filename, bool fromSaveFile);

/**
 * @brief Reads the file to load all events
 * @author Daniel Gómez
 * 
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @return Status 
 */
Status game_reader_load_events(Game *game, char *filename);

/**
 * @brief Reads the file to load all npcs
 * @author Maksym Polyak
 * 
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @return Status 
 */
Status game_reader_load_npcs(Game *game, char *filename);

/**
 * @brief Loads command info from settings file
 * @author Daniel Gómez
 * 
 * @param game struct that saves all information related to the game
 * @return Status 
 */
Status game_reader_load_commandInfo(Game *game);

/**
 * @brief Loads valid commands for each game state
 * @author Daniel Gómez
 * 
 * @param game struct that saves all information related to the game
 * @return Status 
 */
Status game_reader_load_commandStateTypes(Game *game);

/**
 * @brief Reads the file to load stats to an entity
 * @author Aaron Charameli Mair
 * 
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @return Status 
 */
Status game_reader_load_stats(Game *game, char *filename);

/**
 * @brief Reads the file to load all ability
 * @author Maksym Polyak
 *
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @return Status 
*/
Status game_reader_load_ability(Game *game, char *filename);

/**
 * @brief Reats the file to load all effects
 * @author Aaron Charameli Mair
 * 
 * @param game 
 * @param filename 
 * @return Status 
 */
Status game_reader_load_effects(Game *game, char *filename);

/**
 * @brief Reads the file settings to load the attacks
 * @author Sofía Calvo
 *
 * @param game struct that saves all information related to the game
 * @param filename string that stores the data file name
 * @return Status 
*/
Status game_reader_load_attacks(Game *game);

/**
 * @brief Reads the file to load all graphic descriptions
 * @author Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_reader_load_gdesc(Game *game, char *filename);


/**
 * @brief Gets if the game should generate proceduraly or not by reading from settings file
 * @author Daniel Gómez
 * 
 * @return true 
 * @return false 
 */
bool game_reader_generate_procedural();

/**
 * @brief Reads the filename to load all objects from a file in a different format as usual, for saved game files
 * 
 * @param game 
 * @param filename 
 * @return Status 
 * @note this function loads objects, attacks.
 */
Status game_reader_load_others_from_save_file(Game *game, char *filename);

/**
 * @brief Private function to determine if the file is a save file or not
 * @author Maksym Polyak
 * 
 * @param filename filename
 * @return true if it is a save file
 * @return false if it is not a save file
 */
bool game_reader_is_save_file(char *filename);

/*
* Public functions implementation
*/
Status game_reader_create_from_file(Game **game, char *filename){
  
  if(game_reader_is_save_file(filename) == true){
    return game_reader_create_from_save_file(game, filename);
  }
  
  if (game_create(game) == ERROR){
    debug_log(LOG_ERROR, "Error creating game at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    printf("%c[2J", 27);
    printf("Fatal error. Check the log for details\n");
    debug_force_global_fclose();
    abort();
  }
  /*Loads settings*/
  
  if(game_reader_load_commandInfo(*game) == ERROR){
    printf("%c[2J", 27);
    printf("Fatal error. Check the log for details\n");
    debug_force_global_fclose();
    abort();
  }
  if(game_reader_load_commandStateTypes(*game) == ERROR){
    printf("%c[2J", 27);
    printf("Fatal error. Check the log for details\n");
    debug_force_global_fclose();
    abort();
  }  
  
  if(game_reader_load_gdesc(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading graphic descriptions at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  
  if(game_reader_generate_procedural()){
    if(game_generate_procedural(*game) == ERROR){
      printf("%c[2J", 27);
      printf("Fatal error. Check the log for details\n");
      debug_force_global_fclose();
      abort();
    }
  }else{
    if(game_reader_load_links(*game, filename) == ERROR){
      debug_log(LOG_ERROR, "Error loading links at: game_reader_create_from_file(Game*, char*) in game_reader.c");
      return ERROR;
    }
    if (game_reader_load_spaces(*game, filename, false) == ERROR){
      debug_log(LOG_ERROR, "Error loading spaces at: game_reader_create_from_file(Game*, char*) in game_reader.c");
      return ERROR;
    }
  }
  if (game_reader_load_events(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading events at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if (game_reader_load_npcs(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading npcs at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if(game_reader_load_player(*game, filename, false) == ERROR){
    debug_log(LOG_ERROR, "Error loading player at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if(game_reader_load_stats(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading stats at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if (game_reader_load_objects(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading objects at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if (game_reader_load_ability(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading ability at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if (game_reader_load_effects(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading ability at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if(game_reader_load_attacks(*game) == ERROR) {
    debug_log(LOG_ERROR,"Error loading attacks");
    return ERROR;
  }

  /*Loads data into the game*/
  
  if(game_spatial_map(*game) == ERROR){
    debug_log(LOG_ERROR,"Error maping spatialy spaces");
    return ERROR;
  }
  

  /*Initialization*/
  /*Sets the actual player as the first one read in the .dat*/
  game_switch_player(*game, 0);

  return OK;
}

Status game_reader_create_from_save_file(Game **game, char *filename){
  int active_player_index, is_turn_valid, godmode, current_state, finished, procedural;
  FILE *fIN = NULL;
  char str[WORD_SIZE] = "";

  if(!game || !filename) return ERROR;

  if (game_create(game) == ERROR){
    debug_log(LOG_ERROR, "Error creating game at: game_reader_create_from_save_file in game_reader.c");
    printf("%c[2J", 27);
    printf("Fatal error. Check the log for details\n");
    debug_force_global_fclose();
    abort();
  }

  if(game_reader_load_commandInfo(*game) == ERROR){
    printf("%c[2J", 27);
    printf("Fatal error. Check the log for details\n");
    debug_force_global_fclose();
    abort();
  }
  if(game_reader_load_commandStateTypes(*game) == ERROR){
    printf("%c[2J", 27);
    printf("Fatal error. Check the log for details\n");
    debug_force_global_fclose();
    abort();
  }  

  if((fIN = fopen(filename, "r")) == NULL) return ERROR;

  fgets(str,WORD_SIZE, fIN);
  fscanf(fIN, "%d;%d;%d;%d;%d;%d\n", &active_player_index,\
    &is_turn_valid, &godmode, &finished, &procedural, &current_state);
 
  game_set_basic_info(*game, active_player_index, is_turn_valid,\
   godmode, finished, procedural, current_state);

  fclose(fIN);
  
  if(game_reader_load_gdesc(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading graphic descriptions at: game_reader_create_from_save_file in game_reader.c");
    return ERROR;
  }

  if(game_reader_load_links(*game, filename) == ERROR){
      debug_log(LOG_ERROR, "Error loading links at: game_reader_create_from_file(Game*, char*) in game_reader.c");
      return ERROR;
    }

  if(game_reader_load_spaces(*game, filename, true) == ERROR){
      debug_log(LOG_ERROR, "Error loading links at: game_reader_create_from_file(Game*, char*) in game_reader.c");
      return ERROR;
    }

  /*aqui van los eventos*/

  if (game_reader_load_npcs(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading npcs at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if(game_reader_load_player(*game, filename, true) == ERROR){
    debug_log(LOG_ERROR, "Error loading player at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if(game_reader_load_stats(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading stats at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if(game_reader_load_others_from_save_file(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading stats at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }

  game_switch_player(*game, active_player_index);

  return OK;
}

Status game_reader_create_save_file(char *save_file, Game *game){
  FILE *Poriginal=NULL;
  FILE *Psave_file=NULL;
  int i;
  int size=0; /*aux size variable for multiple uses*/

  if(!save_file || !game) return ERROR;

  if((Psave_file = fopen(save_file, "w")) == NULL){
    fclose(Poriginal);
    return ERROR;
  }

  fprintf(Psave_file, "SAVE\n");

  /*ActivePlayerIndex;Is_Turn_Valid;godmode;finished;procedural;GameState*/
  fprintf(Psave_file, "%d;%d;%d;%d;%d;%d\n", \
    game_get_active_player_index(game), (int)game_get_is_turn_valid(game),\
    (int)game_get_god_mode(game), (int)game_get_finished(game), \
    (int)game_get_is_procedural(game), (int)game_get_state(game));
  
  /*Saves gdescs to the file*/
  size = collection_length(game_get_gdescs(game));
  for(i = 0; i < size; i++){
    gdesc_save_on_file(game_get_gdesc_at(game, i), Psave_file);
  }

  /*Saves links to the file*/
  size = game_get_n_links(game);
  for(i=0; i<size; i++){
    link_save_to_file(Psave_file, game_get_link_at(game, i));
  } 

  /*Saves spaces to the file*/
  size = game_get_n_spaces(game);
  for(i=0; i<size; i++){
    space_save_to_file(Psave_file, game_get_space_at(game, i));
  }

  /*Saves npcs to the file*/
  size = collection_length(game_get_npcs(game));
  fprintf(Psave_file,"%d\n", size);
  for(i=0; i<size; i++){
    npc_save_to_file(Psave_file, collection_get_element_at(game_get_npcs(game), i));
  }

  /*Saves players to the file*/
  for(i=0; i<MAX_PLAYERS; i++){
    player_save_to_file(Psave_file, game_get_player_at(game, i));
  }
  
  /*Saves objects to the file*/
  fprintf(Psave_file, "OBJ\n");
  size = collection_length(game_get_objects(game));
  fprintf(Psave_file, "%d\n", size);
  for(i = 0; i < size; i++){
    object_save_on_file(collection_get_element_at(game_get_objects(game), i), Psave_file);
  }

  /*Saves attacks to the file*/
  size = collection_length(game_get_attacks(game));
  fprintf(Psave_file, "%d\n", size);
  for(i = 0; i < size; i++){
    attack_save_on_file(collection_get_element_at(game_get_attacks(game), i), Psave_file);
  }

  /*Saves events to the file*/
  event_manager_save_on_file(game_get_event_manager(game), Psave_file);

  /*Saves abilities to the file*/
  ability_manager_save_on_file(game_get_ability_manager(game), Psave_file);

  /*Saves command to the file*/
  command_save_on_file(game_get_last_command(game),Psave_file);

  /*Saves effects to the file*/
  size = collection_length(effect_manager_get_effects(game_get_effect_manager(game)));
  fprintf(Psave_file,"%d\n", size);
  for(i=0; i < size; i++){
    effect_save_to_file(Psave_file, collection_get_element_at(effect_manager_get_effects(game_get_effect_manager(game)),i));
  }

  fclose(Psave_file);


return OK;
}

/*
* Private functions implementation
*/

Status game_reader_load_links(Game *game, char *filename){
  FILE *file = NULL;

  char line[WORD_SIZE] = "";
  char *toks = NULL;

  Id id = NO_ID, space1 = NO_ID, space2 = NO_ID;
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


      debug_log(PRINT,"Read Link: #l:%ld|%ld|%ld|%ld|%ld", id, space1, space2, adjacent,locked);

      link = link_create(id,space1, space2, adjacent,locked);
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


Status game_reader_load_spaces(Game *game, char *filename, bool fromSaveFile) {
  FILE *file = NULL;

  char line[WORD_SIZE] = "";
  char *toks = NULL;
  
  char name[WORD_SIZE] = "";
  Id id = NO_ID, north = NO_ID, east = NO_ID, south = NO_ID, west = NO_ID, up = NO_ID, down = NO_ID;
  Id gdesc = NO_ID;
  bool isDiscovered=false;
  
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
      gdesc = atol(toks);

      toks = strtok(NULL, "|");
      north = atol(toks);
      
      toks = strtok(NULL, "|");
      east = atol(toks);
      
      toks = strtok(NULL, "|");
      south = atol(toks);
      
      toks = strtok(NULL, "|");
      west = atol(toks);
      
      toks = strtok(NULL, "|");
      up = atol(toks);

      toks = strtok(NULL, "|");
      down = atol(toks);

      if(fromSaveFile){
        toks = strtok(NULL, ";");
        isDiscovered = (bool) atoi(toks);
      }

      debug_log(PRINT,"Read Space: #s:%ld|%s|%ld|%ld|%ld|%ld|gdesc", id, name, north, east, south, west);

      /*Creates a space with space_create, and sets the ID on that space
      then saves it on the game with game_add_space*/
      space = space_create(id);
      if (space == NULL) {
        debug_log(LOG_ERROR, "Error allocating memory for space creation");
        status = ERROR;
        break;
      }
      space_set_name(space, name);
      space_set_north(space, game_get_link_by_id(game, north));
      space_set_east(space, game_get_link_by_id(game, east));
      space_set_south(space, game_get_link_by_id(game, south));
      space_set_west(space, game_get_link_by_id(game, west));
      space_set_up(space, game_get_link_by_id(game, up));
      space_set_down(space, game_get_link_by_id(game, down));
      space_set_graphic_description(space, game_get_gdesc_by_id(game, gdesc));
      space_set_isDiscovered(space, isDiscovered);

      game_add_space(game, space);
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
  char data[WORD_SIZE] = "";
  char description[WORD_SIZE] = "";
  char *toks = NULL;
  long objectid, objectlocation;
  Id dependency_object = NO_ID;
  InventoryType objectlocationtype;
  Object *object = NULL;

  int is_consumable = 0, is_movable = 0, cost;

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
      strcpy(data, toks);

      toks = strtok(NULL, "|");
      strcpy(description, toks);

      toks = strtok(NULL, "|");
      cost = atoi(toks);

      toks = strtok(NULL, "|");
      dependency_object = atol(toks);

      toks = strtok(NULL, "|");
      is_movable = atoi(toks);

      toks = strtok(NULL, "|");
      is_consumable = atoi(toks);
      
      toks = strtok(NULL, "|");
      objectlocation = atol(toks);

      toks = strtok(NULL, "|");
      objectlocationtype = atol(toks) + UNKNOWN_INVENTORY;

      /*Formato Object: #o:ID|Nombre|Data|Descripcion|Dependency_id|is_movable|is_consumable|LocationID|InventoryType*/
      debug_log(PRINT,"Read Object: #o:%ld|%s|%s|%s|%d|%d|%d|%d|%ld|%ld", objectid, name, data, description, cost, dependency_object, is_movable, is_consumable, objectlocation, objectlocationtype);

      /*Creates an object with object_create then saves it on the game with game_add_space*/
      object = object_create(objectid, name, data, description, cost, dependency_object, is_movable, is_consumable, objectlocation, objectlocationtype);
      if (object != NULL) {
        game_add_object(game, object);
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

Status game_reader_load_player(Game *game, char *filename, bool fromSaveFile){
  FILE *file = NULL;
  Player *player = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  long playerid, startinglocation;
  int xp, next_xp, level, skill_points, money;
  int i,n_followers=0;
  Id aux_id=NO_ID;
  EntityType ET;
  

  Status status = OK;

  if (!filename) {
    debug_log(LOG_ERROR, "Missing file name at: game_reader_load_player(Game*, char*) in game_reader.c");
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_player(Game*, char*) in game_reader.c");
    return ERROR;
  }


  /*Gets each line of the data file, uses strtok to shred it and 
  saves each location ID on static memory*/
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#p:", line, 3) == 0) {
      toks = strtok(line + 3, "|");
      playerid = atol(toks);
      
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      
      toks = strtok(NULL, "|");
      startinglocation = (game_get_is_procedural(game) == true) ? 1 : atol(toks);

      toks = strtok(NULL, "|");
      money = atoi(toks);
      
      toks = strtok(NULL, "|");
      xp = atoi(toks);

      toks = strtok(NULL, "|");
      next_xp = atoi(toks);

      toks = strtok(NULL, "|");
      level = atoi(toks);

      toks = strtok(NULL, "|");
      skill_points = atoi(toks);

      toks = strtok(NULL, "|");

      debug_log(PRINT,"Read Player: #p:%ld|%s|%ld|%d|%d|%d|%d|%d|gdesc", playerid, name, startinglocation, money, xp, next_xp, level, skill_points);

      /*Creates a player with player_create then saves it on the game with game_add_player*/
      player = player_create(name, playerid, startinglocation, money, xp, next_xp, level, skill_points);
      if (player == NULL){
        status = ERROR;
        break;
      }

      if(entity_set_graphic_description(player_get_entity(player), toks) == ERROR){
        status = ERROR;
        break;
      }

    /*#p:ID|Nombre|LocationID|Money|XP actual|XP siguiente nvl|nivel|skill points|gdesc|n_followers;id_following1-type;id_following2-type...*/
      if(fromSaveFile){
        /*adding followers*/
        toks = strtok(NULL, ";");

        n_followers = atoi(toks);

        for(i=0; i<n_followers; i++){
          toks = strtok(NULL, "-");
          aux_id = atol(toks);
          toks = strtok(NULL, ";");
          ET = atoi(toks);

          if(ET == PLAYER_TYPE){
            if(player_add_follower(player, player_get_entity(game_get_player_by_id(game, aux_id))) == ERROR){
              status = ERROR;
              break;
            }
          }
          else if(ET == NPC_TYPE){
            if(player_add_follower(player, npc_get_entity(game_get_NPC_by_id(game, aux_id))) == ERROR){
              status = ERROR;
              break;
            }
          }
        }
        
        /*adding command*/
        
        if(command_info_read_from_file(player_get_cmdData(player), file) == ERROR){
          status = ERROR;
          break;
        }

      }

      if (game_add_player(game, player) == ERROR){
        player_destroy(player);
        status = ERROR;
        break;
      }
    }
  }


  if (ferror(file)) {
    status = ERROR;
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_player(Game*, char*) in game_reader.c");
  }

  fclose(file);

  return status;
}

Status game_reader_load_events(Game *game, char *filename){
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char *toks = NULL;

  int i;

  Event *event = NULL;
  Id id = NO_ID;
  EventType type = NO_EVENT;
  CommandCode commands[N_CMD];
  bool removeOnTrigger = false;

  int numTriggers = 0;

  Status status = OK;

  if (!filename) {
    debug_log(LOG_ERROR, "Missing file name at: game_reader_load_events(Game*, char*) in game_reader.c");
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_events(Game*, char*) in game_reader.c");
    return ERROR;
  }


  /*#e:Id|Type|cmdTriggersNum| cmdName1 , ...|removeOnTrigger|data*/
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#e:", line, 3) == 0) {
      /*Reads id*/
      toks = strtok(line + 3, "|");
      id = atol(toks);
      /*Reads type*/
      toks = strtok(NULL, "|");
      type = event_type_from_str(toks);
      /*Reads the number of command that trigger the event*/
      toks = strtok(NULL, "|");
      numTriggers = atoi(toks);

      if(numTriggers > N_CMD)
      debug_log(LOG_WARNING, "Number of commands to trigger event was superior to the number of commands in game");
      
      /*Reads each command*/
      for (i = 0; i < numTriggers && i < N_CMD; i++)
      {
        toks = strtok(NULL, ",|");
        commands[i] = command_get_code_from_str(toks);
        debug_log(DEBUG, "Event: read command %s, command type %d", toks, commands[i]);
      }
      /*Reads if event is destroyed on trigger*/
      toks = strtok(NULL, "|");
      removeOnTrigger = atoi(toks);
      /*reads data*/
      toks = strtok(NULL, "|\r\n");
      
      debug_log(PRINT,"Read Event: #e:%ld|%d|%d|...|%d|%s", id, type, numTriggers, removeOnTrigger,toks);

      event = event_create(id, type, commands, numTriggers, toks, removeOnTrigger);
      if(event == NULL)
        debug_log(LOG_ERROR,"Error creating event when reading from file");
      if(game_add_event(game, event) == ERROR)
        debug_log(LOG_ERROR,"Error adding event to event manager");
    }
  }

  if (ferror(file)) {
    status = ERROR;
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_spaces(Game*, char*) in game_reader.c");
  }

  fclose(file);

  return status;

}

Status game_reader_load_npcs(Game *game, char *filename){
  FILE *file = NULL;
  NPC *npc = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  int can_follow, dialogue_state;
  long npcid, startinglocation;
  NPC_status statusnpc;

  Status status = OK;

  if (!filename) {
    debug_log(LOG_ERROR, "Missing file name at: game_reader_load_npcs(Game*, char*) in game_reader.c");
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_npcs(Game*, char*) in game_reader.c");
    return ERROR;
  }

  /*Gets each line of the data file, uses strtok to shred it and stores it in static memory*/
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#n:", line, 3) == 0) {
      toks = strtok(line + 3, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      npcid = atol(toks);
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      dialogue_state = atoi(toks);
      toks = strtok(NULL, "|");
      if(!toks){
	      printf("toks is null");
	      return ERROR;
      }
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      startinglocation = atol(toks);
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      can_follow = atoi(toks);
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      statusnpc = atoi(toks) + UNKNOWN_STATUS;
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      
      debug_log(PRINT,"Read NPC: #n:%ld|%d|%s|%ld|%d|%d|gdesc", npcid, dialogue_state, name, startinglocation, (bool)can_follow, (int)statusnpc);
      /*Creates an NPC with npc_create then saves it on the game with game_add_npc*/
      /*by default, lvl 1 stats are set. If .dat containts a stats line for this npc, they will be set afterwards.*/
      npc = npc_create(statusnpc, (bool)can_follow, dialogue_state, name, npcid, startinglocation);
      if (npc == NULL) {
        status = ERROR;
        break;
      }
      
      entity_set_graphic_description(npc_get_entity(npc), toks);
      if (game_add_npc(game, npc) == ERROR){
        printf("npc creation wrong");
        npc_destroy(npc);
        status = ERROR;
        break;
      }
    }
  }


  if (ferror(file)) {
    status = ERROR;
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_npc(Game*, char*) in game_reader.c");
  }

  fclose(file);

  return status;
}

Status game_reader_load_stats(Game *game, char *filename){
  FILE *file=NULL;
  void *entity=NULL;
  char line[WORD_SIZE] = "";
  char *toks=NULL;
  double maxhealth, health, baseDamage;
  int strength, defense, magicLevel;
  Id id;
  EntityType et;

  Status status;

  if (!filename) {
    debug_log(LOG_ERROR, "Missing file name at: game_reader_load_stats(Game*, char*) in game_reader.c");
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_stats(Game*, char*) in game_reader.c");
    return ERROR;
  }

   /*Gets each line of the data file, uses strtok to shred it and stores it in static memory*/
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#st:", line, 4) == 0) {
      toks = strtok(line + 4, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      id = atol(toks);
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      et = atoi(toks) + UNKNOWN_ENTITY;
      toks = strtok(NULL, "|");   
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      maxhealth = strtod(toks, NULL);
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      health = strtod(toks, NULL);
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      baseDamage = strtod(toks, NULL);
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      strength = atoi(toks);
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      defense = atoi(toks);
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }   
      magicLevel = atoi(toks);
      
      /*For the time being, we'll reject negative stats. Can be modified in a future if needed*/
      if((maxhealth<0) || (health < 0) || (baseDamage<0) || (strength<0) || (defense<0) || (magicLevel<0)){
        debug_log(LOG_ERROR,"Error when adding stats (negative stats not allowed)");
        return ERROR;
      }

      if(et == PLAYER_TYPE){
        entity = player_get_entity(game_get_player_by_id(game, id));
      }
      else if(et == NPC_TYPE){
        entity = npc_get_entity(game_get_NPC_by_id(game, id));
      }
      else
        entity = NULL;
      
      if(entity){
        entity_set_max_health(entity, maxhealth);
        entity_set_health(entity, health);
        entity_set_baseDamage(entity, baseDamage);
        entity_set_strength(entity, strength);
        entity_set_defense(entity, defense);
        entity_set_magicLevel(entity, magicLevel);
      }else{
        fclose(file);
        return ERROR;
      }
    } 
  }


  if (ferror(file)) {
    status = ERROR;
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_stats(Game*, char*) in game_reader.c");
  }
  else
    status = OK;

  fclose(file);

  return status;
}

Status game_reader_load_ability(Game *game, char *filename){
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char *toks = NULL;

  Ability *ability = NULL;
  Id id = NO_ID;
  AbilityType type = NO_SKILL;

  char name[WORD_SIZE];
  Id entityid;
  int is_player_ability;
  int is_object_use;

  int cd_count, cd_length;

  int i, len;

  Status status = OK;

  if (!filename) {
    debug_log(LOG_ERROR, "Missing file name at: game_reader_load_ability(Game*, char*) in game_reader.c");
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_ability(Game*, char*) in game_reader.c");
    return ERROR;
  }


  /*#sk:Id|Type|Name|Entityid|is_player_ability|is_object_use|cd_count|cd_length|data*/
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#sk:", line, 4) == 0) {

      len = strlen(line);
      for (i = 0; i < len; i++)
      {
        if(line[i] == '\r' || line[i] == '\n'){
          line[i] = 0;
        }
      }
      

      /*Reads id*/
      toks = strtok(line, ":");
      toks = strtok(NULL, "|");
      id = atol(toks);
      /*Reads type*/
      toks = strtok(NULL, "|");
      type = ability_type_from_str(toks);
      /*Reads ability name*/
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      /*Reads the entityid*/
      toks = strtok(NULL, "|");
      entityid = atol(toks);
      /*Reads if the ability is from a player*/
      toks = strtok(NULL, "|");
      is_player_ability = atoi(toks);
      /*Reads if the ability is from an object*/
      toks = strtok(NULL, "|");
      is_object_use = atoi(toks);
      /*Reads the cd count*/
      toks = strtok(NULL, "|");
      cd_count = atoi(toks);
      /*Reads the cd length*/
      toks = strtok(NULL, "|");
      cd_length = atoi(toks);
      /*reads data*/
      toks = strtok(NULL, "|");
      
      //printf("TEST");
      

      debug_log(PRINT,"Read Skill: #s:%ld|%d|%s|%ld|%d|%d|%d|%d|%s", id, type, name, entityid, is_player_ability, is_object_use, cd_count, cd_length, toks);

      ability = ability_create(id, toks, name, type, entityid, (bool)is_player_ability, (bool)is_object_use, cd_count, cd_length);

      if(ability == NULL)
        debug_log(LOG_ERROR,"Error creating ability when reading from file");

      if(game_add_ability(game, ability) == ERROR){
        //ability_destroy(ability);
        debug_log(LOG_ERROR,"Error adding ability to ability manager or entity");
      }
    }
  }

  if (ferror(file)) {
    status = ERROR;
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_ability(Game*, char*) in game_reader.c");
  }

  fclose(file);

  return status;
}

Status game_reader_load_commandInfo(Game *game){
  FILE *file = NULL;
  char line[WORD_SIZE];
  char str[WORD_SIZE];

  int i, j;
  int length;

  file = fopen(SETTINGS_FILE_PATH, "r");
  if(!file) return ERROR;

  while(fgets(line, WORD_SIZE - 1, file)){
    if(strncmp(line,"[CmdInfo]", 9) != 0){
      continue;
    }
    for (i = 0; i < N_CMD; i++)
    {
      if(!fgets(line, WORD_SIZE -1 , file)){
        fclose(file);
        return ERROR;
      } 

      length = strlen(line);
      for (j = 0; j < length; j++)
      {
        if(!isalnum(line[j]) && line[j] != ' ' && line[j] != ',' 
        && line[j] != '.' && line[j] != ':' && line[j] != '_' && line[j] != '-'
        && line[j] != '(' && line[j] != ')' && line[j] != '\"') break;
        str[j] = line[j];
      }
      str[j] = 0;

      if(command_set_info(game_get_last_command(game), i + NO_CMD, str) == ERROR){
        fclose(file);
        return ERROR;
      }
    }
    break;
  }
  fclose(file);
  return OK;
}

Status game_reader_load_attacks(Game *game) {

  FILE *file = NULL;
  char line[WORD_SIZE];

  char *toks = NULL;
  int numAttcks;
  int i = 0;
  double mult;
  double chance;

  Attack *at = NULL;
  bool needs_target;
  Collection *collection = NULL;
  if (!game) return ERROR;
  
  file = fopen(SETTINGS_FILE_PATH, "r");
  if (!file) return ERROR;

  while (fscanf(file, "%s", line) != 0)
  {
    if (strncmp(line, "[Attacks", 8) != 0){
      continue;
    }
    /*reads the number of attacks*/
    else {
      toks = strtok(line, ":");
      toks = strtok(NULL, "]");
      numAttcks = atoi(toks);
      /*starts saving the attacks in the combat*/

    break;
    }
  }
  
  collection = game_get_attacks(game);

  for (i = 0; i < numAttcks; i++)
  {
    fscanf(file, "%s", line);
    /*will now read the name of the attack*/
    toks = strtok(line, "|");
    /*will now create the attack with the name*/
    at = attack_create(toks);
    /*will now get the multiplicator of damage*/
    toks = strtok(NULL, "|");
    mult = atof(toks);
    attack_set_damage_multiplicator(at, mult);
    /*will now get the chances of failing*/
    toks = strtok(NULL, "|");
    chance = atof(toks);
    attack_set_failure_chance(at, chance);
    /*Will now get if it needs target or not*/
    toks = strtok(NULL, "|");
    needs_target = atoi(toks);
    attack_set_target_bool(at, needs_target);
    /*Will now copy this attack into the collection*/
    collection_add(collection, at);
  }

  fclose(file);
  return OK;
}

Status game_reader_load_commandStateTypes(Game *game){
  FILE *file = NULL;
  char line[WORD_SIZE];
  char str[WORD_SIZE] = "";

  CommandCode code = NO_CMD;

  int i,j;
  int curChar = 0;
  int word = 0;
  int length;


  file = fopen(SETTINGS_FILE_PATH, "r");
  if(!file) return ERROR;

  while(fgets(line, WORD_SIZE - 1, file)){
    if(strncmp(line,"[CmdStateTypes]", 15) != 0){
      continue;
    }
    for (i = 0; i < N_GAME_STATES; i++)
    {
      if(!fgets(line, WORD_SIZE -1 , file)){
        fclose(file);
        return ERROR;
      } 

      word = 0;
      curChar = 0;
      
      length = strlen(line);
      
      if(strncmp(line, "all", 3) == 0){
        for ( j = 0; j < N_CMD; j++)
        {
          command_state_add_type(game_get_last_command(game), i + ERROR_STATE, j + NO_CMD);
        }
      }else{
        while(curChar <= length){
          if(line[curChar] == ','){
            str[word + 1] = '\00';
            

            code = command_get_code_from_str(str);
            command_state_add_type(game_get_last_command(game), i + ERROR_STATE, code);
            
            memset(str,0, strlen(str));
            word = 0;
            curChar++;
          }
          str[word] = line[curChar];
          curChar++;
          word++;
        }
      }
    }
    break;
  }
  fclose(file);
  return OK;
}

Status game_reader_load_effects(Game *game, char *filename){
  FILE *file=NULL;
  char line[WORD_SIZE]="";
  char name[WORD_SIZE]="";
  char data[WORD_SIZE]="";
  char *toks=NULL;
  bool inf_turns;
  char inf_char; /*infinite turns y/N char*/
  int default_turns;

  Effect *effect=NULL;
  Id id;
  EffectType ET;
  EffectAffects EA;

  Status status=OK;

  if(!game || !filename) return ERROR;

  if (!filename) {
    debug_log(LOG_ERROR, "Missing file name at: game_reader_load_effects(Game*, char*) in game_reader.c");
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_effects(Game*, char*) in game_reader.c");
    return ERROR;
  }

  /*#ef:Id|name|EffectType|EffectAffects|inf_turns|default_turns|data*/
  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#ef:", line, 4) == 0) {
      toks = strtok(line + 4, "|");
        if(!toks){
        printf("toks is null");
        return ERROR;
      }
      id = atoi(toks);

      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      strcpy(name,toks);

      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      ET = atoi(toks);
      
      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      if((toks[0] == 'p') || (toks[0] == 'P'))
        EA = AFFECTS_PLAYER;
      else if((toks[0] == 'a') || (toks[0] == 'A'))
        EA = AFFECTS_ALLY;
      else if((toks[0] == 'e') || (toks[0] == 'E'))
        EA = AFFECTS_ENEMY;
      else{
        debug_log(LOG_ERROR,"Error creating effect when reading from file (Affected)");
        abort();
      }

      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      inf_char = atoi(toks);
      if((inf_char == 'y') || (inf_char == 'Y'))
        inf_turns=true;
      else
        inf_turns=false;

      toks = strtok(NULL, "|");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      default_turns = atoi(toks);

      toks = strtok(NULL, "|\r\n");
      if(!toks){
        printf("toks is null");
        return ERROR;
      }
      strcpy(data, toks);

      debug_log(PRINT,"Read Effect: #ef:%ld|%s|%d|%d|%d|%s", id, name, ET, inf_turns, default_turns, data);

      effect = effect_create(id, name, data, ET, EA, inf_turns, default_turns);

      if(effect == NULL){
        debug_log(LOG_ERROR,"Error creating effect when reading from file");
        return ERROR;
      }

      if(game_add_effect(game, effect) == ERROR){
        debug_log(LOG_ERROR,"Error adding effect to effect manager");
        effect_destroy(effect);
        return ERROR;
      }
    }
  }

  if (ferror(file)) {
    status = ERROR;
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_effects(Game*, char*) in game_reader.c");
  }

  fclose(file);

  return status;
}

bool game_reader_generate_procedural(){
  FILE *file = NULL;
  char line[WORD_SIZE];

  file = fopen(SETTINGS_FILE_PATH, "r");
  if(!file) return false;

  while(fgets(line, WORD_SIZE - 1, file)){
    if(strncmp(line,"procedural-gen=true", 19) == 0){
      fclose(file);
      return true;
    }
  }
  fclose(file);
  return false;
}

Status game_reader_load_gdesc(Game *game, char *filename){
  FILE *file = NULL;
  char line[WORD_SIZE];
  char *toks = NULL;

  Id id = NO_ID;
  int height = 0;
  int width = 0;
  GDescType type = NO_DESC;

  GDesc *gdesc = NULL;

  int i;

  if (!filename) {
    debug_log(LOG_ERROR, "Missing file name at: game_reader_load_gdesc(Game*, char*) in game_reader.c");
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_gdesc(Game*, char*) in game_reader.c");
    return ERROR;
  }

  /*format: 
  #gd:ID|height|width|type
  line1
  line2
  ...
  lineHeight
  */
  while(fgets(line, WORD_SIZE - 1, file)){
    if(strncmp(line,"#gd:", 4) != 0){
      continue;
    }
    toks = strtok(line + 4, "|");
    id = atol(toks);

    toks = strtok(NULL, "|");
    height = atoi(toks);

    toks = strtok(NULL, "|");
    width = atoi(toks);

    toks = strtok(NULL, "|");
    type = atoi(toks) + NO_DESC;

    gdesc = gdesc_create(id, height, width, type);
    if(!gdesc){
      fclose(file);
      return ERROR;
    }

    for (i = 0; i < height; i++)
    {
      if(!fgets(line, WORD_SIZE - 1, file)){
        fclose(file);
        return ERROR;
      }
      line[strlen(line) - 2] = 0;
      if(gdesc_set_line(gdesc, i, line) == ERROR){
        fclose(file);
        return ERROR;
      }
    }
    
    game_add_gdesc(game, gdesc);

  }
  fclose(file);
  return OK;
}

Status game_reader_load_others_from_save_file(Game *game, char *filename){
  FILE *fIN=NULL;
  char str[WORD_SIZE]="";
  char *toks=NULL;
  int size,i;
  Object *obj=NULL;
  Attack *at=NULL;
  Status status=OK;

  if(!game || !filename) return ERROR;


  fIN = fopen(filename, "r");
  if(!fIN){
    debug_log(LOG_ERROR, "Error on filename at: game_reader_load_objects_from_save_file in game_reader.c");
    return ERROR;
  }
  
  toks = fgets(str, WORD_SIZE, fIN);
  while(strncmp(str,"OBJ\n",4) != 0 && toks != NULL){
    toks = fgets(str, WORD_SIZE, fIN);
  }
  if(toks == NULL) return ERROR;

  /*object & equipment load*/
  fscanf(fIN, "%d\n", &size);
  for(i=0; i<size; i++){
    obj = object_create_from_file(fIN);    
    
    if(game_add_object(game, obj) == ERROR) return ERROR;

    if(object_get_is_equipped(obj)){
      equipment_add_piece(player_get_entity(game_get_player_by_id(game, object_get_location(obj))),\
      player_get_equipment(game_get_player_by_id(game, object_get_location(obj))), obj);
    }
  }

  /*attack load*/
  fscanf(fIN, "%d\n", &size);
  for(i=0; i<size; i++){
    at = attack_create_from_file(fIN);
    
    if(collection_add(game_get_attacks(game), at) == ERROR) return ERROR;
  }

  /*event load*/
  if(event_manager_read_from_file(game_get_event_manager(game), fIN) == ERROR) return ERROR;

  /*ability load*/
  if(ability_manager_read_from_file(game_get_ability_manager(game), fIN) == ERROR) return ERROR;

  /*Loads settings saved*/
  if(command_read_from_file(game_get_last_command(game), fIN) == ERROR) return ERROR;


  if (ferror(fIN)) {
    status = ERROR;
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_effects(Game*, char*) in game_reader.c");
  }

  fclose(fIN);
  
  return status;
}

bool game_reader_is_save_file(char *filename){
  FILE *fIN = NULL;
  char str[WORD_SIZE] = "";

  if(!filename) return false;

  fIN = fopen(filename,"r");
  if(!fIN) return false;

  if(fgets(str,WORD_SIZE,fIN) == NULL){
    fclose(fIN);
    return false;
  }

  if(strncmp(str, "SAVE", 4) == 0){
    fclose(fIN);
    return true;
  }

  fclose(fIN);

  return false;
}