/**
 * @brief It defines the effects module interface
 *
 * 
 *
 * @file effect_manager.h
 * @author Aaron Charameli Mair
 * @version 0
 * @date 10-04-2025
 * @copyright GNU Public License
 */

#ifndef EFFECT_H
#define EFFECT_H

#define N_EFFECTS 3 /*!< Number of effects implemented*/

#include "types.h"
#include <stdio.h>

typedef enum{ENTITY_EFFECT, SPACE_EFFECT}EffectIn; /*!< says if the effect applies to one single entity or all entities located in a space*/

typedef enum{UNKNOWN_EFFECT, REGENERATION, POISON}EffectType; /*!< The type of effect*/

typedef struct _Effect Effect;

typedef struct _EffectsManager EffectManager;

/**
 * @brief This function creates an effect and stores information related to it
 * @author Aaron Charameli Mair
 * 
 * @param id the id of the effect
 * @param name a string containing the effect's name
 * @param data a string containing the effect's information 
 * @param Eloc the EffectIn which says if the effect applies to an entity or a space 
 * @param ET the EffectType
 * @return pointer to Effect or NULL if error
 */
Effect *effect_create(Id id, char *name, char *data, EffectIn Eloc, EffectType ET);

/**
 * @brief This function frees all the memory of an effect
 * 
 * @param e a pointer to the effect
 */
void effect_destroy(Effect *e);

/**
 * @brief This function updates the affected entities by an effect.
 * 
 * @param effect a pointer to the effect
 * @return Status 
 */
Status effect_update(Effect *effect);

int effect_cmp(const Effect*e1, const Effect*e2);

void effect_print(FILE *pf, const Effect*effect);





#endif