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
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief ADT that stores all the information of a space.
 */
struct _Space {
  Id id;                                /*!< Id number of the space, it must be unique */
  char name[WORD_SIZE + 1];             /*!< Name of the space */
  bool discovered;                      /*!< Sets wheter a space has been discovered by players(true) or not(false)*/

  Link *north;                          /*!< pointer of the Link at the north of the space */
  Link *south;                          /*!< pointer of the Link at the south of the space */
  Link *east;                           /*!< pointer of the Link at the east of the space  */
  Link *west;                           /*!< pointer of the Link at the west of the space  */
  Link *up;                             /*!< pointer of the Link up of the space  */
  Link *down;                           /*!< pointer of the Link down of the space  */
  
  GDesc *gdesc;                         /*!< pointer to a graphic description*/

  Vector2 position;                     /*!< Position vector on the block*/
  bool mapped;                          /*!< Bool to determine if space is mapped(true) or not*/
  int mapBlock;                         /*!< Block where the space belongs*/
  Space *neighbours[DIRECTION_NUMBER];  /*!< Adjacent spaces to the original space*/

  Inventory *inventory;                 /*!< Inventory of the space*/

  char description[WORD_SIZE];                     /*!< Description of the place that the space represents*/
  
  Collection *npcs;                     /*!< Collection of npcs in the given space*/
};

/*Space public functions*/

Space* space_create(Id id) {
  Space* newSpace = NULL;

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
  newSpace->up = NULL;
  newSpace->down = NULL;

  newSpace->discovered = false;
  newSpace->gdesc = NULL;
  
  newSpace->npcs = collection_create(SPACE_INITIAL_SIZE_NPCS, false, true, npc_cmp, npc_print);
  if(!(newSpace->npcs)){
    debug_log(LOG_ERROR, "space_create, couldn't create memory for npc collection, id:%ld", id);
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
  collection_destroy(space->npcs); /*Dynamic memory from NPCs is controlled by the main*/

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

char *space_get_description(Space *space){
  if(!space){
    return NULL;
  }
  return space->description;
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

Status space_set_description(Space *space, char *descr){
  if(!space || !descr) return ERROR;

  strcpy(space->description, descr);

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

Status space_set_up(Space* space, Link* link) {
  if (!space || !link) {
    return ERROR;
  }
  space->up = link;
  return OK;
}

Status space_set_down(Space* space, Link* link) {
  if (!space || !link) {
    return ERROR;
  }
  space->down = link;
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

Status space_set_graphic_description(Space *space, GDesc *gdesc){
  if(!space || !gdesc) return ERROR;
  space->gdesc = gdesc;
  return OK;
}

Status space_set_isDiscovered(Space *space, bool status){
  if(!space) return ERROR;
  space->discovered = status;
  return OK;
}

/*Space GETTERS*/

bool space_get_isDiscovered(Space *space){
  if(!space) return false;
  return space->discovered;
}

GDesc *space_get_graphic_description(Space *space){
  if(!space) return NULL;
  return space->gdesc;
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

Link* space_get_up(Space* space) {
  if (!space) {
    return NULL;
  }
  return space->up;
}

Link* space_get_down(Space* space) {
  if (!space) {
    return NULL;
  }
  return space->down;
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

int space_cmp(void *e1, void *e2){
  return ((Space *)e1)->id - ((Space *)e2)->id;
}

Status space_save_to_file(FILE *file, Space *s){
  Id north,south,west,east,up,down;
  char *descr = NULL;
  if(!file || !s) return ERROR;

  north = link_get_id(space_get_north(s));
  south = link_get_id(space_get_south(s));
  east = link_get_id(space_get_east(s));
  west = link_get_id(space_get_west(s));
  up = link_get_id(space_get_up(s));
  down = link_get_id(space_get_down(s));
  
  descr = space_get_description(s);

  /*#s:ID|Name|gdescId|northLink|eastLink|southLink|westLink|upLink|downLink|descr|isDiscovered*/
  fprintf(file, "#s:%ld|%s|%ld|%ld|%ld|%ld|%ld|%ld|%ld|%s|%d\n", space_get_id(s), space_get_name(s),\
  gdesc_get_id(space_get_graphic_description(s)), north, east, south, west, up, down, descr == NULL ? "NODESCR": descr, space_get_isDiscovered(s));

  
  return OK;
}

Space *space_create_from_file(FILE *fIN, Collection *gdescs, Link **links, int n_links){
  Space *space = NULL;
  Id id, aux_id;
  GDesc *gdesc = NULL;
  Link *link = NULL;
  int i, size;
  char str[WORD_SIZE] = "", *toks = NULL;

  if(!fIN || !gdescs || !links) return NULL;

  fgets(str, WORD_SIZE, fIN);
  toks = strtok(str,"|");
  id = atol(toks);

  space = space_create(id);

  toks = strtok(NULL,"|");
  aux_id = atol(toks);
  
  size = collection_length(gdescs);
  for(i = 0; i < size; i++){
    gdesc = collection_get_element_at(gdescs, i);
    if(gdesc_get_id(gdesc) == aux_id){
      space->gdesc = gdesc;
      break;
    }
  }

  toks = strtok(NULL,"|");
  aux_id = atol(toks);
  
  for(i = 0; i < n_links; i++){
    link = links[i];
    if(link_get_id(link) == aux_id){
      space->north = link;
      break;
    }
  }

  toks = strtok(NULL,"|");
  aux_id = atol(toks);
  
  for(i = 0; i < n_links; i++){
    link = links[i];
    if(link_get_id(link) == aux_id){
      space->east = link;
      break;
    }
  }

  toks = strtok(NULL,"|");
  aux_id = atol(toks);
  
  for(i = 0; i < n_links; i++){
    link = links[i];
    if(link_get_id(link) == aux_id){
      space->south = link;
      break;
    }
  }

  toks = strtok(NULL,"|");
  aux_id = atol(toks);
  
  for(i = 0; i < n_links; i++){
    link = links[i];
    if(link_get_id(link) == aux_id){
      space->west = link;
      break;
    }
  }

  toks = strtok(NULL,"|");
  aux_id = atol(toks);
  
  for(i = 0; i < n_links; i++){
    link = links[i];
    if(link_get_id(link) == aux_id){
      space->up = link;
      break;
    }
  }

  toks = strtok(NULL,"|");
  aux_id = atol(toks);
  
  for(i = 0; i < n_links; i++){
    link = links[i];
    if(link_get_id(link) == aux_id){
      space->down = link;
      break;
    }
  }

  toks = strtok(NULL,"|");
  size = atoi(toks);

  space->discovered = size;

  return space;
}