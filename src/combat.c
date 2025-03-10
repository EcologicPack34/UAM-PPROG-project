/**
 * @file combat.c
 * @author Maksym Polyak
 * @brief This module implements all the functionality from combat.h
 * @version 0.1
 * @date 2025-03-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "combat.h"
#include <stdio.h>
#include <stdlib.h>

#define NPC_MAX_ALLIES 3
#define NPC_MAX_ENEMIES 4

typedef struct {
    Entity_Stats stats;             /*!< Copy of the stats in order to buff/debuff and not modify the original stats*/
    Entity *entity;                 /*!< Entity from which the stats are saved*/
}Stats;

struct _Combat{
    Stats player_stats;         /*!< Stats of the player*/
    Stats *allies_stats;        /*!< Stats of the allies*/
    int allies_count;           /*!< Number of allies*/
    Stats *enemies_stats;       /*!< Stats of the enemies*/
    int enemies_count;          /*!< Number of enemies*/

    Queue *turns;               /*!< Queue with the order of the turns*/
    Entity *actual_entity;      /*!< Entity with the actual turn*/
};