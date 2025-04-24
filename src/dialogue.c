/**
 * @file dialogue.c
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-04-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "dialogue.h"
#include <stdio.h>
#include <stdlib.h>


struct _Dialogue{
    char *actual_text;
    int dialogue_state;
    NPC *npc;

    char **player_replies;
    int player_replies_num;

    FILE *dialogue_file;
};

Dialogue *dialogue_create(Game *game, NPC *npc, FILE *fIN){
    Dialogue *dialogue = NULL;
    
    if(!game || !npc) return NULL;

    dialogue = (Dialogue *)calloc(1,sizeof(Dialogue));
    if(!dialogue) return NULL;

    dialogue->dialogue_file = fIN;
    dialogue->npc = npc;
    dialogue->dialogue_state = npc_get_dialogue_state(npc);

    /*It means the dialogue has not been found */
    if(dialogue_update(game, dialogue) == ERROR){
        free(dialogue);
        return NULL;
    }

    return dialogue;
}

void dialogue_destroy(Dialogue *dialogue){
    int i;
    
    if(dialogue){
        free(dialogue->actual_text);
        for(i = 0; i < dialogue->player_replies_num; i++){
            free(dialogue->player_replies[i]);
        }
        free(dialogue->player_replies);

        fclose(dialogue->dialogue_file);
    }
}
/*
Dialogue read example NOTE: DIALOGUE_OUTCOME is 0 if not outcome
---------

ID:{NPCID} //Has to be of the type: ID:1
DS:{DIALOGUE_STATE}
AT:{NUMBER_OF_LINES}
{1-ACTUAL_TEXT}
{....}
NR:{NUMBER_OF_REPLIES}
{PLAYER REPLY}:{DIALOGUE_OUTCOME}:{DIALOGUE_STATE} 
{PLAYER REPLY}:{DIALOGUE_OUTCOME}:{DIALOGUE_STATE} 
{PLAYER REPLY}:{DIALOGUE_OUTCOME}:{DIALOGUE_STATE} 
{...}

DS:{DIALOGUE_STATE}
AT:{NUMBER_OF_LINES}
{1-ACTUAL_TEXT}
{....}
NR:{NUMBER_OF_REPLIES}
{PLAYER REPLY}:{DIALOGUE_OUTCOME}:{DIALOGUE_STATE} 
{PLAYER REPLY}:{DIALOGUE_OUTCOME}:{DIALOGUE_STATE} 
{PLAYER REPLY}:{DIALOGUE_OUTCOME}:{DIALOGUE_STATE} 
{...}

DS:{DIALOGUE_STATE}
AT:{NUMBER_OF_LINES}
{1-ACTUAL_TEXT}
{....}
NR:{NUMBER_OF_REPLIES}
{PLAYER REPLY}:{DIALOGUE_OUTCOME}:{DIALOGUE_STATE} 
{PLAYER REPLY}:{DIALOGUE_OUTCOME}:{DIALOGUE_STATE} 
{PLAYER REPLY}:{DIALOGUE_OUTCOME}:{DIALOGUE_STATE} 
{...}

DS:{DIALOGUE_STATE} //Last one only has to have one reply and it sends itself to the next dialogue_state
AT:{NUMBER_OF_LINES}
{1-ACTUAL_TEXT}
{....}
NR:{NUMBER_OF_REPLIES}
{PLAYER REPLY}:{DIALOGUE_OUTCOME}:{DIALOGUE_STATE} 

---------

*/

Status dialogue_update(Game *game, Dialogue *dialogue){
    char str[WORD_SIZE] = "", aux_str[WORD_SIZE] = "";
    Id id, entityid;
    
    if(!game || !dialogue) return ERROR;

    rewind(dialogue->dialogue_file);

    //ADD ERROR CONTROL IF NOT FOUND
    entityid = entity_get_id(npc_get_entity(dialogue->npc));
    if(entityid < 0) return ERROR;

    fgets(str,WORD_SIZE, dialogue->dialogue_file);
    if(strncmp(str, "ID:", 3) == 0){
        sscanf(str,"ID:%ld", &id);
    }
    while((id - entityid) != 0){
        fgets(str,WORD_SIZE, dialogue->dialogue_file);
        if(strncmp(str, "ID:", 3) == 0){
            sscanf(str,"ID:%ld", &id);
        }
    }



    
}

Status dialogue_outcomes(Dialogue *dialogue, int index);