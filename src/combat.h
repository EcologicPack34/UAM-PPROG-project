/**
 * @file combat.h
 * @author Maksym Polyak
 * @brief The combat module takes care of the game state FIGHT, checks stats of all
 * entities involved and then creates all the logic of the combat
 * @version 0.1
 * @date 2025-03-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef COMBAT_H
#define COMBAT_H

#include "collection.h"
#include "debug_printing.h"
#include "queue.h"
#include "entity.h"
#include "npc.h"
#include "space.h"
#include "player.h"

typedef struct _Combat Combat;

Combat *combat_initialize(Space *space, Player *player);

void combat_end(Combat *combat);


#endif