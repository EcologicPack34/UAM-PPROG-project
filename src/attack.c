/**
 * @file attack.c
 * @author Sofía Calvo
 * @brief Determines attacks that the game uses
 * @version 0.1
 * @date 2025-04-11
 * 
 * @copyright Copyright (c) 2025
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include <stdbool.h>
#include "attack.h"
#include <string.h>

/**
 * @brief Struct of the attack module to save the data of the attack
 */
struct _Attack{

    char name[WORD_SIZE];
    double damage_multiplication;
    bool needs_target;
    double no_missing_chance;
};

Attack *attack_create(char *name) {

    Attack *at = NULL;

    if (!name)
        return NULL;
    
    at = (Attack*) malloc(sizeof(Attack));
    if (!at)
        return NULL;

    strcpy(at->name, name);
    return at;
}

void attack_free(Attack *at) {

    free(at);
}


/*------SETTERS------*/


Status attack_set_damage_multiplicator(Attack *at, double mult) {

    if (!at || mult <= 0)
        return ERROR;
    
    at->damage_multiplication = mult;
    return OK;
}

Status attack_set_target_bool(Attack *at, bool target_need) {

    if (!at)
        return ERROR;
    
    at->needs_target = target_need;
    return OK;
}

Status attack_set_failure_chance(Attack *at, double probability) {

    if (!at || probability <= 0)
        return ERROR;
    
    at->no_missing_chance = probability;
    return OK;
}

/*-------GETTERS-------*/

double attack_get_damage_multiplicator(Attack *at) {

    if (!at)
        return -1;
    
    return at->damage_multiplication;
}

bool attack_get_target_bool(Attack *at) {

    if (!at)
        return -1;
    
    return at->needs_target;
}

double attack_get_success_chance(Attack *at) {

    if (!at)
        return -1;
    
    return at->no_missing_chance;
}

char *attack_get_name(Attack *at) {

    if (!at)
        return NULL;

    return at->name;
}


