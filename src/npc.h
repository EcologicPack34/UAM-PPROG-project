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

typedef enum {NEUTRAL, ALLY, ENEMY}NPC_status;

typedef struct _NPC NPC;

NPC *npc_create();

void npc_destroy(void *npc);

/**
 * @brief Compares two npcs
 * @author Maksym Polyak
 * 
 * @param npc1 
 * @param npc2 
 * @return int 
 */
int npc_cmp(void *npc1, void *npc2);


#endif