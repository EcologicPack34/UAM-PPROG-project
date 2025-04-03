/**
 * @brief The NPC module controls the creation and actions of the NPCs on the game, each
 * NPC is an entity, has a message and a status to determine their behaviour towards the
 * player.
 * 
 * @file npc.h
 * @author Maksym Polyak
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
*/

#ifndef NPC_H
#define NPC_H

#include <stdio.h>
#include <string.h>

#include "entity.h"
#include "types.h"
#include "debug_printing.h"

/**
 * @brief Enum to determine wether an NPC is neutral with the player, is an ally or an enemy 
 */
typedef enum {UNKNOWN_STATUS, NEUTRAL, ALLY, ENEMY}NPC_status;

/**
 * @brief Struct that contains all the entity information related to an NPC, their dialogues and their NPC_status
 */
typedef struct _NPC NPC;

/*
* PUBLIC INTERFACE FUNCTIONS
*/

/**
 * @brief Allocates memory for an NPC struct and returns its pointer
 * @author Maksym Polyak
 * 
 * @param status status with the player: NPC_status
 * @param can_follow determines if the NPC becomes a follower after chat
 * @param message message of the NPC on start
 * @param name name of the NPC
 * @param id id of the NPC
 * @param location id of the space where the NPC is located
 *
 * @return NPC* or NULL if error
 */
NPC *npc_create(NPC_status status, bool can_follow, char *message, char *name, Id id, Id location);

/**
 * @brief Frees all the memory related to an npc
 * @author Maksym Polyak
 * 
 * @param npc struct pointer of the NPC
 */
void npc_destroy(void *npc);

/*NPC GETTERS*/

/**
 * @brief Gets the entity struct of an NPC
 * @author Maksym Polyak
 * 
 * @param npc struct pointer of the NPC
 * @return Entity* or NULL if error
 */
Entity *npc_get_entity(NPC *npc);

/**
 * @brief Gets the NPC_status of an NPC
 * @author Maksym Polyak
 * 
 * @param npc struct pointer of the NPC
 * @return NPC_status or UNKNOWN_STATUS if error
 */
NPC_status npc_get_status(NPC *npc);

/**
 * @brief Gets the char * with the message of the NPC
 * @author Maksym Polyak
 * 
 * @param npc npc struct
 * @return char* or NULL if error
 */
char *npc_get_message(NPC *npc);

/**
 * @brief Gets if the NPC can be a follower after chat or not
 * @author Maksym Polyak
 * 
 * @param npc npc struct
 * @return true if npc can be a follower
 * @return false if npc cannot be a follower
 */
bool npc_get_can_follow(NPC *npc);

/*NPC SETTERS*/

/**
 * @brief Sets an npc status towards the player
 * @author Maksym Polyak
 * 
 * @param npc struct pointer of the NPC
 * @param status status to be set
 * @return Status
 */
Status npc_set_status(NPC *npc, NPC_status status);

/**
 * @brief Sets the message of the NPC to the one received as an input
 * @author Maksym Polyak
 * 
 * @param npc npc struct
 * @param message message string to set
 * @return Status 
 */
Status npc_set_message(NPC *npc, char *message);

/**
 * @brief Sets the can_follow bool on npc
 * @author Maksym Polyak
 * 
 * @param npc npc struct
 * @param can_follow bool 1 if can follow
 * @return Status 
 */
Status npc_set_is_follower(NPC *npc, bool can_follow);

/**
 * @brief Compares two npcs
 * @author Maksym Polyak
 * 
 * @param npc1 npc struct 1
 * @param npc2 npc struct 2
 * @return int = 0 if their id is equal, 1 if the id of npc1 is greater, -1 if the id of npc2 is greater or -2 if there was an error
 */
int npc_cmp(void *npc1, void *npc2);

/**
 * @brief Prints on screen the NPC information
 * @author Maksym Polyak
 * 
 * @param npc npc struct
 */
void npc_print(void *npc);

/**
 * @brief Gets a string without \ n with the information of an NPC on str, str starts with (index).
 * @author Maksym Polyak
 * 
 * @param npc npc struct
 * @param str string where description is saved
 * @param index index where the NPC is located
 * @return Status 
 */
Status npc_get_str_descr(NPC *npc, char *str, int index);


#endif