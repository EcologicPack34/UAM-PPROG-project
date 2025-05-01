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
#include "utils.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

char doutput_to_str[DOUTPUTS_NUM][LINE_LENGTH] = {"Nothing...", "Stop Dialogue", "Fight!", "Access Store", "Follow", "Unfollow"};

/**
 * @brief Dialogue ADT implementation
 * 
 */
struct _Dialogue{
    char actual_text[WORD_SIZE];        /*!< Text of the NPC*/
    int dialogue_state;                 /*!< Actual state of dialogue*/
    NPC *npc;                           /*!< NPC with who the dialogue is ocurring*/

    char **player_replies;              /*!< Possible replies of the player*/
    Dialogue_Outputs *outcomes;         /*!< Outcomes of each reply*/
    int *next_dialogue_states;          /*!< Next dialogue_state for each reply*/
    int player_replies_num;             /*!< Number of replies*/

    FILE *dialogue_file;                /*!< FILE stream where the dialogue is located*/
};

/**
 * @brief Frees memory from player replies
 * 
 * @param dialogue 
 */
void dialogue_strings_destroy(Dialogue *dialogue);

Dialogue *dialogue_create(NPC *npc, FILE *fIN){
    Dialogue *dialogue = NULL;
    
    if(!npc) return NULL;

    dialogue = (Dialogue *)calloc(1,sizeof(Dialogue));
    if(!dialogue) return NULL;

    dialogue->dialogue_file = fIN;
    dialogue->npc = npc;
    dialogue->dialogue_state = npc_get_dialogue_state(npc);

    /*It means the dialogue has not been found */
    if(dialogue_update(dialogue) == ERROR){
        free(dialogue);
        return NULL;
    }

    return dialogue;
}

void dialogue_destroy(Dialogue *dialogue){ 
    if(dialogue){

        dialogue_strings_destroy(dialogue);

        fclose(dialogue->dialogue_file);

        free(dialogue);
    }
}

void dialogue_strings_destroy(Dialogue *dialogue){
    int i;
    
    if(dialogue){
        for(i = 0; i < dialogue->player_replies_num; i++){
            free(dialogue->player_replies[i]);
        }
        free(dialogue->player_replies);
        free(dialogue->outcomes);
        free(dialogue->next_dialogue_states);
        dialogue->player_replies_num = 0;
    }
}

NPC *dialogue_get_NPC(Dialogue *dialogue){
    if(!dialogue) return NULL;

    return dialogue->npc;
}

char *dialogue_get_npc_text(Dialogue *dialogue){
    if(!dialogue) return NULL;

    return dialogue->actual_text;
}

char **dialogue_get_player_replies(Dialogue *dialogue){
    if(!dialogue) return NULL;

    return dialogue->player_replies;
}

int dialogue_get_num_player_replies(Dialogue *dialogue){
    if(!dialogue) return -1;

    return dialogue->player_replies_num;
}

Dialogue_Outputs dialogue_get_output_at(Dialogue *dialogue, int i){
    if(!dialogue) return NO_OUTPUT;

    return dialogue->outcomes[i];
}

char *dialogue_get_outcome_as_string(Dialogue_Outputs output){
    
    if(output < 0 || (int)output >= DOUTPUTS_NUM) return NULL;

    return doutput_to_str[(int)output];
}

/*
Dialogue read example NOTE: DIALOGUE_OUTCOME is 0 if not outcome
---------
// Maximum number of lines in AT is 4 and each line can have up to 124 char
// Final dialogue state is the DS:0 and the initial DS is DS:1
// Maximum length of a line has to be 50

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

Status dialogue_update(Dialogue *dialogue){
    char str[WORD_SIZE] = "", aux_str[WORD_SIZE] = "", *dyn_str = NULL;
    char *toks = NULL;
    Id id, entityid;
    int i, state = -1, line_num;
    
    if(!dialogue) return ERROR;

    rewind(dialogue->dialogue_file);

    dialogue_strings_destroy(dialogue);

    entityid = entity_get_id(npc_get_entity(dialogue->npc));
    if(entityid < 0) return ERROR;

    fgets(str,WORD_SIZE, dialogue->dialogue_file);
    if(strncmp(str, "ID:", 3) == 0){
        sscanf(str,"ID:%ld", &id);
    }
    while((id != entityid) && (fgets(str,WORD_SIZE, dialogue->dialogue_file) != NULL)){
        if(strncmp(str, "ID:", 3) == 0){
            sscanf(str,"ID:%ld", &id);
        }
    }

    while((dialogue->dialogue_state != state) && (fgets(str,WORD_SIZE, dialogue->dialogue_file))){
        if(strncmp(str, "DS:", 3) == 0){
            sscanf(str,"DS:%d", &state);
        }
    }

    if(fgets(str,WORD_SIZE, dialogue->dialogue_file) == NULL) return ERROR;
    sscanf(str, "%d", &line_num);

    strcpy(aux_str, "");
    for(i = 0; i < line_num && i < LINE_MAX; i++){
        fgets(str, LINE_LENGTH, dialogue->dialogue_file);
        string_remove_endofline_escape_sequence_on_end_to_newline(str);
        strcat(aux_str, str);
    }
    
    strcpy(dialogue->actual_text, aux_str);

    fgets(str,WORD_SIZE, dialogue->dialogue_file);
    sscanf(str, "%d", &line_num);
    if(line_num < 0 || line_num > LINE_MAX) return ERROR;

    dialogue->player_replies = (char **)calloc(line_num, sizeof(char *));
    if(!dialogue->player_replies) return ERROR;
    dialogue->outcomes = (Dialogue_Outputs *)calloc(line_num, sizeof(Dialogue_Outputs));
    if(!dialogue->outcomes) return ERROR;
    dialogue->next_dialogue_states = (int *)calloc(line_num, sizeof(int));
    if(!dialogue->next_dialogue_states) return ERROR;
    dialogue->player_replies_num = 0;

    for(i = 0; i < line_num; i++){
        fgets(str, WORD_SIZE, dialogue->dialogue_file);
        string_remove_endofline_escape_sequence_on_end_to_newline(str);
        toks = strtok(str, ":");
        if(toks == NULL) return ERROR;
        dyn_str = (char *)malloc((LINE_LENGTH + 1)*sizeof(char));
        if(!dyn_str) return ERROR;
        strcpy(dyn_str, toks);
        dialogue->player_replies[dialogue->player_replies_num] = dyn_str;

        toks = strtok(NULL,":");
        dialogue->outcomes[dialogue->player_replies_num] = (Dialogue_Outputs)atoi(toks);
        toks = strtok(NULL,"\0");
        dialogue->next_dialogue_states[dialogue->player_replies_num] = atoi(toks);
        
        (dialogue->player_replies_num)++;
    }

    return OK;
}

Dialogue_Outputs dialogue_outcomes(Command *last_cmd, Dialogue *dialogue){
    char **args = NULL;
    int n_arg, reply_num;
    Dialogue_Outputs out;

    if(!last_cmd || !dialogue) return NO_OUTPUT;

    n_arg = command_get_arguments_count(last_cmd);

    if(n_arg != 1) return NO_OUTPUT;

    args = command_get_arguments(last_cmd);
    if(!args) return NO_OUTPUT;

    reply_num = atoi(args[0]) - 1;

    if(reply_num < 0 || reply_num >= dialogue->player_replies_num) return NO_OUTPUT;

    out = dialogue->outcomes[reply_num];
    npc_set_dialogue_state(dialogue->npc, dialogue->next_dialogue_states[reply_num]);
    if(out < NO_OUTPUT || (int)out >= DOUTPUTS_NUM) return NO_OUTPUT;
    dialogue->dialogue_state = dialogue->next_dialogue_states[reply_num];
    if(dialogue->dialogue_state < 0){
        dialogue->dialogue_state = 0;
        return NO_OUTPUT;
    }
    
    return out;
}