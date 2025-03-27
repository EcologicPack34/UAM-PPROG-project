/**
 * @brief It defines the player module interface
 * 
 * The player is the main character inside the game, the one with the ability to move,
 * take objects and more. For now the player module is just an entity, but further functionality will be added.
 * 
 * The player memory in the game is controlled by the game struct in game.h
 * 
 * The player has its own stats, on I3 the idea is to develop an equipment and powers/ability system.
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

/*Player public functions*/

/**
 * @brief Creates a player struct
 * @author Profesores PPROG
 *
 * @param name name of the player
 * @param identity id of the entity
 * @param location id of the space where the player is located
 * @param maxhealth max health of the entity
 * @param health health of the entity
 * @param baseDamage base damage of the entity
 * @param strength strength of the entity
 * @param defense defense of the entity
 * @param magicLevel magic level of the entity
 * @return player pointer if everything went fine or NULL if there was a mistake
 */
Player *player_create(char *name, Id identity, Id location, double maxhealth, double health, double baseDamage, int strength, int defense, int magicLevel);

/**
 * @brief Frees a player struct
 * @author Profesores PPROG
 *
 * @param player struct with the information of a player
 */
void player_destroy(Player *player);

/*Player GETTERS*/

/**
 * @brief returns the entity of the player
 * @author Daniel Gómez
 * 
 * @param player struct with the information of a player
 * @return Entity* 
 */
Entity *player_get_entity(Player *player);

/**
 * @brief Gets a string description of player and copies it into str
 * @author Daniel Gómez
 * 
 * @param player 
 * @param str 
 * @return Status 
 */
Status player_get_str_desc(Player *player, char *str);

/*Player SETTERS*/

/**
 * @brief Prints a player struct
 * @author Profesores PPROG
 *
 * @param player struct with the information of a player
 */
void player_print(Player *player);

#endif