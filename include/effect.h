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

/**
 * @brief ADT containing basic info about effect
 * 
 */
typedef struct _Effect Effect;

/**
 * @brief stores effects
 * 
 */
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
 * @brief This function creates an effect manager
 * @author Aaron Charameli Mair
 * 
 * @return EffectManager* or NULL if error 
 */
EffectManager *effect_manager_create();

/**
 * @brief This function frees al the memory of an effect manager
 * @author Aaron Charameli Mair 
 * 
 * @note this function doesn't free the effects contained in the effect manager
 */
void effect_manager_destroy(EffectManager *em);

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

/**
 * @brief This function gets an effect as a string
 * @author Aaron Charameli Mair
 * 
 * @param effect the effect to get in a string format
 * @param destiny a string where the effect will be stored
 * @return Status 
 */
Status effect_get_as_str(Effect *effect, char *destiny);

/**
 * @brief This function compares two effects
 * @author Aaron Charameli Mair
 * 
 * @param e1 effect 1
 * @param e2 effect 2
 * @return 1 if e1>e2, 0 if equal, -1 if e1<e2 or -2 if error 
 */
int effect_cmp(void*e1, void*e2);

/**
 * @brief This function prints an effect
 * @author Aaron Charameli Mair
 * 
 * @param effect 
 * @note the effect is printed in stdout
 */
void effect_print(void*effect);





#endif