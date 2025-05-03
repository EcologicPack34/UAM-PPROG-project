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

#define N_EFFECT_TYPES 4 /*!< Number of effect types implemented*/

#include "types.h"
#include "entity.h"
#include "combat.h"
#include <stdio.h>

typedef enum{UNKNOWN_EFFECT, REGENERATION, POISON, FIRE}EffectType; /*!< The type of effect*/

typedef enum{NO_EFFECT=-1 ,AFFECTS_PLAYER, AFFECTS_ALLY, AFFECTS_ENEMY}EffectAffects; /*!< The type of entity affected by an effect in a combat*/

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
 * @param ET the EffectType
 * @param EA the EffectedAffects that determines if an effect affects a player, an ally or an enemy if in combat
 * @param inf_turns a bool to specify if the effect is applied for an infinite amount of turns (if true) or a finite amount (if false)
 * @param default_turns an int describing the amount of turns an effect is applied for (if infinite, this int will be ignored)
 * @return pointer to Effect or NULL if error
 */
Effect *effect_create(Id id, char *name, char *data, EffectType ET, EffectAffects EA,bool inf_turns, int default_turns);

/**
 * @brief This function creates an effect manager
 * @author Aaron Charameli Mair
 * 
 * @return EffectManager* or NULL if error 
 */
EffectManager *effect_manager_create();

/**
 * @brief This function frees all the memory of an effect manager
 * @author Aaron Charameli Mair 
 * 
 * @note this function also free's the effects contained in the effect manager
 */
void effect_manager_destroy(EffectManager *em);

/**
 * @brief This function gets the effects collection of an effect manager
 * @author Aaron Charameli Mair
 * 
 * @param em a pointer to the effect manager
 * @return Collection* or NULL if error
 */
Collection *effect_manager_get_effects(EffectManager *em);

/**
 * @brief This function gets an effect by its if
 * @author Aaron Charameli Mair
 * 
 * @param em pointer to effect manager
 * @param id the id of the effect
 * @return Effect* or NULL if not found or ERROR
 */
Effect *effect_get_by_id(EffectManager *em, Id id);

/**
 * @brief This function frees all the memory of an effect
 * @author Aaron Charameli Mair
 * 
 * @param e a pointer to the effect
 */
void effect_destroy(void *e);

/**
 * @brief This function adds an effect to an effect manager
 * @author Aaron Charameli Mair
 * 
 * @param em the effect manager
 * @param effect the effect
 * @return Status 
 */
Status effect_manager_add_effect(EffectManager *em, Effect *effect);

/**
 * @brief This funtion adds an effect to an entity
 * @author Aaron Charameli Mair
 * 
 * @param e a pointer to the effect
 * @param ent a pointer to the entity/affected
 * @return Status 
 */
Status effect_add_affected(Effect *e, Entity *ent);

/**
 * @brief This function checks if an entity is affected by an effect
 * @author Aaron Charameli Mair
 * 
 * @param e a pointer to the effect
 * @param ent a pointer to the entity
 * @return true 
 * @return false 
 */
bool effect_has_affected(Effect *e, Entity *ent);

/**
 * @brief This function updates the affected entities by an effect.
 * 
 * @param effect a pointer to the effect
 * @param ent_stats an array of entity Stats, needed if in combat. Otherwise, leave as NULL
 * @param ent_count an int describing the number of elements in the Stats array
 * @return Status 
 * @note If in combat, a pointer to the entities Stats will be required otherwise, NULL can be passed as argument
 */
Status effect_update(Effect*effect, Stats* ent_stats, int ent_count);

/**
 * @brief This function writes in a text file the data of affected entities from an effect
 * @author Aaron Charameli Mair
 * 
 * @param effect a pointer to the effect
 * @param filename the file name where the data will be written
 * @return Status 
 */
Status effect_write_affected_save_data(Effect *effect, char *filename);

/**
 * @brief This function gets an effect as a string
 * @author Aaron Charameli Mair
 * 
 * @param effect the effect to get in a string format
 * @param destiny a string where the effect will be stored
 * @return Status 
 */
Status effect_get_as_str(Effect *effect, long destiny_size ,char *destiny);

/**
 * @brief This function gets the type of effect an effect is
 * @author Aaron Charameli Mair
 * 
 * @param effect a pointer to the effect
 * @return EffectType or -2 if error
 */
EffectType effect_get_effect_type(Effect *effect);

/**
 * @brief This function gets the name of an effect
 * @author Aaron Charameli Mair
 * 
 * @param effect 
 * @return char* or NULL if error
 */
char *effect_get_name(Effect *effect);

/**
 * @brief This function gets the data string of an effect
 * @author Aaron Charameli Mair
 * 
 * @param effect 
 * @return char* or NULL if error
 */
char *effect_get_data(Effect *effect);

/**
 * @brief This function gets the id of an effect
 * @author Aaron Charameli Mair
 * 
 * @param effect 
 * @return Id or NO_ID if error
 */
Id effect_get_id(Effect *effect);

/**
 * @brief This function gets the type EffectedAffects from an effect
 * @author Aaron Charameli Mair
 * 
 * @param effect 
 * @return the corresponding EffectAffects or NO_EFFECT if ERROR
 */
EffectAffects effect_get_effectAffects(Effect *effect);

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