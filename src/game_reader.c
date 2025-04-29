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
 * @return OK if everything goes well or ERROR if there was some error
 */
Status game_reader_load_spaces(Game *game, char *filename);

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
 * @return OK if everything goes well or ERROR if there was some error
 */
Status game_reader_load_player(Game *game, char *filename);

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

/*
* Public functions implementation
*/
Status game_reader_create_from_file(Game **game, char *filename){
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
    if (game_reader_load_spaces(*game, filename) == ERROR){
      debug_log(LOG_ERROR, "Error loading spaces at: game_reader_create_from_file(Game*, char*) in game_reader.c");
      return ERROR;
    }
<<<<<<< Updated upstream
=======
    
    /*Temp loading*/
    if (game_reader_load_events(*game, filename) == ERROR){
      debug_log(LOG_ERROR, "Error loading events at: game_reader_create_from_file(Game*, char*) in game_reader.c");
      return ERROR;
    }
    if (game_reader_load_npcs(*game, filename) == ERROR){
      debug_log(LOG_ERROR, "Error loading npcs at: game_reader_create_from_file(Game*, char*) in game_reader.c");
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
>>>>>>> Stashed changes
  }
  
  /*Temp loading*/
  if (game_reader_load_events(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading events at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if (game_reader_load_npcs(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading npcs at: game_reader_create_from_file(Game*, char*) in game_reader.c");
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

  /*Loads data into the game*/
  if(game_reader_load_player(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading player at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  game_switch_player(*game, 0);

  if(game_reader_load_stats(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading stats at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if(game_spatial_map(*game) == ERROR){
    debug_log(LOG_ERROR,"Error maping spatialy spaces");
    return ERROR;
  }

  if(game_reader_load_attacks(*game) == ERROR) {
    debug_log(LOG_ERROR,"Error loading attacks");
    return ERROR;
  }

  /*Initialization*/
  /*Sets the actual player as the first one read in the .dat*/
  game_switch_player(*game, 0);

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


Status game_reader_load_spaces(Game *game, char *filename) {
  FILE *file = NULL;

  char line[WORD_SIZE] = "";
  char *toks = NULL;
  
  char name[WORD_SIZE] = "";
  Id id = NO_ID, north = NO_ID, east = NO_ID, south = NO_ID, west = NO_ID, up = NO_ID, down = NO_ID;
  Id gdesc = NO_ID;
  
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
  Entity *ent = NULL;
  Object *object = NULL;

  int is_consumable = 0, is_movable = 0;

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
      debug_log(PRINT,"Read Object: #o:%ld|%s|%s|%s|%d|%d|%d|%ld|%ld", objectid, name, data, description, dependency_object, is_movable, is_consumable, objectlocation, objectlocationtype);

      /*Creates a object with object_create then saves it on the game with game_add_space*/
      object = object_create(objectid, name, data, description, dependency_object, is_movable, is_consumable, objectlocation, objectlocationtype);
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

Status game_reader_load_player(Game *game, char *filename){
  FILE *file = NULL;
  Player *player = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  long playerid, startinglocation;

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

      debug_log(PRINT,"Read Player: #p:%ld|%s|%ld|gdesc", playerid, name, startinglocation);

      /*Creates a player with player_create then saves it on the game with game_add_player*/
      player = player_create(name, playerid, startinglocation);
      if (player == NULL){
        status = ERROR;
        break;
      }

      entity_set_graphic_description(player_get_entity(player), toks);

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
      toks = strtok(NULL, "|");
      
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
  char message[WORD_SIZE] = "";
  int can_follow;
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
      strcpy(message, toks);
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
      
      debug_log(PRINT,"Read NPC: #n:%ld|%s|%s|%ld|%d|%d|gdesc", npcid, message, name, startinglocation, (bool)can_follow, (int)statusnpc);
      /*Creates an NPC with npc_create then saves it on the game with game_add_npc*/
      /*by default, lvl 1 stats are set. If .dat containts a stats line for this npc, they will be set afterwards.*/
      npc = npc_create(statusnpc, (bool)can_follow, message, name, npcid, startinglocation);
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
        entity = npc_get_entity(game_get_npc_by_id(game, id));
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
    debug_log(LOG_ERROR, "Missing file name at: game_reader_load_ability(Game*, char*) in game_reader.c");
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    debug_log(LOG_ERROR, "Error in file at: game_reader_load_ability(Game*, char*) in game_reader.c");
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