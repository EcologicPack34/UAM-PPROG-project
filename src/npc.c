/**
 * @file npc.c
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include "entity.h"
#include "npc.h"



struct _NPC{
    Entity *entity;

    NPC_status status;
};


void npc_destroy(void *npc){
    return;
}

int npc_cmp(void *npc1, void *npc2){
    return 0;
}