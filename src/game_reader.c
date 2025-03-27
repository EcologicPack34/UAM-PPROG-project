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
#include "event_manager.h"

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
 * @param game 
 * @param filename 
 * @return Status 
 */
Status game_reader_load_events(Game *game, char *filename);

/**
 * @brief Reads the file to load all npcs
 * @author Maksym Polyak
 * 
 * @param game 
 * @param filename 
 * @return Status 
 */
Status game_reader_load_npcs(Game *game, char *filename);

/**
 * @brief Loads command info from settings file
 * 
 * @param game 
 * @return Status 
 */
Status game_reader_load_commandInfo(Game *game);

/*
* Public functions implementation
*/
Status game_reader_create_from_file(Game **game, char *filename){
  if (game_create(game) == ERROR){
    debug_log(LOG_ERROR, "Error creating game at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    printf("%c[2J", 27);
    printf("Fatal error. Check the log for details\n");
    abort();
  }
  /*Loads settints*/
  if(game_reader_load_commandInfo(*game) == ERROR){
    printf("%c[2J", 27);
    printf("Fatal error. Check the log for details\n");
    abort();
  }

  /*Loads data into the game*/
  if(game_reader_load_player(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading player at: game_reader_create_from_file(Game*, char*) in game_reader.c");
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
  if (game_reader_load_events(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading events at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }
  if (game_reader_load_npcs(*game, filename) == ERROR){
    debug_log(LOG_ERROR, "Error loading npcs at: game_reader_create_from_file(Game*, char*) in game_reader.c");
    return ERROR;
  }

  if(game_spatial_map(*game) == ERROR){
    debug_log(LOG_ERROR,"Error maping spatialy spaces");
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
  char *toks = NULL;
  
  char name[WORD_SIZE] = "";
  Id id = NO_ID, north = NO_ID, east = NO_ID, south = NO_ID, west = NO_ID;
  
  Space *space = NULL;
  Status status = OK;

  int i;

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

      for (i = 0; i < SPACE_GRAPHIC_HEIGHT; i++)
      {
        toks = strtok(NULL, ";");
        if(toks){
          space_set_graphic_description(space, toks, i);
        }else{
          space_set_graphic_description(space, " \00", i);
        }
      }

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
  char description[WORD_SIZE] = "";
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
      strcpy(description, toks);
      toks = strtok(NULL, "|");
      objectlocation = atol(toks);
      toks = strtok(NULL, "|");
      objectlocationtype = (InventoryType)atol(toks);

      debug_log(PRINT,"Read Object: #o:%ld|%s|%s|%ld|%ld", objectid, name, description, objectlocation, objectlocationtype);

      /*Creates a object with object_create then saves it on the game with game_add_space*/
      object = object_create(objectid, name, description, objectlocation, objectlocationtype);
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

Status game_reader_load_player(Game *game, char *filename){
  FILE *file = NULL;
  Player *player = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  double maxhealth, health, baseDamage;
  int strength, defense, magicLevel;
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
      startinglocation = atol(toks);

      toks = strtok(NULL, "|");
      maxhealth = strtod(toks, NULL);
      
      toks = strtok(NULL, "|");
      health = strtod(toks, NULL);
      
      toks = strtok(NULL, "|");
      baseDamage = strtod(toks, NULL);
      
      toks = strtok(NULL, "|");
      strength = atoi(toks);
      
      toks = strtok(NULL, "|");
      defense = atoi(toks);
      
      toks = strtok(NULL, "|");
      magicLevel = atoi(toks);
      
      toks = strtok(NULL, "|");

      debug_log(PRINT,"Read Player: #p:%ld|%s|%ld|%lf|%lf|%lf|%d|%d|%d|gdesc", playerid, name, startinglocation, maxhealth, health, baseDamage, strength, defense, magicLevel);

      /*Creates a player with player_create then saves it on the game with game_add_player*/
      player = player_create(name, playerid, startinglocation, maxhealth, health, baseDamage, strength, defense, magicLevel);
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
  double maxhealth, health, baseDamage;
  int strength, defense, magicLevel;
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
      statusnpc = atoi(toks) + UNKNOWN_STATUS;
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

      toks = strtok(NULL, "|");
      
      debug_log(PRINT,"Read NPC: #n:%ld|%s|%s|%ld|%d|%lf|%lf|%lf|%d|%d|%d|gdesc", npcid, message, name, startinglocation, (int)statusnpc, maxhealth, health, baseDamage, strength, defense, magicLevel);
      /*Creates an NPC with npc_create then saves it on the game with game_add_npc*/
      npc = npc_create(statusnpc, message, name, npcid, startinglocation, maxhealth, health, baseDamage, strength, defense, magicLevel);
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

Status game_reader_load_commandInfo(Game *game){
  FILE *file = NULL;
  char line[WORD_SIZE];

  int i;

  file = fopen(SETTINGS_FILE_PATH, "r");
  if(!file) return ERROR;

  while(fgets(line, WORD_SIZE - 1, file)){
    if(strncmp(line,"[CmdInfo]", 8) != 0){
      continue;
    }
    for (i = 0; i < N_CMD; i++)
    {
      if(!fgets(line, WORD_SIZE -1 , file)) return ERROR;

      if(command_set_info(game_get_last_command(game), i + UNKNOWN + 1, line) == ERROR){
        return ERROR;
      }
      printf("%s", command_get_info(game_get_last_command(game), i + UNKNOWN + 1));
    }
    break;
  }
  return OK;
}
