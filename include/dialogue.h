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
#include "game.h"

typedef enum{NO_OUTPUT, DIALOGUE_STOP, FIGHT, STORE}Dialogue_Outputs;

typedef struct _Dialogue Dialogue;

/**
 * @brief Creates a dialogue to initialize a dialogue
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @param npc npc who has the dialogue
 * @param fIN file of the dialogues on the start of file
 * @return Dialogue* or NULL if error
 */
Dialogue *dialogue_create(Game *game, NPC *npc, FILE *fIN);

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
 * @param game game struct
 * @param dialogue dialogue struct
 * @return Status 
 */
Status dialogue_update(Game *game, Dialogue *dialogue);

/**
 * @brief Controls the outcomes of the dialogue and destroys the dialogue if necessary
 * @author Maksym Polyak
 * 
 * @param dialogue dialogue struct
 * @param outcome outcome code
 * @return Status 
 */
Status dialogue_outcomes(Dialogue *dialogue, Dialogue_Outputs outcome);

#endif