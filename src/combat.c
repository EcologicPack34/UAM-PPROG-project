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
#include "attack.h"
#include <stdio.h>
#include <stdlib.h>
#include "message.h"

#define MAX_LVL 999999      /*!< Maximum level the stats should take*/

/*Damage multipliers for each attack*/
#define LIGHT_ATTACK 1      /*!< Light attack damage multiplier*/
#define HEAVY_ATTACK 1.5    /*!< heavy attack damage multiplier*/
#define QUICK_ATTACK 0.8    /*!< quick attack damage multiplier*/
#define SWIFT_ATTACK 0.8    /*!< swift attack damage multiplier*/

#define MAX_ATTACKS 64       /*!< Number of attack types that exist*/

/*Probabilites for succcess on an attack*/
#define LIGHT_PROB 90       /*!< Probability for a light attack to be succesful*/
#define HEAVY_PROB 50       /*!< Probability for a heavy attack to be succesful*/
#define QUICK_PROB 98       /*!< Probability for a quick attack to be succesful*/
#define SWIFT_PROB 40       /*!< Probability for a swift attack to be succesful*/
#define RUN_AWAY_PROB 80    /*!< Probability to run away without failing in combat*/

/**
 * @brief Internal struct that holds all the information related to the combat
 */
struct _Combat{
    Stats allies_stats[NPC_MAX_ALLIES];     /*!< Stats of the allies*/
    int allies_count;                       /*!< Number of allies*/
    int total_allies;                       /*!< Total number of allies*/
    int players_count;                      /*!< Number of players*/
    int players_turn;                       /*!<Turn of the players*/
    Stats enemies_stats[NPC_MAX_ENEMIES];   /*!< Stats of the enemies*/
    int enemies_count;                      /*!< Number of enemies*/

    Stats dead_entities[COMBAT_MAX_ENTITIES];   /*!< Stores the dead entities*/
    int n_dead_entities;                        /*!< Count of how many dead entities are*/

    bool is_player_turn;                    /*!< Determines who starts, true --> player party, false --> enemy party*/
    Space *space;                           /*!< Space where the combat is located*/

    bool endCombat;                         /*!< True means combat has ended*/

    Queue *messages;                        /*!< Saves the messages of the actions on the queue*/

    Collection *attacks;                    /*!< array of attacks*/
    int attacks_count;                      /*!< number of attacks*/
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
 * @brief Tries to make a generic attack
 * @author Sofía Calvo
 * 
 * @param cmb combat struct
 * @param at attack struct
 * @param attacker stats of the attacker
 * @param victim stats of the victim
 * @return Status 
 */
Status combat_attack(Combat *cmb, Attack *at, Stats *attacker, Stats *victim);

/**
 * @brief Tries to make a generic attack
 * @author Sofía Calvo
 * 
 * @param cmb combat struct
 * @param at attack struct
 * @param attacker stats of the attacker
 * @param victims stats of all the victims (array)
 * @param n_victims number of victims
 * @return Status 
 */
Status combat_attack_all(Combat *cmb, Attack *at, Stats *attacker, Stats *victims, int n_victims);

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
 * @author Daniel Gómez
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

/**
 * @brief Copy the collection of attacks in game into combat
 * 
 * @param combat combat struct
 * @param colc collection struct
 * @return OK if it was succesful or ERROR if there was an error
 */
Status combat_copy_attacks_into_array(Combat *combat, Collection *colc);

/**
 * @brief Releases the objects of the dead entity's inventory
 * 
 * @param cmb combat struct
 * @param st stats struct
 * @return OK if it was succesful or ERROR if there was an error
 */
Status combat_release_dead_loot(Combat *cmb, Stats *st);

/**
 * @brief Gets the damage multiplier relative to the strength stat,
 * if strength is greater than MAX_STRENGTH it doesnt have an effect
 * 
 * @param stats entity stats struct
 * @return double 
 */
double stats_get_strength_dmg_multiplier(EntityStats *stats);

/**
 * @brief Gets the multiplier with the reduced damage incoming, if defense
 * is greather than MAX_DEFENSE it does not have any effect
 * 
 * @param stats entity stats struct
 * @return double 
 */
double stats_get_defense_reduced_damage_multiplier(EntityStats *stats);

/**
 * @brief Adds a message to the log
 * @author Maksym Polyak
 * 
 * @param combat combat struct
 * @param level level of the message
 * @param content content of the message
 * @return Status 
 */
Status combat_add_message_log(Combat *combat, MessageType level, char *content);

/*-----------IMPLENTATIONS-------------*/

Status combat_add_message_log(Combat *combat, MessageType level, char *content){
    Message *mess = NULL;
    
    if(!combat || level < 0 || !content) return ERROR;

    mess = message_new(level, content);
    if(!mess) return ERROR;

    return queue_push(combat->messages, (void *)mess);
}

double stats_get_strength_dmg_multiplier(EntityStats *stats){
    int strength;
    
    if(!stats) return 0;

    if(stats->strength >= MAX_STRENGTH)
        strength = MAX_STRENGTH;
    else
        strength = stats->strength;

    return (strength - 1)*STRENGTH_DAMAGE_MULTIPLIER/100 + 1;
}

double stats_get_defense_reduced_damage_multiplier(EntityStats *stats){
    int defense;
    
    if(!stats) return 0;

    if(stats->defense >= MAX_DEFENSE)
    defense = MAX_DEFENSE;
    else
    defense = stats->defense;

    return 1 - (defense)*DEFENSE_REDUCED_DAMAGE_PERCENTAGE/100;
}

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



    for (i = 0; i < combat->total_allies; i++)
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

Attack *combat_find_attack_by_name(Combat *cmb, char *name) {

    int i;
    int boolean = 1;
    Attack *at = NULL;

    if (!cmb || !name)
        return NULL;
    
    for (i = 0; (i < cmb->attacks_count) && (boolean == 1); i++)
    {
        at = (Attack *)collection_get_element_at(cmb->attacks, i);
        if (strcmp(attack_get_name(at), name) == 0)
        {
            boolean = 0;
        }
    }

    if (boolean == 1)
        return NULL;
    
    return at;
}

Status combat_entity_received_damage(Stats *stats, double damage){
    
    if(!stats) return ERROR;
    
    if(!entity_stats_is_dead(&(stats->stats))){
        stats->stats.health -= damage*stats_get_defense_reduced_damage_multiplier(&(stats->stats));
    }

    return OK;
}

Status combat_attack(Combat * cmb, Attack *at, Stats *attacker, Stats *victim) {

    double damage;
    double chance;
    char str[WORD_SIZE] = "";

    if (!attacker || !victim || !at)
        return ERROR;
    
    chance = rand()%100;
    if (chance > attack_get_success_chance(at)){
        sprintf(str,"%s failed to attack %s!", entity_get_name(attacker->entity), entity_get_name(victim->entity));
        combat_add_message_log(cmb, MESSAGE_LOG, str);
        return OK;
    }
    
    damage = attacker->stats.baseDamage*attack_get_damage_multiplicator(at)*stats_get_strength_dmg_multiplier(&(attacker->stats));
    combat_entity_received_damage(victim, damage);
    
    sprintf(str,"%s dealt %.2lf of damage to %s!", entity_get_name(attacker->entity), damage, entity_get_name(victim->entity));
    combat_add_message_log(cmb, MESSAGE_LOG, str);

    return OK;
}

Status combat_attack_all(Combat *cmb, Attack *at, Stats *attacker, Stats *victims, int n_victims) {

    int i;
    double damage, chance;
    char str[WORD_SIZE] = "";

    if (!cmb || !at || !attacker || !victims)
        return ERROR;

    chance = rand()%100;
    if (chance > attack_get_success_chance(at)){
        sprintf(str,"%s failed to attack everyone!", entity_get_name(attacker->entity));
        combat_add_message_log(cmb, MESSAGE_LOG, str);
        return OK;
    }
    
    damage = attacker->stats.baseDamage*attack_get_damage_multiplicator(at);

    for (i = 0; i < n_victims; i++)
    {
        combat_entity_received_damage(&victims[i], damage);
    }

    sprintf(str,"%s dealt everyone %.2lf of damage!", entity_get_name(attacker->entity), damage);
    combat_add_message_log(cmb, MESSAGE_LOG, str);
    
    return OK;
}

Status combat_enemies_turn(Combat *cmb) {

    Stats *stAl = NULL, *stEn = NULL;
    Attack *at = NULL;
    int numEn, numAl;
    int i;
    int randomNumAttack, randomNumAll;
    bool needs_target;

    if (!cmb){
        return ERROR;
    }

    numEn = combat_get_enemies_count(cmb);
    numAl = cmb->total_allies;

    stAl = combat_get_allies_stats(cmb);
    stEn = combat_get_enemies_stats(cmb);

    if(numEn <= 0 || numAl <= 0) return OK;

    for (i = 0; i < numEn; i++)
    {
        randomNumAll = rand()%numAl;
        randomNumAttack = rand()%cmb->attacks_count;

        at = (Attack*) collection_get_element_at(cmb->attacks, randomNumAttack);
        needs_target = attack_get_target_bool(at);
        if (needs_target == true)
        {
            combat_attack(cmb, at, &stEn[i], &stAl[randomNumAll]);
        }
        else
        {
            combat_attack_all(cmb, at, &stEn[i], stAl, cmb->allies_count);
        }  
        combat_update_deaths(cmb);
    }
    
    return OK;
}

Status combat_allies_turn(Combat *cmb){

    Stats *stEn = NULL, *stAl = NULL;
    Attack *at = NULL;
    int numAl, numEn, i, randomNumAttack, randomNumEnemy;
    bool needs_target;

    if (!cmb)
        return ERROR;
    
    numAl = cmb->allies_count;
    numEn = combat_get_enemies_count(cmb);
    stEn = combat_get_enemies_stats(cmb);
    stAl = combat_get_allies_stats(cmb);

    if(numEn <= 0 || numAl <= 0) return OK;

    for (i = cmb->players_count; i < numAl + cmb->players_count; i++)
    {
        randomNumEnemy = rand()%numEn;
        randomNumAttack = rand()%cmb->attacks_count;

        at = (Attack*) collection_get_element_at(cmb->attacks, randomNumAttack);
        needs_target = attack_get_target_bool(at);
        if (needs_target == true)
        {
            combat_attack(cmb, at, &stAl[i], &stEn[randomNumEnemy]);
        }
        else
        {
            combat_attack_all(cmb, at, &stAl[i], stEn, cmb->enemies_count);
        } 
        combat_update_deaths(cmb);
    }
    
    return OK;
}

Status combat_update_player_attack(Combat *cmb, Command *last_cmd){
    int numEnemy, n_args;
    char **args = NULL;
    Stats *stEn = NULL, *player = NULL;
    Attack *atc = NULL;
    CommandCode cmd;
    
    if (!cmb || !last_cmd)
        return ERROR;
        
    args = command_get_arguments(last_cmd);

    cmd = command_get_code(last_cmd);
    if(cmd != ATTACK && cmd != ABILITY && cmd != OBJECT_USE && cmd != RUN_AWAY) return ERROR;

    n_args = command_get_arguments_count(last_cmd);
    if(n_args == 0 && cmd != RUN_AWAY) return ERROR;

    if(cmd != ATTACK) return OK; 

    player = &(cmb->allies_stats[cmb->players_turn]);
    atc = combat_find_attack_by_name(cmb, args[0]);
    if(atc == NULL) return ERROR;

    if (attack_get_target_bool(atc) == true)
    {
        numEnemy = atoi(args[1]) - 1;
        if(numEnemy < 0 || numEnemy >= combat_get_enemies_count(cmb)) return ERROR;

        stEn = combat_get_enemies_stats_at(cmb, numEnemy);
        combat_attack(cmb, atc, player, stEn);
    }
    else
    {
        stEn = combat_get_enemies_stats(cmb);
        combat_attack_all(cmb, atc, player, stEn, cmb->enemies_count);
    }
    return OK;
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

    return OK;
}

Status combat_update_deaths(Combat *cmb){

    /*Check if all the enemies are dead to end the combat*/
    int i, j;

    if (!cmb){
        return ERROR;
    }

    for (i = 0; i < cmb->total_allies; i++)
    {
        if(!entity_stats_is_dead(&(cmb->allies_stats[i].stats))){
            continue;
        }
        combat_release_dead_loot(cmb, &(cmb->allies_stats[i]));
        combat_copy_stats(&(cmb->allies_stats[i]), &(cmb->dead_entities[(cmb->n_dead_entities)++]));
        for (j = i; j < cmb->total_allies - 1; j++)
        {
            combat_copy_stats(&(cmb->allies_stats[j + 1]), &(cmb->allies_stats[j]));
        }

        cmb->total_allies--;
        if(entity_get_entityType((cmb->allies_stats[i].entity)) == PLAYER_TYPE){
            cmb->players_count--;
            if(cmb->players_count > 0){
                cmb->players_turn %= cmb->players_count;
            }
        }else{
            cmb->allies_count--;
        }
    }
    
    for (i = 0; i < cmb->enemies_count; i++)
    {
        if(!entity_stats_is_dead(&(cmb->enemies_stats[i].stats))){
            continue;
        }
        combat_release_dead_loot(cmb, &(cmb->enemies_stats[i]));
        combat_copy_stats(&(cmb->enemies_stats[i]), &(cmb->dead_entities[(cmb->n_dead_entities)++]));
        for (j = i; j < cmb->enemies_count - 1; j++)
        {
            combat_copy_stats(&(cmb->enemies_stats[j + 1]), &(cmb->enemies_stats[j]));
        }
        cmb->enemies_count--;
    }

    if (cmb->players_count == 0)
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
        for(i = NPC_MAX_ALLIES - 1; i >= 0; i--){
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

Combat *combat_initialize(Space *space, Player *pl, CommandCode code, Collection *attacks){
    Combat *combat = NULL;
    NPC *npc = NULL;
    int i, npc_count = 0, npc_allies, npc_enemies = 0, followerNum;
    Entity *ent = NULL;

    if(!space || !pl)
        return NULL;
    
    combat = (Combat *)calloc(1,sizeof(Combat));
    if(!combat) return NULL;
    /*Saves the player stats on the combat struct*/

    combat_copy_entity_stats(player_get_entity(pl), &(combat->allies_stats[0]));
    combat->players_count = 1;

    npc_count = space_get_npc_count(space);
    followerNum = player_get_follower_num(pl);
    
    for (i = 0; i < followerNum; i++)
    {
        if(entity_get_entityType(player_get_follower_at(pl, i)) == PLAYER_TYPE){
            combat_copy_entity_stats(player_get_follower_at(pl,i), &(combat->allies_stats[combat->players_count]));
            combat->players_count++;
        }
    }

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
    }

    npc_allies = 0;
    for (i = 0; i < followerNum && npc_allies + combat->players_count <= NPC_MAX_ALLIES; i++)
    {
        ent  = player_get_follower_at(pl, i);

        if (entity_get_entityType(ent) == NPC_TYPE)
        {
            combat_copy_entity_stats(ent, &(combat->allies_stats[combat->players_count + npc_allies]));
            npc_allies++;
        }
    }
    
    combat->enemies_count = npc_enemies;
    combat->total_allies = npc_allies + combat->players_count;
    combat->allies_count = npc_allies;
    if(combat->allies_count < 0) combat->allies_count = 0;
    combat->players_turn = 0;

    if(combat->enemies_count == 0){
        free(combat);
        return NULL;
    }

    combat->messages = queue_create();
    if(!combat->messages){
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

    /*Will now copy the data from the collection into combat*/

    combat->attacks = attacks;
    combat->attacks_count = collection_length(attacks);
    //printf("%d", combat->allies_count);

    return combat;
}

void combat_free(Combat *combat){
    if(!combat)
        return;

    
    queue_free_elements(combat->messages, message_destroy);
    queue_destroy(combat->messages);
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
    
    if(combat->players_turn % combat->players_count != combat->players_count - 1){
        combat->players_turn++;
        combat->players_turn %= combat->players_count;    
        return OK;
    }
    combat->players_turn++;
    combat->players_turn %= combat->players_count;
    
    
    
    if(combat->is_player_turn == false){
        combat_enemies_turn(combat);
        combat_allies_turn(combat);
    }
    
    if(combat->is_player_turn == true){
        combat_allies_turn(combat);
        combat_enemies_turn(combat);
    }

    combat_update_deaths(combat);

    return OK;
}

Status combat_runaway(Combat *combat){
    
    int chance;
    
    if(!combat) return ERROR;

    chance = rand()%100;

    if (chance >= RUN_AWAY_PROB)
    {
        return ERROR;
    }
    combat_finalize(combat);
    return OK;
}

int combat_get_enemies_count(Combat *combat){
    if(!combat) return 0;
    return combat->enemies_count;
}

int combat_get_allies_count(Combat *combat){
    if(!combat) return 0;
    return combat->total_allies;
}

int combat_get_player_count(Combat *cmb){
    if(!cmb) return 0;
    return cmb->players_count;
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

Status combat_set_num_attacks(Combat *combat, int num) {

    if (!combat || num <= 0)
        return ERROR;
    
    combat->attacks_count = num;
    return OK;
}

Status combat_release_dead_loot(Combat *cmb, Stats *st) {

    Inventory *inv = NULL;
    Inventory *space_inventory = NULL;
    Id objID;
    int inventory_size;
    long i;
    Object *obj = NULL;

    if (!cmb || !st)
        return ERROR;
    
    if(!(inv = entity_get_inventory(st->entity))) return ERROR;
    if(!(space_inventory = space_get_inventory(cmb->space))) {
        return ERROR;
    }
    inventory_size = inventory_get_size(inv);

    for (i = 0; i < inventory_size; i++)
    {
        if(!(obj = inventory_get_object_at(inv, i))) {
            return ERROR;
        }
        objID = object_get_id(obj);
        inventory_move_object(inv, space_inventory, objID);
    }

    return OK; 
}

int combat_get_dead_entities_num(Combat *combat){

    if(!combat) return 0;

    return combat->n_dead_entities;
}

Entity *combat_get_dead_entity_at(Combat *combat, int i){

    if(!combat || i < 0 || i >= combat_get_dead_entities_num(combat)) return NULL;

    return combat->dead_entities[i].entity;
}

Queue *combat_get_messages(Combat *combat){
    if(!combat) return NULL;

    return combat->messages;
}

int combat_get_turn(Combat *cmb){
    if (!cmb)
        return -1;
        
    return cmb->players_turn;
}