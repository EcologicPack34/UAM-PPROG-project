/**
 * @file equipment.c
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-03-29
 * 
 * @copyright Copyright (c) 2025
 * 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipment.h"
#include "object.h"
#include "entity.h"


#define N_PIECES 8

char *equip_to_str[N_PIECES] = {{""},{"helmet"},{"chest"},{"arms"},{"leg_armor"},{"shoes"},{"two_handed"},{"one_handed"}};

char *stat_to_str[N_STATS] = {{""},{"max_health"},{"health"},{"base_Damage"},{"strength"},{"defense"},{"magic_Level"}};



struct _Equipment{
    Object *helmet;
    Object *arms;
    Object *chest;
    Object *leg_armor;
    Object *shoes;

    bool is_two_handed;
    Object *weapon_two_hands;
    Object *weapon1_one_hand;
    Object *weapon2_one_hand;
};

/*
    * PRIVATE FUNCTIONS
*/

/*
    * PUBLIC FUNCTIONS
*/

Equipment *equipment_create(){
    Equipment *equipment = NULL;

    equipment = (Equipment *)calloc(1, sizeof(Equipment));
    if(!equipment)
        return NULL;

    equipment->is_two_handed = 1;

    return equipment;
}

void equipment_destroy(Equipment *equipment){
    if(equipment)
        free(equipment);
}

EquipmentCode equipment_code_from_str(char *data){
    int i;
    
    if(!data) return EQUIPMENT_ERROR;

    for(i = 0; i < N_PIECES; i++){
        if(strcmp(equip_to_str[i], data) == 0)
            return i + EQUIPMENT_ERROR;
    }

    return EQUIPMENT_ERROR;
}

StatCode equipment_statcode_from_str(char *data){
    int i;
    
    if(!data) return NO_SKILL;

    for(i = 0; i < N_STATS; i++){
        if(strcmp(stat_to_str[i], data) == 0)
            return i + NO_STAT;
    }

    return NO_STAT;
}

Status equipment_add_piece(Equipment *equipment, Object *object){
    char *data = NULL;
    char line[WORD_SIZE];
    char *toks = NULL;
    EquipmentCode code = EQUIPMENT_ERROR;
    
    if(!equipment || !object) return ERROR;

    data = object_get_data(object);
    if(data == NULL)
        return ERROR;

    strcpy(line, data);

    toks = strtok(line, " ");
    if(strcmp(toks, "wearable") != 0)
        return ERROR;

    toks = strtok(NULL, " ");
    if(equipment_code_from_str(toks) == EQUIPMENT_ERROR)
        return ERROR;

    //FINISH 
    
    
}