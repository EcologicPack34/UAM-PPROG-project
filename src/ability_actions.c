/**
 * @file ability_actions.c
 * @author Maksym Polyak
 * @brief Determines game interactions with abilities
 * @version 0.1
 * @date 2025-03-25
 * 
 * @copyright Copyright (c) 2025
 * 
*/

#include "ability_actions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug_printing.h"

/*
  * Private functions
*/

/**
 * @brief Ability that heals the actual player by the amount determined by the ability data
 * @author Maksym Polyak
 * 
 * RIGHT NOW ABILITY ACTION DOES UPDATE ALL THE COOLDOWNS AND ABILITIES AT THE END OF THE GAME_LOOP_RUN ITERATION
 * THAT MEANS THAT WE HAVE TO CHANGE IT SO EVERY ENTITY CAN DECIDE THROUGH THE REAL STATS WHICH ACTION TO USE,
 * for example if an entity is dead by an ability but the ability has not been activated yet, and an entity tries
 * to heal it, when the entity tries to heal it is late
 * 
 * @param ability ability with the necessary data for the function to work
 * @param game game struct
 * @return Status 
 */
Status ability_heal_self(Ability *ability, Game *game);

/**
 * @brief Heals an ally if the entity is an ally of the player or is the player, if not,
 * the enemies in a combat heal another enemy
 * @author Maksym Polyak
 * 
 * @param ability ability with the necessary data for the function to work
 * @param game game struct
 * @return Status 
 */
Status ability_heal_ally(Ability *ability, Game *game);

/**
 * @brief Adds an amount of money to the active player
 * @author Maksym Polyak
 * 
 * @param ability ability with the necessary data for the function to work
 * @param game game struct
 * @return Status 
 */
Status ability_money_bag(Ability *ability, Game *game);

/**
 * @brief Unlocks a link related to the ability data
 *      Data of the ability stores the link which will be unlocked when using ability
 * @author Daniel Gómez
 * 
 * @param ability 
 * @param game 
 * @return Status 
 */
Status ability_unlock_link(Ability *ability, Game *game);

Status ability_heal_self(Ability *ability, Game *game){
    Combat *combat = NULL;
    Stats *stats = NULL;
    char *data = NULL;
    Player *player = NULL;
    Entity *entity = NULL;

    double health_recovered, health, max_health;
    
    if(!ability || !game) return ERROR;

    if(ability_get_cooldown_count(ability) != 0)
        return ERROR;

    data = ability_get_data(ability);
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

Status ability_heal_ally(Ability *ability, Game *game){
    Command *command = NULL;
    Combat *combat = NULL;
    Stats *stats = NULL;
    int n_arg, random, count;
    char *data = NULL;

    double health_recovered;
    
    if(!ability || !game) return ERROR;

    if(ability_get_cooldown_count(ability) != 0)
        return ERROR;

    data = ability_get_data(ability);
    if(!data) return ERROR;

    health_recovered = atof(data);

    if(ability_get_is_player_ability(ability) == true){
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

Status ability_money_bag(Ability *ability, Game *game){
    char *data = NULL;
    int value;
    
    if(!ability || !game) return ERROR;

    data = ability_get_data(ability);
    if(!data) return ERROR;

    sscanf(data, "%d", &value);

    return player_add_money(game_get_player(game), value);
}

Status ability_unlock_link(Ability *ability, Game *game){
    Link *link = NULL;
    Entity *player = NULL;
    Id linkId = NO_ID;

    if(!ability || !game) return ERROR;
    
    linkId = atoi(ability_get_data(ability));

    link = game_get_link_by_id(game, linkId);

    player = player_get_entity(game_get_player(game));

    if(!link || !player) return ERROR;

    return link_unlock(link, player);
}

/*
  * Public functions
*/

Status ability_actions_use_ability(Game *game){
    AbilityManager *sm = NULL;
    Ability *ability = NULL;
    Status status;

    Object *object = NULL;
    Entity *entity = NULL;

    int cd;
    
    if(!game) return ERROR;

    sm = game_get_ability_manager(game);

    /*Evaluates the ability received and tries to execute it*/
    status = ERROR; cd = -1;

    /*Gets the ability to use*/
    ability = ability_manager_get_evaluated_ability(sm);
    if(!ability) return ERROR;

    /*Checks cooldown to see if it is still on cd*/
    cd = ability_get_cooldown_count(ability);
    if(cd > 0)
        return ERROR;

    switch(ability_get_type(ability)){
        case NO_SKILL:
            break;
        case HEAL_SELF:
            status = ability_heal_self(ability, game);
            break;
        case HEAL_ALLY:
            status = ability_heal_ally(ability, game);
            break;
        case LINK_UNLOCK:
            status = ability_unlock_link(ability, game);
            break;
        default:
            break;
    }

    /*Removes the object from the inventory if its a consumable*/
    if(status == OK && ability_get_is_object_use(ability) == true){
        object = game_get_object_by_id(game, ability_get_entityid(ability));
        entity = player_get_entity(game_get_player_by_id(game, object_get_location(object)));

        if(object && entity){
            if(object_get_is_consumable(object) == true){
                inventory_remove_object(entity_get_inventory(entity), object);
            }
        }
    }

    /*Sets the cooldown to its max length and pushes it into the cooldown queue*/
    if(status == OK && (cd = ability_get_cooldown_length(ability)) > 0 ){
        ability_set_cooldown_to_length(ability);
        queue_push(ability_manager_get_queue(sm), (void *)ability);
    }

    return OK;
}

Status ability_actions_manage_cooldowns(Game *game){
    AbilityManager *sm = NULL;
    Queue *queue = NULL;
    Queue *auxqueue = NULL;
    Ability *ability = NULL;
    int cd;
    
    sm = game_get_ability_manager(game);

    queue = ability_manager_get_queue(sm);

    auxqueue = queue_create();
    if(!auxqueue) return ERROR;

    /*Updates the cooldowns in the queue*/
    while(queue_isEmpty(queue) == false){
        ability = (Ability *)queue_pop(queue);
    
        cd = ability_get_cooldown_count(ability);
        if(cd > 0)
            ability_reduce_cooldown(ability);

        if(cd > 1)
            queue_push(auxqueue, (void *)ability);
    }

    while(queue_isEmpty(auxqueue) == false){
        queue_push(queue, queue_pop(auxqueue));
    }

    queue_destroy(auxqueue);

    return OK;
}