/**
 * @file dialogue.h
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-04-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "npc.h"
#include "command.h"
#include "utils.h"

#define DOUTPUTS_NUM 4          /*!< Number of dialogue_outputs implemented*/

/**
 * @brief Dialogue outputs enum
 * 
 */
typedef enum{NO_OUTPUT, DIALOGUE_STOP, FIGHT, STORE}Dialogue_Outputs;

/**
 * @brief Dialogue ADT definition
 * 
 */
typedef struct _Dialogue Dialogue;

/**
 * @brief Creates a dialogue to initialize a dialogue
 * @author Maksym Polyak
 * 
 * @param npc npc who has the dialogue
 * @param fIN file of the dialogues on the start of file
 * @return Dialogue* or NULL if error
 */
Dialogue *dialogue_create(NPC *npc, FILE *fIN);

/**
 * @brief Frees a dialogue struct
 * @author Maksym Polyak
 * 
 * @param dialogue 
 */
void dialogue_destroy(Dialogue *dialogue);

/**
 * @brief Updates dialogues to the next states and controls outcomes
 * @author Maksym Polyak
 * 
 * @param dialogue dialogue struct
 * @return Status 
 */
Status dialogue_update(Dialogue *dialogue);

/**
 * @brief Controls the outcomes of the dialogue and updates dialogue state
 * @author Maksym Polyak
 * 
 * @param last_cmd last command from player
 * @param dialogue dialogue struct
 * @param next_dialogue_state has to be created, has the direction of the next_dialogue_state to save on entity
 * @return Status
 */
Dialogue_Outputs dialogue_outcomes(Command *last_cmd, Dialogue *dialogue, int *next_dialogue_state);
#endif