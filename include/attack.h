/**
 * @file attack.h
 * @author Sofía Calvo
 * @brief The attack module saves the and creates the different attack types that will be read from file
 * 
 * Every time a game starts, the attack struct is created and takes all the information
 * from attacks in the file.
 * 
 * @version 0.1
 * @date 2025-04-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ATTACK_H
#define ATTACK_H

#include "types.h"
#include <stdbool.h>

/**
 * @brief Struct with all the information related to an attack
 */
typedef struct _Attack Attack;

/**
 * @brief Creates an attack
 * @author Sofía Calvo
 *
 * @param name the name of the attack to create
 * @return Attack pointer if everything goes well or NULL if there was a mistake
 */
Attack *attack_create(char *name);

/**
 * @brief Frees the memory of the struct attack
 * @author Sofía Calvo
 * 
 * @param at attack struct to free
 */
void attack_free(Attack *at);


/*------SETTERS------*/

/**
 * @brief Sets the damage multiplicator of the attack
 * @author Sofía Calvo
 *
 * @param at pointer to the structure that contains the information of the attack
 * @param mult the multiplicator to be set in the structure
 * @return OK if it worked or ERROR if there was a mistake
 */
Status attack_set_damage_multiplicator(Attack *at, double mult);

/**
 * @brief Sets wether the attack needs a target or not
 * @author Sofía Calvo
 *
 * @param at pointer to the structure that contains the information of the attack
 * @param target_need the bool true of false
 * @return OK if it worked or ERROR if there was a mistake
 */
Status attack_set_target_bool(Attack *at, bool target_need);

/**
 * @brief Sets the probability in which the attack can fail
 * @author Sofía Calvo
 *
 * @param at pointer to the structure that contains the information of the attack
 * @param probability the chance of failure
 * @return OK if it worked or ERROR if there was a mistake
 */
Status attack_set_failure_chance(Attack *at, double probability);

/*-------GETTERS-------*/

/**
 * @brief Gets the damage multiplicator of the attack
 * @author Sofía Calvo
 *
 * @param at pointer to the structure that contains the information of the attack
 * @return the multiplicator if it worked or -1 if there was an error
 */
double attack_get_damage_multiplicator(Attack *at);

/**
 * @brief  gets wether the attack needs a target or not
 * @author Sofía Calvo
 *
 * @param at pointer to the structure that contains the information of the attack
 * @return true if it needs a target, false if it doesn't
 */
bool attack_get_target_bool(Attack *at);

/**
 * @brief Gets the probability in which the attack can fail
 * @author Sofía Calvo
 *
 * @param at pointer to the structure that contains the information of the attack
 * @return the chance of missing if there weren't errors, -1 if there was a mistake
 */
double attack_get_success_chance(Attack *at);

/**
 * @brief Gets the name of the attack
 * @author Sofía Calvo
 *
 * @param at pointer to the structure that contains the information of the attack
 * @return the name if there weren't errors, NULL if there was a mistake
 */
char *attack_get_name(Attack *at);

/**
 * @brief Compares two attacks
 * @author Sofía Calvo
 *
 * @param at pointer to the structure that contains the information of the attack
 * @return the strcmp of the names, 0 if there was a mistake
 */
int attack_compare(void *e1, void *e2);

/**
 * @brief Prints the name of an attack
 * @author Sofía Calvo
 *
 * @param at pointer to the structure that contains the information of the attack
 */
void attack_print(void *at);
#endif