/**
 * @file leveling.c
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-05-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "leveling.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct _Leveling{
    int XP;
    int next_XP;

    int level;
    int skill_points;
};

/*
    *Public functions
*/

Leveling *leveling_create(int XP, int next_XP, int level, int skill_points){
    Leveling *lv = NULL;

    lv = (Leveling *)malloc(sizeof(Leveling));
    if(!lv) return NULL;

    lv->level = level;
    lv->XP = XP;
    lv->next_XP = next_XP;
    lv->skill_points = skill_points;
    

    leveling_update_level_up(lv);
    
    return lv;
}

void leveling_destroy(Leveling *leveling){
    if(leveling) free(leveling);
}

int leveling_get_XP(Leveling *leveling){
    if(!leveling) return -1;

    return leveling->XP;
}

int leveling_get_next_XP(Leveling *leveling){
    if(!leveling) return -1;

    return leveling->next_XP;
}

int leveling_get_level(Leveling *leveling){
    if(!leveling) return -1;

    return leveling->level;
}

int leveling_get_skill_points(Leveling *leveling){
    if(!leveling) return -1;

    return leveling->skill_points;
}

Status leveling_set_XP(Leveling *leveling, int XP){
    if(!leveling || XP < 0) return ERROR;

    leveling->XP = XP;

    return OK;
}

Status leveling_set_next_XP(Leveling *leveling, int next_XP){
    if(!leveling || next_XP < 0) return ERROR;

    leveling->next_XP = next_XP;

    return OK;
}

Status leveling_set_level(Leveling *leveling, int level){
    if(!leveling || level < 0) return ERROR;

    leveling->level = level;

    return OK;
}

Status leveling_set_skill_points(Leveling *leveling, int skill_points){
    if(!leveling || skill_points < 0) return ERROR;

    leveling->skill_points = skill_points;

    return OK;
}

bool leveling_check_level_up(Leveling *leveling){
    if(leveling){
        if(leveling->XP >= leveling->next_XP)
            return true;
    }

    return false;
}

Status leveling_update_level_up(Leveling *leveling){
    if(!leveling) return ERROR;

    while(leveling_check_level_up(leveling) != false){
        leveling_set_level(leveling, leveling->level + 1);
        leveling_set_XP(leveling, leveling->XP - leveling->next_XP);
        leveling_set_next_XP(leveling, BASE_XP_QUANTITY*((pow(leveling->level, LEVELING_DIFFICULTY_MULTIPLIER))/(leveling->level)));
        leveling_set_skill_points(leveling, leveling->skill_points + SKILL_POINTS_PER_LEVEL);
    }

    return OK;
}