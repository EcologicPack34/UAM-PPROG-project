/**
 * @brief It defines the player module interface
 *
 * @file object.h
 * @author Maksym Polyak
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

typedef struct _Player Player;


/**
 * @brief Creates a player struct
 * @author Profesores PPROG
 *
 * @param name name of the player
 * @param identity id of the entity
 * @param location id of the space where the player is located
 * @return player pointer if everything went fine or NULL if there was a mistake
 */
Player *player_create(char *name, Id identity, Id location);

/**
 * @brief Frees a player struct
 * @author Profesores PPROG
 *
 * @param player struct with the information of a player
 */
void player_destroy(Player *player);

/**
 * @brief Gets a player entity
 * @author Profesores PPROG
 *
 * @param player struct with the information of a player
 * @return entity pointer or NULL if wrong
 */
Entity *player_get_entity(Player *player);

/**
 * @brief Prints a player struct
 * @author Profesores PPROG
 *
 * @param player struct with the information of a player
 */
void player_print(Player *player);

#endif