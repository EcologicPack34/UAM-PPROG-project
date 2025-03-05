/**
 * @file npc.h
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
*/

#ifndef NPC_H
#define NPC_H

#include <stdio.h>
#include "types.h"
#include "debug_printing.h"

/*
 *Enum to determine wether an NPC is neutral with the player, is an ally or an enemy 
 */
typedef enum {UNKNOWN_STATUS, NEUTRAL, ALLY, ENEMY}NPC_status;

/**
 * @brief Struct that contains all the entity information related to an NPC, their dialogues and their NPC_status
 * 
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
 * @param name name of the NPC
 * @param id id of the NPC
 * @param location id of the space where the NPC is located
 * @param health health of the NPC
 * @param baseDamage base damage of the NPC
 * @param strength strength stat of the NPC
 * @param defense defense stat of the NPC
 * @param magicLevel magic level stat of the NPC
 * @return NPC* or NULL if error
 */
NPC *npc_create(NPC_status status, char *name, Id id, Id location, double health, double baseDamage, int strength, int defense, int magicLevel);

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
 * @brief Compares two npcs
 * @author Maksym Polyak
 * 
 * @param npc1 
 * @param npc2 
 * @return int = 0 if their id is equal, 1 if the id of npc1 is greater, -1 if the id of npc2 is greater or -2 if there was an error
 */
int npc_cmp(void *npc1, void *npc2);


#endif