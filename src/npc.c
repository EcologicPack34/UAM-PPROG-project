/**
 * @file npc.c
 * @author Maksym Polyak
 * @brief Controls NPC internal functionality
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
*/
#include "npc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief ADT that stores all the information of an NPC
 */
struct _NPC{
    Entity *entity;                 /*!< Entity struct of the NPC*/

    char message[WORD_SIZE];        /*!< Message of the NPC for the player*/
    int dialogue_state;             /*!< Determines the state of the dialogue in general*/

    bool can_follow;                /*!< Determines if the NPC can follow the player or not*/
    Id player_following_id;         /*!< Saves the id of the player its following*/
    NPC_status status;              /*!< Determines if the NPC is an ALLY, ENEMY or NEUTRAL towards players*/
};

/*
* PRIVATE FUNCTIONS
*/



/*
* PUBLIC INTERFACE IMPLEMENTATION
*/

NPC *npc_create(NPC_status status, bool can_follow, char *message, char *name, Id id, Id location){
    NPC *npc = NULL;

    if(!name)
        return NULL;

    npc = (NPC *)calloc(1,sizeof(NPC));
    if(!npc){
        debug_log(LOG_ERROR, "npc_create dynamic memory error at npc struct on npc: id: %ld locationid: %d", id, location);
        return NULL;
    }

    strcpy(npc->message, message);
    npc->status = status;
    npc->can_follow = can_follow;
    npc->player_following_id = NO_ID;

    /*by default, all stats are set to level 1*/
    npc->entity = entity_create(name, id, location, NPC_INVENTORY);
    if(npc->entity == NULL){
        debug_log(LOG_ERROR, "npc_create dynamic memory error at entity_create on npc: id: %ld locationid: %d", id, location);
        free(npc);
        return NULL;
    }

    return npc;
}

void npc_destroy(void *npc){ 
    if(npc){
        entity_destroy(((NPC *)npc)->entity);
        free(npc);
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

char *npc_get_message(NPC *npc){
    if(!npc)
        return NULL;

    return npc->message;
}

bool npc_get_can_follow(NPC *npc){
    if(!npc) return false;

    return npc->can_follow;
}

Id npc_get_player_following_id(NPC *npc){
    if(!npc) return false;

    return npc->player_following_id;
}

int npc_get_dialogue_state(NPC *npc){
    if(!npc) return -1;

    return npc->dialogue_state;
}

/*NPC SETTERS*/

Status npc_set_status(NPC *npc, NPC_status status){
    if(!npc)
        return ERROR;

    npc->status = status;

    return OK;
}

Status npc_set_message(NPC *npc, char *message){
    if(!npc || !message)
        return ERROR;

    strcpy(npc->message, message);

    return OK;
}

Status npc_set_is_follower(NPC *npc, bool can_follow){
    if(!npc) return ERROR;

    npc->can_follow = can_follow;

    return OK;
}

Status npc_set_player_following_id(NPC *npc, Id player_follower_id){
    if(!npc) return ERROR;

    npc->can_follow = player_follower_id;

    return OK;
}

Status npc_set_dialogue_state(NPC *npc, int dialogue_state){
    if(!npc) return ERROR;

    npc->dialogue_state = dialogue_state;

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

void npc_print(void *npc){
    Entity *ent = NULL;
    
    if(!npc)
        return;

    ent = npc_get_entity((NPC *)npc);

    printf("Id: %ld | Name: %s", entity_get_id(ent), entity_get_name(ent));
}

Status npc_get_str_descr(NPC *npc, char *str, int index){
    Entity *ent = NULL;
    
    if(!npc || !str)
        return ERROR;

    ent = npc_get_entity(npc);
    sprintf(str, "%s (%s): H:%.1lf,L:%ld (%ld)", entity_get_graphic_description(ent), entity_get_name(ent), entity_get_health(ent),entity_get_location(ent), entity_get_id(ent));

    return OK;
}