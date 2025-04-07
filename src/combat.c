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

#define MAX_LVL 999999      /*!< Maximum level the stats should take*/

/*Damage multipliers for each attack*/
#define LIGHT_ATTACK 1      /*!< Light attack damage multiplier*/
#define HEAVY_ATTACK 1.5    /*!< heavy attack damage multiplier*/
#define QUICK_ATTACK 0.8    /*!< quick attack damage multiplier*/
#define SWIFT_ATTACK 0.8    /*!< swift attack damage multiplier*/

#define MAX_ATTACKS 4       /*!< Number of attack types that exist*/

/*Probabilites for succcess on an attack*/
#define LIGHT_PROB 90       /*!< Probability for a light attack to be succesful*/
#define HEAVY_PROB 50       /*!< Probability for a heavy attack to be succesful*/
#define QUICK_PROB 98       /*!< Probability for a quick attack to be succesful*/
#define SWIFT_PROB 40       /*!< Probability for a swift attack to be succesful*/

/**
 * @brief Internal struct that holds all the information related to the combat
 */
struct _Combat{
    Stats allies_stats[NPC_MAX_ALLIES];     /*!< Stats of the allies*/
    int allies_count;                       /*!< Number of allies*/
    Stats enemies_stats[NPC_MAX_ENEMIES];   /*!< Stats of the enemies*/
    int enemies_count;                      /*!< Number of enemies*/

    Stats dead_entities[COMBAT_MAX_ENTITIES - 1];   /*!< Stores the dead entities*/
    int n_dead_entities;                            /*!< Count of how many dead entities are*/

    bool is_player_turn;                    /*!< Determines who starts, true --> player party, false --> enemy party*/
    Space *space;                           /*!< Space where the combat is located*/

    bool endCombat;                         /*!< if true means combat has ended*/
};


/*
    * PRIVATE FUNCTIONS
*/

/**
 * @brief Copies the entity stats to a new stats struct for the combat
 * @author Maksym Polyak
 * 
 * @param entity entity where the stats are copied
 * @param stats stats where the stats are saved
 * @return Status
 */
Status combat_copy_entity_stats(Entity *entity, Stats *stats);

/**
 * @brief Saves the health of each NPC and player in the combat and sets the combat
 * bool endCombat to true
 * @author Daniel Gómez
 * 
 * @param combat combat struct
 */
void combat_finalize(Combat *combat);

/**
 * @brief Sub function of combat_update that manages player attack type and allies/enemies turn
 * @author Sofía Calvo && Daniel Gómez
 * 
 * @param cmb combat struct
 * @param last_cmd last command of the player on the combat
 * @return Status 
 */
Status combat_update_player_attack(Combat *cmb, Command *last_cmd);

/**
 * @brief Applies the received damage to the npc
 * @author Maksym Polyak
 * 
 * @param stats  stats of the entity that received damage
 * @param damage value of damage received
 * @return Status 
 */
Status combat_entity_received_damage(Stats *stats, double damage);

/**
 * @brief Tries to make a swift(area) attack from the attacker to the victims
 * @author Sofía Calvo
 * 
 * @param attacker stats of the attacker
 * @param victims stats of victims
 * @param numVictims number of victims
 * @return Status 
 */
Status combat_attack_swift(Stats *attacker, Stats *victims, int numVictims);

/**
 * @brief Tries to make a quick attack from the attacker to the victim
 * @author Sofía Calvo
 * 
 * @param attacker stats of the attacker
 * @param victim stats of the victim
 * @return Status 
 */
Status combat_attack_quick(Stats *attacker, Stats *victim);

/**
 * @brief Tries to make a strong attack from the attacker to the victim
 * @author Sofía Calvo
 * 
 * @param attacker stats of the attacker
 * @param victim stats of the victim
 * @return Status 
 */
Status combat_attack_strong(Stats *attacker, Stats *victim);

/**
 * @brief Tries to make a light attack from the attacker to the victim
 * @author Sofía Calvo
 * 
 * @param attacker stats of the attacker
 * @param victim stats of the victim
 * @return Status 
 */
Status combat_attack_light(Stats *attacker, Stats *victim);

/**
 * @brief Manages enemies turn with random attacks to random allies
 * @author Sofía Calvo
 * 
 * @param cmb combat struct
 * @return Status 
 */
Status combat_enemies_turn(Combat *cmb);

/**
 * @brief Manages allies turn with random attacks to random enemies
 * @author Sofía Calvo
 * 
 * @param cmb combat struct
 * @return Status 
 */
Status combat_allies_turn(Combat *cmb);

/**
 * @brief Updates the deaths of enemies and the player
 * @author Sofía Calvo
 * 
 * @param cmb combat struct
 * @return Status 
 */
Status combat_update_deaths(Combat *cmb);

/**
 * @brief Copies stats from one stats struct to another, sets as dead the stat_from struct
 * @author Maksym Polyak
 * 
 * @param stat_from from where they are taken
 * @param stat_to where they are written
 * @return Status 
 */
Status combat_copy_stats(Stats *stat_from, Stats *stat_to);

/**
 * @brief Returns the last entity Stats *, if mode == 0 searches for allies,
 * if mode == 1 searches for enemies
 * 
 * @param cmb combat struct
 * @param mode operation mode
 * @return Stats* or NULL if error or not found
 */
Stats *combat_get_last_alive(Combat *cmb, int mode);

/*-----------IMPLENTATIONS-------------*/

Status combat_copy_entity_stats(Entity *entity, Stats *stats){

    if(!entity || !stats)    return ERROR;

    stats->stats.maxhealth = entity_get_max_health(entity);
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

    for (i = 0; i < combat->n_dead_entities; i++)
    {
        entity_set_health(combat->dead_entities[i].entity ,combat->dead_entities[i].stats.health);
    }
}

Status combat_entity_received_damage(Stats *stats, double damage){
    if(!stats) return ERROR;

    if(!entity_stats_is_dead(&(stats->stats))){
        stats->stats.health -= damage;
    }

    return OK;
}

Status combat_attack_swift(Stats *attacker, Stats *victims, int numVictims) {
    int probFail, i;
    Status status = OK;
    
    if (!attacker || !victims){
        return ERROR;
    }

    probFail = rand()%100;

    if (probFail > SWIFT_PROB){
        return OK;
    }
    
    for (i = 0; i < numVictims; i++)
    {
        status = combat_entity_received_damage(&victims[i], SWIFT_ATTACK*(attacker->stats.baseDamage));
    }
    
    return status;
}

Status combat_attack_quick(Stats *attacker, Stats *victim){

    int  probFail;

    if (!attacker || !victim){
        return ERROR;
    }
    probFail = rand()%100;

    if (probFail > QUICK_PROB){
        return OK;
    }
    
    return combat_entity_received_damage(victim, QUICK_ATTACK*(attacker->stats.baseDamage));
}

Status combat_attack_strong(Stats *attacker, Stats *victim) {

    int probFail;

    if (!attacker || !victim){
        return ERROR;
    }
    probFail = rand()%100;

    if (probFail > HEAVY_PROB){
        return OK;
    }
    
    return combat_entity_received_damage(victim, HEAVY_ATTACK*(attacker->stats.baseDamage));
}

Status combat_attack_light(Stats *attacker, Stats *victim){

    int probFail;

    if (!attacker || !victim){
        return ERROR;
    }
    probFail = rand()%100;

    if (probFail > LIGHT_PROB){
        return OK;
    }
    
    return combat_entity_received_damage(victim, LIGHT_ATTACK*(attacker->stats.baseDamage));
}

Status combat_enemies_turn(Combat *cmb) {

    Stats *stAl = NULL, *stEn = NULL;
    int numEn, numAl;
    int i;
    int randomNumAttack, randomNumAll;

    if (!cmb){
        return ERROR;
    }

    numEn = combat_get_enemies_count(cmb);
    numAl = combat_get_allies_count(cmb);

    stAl = combat_get_allies_stats(cmb);
    stEn = combat_get_enemies_stats(cmb);

    if(numEn <= 0 || numAl <= 0) return OK;

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

        combat_update_deaths(cmb);
    }
    return OK;
}

Status combat_allies_turn(Combat *cmb){

    Stats *stEn = NULL, *stAl = NULL;
    int numAl, numEn, i, randomNumAttack, randomNumEnemy;

    if (!cmb)
        return ERROR;
    
    numAl = combat_get_allies_count(cmb);
    numEn = combat_get_enemies_count(cmb);
    stEn = combat_get_enemies_stats(cmb);
    stAl = combat_get_allies_stats(cmb);

    if(numEn <= 0 || numAl <= 0) return OK;

    for (i = 1; i < numAl; i++)
    {
        randomNumEnemy = rand()%numEn;
        randomNumAttack = rand()%MAX_ATTACKS + 1;

        switch (randomNumAttack)
        {
        case 1:
            combat_attack_light(&stAl[i], &stEn[randomNumEnemy]);
            break;
        case 2:
            combat_attack_strong(&stAl[i], &stEn[randomNumEnemy]);
            break;
        case 3:
            combat_attack_quick(&stAl[i], &stEn[randomNumEnemy]);
            break;
        case 4:
            combat_attack_swift(&stAl[i], stEn, numEn);
            break;

        default:
            break;
        }

        combat_update_deaths(cmb);
    } 
    return OK;
}

Status combat_update_player_attack(Combat *cmb, Command *last_cmd){
    int numEnemy, enemycount;
    char **args = NULL;
    Stats *stEn = NULL;
    
    if (!cmb || !last_cmd)
        return ERROR;
        
    args = command_get_arguments(last_cmd);

    stEn = combat_get_enemies_stats(cmb);
    enemycount = combat_get_enemies_count(cmb);
    
    /*Checks if only one argument, and the posible combat options for it*/
    if(command_get_arguments_count(last_cmd) == 1){
        if(strcmp(args[0], "swift") == 0)
        {
            return combat_attack_swift(&cmb->allies_stats[0], stEn, enemycount);
        }
    }

    /*Checks the combat options which need to specify a target*/

    
    /*Checks if number of arguments is 2: format= attackName targetNum*/
    if(command_get_arguments_count(last_cmd) != 2) return ERROR;
    numEnemy = atoi(args[1]);


    if(numEnemy < 1 || numEnemy > enemycount) return ERROR;

    if(strcmp(args[0], "light") == 0)
    {
        return combat_attack_light(&cmb->allies_stats[0], &stEn[numEnemy - 1]);
    }
    else if(strcmp(args[0], "strong") == 0)
    {
        return combat_attack_strong(&cmb->allies_stats[0], &stEn[numEnemy - 1]);
    }
    else if(strcmp(args[0], "quick") == 0)
    {
        return combat_attack_quick(&cmb->allies_stats[0], &stEn[numEnemy - 1]);
    }
    /*Caso como si fuera un else, es decir, todos los ifs han fallado*/
    return ERROR;
}


Status combat_copy_stats(Stats *stat_from, Stats *stat_to){
    if(!stat_from || !stat_to) return ERROR;

    stat_to->entity = stat_from->entity;

    stat_to->stats.baseDamage = stat_from->stats.baseDamage;
    stat_to->stats.defense = stat_from->stats.defense;
    stat_to->stats.health = stat_from->stats.health;
    stat_to->stats.maxhealth = stat_from->stats.maxhealth;
    stat_to->stats.magicLevel = stat_from->stats.magicLevel;
    stat_to->stats.strength = stat_from->stats.strength;

    stat_from->stats.health = 0;

    return OK;
}

Status combat_update_deaths(Combat *cmb){

    /*Check if all the enemies are dead to end the combat*/
    int i;
    Stats *last_stats = NULL;

    if (!cmb)
        return ERROR;

    for (i = 0; i < cmb->enemies_count; i++)
    {
        if (entity_stats_is_dead(&(cmb->enemies_stats[i].stats)))
        {
            /*Copies enemie into dead entities array*/
            combat_copy_stats(&(cmb->enemies_stats[i]), &(cmb->dead_entities[(cmb->n_dead_entities)++]));

            /*Checks if it's not the last in the array to avoid exceptions*/
            if(i > (cmb->enemies_count - 1)){
                continue;
            }
            last_stats = combat_get_last_alive(cmb, 1);
            if (last_stats){
                combat_copy_stats(last_stats, &(cmb->enemies_stats[i]));
            }

            cmb->enemies_count--;
        }
    }

    for(i = 1; i < cmb->allies_count; i++)
    {
        if (entity_stats_is_dead(&(cmb->allies_stats[i].stats)))
        {
            combat_copy_stats(&(cmb->allies_stats[i]), &(cmb->dead_entities[(cmb->n_dead_entities)++]));
            if(i > (cmb->allies_count - 1)){
                continue;
            }

            last_stats = combat_get_last_alive(cmb, 0);
            if (last_stats){
                combat_copy_stats(last_stats, &(cmb->allies_stats[i]));
            }

            cmb->allies_count--;
        }
    }

    if (entity_stats_is_dead(&(cmb->allies_stats[0].stats)))
    {
        combat_finalize(cmb);
        return OK;
    }

    if(cmb->enemies_count == 0)
    {
        combat_finalize(cmb);
        return OK;
    }

    return OK;
}

Stats *combat_get_last_alive(Combat *cmb, int mode){
    int i;
    if(!cmb) return NULL;

    if(mode == 0){
        for(i = NPC_MAX_ALLIES - 1; i >= 1; i--){
            if(cmb->allies_stats[i].stats.health > 0){
                return &(cmb->allies_stats[i]);
            }
        }
    }
    else if(mode == 1){
        for(i = NPC_MAX_ENEMIES - 1; i >= 0; i--){
            if(cmb->enemies_stats[i].stats.health > 0){
                return &(cmb->enemies_stats[i]);
            }
        }
    }

    return NULL;
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
        if(npc_get_status(npc) == ENEMY && npc_enemies < 4){
            if(entity_get_health(npc_get_entity(npc)) <= 0){
                continue;
            } 

            combat_copy_entity_stats(npc_get_entity(npc), &(combat->enemies_stats[npc_enemies]));
            npc_enemies++;
        }
        if(npc_get_status(npc) == ALLY && npc_allies < 3){
            if(entity_get_health(npc_get_entity(npc)) <= 0){
                continue;
            } 

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

    combat->n_dead_entities = 0;
    
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

Status combat_update(Combat *combat, Command *last_cmd){
    Status st = OK;

    if(!combat || !last_cmd){
        return ERROR;
    }

    if(combat->endCombat){
        return OK;
    }
    
    st = combat_update_player_attack(combat, last_cmd);
    if(st == ERROR){
        return ERROR;
    }

    combat_update_deaths(combat);

    if(combat->is_player_turn == false){
        combat_enemies_turn(combat);
        combat_allies_turn(combat);
    }

    if(combat->is_player_turn == true){
        combat_allies_turn(combat);
        combat_enemies_turn(combat);
    }

    
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