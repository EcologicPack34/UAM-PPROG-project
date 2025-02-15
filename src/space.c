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
  Id id;                    /*!< Id number of the space, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the space */
  Link *north;                 /*!< Id of the space at the north */
  Link *south;                 /*!< Id of the space at the south */
  Link *east;                  /*!< Id of the space at the east */
  Link *west;                  /*!< Id of the space at the west */
  bool object;              /*!< Whether the space has an object or not */
};

/*Space public functions*/
#pragma region SPACE

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
  newSpace->north = NULL;
  newSpace->south = NULL;
  newSpace->east = NULL;
  newSpace->west = NULL;
  newSpace->object = false;

  return newSpace;
}

Status space_destroy(Space* space) {
  if (!space) {
    return ERROR;
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

/*Space SETTERS*/
#pragma region SETTERS

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

Status space_set_object(Space* space, bool value) {
  if (!space) {
    return ERROR;
  }
  space->object = value;
  return OK;
}

#pragma endregion

/*Space GETTERS*/
#pragma region GETTERS

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

bool space_get_object(Space* space) {
  if (!space) {
    return false;
  }
  return space->object;
}

#pragma endregion

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

  /* 3. Print if there is an object in the space or not */
  if (space_get_object(space)) {
    fprintf(stdout, "---> Object in the space.\n");
  } else {
    fprintf(stdout, "---> No object in the space.\n");
  }

  #pragma endregion

  return OK;
}
