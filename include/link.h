/**
 * @brief Header file definin links between spaces.
 * 
 * Controls the movement of the player between two spaces, each space can have
 * up to four links which can be locked and has to be unlocked by an object and more.
 * 
 * @file link.h
 * @author Daniel Gómez Rodríguez
 * @version 0.1
 * @date 2025-02-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef LINK_H
#define LINK_H

#include "types.h"
#include "entity.h"

/**
 * @brief ADT to determine how spaces are linked between them
 */
typedef struct _Link Link;

/**
 * @brief Function to create and initialize a link
 * @author Daniel Gómez
 * 
 * @param id id of the link
 * @param space1 space connected by link
 * @param space2 space connected by link
 * @param adjacent bool true if spaces are adjacent false if not
 * @param locked if the link is blocked and cant be used or not
 * @return Link* 
 */
Link * link_create(Id id,Id space1, Id space2, bool adjacent, bool locked);

/**
 * @brief frees dynamic memory in use by a link
 * @author Daniel Gómez
 * 
 * @param link link struct
 */
void link_destroy(Link* link);

/**
 * @brief Sets de Id of the link
 * @author Daniel Gómez
 * 
 * @param link link struct
 * @param id id to set
 * @return Status 
 */
Status link_set_id(Link *link, Id id);

/**
 * @brief Sets the ids of spaces that the link connects
 * @author Daniel Gómez
 * 
 * @param link link struct
 * @param id1 id of space 1
 * @param id2 id of space 2
 * @return Status 
 */
Status link_set_spaces(Link* link, Id id1, Id id2);

/**
 * @brief Sets of the spaces connected are adjacent or not
 * @author Daniel Gómez
 * 
 * @param link link struct
 * @param status if it is adjacent(1) or not(0)
 * @return Status 
 */
Status link_set_is_adjacent(Link *link, bool status);

/**
 * @brief Sets if the link is locked
 * @author Daniel Gómez
 * 
 * @param link link struct
 * @param status if it is locked(1) or not(0)
 * @return Status 
 */
Status link_set_locked(Link* link, bool status);

/**
 * @brief Gets the id of a link
 * @author Daniel Gómez
 * 
 * @param link link struct
 * @return Id or NO_ID if error
 */
Id link_get_id(Link *link);

/**
 * @brief gets the id one of the spaces that is connected by the link
 * @author Daniel Gómez
 * 
 * @param link link struct
 * @return Id or NO_ID if error
 */
Id link_get_space1(Link *link);

/**
 * @brief gets the id one of the spaces that is connected by the link
 * @author Daniel Gómez
 * 
 * @param link link struct
 * @return Id or NO_ID if error
 */
Id link_get_space2(Link *link);

/**
 * @brief Gets the id of the oposite side of the link if posible
 * @author Daniel Gómez
 * 
 * @param link link struct
 * @param currentSpace id of the space to mirror
 * @return Id or NO_ID if error
 */
Id link_get_oposite_space(Link *link, Id currentSpace);

/**
 * @brief Gets the state of adjacency between the spaces
 * @author Daniel Gómez
 * 
 * @param link link struct
 * @return true if spaces are adjacent
 * @return false if spaces are not adjacent
 */
bool link_is_adjacent(Link *link);

/**
 * @brief gets if the link is locked
 * @author Daniel Gómez
 * 
 * @param link link struct
 * @return true if it is locked
 * @return false if it is not locked
 */
bool link_is_locked(Link *link);

/**
 * @brief Moves an entity from one end of the link to another if posible
 * @author Daniel Gómez
 * 
 * @param link link struct
 * @param entity entity to move
 * @return Ok if moved entity, Error if blocked link or any other error
 */
Status link_move_entity(Link* link, Entity* entity);

/**
 * @brief Tries to unlock a link using a certain object
 * 
 * @param link link struct
 * @param player player that used it
 * @return Status 
 */
Status link_unlock(Link *link, Entity *player);

#endif