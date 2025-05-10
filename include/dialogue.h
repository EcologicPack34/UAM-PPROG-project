/**
 * @file dialogue.h
 * @author Maksym Polyak
 * @brief ADT that controls dialogue outputs, if the game is not reading from
 * a save takes the dialogues from the DIALOGUE_INIT_PATH on game reader.
 * If reading from a save, then enters the save file and gets the dialogues.
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

#define DOUTPUTS_NUM 6          /*!< Number of dialogue_outputs implemented*/

/**
 * @brief Dialogue outputs types enum
 * 
 */
typedef enum{NO_OUTPUT, DIALOGUE_STOP, FIGHT, STORE, FOLLOW, UNFOLLOW}Dialogue_Outputs;

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
 * @param dialogue dialogue struct
 */
void dialogue_destroy(Dialogue *dialogue);

/**
 * @brief Gets the NPC of a dialogue
 * 
 * @param dialogue dialogue struct
 * @return NPC* or NULL if error
 */
NPC *dialogue_get_NPC(Dialogue *dialogue);

/**
 * @brief Gets the text of an NPC in the actual dialogue state
 * @author Maksym Polyak
 * 
 * @param dialogue dialogue struct
 * @return char* 
 */
char *dialogue_get_npc_text(Dialogue *dialogue);

/**
 * @brief Gets the player possible replies
 * @author Maksym Polyak
 * 
 * @param dialogue dialogue struct
 * @return char** or NULL if error, at least one has to exist
 */
char **dialogue_get_player_replies(Dialogue *dialogue);

/**
 * @brief Gets the number of player replies available
 * @author Maksym Polyak
 * 
 * @param dialogue dialogue struct
 * @return int or -1 if error
 */
int dialogue_get_num_player_replies(Dialogue *dialogue);

/**
 * @brief Returns the dialogue output code for a reply placed at i
 * 
 * @param dialogue dialogue struct
 * @param i index
 * @return Dialogue_Outputs or NO_OUTPUT if error
 */
Dialogue_Outputs dialogue_get_output_at(Dialogue *dialogue, int i);

/**
 * @brief Returns a static string(No need to free) depending on the output code
 * received
 * @author Maksym Polyak
 * 
 * @param output dialogue struct
 * @return char* or NULL if not found or error
 */
char *dialogue_get_outcome_as_string(Dialogue_Outputs output);

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
 * @return Status
 */
Dialogue_Outputs dialogue_outcomes(Command *last_cmd, Dialogue *dialogue);
#endif