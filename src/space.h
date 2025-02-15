/**
 * @brief It defines the space module interface
 *
 * This module regulates the game map by the space ADT, is the base of the movement functionality and other upcoming features.
 * A space is defined by his unique id, but it also contains the name of the space and the ids of the links connected to
 * the north, south, east and west. Also for now, each space has a bool that determines if it has the object or not.
 *
 * IMPORTANT - All spaces are stored inside the game struct in game.h, the memory of the spaces is tracked by that struct.
 *
 *
 * @file space.h
 * @author Original: Profesores PPROG. Modified by: Daniel Gómez
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef SPACE_H
#define SPACE_H

#include "types.h"
#include "object.h"
#include "link.h"

#include <stdbool.h>

typedef struct _Space Space;

/*Space public functions*/
#pragma region SPACE

/**
 * @brief It creates a new space, allocating memory and initializing its members
 * @author Profesores PPROG
 *
 * @param id the identification number for the new space
 * @return a new space, initialized
 */
Space *space_create(Id id);

/**
 * @brief It destroys a space, freeing the allocated memory
 * @author Profesores PPROG
 *
 * @param space a pointer to the space that must be destroyed
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_destroy(Space *space);

/*Space SETTERS*/
#pragma region SETTERS

/**
 * @brief It sets the name of a space
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @param name a string with the name to store
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_name(Space *space, char *name);

/**
 * @brief It sets the id of the space located at the north
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @param link reference of the link to the north
 * @return OK, if everything goes well or ERROR if there was some mistake
 */

Status space_set_north(Space *space, Link *link);

/**
 * @brief It sets the id of the space located at the south
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @param link reference of the link to the south
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_south(Space *space, Link *link);

/**
 * @brief It sets the id of the space located at the east
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @param link reference of the link to the east
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_east(Space *space, Link *link);

/**
 * @brief It sets the id of the space located at the west
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @param link reference of the link to the west
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_west(Space *space, Link *link);

/**
 * @brief It sets whether the space has an object or not
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @param value a boolean, specifying if in the space there is an object (true) or not (false)
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_object(Space *space, bool value);

#pragma endregion

/*Space GETTERS*/
#pragma region GETTERS

/**
 * @brief It gets the id of a space
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return the id of space
 */
Id space_get_id(Space *space);

/**
 * @brief It gets the name of a space
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return  a string with the name of the space
 */
const char *space_get_name(Space *space);

/**
 * @brief It gets the link of the space located at the north
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return reference of the link in the position
 */
Link *space_get_north(Space *space);

/**
 * @brief It gets the link of the space located at the south
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return reference of the link in the position
 */
Link *space_get_south(Space *space);

/**
 * @brief It gets the link of the space located at the east
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return reference of the link in the position
 */
Link *space_get_east(Space *space);

/**
 * @brief It gets the link of the space located at the west
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return reference of the link in the position
 */
Link *space_get_west(Space *space);

/**
 * @brief It gets whether the space has an object or not
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return a boolean, specifying if in the space there is an object (true) or not (false)
 */
bool space_get_object(Space *space);

#pragma endregion

/**
 * @brief It prints the space information
 * @author Profesores PPROG
 *
 * This fucntion shows the id and name of the space, the spaces that surrounds it and wheter it has an object or not.
 * @param space a pointer to the space
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_print(Space *space);

#pragma endregion

#endif
