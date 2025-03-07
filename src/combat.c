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

struct _Combat{
    Collection *allies;
    Collection *allies_stats;
    Collection *enemies;
    Collection *allies_stats;

    Queue *turns;
}