/**
 * @file combat.c
 * @author Maksym Polyak
 * @brief This module implements all the functionality from combat.h
 * @version 0.1
 * @date 2025-03-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "combat.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LVL 999999
/*Damage multipliers for each attack*/
#define LIGHT_ATTACK 1
#define STRONG_ATTACK 1.5
#define QUICK_ATTACK 0.8
#define SWIFT_ATTACK 0.8

#define MAX_ATTACKS 4

/*Probabilites for succcess on an attack*/
#define LIGHT_PROB 90
#define STRONG_PROB 70
#define QUICK_PROB 40
#define SWIFT_PROB 40

/**
 * @brief Struct that holds all the information related to the combat
 * 
 */

struct _Combat{
    Stats allies_stats[NPC_MAX_ALLIES];     /*!< Stats of the allies*/
    int allies_count;                       /*!< Number of allies*/
    Stats enemies_stats[NPC_MAX_ENEMIES];   /*!< Stats of the enemies*/
    int enemies_count;                      /*!< Number of enemies*/

    bool is_player_turn;                    /*!< Determines who starts, true --> player party, false --> enemy party*/
    Space *space;                           /*!< Space where the combat is located*/

    bool endCombat;
};


/*
    * PRIVATE FUNCTIONS
*/

/**
 * @brief Copies the entity stats to a new stats struct for the combat
 * @author Maksym Polyak
 * 
 * @param entity 
 * @param stats
 * @return Status
 */
Status combat_copy_entity_stats(Entity *entity, Stats *stats);

/**
 * @brief Saves the health of each NPC and player in the combat and sets the combat
 * bool endCombat to true
 * @author Daniel Gómez
 * 
 * @param combat 
 */
void combat_finalize(Combat *combat);

/**
 * @brief Applies the received damage to the npc
 * @author Maksym Polyak
 * 
 * @param stats 
 * @param damage 
 * @return Status 
 */
Status combat_entity_received_damage(Stats *stats, double damage);

Status combat_copy_entity_stats(Entity *entity, Stats *stats){

    if(!entity || !stats)    return ERROR;

    stats->stats.health = entity_get_health(entity);
    stats->stats.baseDamage = entity_get_baseDamage(entity);
    stats->stats.strength = entity_get_strength(entity);
    stats->stats.defense = entity_get_defense(entity);
    stats->stats.magicLevel = entity_get_magicLevel(entity);

    stats->entity = entity;

    return OK;
}

void combat_finalize(Combat *combat){
    int i;
    combat->endCombat = true;

    for (i = 0; i < combat->allies_count; i++)
    {
        entity_set_health(combat->allies_stats[i].entity ,combat->allies_stats[i].stats.health);
    }
    for (i = 0; i < combat->enemies_count; i++)
    {
        entity_set_health(combat->enemies_stats[i].entity ,combat->enemies_stats[i].stats.health);
    }

}

Status combat_entity_received_damage(Stats *stats, double damage){
    if(!stats) return ERROR;

    stats->stats.health -= damage;

    return OK;
}

/*
    * PUBLIC FUNCTIONS
*/

Combat *combat_initialize(Space *space, Player *player, CommandCode code){
    Combat *combat = NULL;
    NPC *npc = NULL;
    int i, npc_count = 0, npc_allies = 1, npc_enemies = 0;

    if(!space || !player)
        return NULL;

    
    combat = (Combat *)calloc(1,sizeof(Combat));
    if(!combat) return NULL;
    /*Saves the player stats on the combat struct*/
    combat_copy_entity_stats(player_get_entity(player), &(combat->allies_stats[0]));
    
    npc_count = space_get_npc_count(space);

    /*Saves the first four enemies and the first three allies on the combat struct*/
    for(i = 0; i < npc_count; i++){
        npc = space_get_NPC_at(space, i);
        if(npc_get_status(npc) == ENEMY && npc_enemies <= 4){
            if(entity_get_health(npc_get_entity(npc)) <= 0) continue;

            combat_copy_entity_stats(npc_get_entity(npc), &(combat->enemies_stats[npc_enemies]));
            npc_enemies++;
        }
        if(npc_get_status(npc) == ALLY && npc_allies <= 3){
            if(entity_get_health(npc_get_entity(npc)) <= 0) continue;

            combat_copy_entity_stats(npc_get_entity(npc), &(combat->allies_stats[npc_allies]));
            npc_allies++;
        }
    }
    
    combat->enemies_count = npc_enemies;
    combat->allies_count = npc_allies;
    
    if(combat->enemies_count == 0){
        free(combat);
        return NULL;
    }
    
    /*Initializes turns and the space where the combat is located*/
    
    if(code == ATTACK){
        combat->is_player_turn = true;
    } else {
        combat->is_player_turn = false;
    }
    
    combat->space = space;
    combat->endCombat = false;
    
    return combat;
}

void combat_free(Combat *combat){
    if(!combat)
        return;

    free(combat);
}

bool combat_get_isFinished(Combat *combat){
    if(!combat) return false;
    return combat->endCombat;
}

Status combat_attack_swift(Stats *attacker, Stats *victims, int numVictims) {
    int probFail, i;
    
    if (!attacker || !victims)
        return ERROR;
    probFail = rand()%100;
    if (probFail > SWIFT_PROB)
        return OK;
    
    for (i = 0; i < numVictims; i++)
    {
        combat_entity_received_damage(&victims[i], SWIFT_ATTACK*(attacker->stats.baseDamage));
    }
    
    return OK; 
}
Status combat_attack_quick(Stats *attacker, Stats *victim){

    int  probFail;

    if (!attacker || !victim)
        return ERROR;
    probFail = rand()%100;
    if (probFail > QUICK_PROB)
        return OK;
    
    combat_entity_received_damage(victim, QUICK_ATTACK*(attacker->stats.baseDamage));
    return OK; 
}

Status combat_attack_strong(Stats *attacker, Stats *victim) {

    int probFail;

    if (!attacker || !victim)
        return ERROR;
    probFail = rand()%100;
    if (probFail > STRONG_PROB)
        return OK;
    
    combat_entity_received_damage(victim, STRONG_ATTACK*(attacker->stats.baseDamage));
    return OK;    
}

Status combat_attack_light(Stats *attacker, Stats *victim) {

    int probFail;

    if (!attacker || !victim)
        return ERROR;
    probFail = rand()%100;
    if (probFail > LIGHT_PROB)
        return OK;
    
    combat_entity_received_damage(victim, LIGHT_ATTACK*(attacker->stats.baseDamage));
    return OK;
}

Status combat_enemies_turn(Combat *cmb) {

    Stats *stAl = NULL, *stEn = NULL;
    int numEn, numAl, i, randomNumAttack, randomNumAll;

    if (!cmb)
        return ERROR;

    numEn = combat_get_enemies_count(cmb);
    numAl = combat_get_allies_count(cmb);

    if(numAl == 0){

    }

    stAl = combat_get_allies_stats(cmb);
    stEn = combat_get_enemies_stats(cmb);

    for (i = 0; i < numEn; i++)
    {
        randomNumAll = rand()%numAl;
        randomNumAttack = rand()%MAX_ATTACKS + 1;

        switch (randomNumAttack)
        {
        case 1:
            combat_attack_light(&stEn[i], &stAl[randomNumAll]);
            break;
        case 2:
            combat_attack_strong(&stEn[i], &stAl[randomNumAll]);
            break;
        case 3:
            combat_attack_quick(&stEn[i], &stAl[randomNumAll]);
            break;
        case 4:
            combat_attack_swift(&stEn[i], stAl, numAl);
            break;
        default:
            break;
        }
    }
    return OK;
}

Status combat_allies_turn(Combat *cmb){

    Stats *st = NULL, *stAl = NULL;
    int numA, numE, i, randomNumAttack, randomNumEnemy;

    if (!cmb)
        return ERROR;
    
    numA = combat_get_allies_count(cmb);
    st = combat_get_enemies_stats(cmb);
    stAl = combat_get_allies_stats(cmb);
    numE = combat_get_enemies_count(cmb);

    for (i = 1; i < numA; i++)
    {
        randomNumEnemy = rand()%numE;
        randomNumAttack = rand()%MAX_ATTACKS + 1;

        switch (randomNumAttack)
        {
        case 1:
            combat_attack_light(&stAl[i], &st[randomNumEnemy]);
            break;
        case 2:
            combat_attack_strong(&stAl[i], &st[randomNumEnemy]);
            break;
        case 3:
            combat_attack_quick(&stAl[i], &st[randomNumEnemy]);
            break;
        case 4:
            combat_attack_swift(&stAl[i], st, numE);
            break;

        default:
            break;
        }
    } 
    return OK;
}

Status combat_update_attack(Combat *cmb, Command *last_cmd){
    int numEnemy, enemycount;
    char **args = NULL;
    Stats *stEn = NULL;

    if (!cmb || !last_cmd)
        return ERROR;
        
    args = command_get_arguments(last_cmd);
    numEnemy = atoi(args[1]);
    stEn = combat_get_enemies_stats(cmb);
    enemycount = combat_get_enemies_count(cmb);

    if(cmb->is_player_turn == false){
        combat_enemies_turn(cmb);
        combat_allies_turn(cmb);
    }

    if (strcmp(args[0], "light") == 0)
    {
        combat_attack_light(&cmb->allies_stats[0], &stEn[numEnemy - 1]);
    }
    else if (strcmp(args[0], "strong") == 0)
    {
        combat_attack_strong(&cmb->allies_stats[0], &stEn[numEnemy - 1]);
    }
    else if (strcmp(args[0], "quick") == 0)
    {
        combat_attack_quick(&cmb->allies_stats[0], &stEn[numEnemy - 1]);
    }
    else if (strcmp(args[0], "swift") == 0)
    {
        combat_attack_swift(&cmb->allies_stats[0], stEn, enemycount);
    }

    if (cmb->is_player_turn == true)
    {
        combat_allies_turn(cmb);
        combat_enemies_turn(cmb);
    }

    
    return OK;
}

//comando: nombre del ataque, y el número de enemigo, para los aliados es aleatorio y para los enemigos también, del 1 al 4, para los aliados 1 al 3 (guardado en la constante)
//si el número de enemigos no es adecuado, return ERROR

Status combat_update(Combat *combat, Command *last_cmd){
    
    if(!combat || !last_cmd)
        return ERROR;

    if(command_get_code(last_cmd) == GM){
        /*stats copy are modified for combat*/
        return entity_stats_set_all(&combat->allies_stats[0].stats, MAX_LVL, MAX_LVL, MAX_LVL, MAX_LVL, MAX_LVL, MAX_LVL);
    }

    if(command_get_code(last_cmd) == ATTACK) {

        if(combat_update_attack(combat, last_cmd) == ERROR) return ERROR;
    }

    if (command_get_code(last_cmd) == RUN_AWAY)
    {
        if (combat_runaway(combat) == ERROR) return ERROR;
        
    }
    
    /*
        if(lastcmd == ATTACK) status = combat_update_attack();
        si devuelve error nos salimos con un return error
        if(lastcmd = ABIlyTY) status = cojmbat_update_ability();

    */
    

    /*random = rand() % (100 - 0 + 1);

    if(random > 60){
        for (int i = 0; i < combat->enemies_count; i++)
        {
            combat->enemies_stats[i].stats.health -= 1;
            if(combat->enemies_stats[i].stats.health <= 0){
                entity_set_health(combat->enemies_stats[i].entity, 0);
                deadEnemies++;
            }
        } 
    }
    else{
        combat->player_stats.stats.health -= 1*(combat->enemies_count - deadEnemies);
    }

    if(combat->player_stats.stats.health <= 0){
        combat_finalize(combat);
    }
    if(deadEnemies == combat->enemies_count){
        combat_finalize(combat);
    }
    return OK;*/

    return OK;
}

Status combat_runaway(Combat *combat){
    if(!combat) return ERROR;
    combat_finalize(combat);
    return OK;
}

int combat_get_enemies_count(Combat *combat){
    if(!combat) return 0;
    return combat->enemies_count;
}

int combat_get_allies_count(Combat *combat){
    if(!combat) return 0;
    return combat->allies_count;
}

Stats *combat_get_enemies_stats_at(Combat *combat, int index){
    Stats *stats = NULL;
    
    if(!combat) return NULL;

    stats = &(combat->enemies_stats[index]);
    return stats;
}

Stats *combat_get_allies_stats_at(Combat *combat, int index){
    Stats *stats = NULL;
    
    if(!combat) return NULL;

    stats = &(combat->allies_stats[index]);
    return stats;
}

Stats *combat_get_enemies_stats(Combat *combat){
    if(!combat) return NULL;

    return combat->enemies_stats;
}

Stats *combat_get_allies_stats(Combat *combat){
    if(!combat) return NULL;

    return combat->allies_stats;
}

Stats *combat_get_player_stats(Combat *combat){
    if(!combat) return NULL;
    return &(combat->allies_stats[0]);
}