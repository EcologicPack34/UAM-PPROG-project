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
#include "inventory.h"
#include "npc.h"
#include "vector2.h"

#include <stdbool.h>

#define SPACE_MAX_NPCS 8

typedef struct _Space Space;

/*Space public functions*/

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

Status space_set_position(Space *space, float x, float y);

Status space_set_isMapped(Space *space, bool status);

/*Space GETTERS*/

Vector2 *space_get_position(Space *space);

bool space_get_isMapped(Space *space);

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
 * @brief Gets the inventory pointer of a space struct
 * 
 * @param space a pointer to the space
 * @return Inventory* or NULL if error
 */
Inventory *space_get_inventory(Space *space);

/**
 * @brief It prints the space information
 * @author Profesores PPROG
 *
 * This fucntion shows the id and name of the space, the spaces that surrounds it and wheter it has an object or not.
 * @param space a pointer to the space
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_print(Space *space);

/**
 * @brief Adds an NPC struct to the space NOTE: Sets the location of the npc to the one of the space
 * @author Maksym Polyak
 * 
 * @param space 
 * @param npc 
 * @return Status 
 */
Status space_add_NPC(Space *space, NPC *npc);

/**
 * @brief Removes an NPC struct from the space NOTE: Sets the NPC location to UNKNOWN_ID
 * 
 * @param space 
 * @param npc 
 * @return Status 
 */
Status space_remove_NPC(Space *space, NPC *npc);

/**
 * @brief Moves an NPC from a space to another space NOTE: Sets the location of the npc to the one of the spaceIN
 * @author Maksym Polyak
 * 
 * @param spaceOUT 
 * @param spaceIN 
 * @param npc 
 * @return Status 
 */
Status space_move_NPC(Space *spaceOUT, Space *spaceIN, NPC *npc);

#endif
