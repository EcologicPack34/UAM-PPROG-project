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
#define SPACE_GRAPHIC_HEIGHT 5
#define SPACE_GRAPHIC_WIDTH 9

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

/**
 * @brief Sets the position of a given space to a vector of coordenates (x,y)
 * @author Daniel Gómez
 * 
 * @param space 
 * @param x 
 * @param y 
 * @return Status 
 */
Status space_set_position(Space *space, float x, float y);

/**
 * @brief Sets if the spaces has been mapped spatialy or not
 * @author Daniel Gómez
 * 
 * @param space 
 * @param status 
 * @return Status 
 */
Status space_set_isMapped(Space *space, bool status);

/**
 * @brief Sets the neighbour in a given direction of a given spac
 * @author Daniel Gómez
 * 
 * @param space 
 * @param neighbour 
 * @param direction 
 * @return Status 
 */
Status space_set_neighbour(Space *space, Space *neighbour, Direction direction);

/**
 * @brief Sets the graphic description of a given spaces
 * 
 * @param space 
 * @param desc 
 * @return Status 
 */
Status space_set_graphic_description(Space *space, char *desc, int index);

/*Space GETTERS*/

/**
 * @brief Gets the reference to the graphic description of a space
 * 
 * @param space 
 * @return char** 
 */
char **space_get_graphic_description(Space *space);

/**
 * @brief Gets the pointer to the vector2 storing the position of the space
 * @author Daniel Gómez
 * 
 * @param space 
 * @return Vector2* 
 */
Vector2 *space_get_position(Space *space);

/**
 * @brief Gets if the spaces has been mapped or not
 * @author Daniel Gómez
 * 
 * @param space 
 * @return true 
 * @return false 
 */
bool space_get_isMapped(Space *space);

/**
 * @brief Gets a reference to the neighbour in a certain direction
 * @author Daniel Gómez
 * 
 * @param space 
 * @param direction 
 * @return Pointer to neighbour if exist, NULL if not
 */
Space *space_get_neighbour(Space *space, Direction direction);

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
 * @brief Gets the number of npcs located on a space
 * @author Maksym Polyak
 * 
 * @param space 
 * @return int or -1 if error
 */
int space_get_npc_count(Space *space);

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

/**
 * @brief Gets an NPC on a space in the index
 * 
 * @param space 
 * @param index 
 * @return NPC* or NULL if error
 */
NPC *space_get_NPC_at(Space *space, int index);

/** 
 * @brief Gets an NPC by its name on a space
 * 
 * @param space 
 * @param name
 * @return NPC* or NULL if error
 */
NPC *space_get_NPC_by_name(Space *space, char *name);

/**
 * @brief Gets a list of the npcs in the space with a given length
 * @author Daniel Gómez
 * 
 * @param space 
 * @param str 
 * @param length 
 * @return Status 
 */
Status space_get_NPC_list(Space *space, char *str, int length);

#endif
