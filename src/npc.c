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

    int dialogue_state;             /*!< Determines the state of the dialogue in general*/

    bool can_follow;                /*!< Determines if the NPC can follow the player or not*/
    NPC_status status;              /*!< Determines if the NPC is an ALLY, ENEMY or NEUTRAL towards players*/
};

/*
* PRIVATE FUNCTIONS
*/



/*
* PUBLIC INTERFACE IMPLEMENTATION
*/

NPC *npc_create(NPC_status status, bool can_follow, int dialogue_state, char *name, Id id, Id location){
    NPC *npc = NULL;

    if(!name)
        return NULL;

    npc = (NPC *)calloc(1,sizeof(NPC));
    if(!npc){
        debug_log(LOG_ERROR, "npc_create dynamic memory error at npc struct on npc: id: %ld locationid: %d", id, location);
        return NULL;
    }

    npc->status = status;
    npc->can_follow = can_follow;
    npc->dialogue_state = dialogue_state;

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

bool npc_get_can_follow(NPC *npc){
    if(!npc) return false;

    return npc->can_follow;
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

Status npc_set_is_follower(NPC *npc, bool can_follow){
    if(!npc) return ERROR;

    npc->can_follow = can_follow;

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

Status npc_save_to_file(FILE *file, NPC *p){
    char aux[WORD_SIZE]="";
    Id id, location;
    char *name=NULL;
    char *Gdesc=NULL;
    long mH,h,bD;
    int str,def,magicLvl;

    if(!file || !p) return ERROR;

    id = entity_get_id(p->entity);
    location = entity_get_location(p->entity);
    name = entity_get_name(p->entity);
    Gdesc = entity_get_graphic_description(p->entity);

    /*
   #n:ID|Dialogue State - 1 means initial - 0 means no dialogue|Nombre|LocationID|is_follower|Status|Gdesc;player_following_id
    #n:1|1|NPC1|11|1|1|^0m"
    */
    
    sprintf(aux, "#n:%ld|%d|%s|%ld|%d|%d|%s;%ld\n",id,p->dialogue_state,name,location,p->can_follow,p->status,Gdesc,p->player_following_id);
    fprintf(file, "%s", aux);

    mH=entity_get_max_health(p->entity);
    h=entity_get_health(p->entity);
    bD=entity_get_baseDamage(p->entity);
    str=entity_get_strength(p->entity);
    def=entity_get_defense(p->entity);
    magicLvl=entity_get_magicLevel(p->entity);

    /*#st:IDEntity|EntityType|VidaMaxima|Vida|DanoBase|Fuerza|Defensa|NivelMagia*/
    sprintf(aux, "#st:%ld|2|%ld|%ld|%ld|%d|%d|%d\n",id,mH,h,bD,str,def,magicLvl);
    fprintf(file, "%s", aux);

    return OK;
}