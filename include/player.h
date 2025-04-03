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

#include "command.h"
#include "entity.h"
#include "equipment.h"
#include "npc.h"

typedef struct _Player Player;

/*Player public functions*/

/**
 * @brief Creates a player struct
 * @author Profesores PPROG
 *
 * @param name name of the player
 * @param identity id of the entity
 * @param location id of the space where the player is located
 * 
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
 * @brief Gets the equipment struct from a player
 * @author Maksym Polyak
 * 
 * @param player 
 * @return Equipment* 
 */
Equipment *player_get_equipment(Player *player);

/**
 * @brief Gets the command data of the player
 * @author Daniel Gómez
 * 
 * @param player 
 * @return CommandInfo* 
 */
CommandInfo *player_get_cmdData(Player *player);

/**
 * @brief It sets a player's stats
 * @brief Aaron Charameli Mair
 * 
 * @param p a pointer to Player 
 * @param maxhealth 
 * @param health 
 * @param baseDamage 
 * @param strength 
 * @param defense 
 * @param magicLevel
 *  
 * @return Status 
 */
Status player_set_stats(Player *p, double maxhealth, double health, double baseDamage, int strength, int defense, int magicLevel);

/**
 * @brief Gets a string description of player and copies it into str
 * @author Daniel Gómez
 * 
 * @param player 
 * @param str 
 * @return Status 
 */
Status player_get_str_desc(Player *player, char *str);

/**
 * @brief Gets the money quantity of the player
 * @author Maksym Polyak
 * 
 * @param player 
 * @return Status 
 */
int player_get_money(Player *player);

/*Player SETTERS*/

/**
 * @brief Adds the value_added int to the player money, can be negative
 * but cant surpass int value or go below 0
 * 
 * @param player 
 * @param value_added 
 * @return Status 
 */
Status player_add_money(Player *player, int value_added);

/**
 * @brief Prints a player struct
 * @author Profesores PPROG
 *
 * @param player struct with the information of a player
 */
void player_print(Player *player);

/**
 * @brief Tries to equip a piece on the player and removes it from the inventory
 * @author Maksym Polyak
 * 
 * @param player 
 * @param object 
 * @return Status 
 */
Status player_equip_piece(Player *player, Object *object);

/**
 * @brief Tries to unequip a piece on the player and adds it to the inventory
 * @author Maksym Polyak
 * 
 * @param player 
 * @param data name of the type of piece that wants to be removed
 * @return Status 
 */
Status player_unequip_piece(Player *player, char *data);

/**
 * @brief Adds a follower to the player
 * @author Maksym Polyak
 * 
 * @param player 
 * @param npc 
 * @return Status 
 */
Status player_add_follower(Player *player, NPC *npc);

/**
 * @brief Removes a follower from the player by a name
 * @author Maksym Polyak
 * 
 * @param player 
 * @param npc_name 
 * @return Status 
 */
Status player_remove_follower_by_name(Player *player, char *npc_name);

/**
 * @brief Gets the array with the followers, of max size NPC_MAX_FOLLOWERS
 * @author Maksym Polyak
 * 
 * @param player 
 * @return Status 
 */
NPC **player_get_followers(Player *player);


#endif