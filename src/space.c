/**
 * @brief It implements the space module
 * 
 * The internal structure of space.c is controlled by links. Each location in the game is defined by
 * a space, but the connections between these spaces are controlled by links, thats why each space
 * has four pointers to link in all directions.
 *
 * @file space.c
 * @author Original: Profesores PPROG. Modified by: Daniel Gómez
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "space.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Space
 *
 * This struct stores all the information of a space.
 */
struct _Space {
  Id id;                        /*!< Id number of the space, it must be unique */
  char name[WORD_SIZE + 1];     /*!< Name of the space */

  Link *north;                  /*!< Id of the space at the north */
  Link *south;                  /*!< Id of the space at the south */
  Link *east;                   /*!< Id of the space at the east */
  Link *west;                   /*!< Id of the space at the west */
  
  Inventory *inventory;         /*!< Inventory of the space*/
  
  NPC **NPCs;                    /*!< Array with the NPCs on the space, not organized*/
  int n_npcs;                   /*!< int with the number of NPCs on the space*/
};

/*Space private functions*/

/*GETTERS*/

/**
 * @brief Gets the number of npcs located on a space
 * @author Maksym Polyak
 * 
 * @param space 
 * @return int or -1 if error
 */
int space_get_n_npcs(Space *space);

/**
 * @brief Gets the NPCs array of the space
 * @author Maksym Polyak
 * 
 * @param space 
 * @return NPC** or NULL if errors
 */
NPC **space_get_NPCs(Space *space);

/*SETTERS*/

/**
 * @brief Sets the number of npcs located on a space
 * @author Maksym Polyak
 * 
 * @param space 
 * @param num
 * @return Status
 */
Status space_set_n_npcs(Space *space, int num);

/*PRIVATE IMPLEMENTATION*/

int space_get_n_npcs(Space *space){
  if(!space) return -1;

  return space->n_npcs;
}

NPC **space_get_NPCs(Space *space){
  if(!space) return NULL;

  return space->NPCs;
}

Status space_set_n_npcs(Space *space, int num){
  if(!space) return ERROR;

  space->n_npcs = num;
  
  return OK;
}

/*Space public functions*/

Space* space_create(Id id) {
  Space* newSpace = NULL;

  /* Error control */
  if (id == NO_ID) return NULL;

  newSpace = (Space*)malloc(sizeof(Space));
  if (newSpace == NULL) {
    return NULL;
  }

  /* Initialization of an empty space*/
  newSpace->id = id;
  newSpace->name[0] = '\0';
  newSpace->inventory = inventory_create(SPACE_INVENTORY, id);
  if(!newSpace->inventory){
    debug_log(LOG_ERROR, "space_create, inventory_create dynamic memory error at space: id:%ld", id);
    free(newSpace);
    return NULL;
  }
  newSpace->north = NULL;
  newSpace->south = NULL;
  newSpace->east = NULL;
  newSpace->west = NULL;

  newSpace->n_npcs = 0;
  newSpace->NPCs = (NPC **)calloc(SPACE_MAX_NPCS, sizeof(NPC *));
  if(!newSpace->NPCs){
    debug_log(LOG_ERROR, "space_create, NPC array dynamic memory error at space: id:", id);
    inventory_destroy(newSpace->inventory);
    free(newSpace);
    return NULL;
  }

  return newSpace;
}

Status space_destroy(Space* space) {
  if (!space) {
    return ERROR;
  }

  inventory_destroy(space->inventory);
  free(space->NPCs); /*Dynamic memory from NPCs is controlled by the main*/

  free(space);
  space = NULL;
  return OK;
}

Id space_get_id(Space* space) {
  if (!space) {
    return NO_ID;
  }
  return space->id;
}

Inventory *space_get_inventory(Space *space){
  if(!space){
    return NULL;
  }
  return space->inventory;
}

/*Space SETTERS*/

Status space_set_name(Space* space, char* name) {
  if (!space || !name) {
    return ERROR;
  }

  if (!strcpy(space->name, name)) {
    return ERROR;
  }
  return OK;
}

Status space_set_north(Space* space, Link* link) {
  if (!space || !link) {
    return ERROR;
  }
  space->north = link;
  return OK;
}

Status space_set_south(Space* space, Link* link) {
  if (!space || !link) {
    return ERROR;
  }
  space->south = link;
  return OK;
}

Status space_set_east(Space* space, Link* link) {
  if (!space || !link) {
    return ERROR;
  }
  space->east = link;
  return OK;
}

Status space_set_west(Space* space, Link* link) {
  if (!space || !link) {
    return ERROR;
  }
  space->west = link;
  return OK;
}

/*Space GETTERS*/

const char* space_get_name(Space* space) {
  if (!space) {
    return NULL;
  }
  return space->name;
}

Link* space_get_north(Space* space) {
  if (!space) {
    return NULL;
  }
  return space->north;
}

Link* space_get_south(Space* space) {
  if (!space) {
    return NULL;
  }
  return space->south;
}



Link* space_get_east(Space* space) {
  if (!space) {
    return NULL;
  }
  return space->east;
}



Link* space_get_west(Space* space) {
  if (!space) {
    return NULL;
  }
  return space->west;
}

Status space_print(Space* space) {
  Id idaux = NO_ID;

  /* Error Control */
  if (!space) {
    return ERROR;
  }

  /* 1. Print the id and the name of the space */
  fprintf(stdout, "--> Space (Id: %ld; Name: %s)\n", space->id, space->name);

  /* 2. For each direction, print its link */
  idaux = link_get_id(space_get_north(space));
  if (idaux != NO_ID) {
    fprintf(stdout, "---> North link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No north link.\n");
  }
  idaux = link_get_id(space_get_south(space));
  if (idaux != NO_ID) {
    fprintf(stdout, "---> South link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No south link.\n");
  }
  idaux = link_get_id(space_get_east(space));
  if (idaux != NO_ID) {
    fprintf(stdout, "---> East link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No east link.\n");
  }
  idaux = link_get_id(space_get_west(space));
  if (idaux != NO_ID) {
    fprintf(stdout, "---> West link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No west link.\n");
  }

  return OK;
}

Status space_add_NPC(Space *space, NPC *npc){
  int n_npcs, i;
  NPC **npcArray = NULL;
  Entity *entity = NULL;
  
  if(!space || !npc)
    return ERROR;

  n_npcs = space_get_n_npcs(space);
  if(n_npcs == SPACE_MAX_NPCS){
    debug_log(LOG_ERROR, "space_add_NPC couldn't add NPC because space is full of NPCs on space with id:", space_get_id(space));
    return ERROR;
  }

  npcArray = space_get_NPCs(space);
  for(i = 0; i < n_npcs; i++){
    if(npcArray[i] != NULL){
      npcArray[i] = npc;
      space_set_n_npcs(space, n_npcs + 1);
      return OK;
    }
  }

  entity = npc_get_entity(npc);

  entity_set_location(entity, space_get_id(space));

  return ERROR;
}

Status space_remove_NPC(Space *space, NPC *npc){
  int n_npcs, i;
  NPC **npcArray = NULL;
  Entity *entity = NULL;
  
  if(!space || !npc)
    return ERROR;

  n_npcs = space_get_n_npcs(space);
  if(n_npcs == 0){
    debug_log(LOG_ERROR, "space_remove_NPC couldn't remove NPC because the space does not have NPCs on space with id:", space_get_id(space));
    return ERROR;
  }

  npcArray = space_get_NPCs(space);
  for(i = 0; i < SPACE_MAX_NPCS; i++){
    if(npcArray[i] == npc){
      npcArray[i] = NULL;
      space_set_n_npcs(space, n_npcs - 1);
      return OK;
    }
  }

  entity = npc_get_entity(npc);

  entity_set_location(entity, UNKNOWN_ENTITY);

  return ERROR;
}

Status space_move_NPC(Space *spaceOUT, Space *spaceIN, NPC *npc){
  if(!spaceOUT || !spaceIN || !npc)
    return ERROR;

  if(space_remove_NPC(spaceOUT, npc) == ERROR){
    debug_log(LOG_ERROR, "space_move_NPC couldn't move NPC because space_remove_NPC failed, on space with id:", space_get_id(spaceOUT));
    return ERROR;
  }

  if(space_add_NPC(spaceIN, npc) == ERROR){
    debug_log(LOG_ERROR, "space_move_NPC couldn't move NPC because space_add_NPC failed, on space with id:", space_get_id(spaceOUT));
    debug_log(LOG_ERROR, "NPC was lost on space_move_NPC on NPC with id: %ld", entity_get_id(npc_get_entity(npc)));
    return ERROR;
  }

  debug_log(PRINT, "space_move_NPC moved the NPC with id: %ld from the space of id: %ld to the space of id: %ld", entity_get_id(npc_get_entity(npc)),space_get_id(spaceOUT), space_get_id(spaceIN));
  return OK;
}