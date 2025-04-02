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

typedef struct _Link Link;

/**
 * @brief Function to create and initialize a link
 * @author Daniel Gómez
 * 
 * @param space1 space connected by link
 * @param space2 space connected by link
 * @param locked if the link is blocked and cant be used or not
 * @param unlockingObject object required to unlock the link
 * @return Link* 
 */
Link * link_create(Id id,Id space1, Id space2, bool adjacent,bool locked, Id unlockingObject);

/**
 * @brief frees dynamic memory in use by a link
 * @author Daniel Gómez
 * 
 * @param link 
 */
void link_destroy(Link* link);

/**
 * @brief Sets de Id of the link
 * @author Daniel Gómez
 * 
 * @param link 
 * @param id 
 * @return Status 
 */
Status link_set_id(Link *link, Id id);

/**
 * @brief Sets the ids of spaces that the link connects
 * @author Daniel Gómez
 * 
 * @param link 
 * @param id1 
 * @param id2 
 * @return Status 
 */
Status link_set_spaces(Link* link, Id id1, Id id2);

/**
 * @brief Sets of the spaces connected are adjacent or not
 * @author Daniel Gómez
 * 
 * @param link 
 * @param status 
 * @return Status 
 */
Status link_set_is_adjacent(Link *link, bool status);

/**
 * @brief Sets if the link is locked
 * @author Daniel Gómez
 * 
 * @param link 
 * @param status 
 * @return Status 
 */
Status link_set_locked(Link* link, bool status);

/**
 * @brief sets the object that unlocks a link
 * @author Daniel Gómez
 * 
 * @param link 
 * @param object 
 * @return Status 
 */
Status link_set_unlocking_object(Link* link, Id object);

/**
 * @brief Gets the id of a link
 * @author Daniel Gómez
 * 
 * @param link 
 * @return Id 
 */
Id link_get_id(Link *link);

/**
 * @brief gets the id one of the spaces that is connected by the link
 * @author Daniel Gómez
 * 
 * @param link 
 * @return Id 
 */
Id link_get_space1(Link *link);

/**
 * @brief gets the id one of the spaces that is connected by the link
 * @author Daniel Gómez
 * 
 * @param link 
 * @return Id 
 */
Id link_get_space2(Link *link);

/**
 * @brief Gets the id of the oposite side of the link if posible
 * @author Daniel Gómez
 * 
 * @param link 
 * @param currentSpace 
 * @return Id 
 */
Id link_get_oposite_space(Link *link, Id currentSpace);

/**
 * @brief Gets the state of adjacency between the spaces
 * @author Daniel Gómez
 * 
 * @param link 
 * @return true 
 * @return false 
 */
bool link_is_adjacent(Link *link);

/**
 * @brief gets if the link is locked
 * @author Daniel Gómez
 * 
 * @param link 
 * @return true 
 * @return false 
 */
bool link_is_locked(Link *link);

/**
 * @brief gets the id of the unlocking object
 * @author Daniel Góme
 * 
 * @param link 
 * @return Id 
 */
Id link_get_unlocking_object(Link *link);

/**
 * @brief Moves an entity from one end of the link to another if posible
 * @author Daniel Gómez
 * 
 * @param link 
 * @param game 
 * @param entity 
 * @return Status 
 */
Status link_move_entity(Link* link, Entity* entity);

/**
 * @brief Tries to unlock a link using a certain object
 * 
 * @param link 
 * @param obj 
 * @return Status 
 */
Status link_unlock(Link *link, Object *obj);

#endif