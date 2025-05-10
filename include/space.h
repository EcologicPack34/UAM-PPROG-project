/**
 * @brief It defines the space module interface
 *
 * This module regulates the game map by the space ADT, is the base of the movement functionality and other upcoming features.
 * A space is defined by his unique id, but it also contains the name of the space and the ids of the links connected to
 * the north, south, east and west.
 *
 * IMPORTANT - All spaces are stored inside the game struct in game.h, the memory of the spaces is tracked by that struct.
 * 
 * Every space now has a graphic description in order to print it with the graphic engine, also each
 * space has been localized by a vector, that allows the graphic engine to print the spaces around it
 * without looking each time. Any space that isn't connected to the main block of spaces won't show up
 * on screen.
 * 
 * To be able to store items easily in a Space, now each space has an inventory. On the other hand, a
 * collection with the npcs has also been added to allow the combat module to search for NPCs in a 
 * designated space.
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
#include "graphic_description.h"

#include <stdbool.h>

#define SPACE_INITIAL_SIZE_NPCS 50      /*!< Maximum number of NPCs the space can hold*/
#define SPACE_GRAPHIC_HEIGHT 5          /*!< Height of a space graphical description*/
#define SPACE_GRAPHIC_WIDTH 9           /*!< Width of a space graphical description*/

#define SPACE_DIRECTIONS 6              /*!< Number of total directions implemented in the game*/

/**
 * @brief ADT with all the information related to a space
 */
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
 * @brief Sets the description of a space
 * @author Maksym Polyak
 * 
 * @param space space struct
 * @param descr descr char *
 * @return Status 
 */
Status space_set_description(Space *space, char *descr);

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
 * @brief It sets the id of the space located at the top
 * @author Daniel Gómez
 *
 * @param space a pointer to the space
 * @param link reference of the link to the west
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_up(Space *space, Link *link);

/**
 * @brief It sets the id of the space located down
 * @author Daniel Gómez
 *
 * @param space a pointer to the space
 * @param link reference of the link to the west
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_down(Space *space, Link *link);

/**
 * @brief Sets the position of a given space to a vector of coordenates (x,y)
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @param x float with x coordinate
 * @param y float with y coordinate
 * @return Status 
 */
Status space_set_position(Space *space, float x, float y);

/**
 * @brief Sets if the spaces has been mapped spatialy or not
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @param status bool true if mapped false if not
 * @return Status 
 */
Status space_set_isMapped(Space *space, bool status);

/**
 * @brief Sets the mapped block to which the space corresponds
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @param block block level to set
 * @return Status 
 */
Status space_set_map_block(Space *space, int block);

/**
 * @brief Sets the neighbour in a given direction of a given spac
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @param neighbour neighbour space in given direction
 * @param direction direction to evaluate
 * @return Status 
 */
Status space_set_neighbour(Space *space, Space *neighbour, Direction direction);

/**
 * @brief Sets the graphic description of a given space
 * @author Daniel Gómez
 * 
 * @param space current space
 * @param gdesc reference to graphic desc
 * @return Status 
 */
Status space_set_graphic_description(Space *space, GDesc *gdesc);

/**
 * @brief Sets if the space has been discovered or not.
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @param status bool true if discovered false if not discovered
 * @return Status 
 */
Status space_set_isDiscovered(Space *space, bool status);

/*Space GETTERS*/

/**
 * @brief Gets if a given spaces has been discovered
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @return true if discovered
 * @return false if not discovered
 */
bool space_get_isDiscovered(Space *space);

/**
 * @brief Gets the reference to the graphic description of a space
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @return GDesc* or NULL if error
 */
GDesc *space_get_graphic_description(Space *space);

/**
 * @brief Gets the description of the space
 * @author Maksym Polyak
 * 
 * @param space space struct
 * @return char* 
 */
char *space_get_description(Space *space);

/**
 * @brief Gets the pointer to the vector2 storing the position of the space
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @return Vector2* or NULL if error
 */
Vector2 *space_get_position(Space *space);

/**
 * @brief Gets if the spaces has been mapped or not
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @return true if mapped
 * @return false if not mapped
 */
bool space_get_isMapped(Space *space);

/**
 * @brief Gets the block in which the space has been mapped
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @return int or -1 if error
 */
int space_get_map_block(Space *space);

/**
 * @brief Gets a reference to the neighbour in a certain direction
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @param direction direction to evaluate
 * @return Pointer to neighbour if exist, NULL if not
 */
Space *space_get_neighbour(Space *space, Direction direction);

/**
 * @brief It gets the id of a space
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return the id of space or NO_ID if error
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
 * @brief It gets the link of the space located at the top
 * @author Daniel Gómez
 *
 * @param space a pointer to the space
 * @return reference of the link in the position
 */
Link *space_get_up(Space *space);

/**
 * @brief It gets the link of the space located at the west
 * @author Daniel Gómez
 *
 * @param space a pointer to the space
 * @return reference of the link in the position
 */
Link *space_get_down(Space *space);

/**
 * @brief Gets the inventory pointer of a space struct
 * @author Daniel Gómez
 * 
 * @param space a pointer to the space
 * @return Inventory* or NULL if error
 */
Inventory *space_get_inventory(Space *space);

/**
 * @brief Gets the number of npcs located on a space
 * @author Maksym Polyak
 * 
 * @param space space struct
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
 * @param space space struct
 * @param npc npc struct to add
 * @return Status 
 */
Status space_add_NPC(Space *space, NPC *npc);

/**
 * @brief Removes an NPC struct from the space NOTE: Sets the NPC location to UNKNOWN_ID
 * @author Maksym Polyak
 * 
 * @param space space struct
 * @param npc npc struct to add
 * @return Status 
 */
Status space_remove_NPC(Space *space, NPC *npc);

/**
 * @brief Moves an NPC from a space to another space NOTE: Sets the location of the npc to the one of the spaceIN
 * @author Maksym Polyak
 * 
 * @param spaceOUT space where the NPC is taken
 * @param spaceIN space where the NPC is moved
 * @param npc npc to move
 * @return Status 
 */
Status space_move_NPC(Space *spaceOUT, Space *spaceIN, NPC *npc);

/**
 * @brief Gets an NPC on a space in the index
 * @author Maksym Polyak
 * 
 * @param space space struct
 * @param index index of the NPC
 * @return NPC* or NULL if error
 */
NPC *space_get_NPC_at(Space *space, int index);

/** 
 * @brief Gets an NPC by its name on a space
 * @author Maksym Polyak
 * 
 * @param space space struct
 * @param name name of the NPC
 * @return NPC* or NULL if error or not found
 */
NPC *space_get_NPC_by_name(Space *space, char *name);

/**
 * @brief Gets a list of the npcs in the space with a given length
 * @author Daniel Gómez
 * 
 * @param space space struct
 * @param str string where the NPC are saved
 * @param length number of NPCs to save on the list
 * @return Status 
 */
Status space_get_NPC_list(Space *space, char *str, int length);

/**
 * @brief Sets a space as the bool discovered
 * @author Maksym Polyak
 * 
 * @param space space struct
 * @param discovered bool true if discovered 0 if not discovered
 * @return Status 
 */
Status space_set_discovered(Space *space, bool discovered);

/**
 * @brief Returns wheter a space has been discovered
 * @author Maksym Polyak
 * 
 * @param space space struct
 * @return true if space is discovered
 * @return false if space is not discovered
 */
bool space_is_discovered(Space *space);

/**
 * @brief Compares to spaces
 * 
 * @param e1 space struct
 * @param e2 space struct
 * @return negative if e1<e2, 0 if equal, positive if e1>e2
 */
int space_cmp(void *e1, void *e2);

/**
 * @brief This function saves a space's information to a save file
 * @author Aaron Charameli Mair
 * 
 * @param file a pointer to the opened file
 * @param s a pointer to the space that will be saved
 * @return Status 
 */
Status space_save_to_file(FILE *file, Space *s);

/**
 * @brief Creates a space from a file and its collections
 * @author Maksym Polyak
 * 
 * @param fIN file stream input
 * @param gdescs collection of graphic descriptions
 * @param links links of the game
 * @param n_links num of links
 * @return Space* or NULL if error
 */
Space *space_create_from_file(FILE *fIN, Collection *gdescs, Link **links, int n_links);

#endif