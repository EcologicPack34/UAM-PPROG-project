/**
 * @brief It implements the space module
 * 
 * The internal structure of space.c is controlled by links. Each location in the game is defined by
 * a space, but the connections between these spaces are controlled by links, thats why each space
 * has four pointers to link in all directions.
 *
 * @file space.c
 * @author Original: Profesores PPROG. Modified by: Daniel Gómez & Maksym Polyak
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "space.h"

#include "collection.h"
#include "vector2.h"

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
  bool discovered;              /*!< Sets wheter a space has been discovered by players(true) or not(false)*/

  Link *north;                  /*!< Id of the space at the north */
  Link *south;                  /*!< Id of the space at the south */
  Link *east;                   /*!< Id of the space at the east */
  Link *west;                   /*!< Id of the space at the west */
  
  char *graphicDescription[SPACE_GRAPHIC_HEIGHT];

  Vector2 position;
  bool mapped;
  int mapBlock;
  Space *neighbours[DIRECTION_NUMBER];

  Inventory *inventory;         /*!< Inventory of the space*/
  
  Collection *npcs;             /*!<  Collection of npcs in the given space*/
};

/*Space private functions*/

/*GETTERS*/


/*SETTERS*/


/*PRIVATE IMPLEMENTATION*/


/*Space public functions*/

Space* space_create(Id id) {
  Space* newSpace = NULL;
  int i;

  /* Error control */
  if (id == NO_ID) return NULL;

  newSpace = (Space*)calloc(1,sizeof(Space));
  if (newSpace == NULL) {
    return NULL;
  }

  /* Initialization of an empty space*/
  newSpace->id = id;

  
  
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
  
  newSpace->npcs = collection_create(SPACE_INITIAL_SIZE_NPCS, false, true, npc_cmp, npc_print);
  if(!(newSpace->npcs)){
    debug_log(LOG_ERROR, "space_create, couldn't create memory for npc collection, id:%ld", id);
    inventory_destroy(newSpace->inventory);
    free(newSpace);
    return NULL;
  }
  /*Allocates memory for graphic description*/
  for (i = 0; i < SPACE_GRAPHIC_HEIGHT; i++)
  {
    newSpace->graphicDescription[i] = calloc(SPACE_GRAPHIC_WIDTH + 1, sizeof(char));
    /*Checks if fails and frees memory*/
    if(newSpace->graphicDescription[i] == NULL){
      for (i--; i >= 0; i--){
        free(newSpace->graphicDescription[i]);
      }
      inventory_destroy(newSpace->inventory);
      collection_destroy(newSpace->npcs);
      free(newSpace);
      return NULL;
    }
  }
  return newSpace;
}

Status space_destroy(Space* space) {
  int i;
  if (!space) {
    return ERROR;
  }

  inventory_destroy(space->inventory);
  collection_destroy(space->npcs); /*Dynamic memory from NPCs is controlled by the main*/

  for (i = 0; i < SPACE_GRAPHIC_HEIGHT; i++){
    free(space->graphicDescription[i]);
  }

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

Status space_set_position(Space *space, float x, float y){
  if(!space) return ERROR;
  space->position.x = x;
  space->position.y = y;
  return OK;
}

Status space_set_isMapped(Space *space, bool status){
  if(!space) return ERROR;
  space->mapped = status;
  return OK;
}
Status space_set_map_block(Space *space, int block){
  if(!space) return ERROR;
  space->mapBlock = block;
  return OK;
}
Status space_set_neighbour(Space *space, Space *neighbour, Direction direction){
  if(!space) return ERROR;
  space->neighbours[direction] = neighbour;
  return OK;
}

Status space_set_graphic_description(Space *space, char *desc, int index){
  if(!space || !desc) return ERROR;

  if(index < 0 || index > SPACE_GRAPHIC_HEIGHT) return ERROR;

  strncpy(space->graphicDescription[index], desc, SPACE_GRAPHIC_WIDTH);
  space->graphicDescription[index][SPACE_GRAPHIC_WIDTH] = '\00';
  return OK;
}

/*Space GETTERS*/

char **space_get_graphic_description(Space *space){
  if(!space) return NULL;
  return space->graphicDescription;
}

Space *space_get_neighbour(Space *space, Direction direction){
  if(!space) return NULL;
  return space->neighbours[direction];
}

bool space_get_isMapped(Space *space){
  if(!space) return true;
  return space->mapped;
}

int space_get_map_block(Space *space){
  if(!space) return -1;
  return space->mapBlock;
}

Vector2 *space_get_position(Space *space){
  if(!space) return NULL;
  return &(space->position);
}

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

int space_get_npc_count(Space *space){
  if(!space) return -1;

  return collection_length(space->npcs);
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
  Entity *entity = NULL;
  
  if(!space || !npc)
    return ERROR;

  if(collection_add(space->npcs, (void *)npc) == ERROR){
    debug_log(LOG_ERROR,"space_add_NPC: Could not add NPC to space due to fail on collection_add: SpaceID: %ld", space->id);
    return ERROR;
  }

  entity = npc_get_entity(npc);

  entity_set_location(entity, space->id);

  return OK;
}

Status space_remove_NPC(Space *space, NPC *npc){
  Entity *entity = NULL;
  
  if(!space || !npc)
    return ERROR;

  
  if(collection_remove(space->npcs, (void *)npc) == OK){
    entity = npc_get_entity(npc);
    entity_set_location(entity, UNDEFINED_ID);
    return OK;
  }

  return ERROR;
}

Status space_move_NPC(Space *spaceOUT, Space *spaceIN, NPC *npc){
  if(!spaceOUT || !spaceIN || !npc)
    return ERROR;

  if(space_remove_NPC(spaceOUT, npc) == ERROR){
    debug_log(LOG_ERROR, "space_move_NPC couldn't move NPC because space_remove_NPC failed, on space with id:", spaceOUT->id);
    return ERROR;
  }

  if(space_add_NPC(spaceIN, npc) == ERROR){
    debug_log(LOG_ERROR, "space_move_NPC couldn't move NPC because space_add_NPC failed, on space with id:", space_get_id(spaceOUT));
    debug_log(LOG_ERROR, "NPC was lost on space_move_NPC on NPC with id: %ld", entity_get_id(npc_get_entity(npc)));
    return ERROR;
  }

  debug_log(PRINT, "space_move_NPC moved the NPC with id: %ld from the space of id: %ld to the space of id: %ld", entity_get_id(npc_get_entity(npc)),spaceOUT->id, spaceIN->id);
  return OK;
}

NPC *space_get_NPC_at(Space *space, int index){
  if(!space)
    return NULL;

  return collection_get_element_at(space->npcs, index);
}

NPC *space_get_NPC_by_name(Space *space, char *name){
  NPC *npc = NULL;
  int i, size;

  if(!space || !name)
    return NULL;

  size = space_get_npc_count(space);
  for(i = 0; i < size; i++){
    npc = collection_get_element_at(space->npcs, i);

    if(strcmp(entity_get_name(npc_get_entity(npc)), name) == 0){
      return npc;
    }
  }

  return NULL;
}

Status space_get_NPC_list(Space *space, char *str, int length){
  NPC *npc = NULL;
  int i, size;
  char *aux;
  
  if(!space || !str) return ERROR;
  if(length <= 0) return ERROR;
  
  size = space_get_npc_count(space);

  str[0] = '\00';
  for(i = 0; i < size; i++){
    npc = collection_get_element_at(space->npcs, i);
    
    if(entity_get_health(npc_get_entity(npc)) <= 0) continue;

    aux = entity_get_graphic_description(npc_get_entity(npc));
    strcat(str , aux);
    if(aux[0] != '\00' && i != size -1){
      strcat(str, ";");
    }
  }

  str[length - 3] = '.';
  str[length - 2] = '.';
  str[length - 1] = '.';
  str[length] = '\00';
  return OK;
}

Status space_set_discovered(Space *space, bool discovered){
  if(!space)
    return ERROR;

  space->discovered = discovered;

  return OK;
}

bool space_is_discovered(Space *space){
  if(!space)
    return false;

  if(space->discovered == true)
    return true;

  return false;
}