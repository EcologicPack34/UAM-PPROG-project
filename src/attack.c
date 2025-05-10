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
#include "utils.h"

/**
 * @brief Struct of the attack module to save the data of the attack
 */
struct _Attack{
    char name[WORD_SIZE];           /*!< Name of the attack*/
    double damage_multiplication;   /*!< Damage multiplicator of the attack*/
    bool needs_target;              /*!< Bool to check if attack needs a target or not*/
    double no_missing_chance;       /*!< Chance to succesfully make an attack*/
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

int attack_compare(void *e1, void *e2){

    if (!e1 || !e2)
        return 0;
    
    return strcmp(((Attack*)e1)->name, ((Attack*)e2)->name);
}

void attack_print(void *at) {

    Attack *att = NULL;

    if (!at)
        return;
        
    att = (Attack*)at;
    printf("Name: %s | Damage multiplication: %lf || Needs target: %d || Success chance: %lf", att->name, att->damage_multiplication, att->needs_target, att->no_missing_chance);
}

void attack_destroy(void *at) {

    if (!at)
        return;
    
    free((Attack*)at);
}


int attack_save_on_file(Attack *attack, FILE *fOUT){
    int count = 0;

    if(!attack || !fOUT) return ERROR;
    
    count += fprintf(fOUT, "%lf;%d;%lf\n", attack->damage_multiplication, attack->needs_target, attack->no_missing_chance);
    count += fprintf(fOUT, "%s\n", attack->name);

    return count;
}

Attack *attack_create_from_file(FILE *fIN){
    Attack *attack = NULL;
    char name[WORD_SIZE] = "";
    double dammult, nomisschance;
    int needs_target;

    if(!fIN) return NULL;
    
    fscanf(fIN,"%lf;%d;%lf\n", &dammult, &needs_target, &nomisschance);
    fgets(name,WORD_SIZE,fIN);
    string_remove_newline_escape_sequence_on_end(name);

    attack = attack_create(name);
    if(!attack) return NULL;

    attack->damage_multiplication = dammult;
    attack->needs_target = needs_target;
    attack->no_missing_chance = nomisschance;

    return attack;
}