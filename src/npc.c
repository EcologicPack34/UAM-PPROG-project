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

/*
* PRIVATE FUNCTIONS
*/



/*
* PUBLIC INTERFACE IMPLEMENTATION
*/

NPC *npc_create(NPC_status status, char *name, Id id, Id location, double health, double baseDamage, int strength, int defense, int magicLevel){
    NPC *npc = NULL;

    if(!name)
        return NULL;

    npc = (NPC *)malloc(sizeof(NPC));
    if(!npc){
        debug_log(LOG_ERROR, "npc_create dynamic memory error at npc struct on npc: id: %ld locationid: %d", id, location);
        return NULL;
    }

    npc->status = status;
    npc->entity = entity_create(name, id, location, NPC_INVENTORY, health, baseDamage, strength, defense, magicLevel);
    if(npc->entity == NULL){
        debug_log(LOG_ERROR, "npc_create dynamic memory error at entity_create on npc: id: %ld locationid: %d", id, location);
        return NULL;
    }

    return npc;
}

void npc_destroy(void *npc){
    NPC *auxent = NULL;
    
    if(npc){
        auxent = (NPC *)auxent;
        entity_destroy(auxent->entity);
        free(auxent);
    }
}

/*NPC GETTERS*/

Entity *npc_get_entity(NPC *npc){
    if(!npc)
        return NULL;

    return npc->entity;
}

NPC_status npc_get_status(NPC *npc){
    if(!npc)
        return UNKNOWN_STATUS;
    
    return npc->status;
}

/*NPC SETTERS*/

Status npc_set_status(NPC *npc, NPC_status status){
    if(!npc)
        return ERROR;

    npc->status = status;

    return OK;
}

int npc_cmp(void *npc1, void *npc2){
    NPC *npcaux1 = NULL, *npcaux2 = NULL;
    Entity *ent1 = NULL, *ent2 = NULL;
    Id id1, id2;

    if(!npc1 || !npc2)
        return -2;

    npcaux1 = (NPC *)npc1;
    npcaux2 = (NPC *)npc2;

    ent1 = npc_get_entity(npcaux1);
    ent2 = npc_get_entity(npcaux2);

    id1 = entity_get_id(ent1);
    id2 = entity_get_id(ent2);
    if(id1 == id2){
        return 0;
    } else if(id1 > id2){
        return 1;
    } else {
        return -1;
    }

    return -2;
}

int npc_print(FILE *pf, NPC *npc){
    int charnum;
    Entity *ent = NULL;
    
    if(!pf || !npc)
        return -1;

    ent = npc_get_entity(npc);

    charnum = fprintf(pf, "Id: %ld | Name: %s", entity_get_id(ent), entity_get_name(ent));
    
    return charnum;
}

Status npc_get_str_descr(NPC *npc, char *str){
    Entity *ent = NULL;
    
    if(!npc || !str)
        return ERROR;

    ent = npc_get_entity(npc);
    sprintf(str, "Id: %ld | Name: %s", entity_get_id(ent), entity_get_name(ent));

    return OK;
}