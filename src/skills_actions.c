/**
 * @file skills_actions.c
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-03-25
 * 
 * @copyright Copyright (c) 2025
 * 
*/

#include "skills_actions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug_printing.h"

/*
  * Private functions
*/

/**
 * @brief Skill that heals the actual player by the amount determined by the skill data
 * @author Maksym Polyak
 * 
 * @param skill 
 * @param game 
 * @return Status 
 */
Status skill_heal_self(Skill *skill, Game *game);

/**
 * @brief Heals an ally if the entity is an ally of the player or is the player, if not,
 * the enemies in a combat heal another enemy
 * @author Maksym Polyak
 * 
 * @param skill 
 * @param game 
 * @return Status 
 */
Status skill_heal_ally(Skill *skill, Game *game);

Status hability_heal_self(Skill *skill, Game *game){
    Combat *combat = NULL;
    PlayerStats *stats = NULL;
    char *data = NULL;
    Player *player = NULL;
    Entity *entity = NULL;

    double health_recovered, health, max_health;
    
    if(!skill || !game) return ERROR;

    data = skill_get_data(skill);
    if(!data) return ERROR;

    health_recovered = atof(data);

    if(game_get_state(game) == COMBAT){
        combat = game_get_combat(game);
        stats = combat_get_player_stats(combat);

        if(stats->stats.health <= 0)
            return ERROR;

        if(stats->stats.maxhealth - stats->stats.health <= health_recovered){

            health_recovered = stats->stats.maxhealth - stats->stats.health;
            if(health_recovered < 0) health_recovered = 0;
        }
        stats->stats.health += health_recovered;
    }else{
        player = game_get_player(game);
        entity = player_get_entity(player);

        health = entity_get_health(entity);
        max_health = entity_get_max_health(entity);

        if(health <= 0)
            return ERROR;

        if(max_health - health <= health_recovered){
            health_recovered = max_health - health;
        }
        entity_set_health(entity, health + health_recovered);
        
    }

    return OK;
}

Status skill_heal_ally(Skill *skill, Game *game){
    Command *command = NULL;
    Combat *combat = NULL;
    Stats *stats = NULL;
    int n_arg, random, count;
    char *data = NULL;

    double health_recovered;
    
    if(!skill || !game) return ERROR;

    data = skill_get_data(skill);
    if(!data) return ERROR;

    health_recovered = atof(data);

    if(skill_get_is_player_skill(skill) == true){
        command = game_get_last_command(game);
        n_arg = command_get_arguments_count(command);
        if(n_arg < 1){
            return ERROR;
        }

        /*NON - IMPLEMENTED ALLIES*/
    }else{
        /*Searches for a random enemy alive and then heals them*/
        combat = game_get_combat(game);
        if(combat){
            count = 0;
            do{
                random = rand() % NPC_MAX_ENEMIES;
                count++;
                stats = combat_get_enemies_stats_at(combat, random);
            }while( stats->stats.health < 0 && count < NPC_MAX_ENEMIES);
            
            if(count < NPC_MAX_ENEMIES){
                stats->stats.health += health_recovered;
            }
        }
    }

    return OK;
}

/*
  * Public functions
*/

Status skill_action_use_skills(Game *game){
    SkillManager *sm = NULL;
    Queue *queue = NULL;
    Skill *skill = NULL;
    Status status;
    
    if(!game) return ERROR;

    sm = game_get_skill_manager(game);

    queue = skill_manager_get_queue(sm);

    while(queue_isEmpty(queue) == FALSE){
        skill = (Skill *)queue_pop(queue);
        status = OK;

        switch(skill_get_type(skill)){
            case NO_SKILL:
                break;
            case HEAL_SELF:
                status = hability_heal_self(skill, game);
                break;
            case HEAL_ALLY:
                status = skill_heal_ally(skill, game);
                break;
            default:
                break;
        }

        if(status == OK)
            skill_set_cooldown_to_length(skill);
    }

    return OK;
}