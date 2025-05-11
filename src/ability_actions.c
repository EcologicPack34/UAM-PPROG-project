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

/**
 * @brief Ability that applies an effect to an enemy. The effect applied is the one which id is in data string
 * @author Aaron Charameli Mair
 * 
 * @param ability ability struct
 * @param game game struct
 * @return Status 
 */
Status ability_effect_enemy(Ability *ability, Game *game);

/**
 * @brief Ability to apply an effect on the player that used it
 * @author Aaron Charameli Mair
 * 
 * @param ability ability struct
 * @param game game struct
 * @return Status 
 */
Status ability_effect_self(Ability *ability, Game *game);

/**
 * @brief Ability to apply an effect on an ally on combat
 * @author Aaron Charameli Mair
 * 
 * @param ability ability struct
 * @param game game struct
 * @return Status 
 */
Status ability_effect_ally(Ability *ability, Game *game);

/**
 * @brief Revives a plaer by name
 * @author Maksym Polyak
 * 
 * @param ability ability struct
 * @param game game struct
 * @return Status 
 */
Status ability_revive_player(Ability *ability, Game *game);

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
        stats->stats.health += health_recovered*((stats->stats.magicLevel - 1)/100 + 1);
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
        entity_set_health(entity, health + health_recovered*((entity_get_magicLevel(player_get_entity(player)) - 1)/100 + 1));
        
    }

    return OK;
}

Status ability_heal_ally(Ability *ability, Game *game){
    Command *command = NULL;
    Stats *stats = NULL, *fin_stats = NULL;
    int n_arg, size, i;
    char **args = NULL;
    char *data = NULL;
    Player *player = NULL;
    Entity *ent = NULL, *fin_ent = NULL;

    double health_recovered, multiplier;
    
    if(!ability || !game) return ERROR;

    if(ability_get_cooldown_count(ability) != 0)
        return ERROR;

    data = ability_get_data(ability);
    if(!data) return ERROR;

    health_recovered = atof(data);
    if(health_recovered == 0) return ERROR;

    if(ability_get_is_player_ability(ability) == true){
        if(game_get_state(game) == DEFAULT){
            command = game_get_last_command(game);
            n_arg = command_get_arguments_count(command);
            args = command_get_arguments(command);
            if(n_arg < 1 || n_arg > 2 || !args){
                return ERROR;
            }

            player = game_get_player_by_id(game, ability_get_entityid(ability));
            if(!player) return ERROR;

            size = player_get_follower_num(player);
            for(i = 0; i < size && fin_ent == NULL; i++){
                ent = player_get_follower_at(player, i);
                if(!ent) return ERROR;
                if(strcmp(entity_get_name(ent), args[1]) == 0){
                    fin_ent = ent;
                }
            }

            if(fin_ent == NULL) return ERROR;

            multiplier = (entity_get_magicLevel(player_get_entity(player)) - 1)/100 + 1;


            entity_set_health(fin_ent, entity_get_health(fin_ent) + health_recovered*multiplier);
        } else if(game_get_state(game) == COMBAT){
            command = game_get_last_command(game);
            n_arg = command_get_arguments_count(command);
            args = command_get_arguments(command);
            if(n_arg < 1 || n_arg > 2 || !args){
                return ERROR;
            }

            player = game_get_player_by_id(game, ability_get_entityid(ability));
            if(!player) return ERROR;

            size = combat_get_allies_count(game_get_combat(game));
            for(i = 0; i < size && fin_stats == NULL; i++){
                stats = combat_get_allies_stats_at(game_get_combat(game), i);
                if(!stats) return ERROR;
                if((strcmp(entity_get_name(stats->entity), entity_get_name(player_get_entity(player))) != 0) && (strcmp(entity_get_name(stats->entity), args[1]) == 0)){
                    fin_stats = stats;
                }
            }

            if(fin_stats == NULL) return ERROR;

            multiplier = (entity_get_magicLevel(player_get_entity(player)) - 1)/100 + 1;


            fin_stats->stats.health += health_recovered*multiplier;
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
    Link *linkCurrent = NULL, *linkData = NULL;
    Id linkId = NO_ID;

    Entity *player = NULL;

    Command *cmd = NULL;
    CommandCode code = NO_CMD;

    if(!ability || !game) return ERROR;
    
    cmd = game_get_last_command(game);

    if(command_get_arguments_count(cmd) != 2){
        game_add_log_message(game, MESSAGE_ERROR, "Invalid number of arguments");
        return ERROR;
    }

    /*Gets the direction to be unlocked from the second argument, first argument was the object to use*/
    code = command_get_code_from_str(command_get_arguments(cmd)[1]);

    switch (code)
    {
    case NORTH:
        linkCurrent = space_get_north(game_get_space(game, game_get_player_location(game)));
        break;
    case SOUTH:
        linkCurrent = space_get_south(game_get_space(game, game_get_player_location(game)));
        break;
    case EAST:
        linkCurrent = space_get_east(game_get_space(game, game_get_player_location(game)));
        break;
    case WEST:
        linkCurrent = space_get_west(game_get_space(game, game_get_player_location(game)));
        break;
    case UP:
        linkCurrent = space_get_up(game_get_space(game, game_get_player_location(game)));
        break;
    case DOWN:
        linkCurrent = space_get_down(game_get_space(game, game_get_player_location(game)));
        break;    
    default:
        game_add_log_message(game, LOG_ERROR, "Invalid argument format, specify a valid direction. Ex: north, south");
        break;
    }

    if(!linkCurrent) return ERROR;

    linkId = atoi(ability_get_data(ability));

    if(linkId != link_get_id(linkCurrent)) return ERROR;

    linkData = game_get_link_by_id(game, linkId);

    player = player_get_entity(game_get_player(game));

    if(!linkData || !player) return ERROR;

    return link_unlock(linkData, player);
}

Status ability_effect_enemy(Ability *ability, Game *game){
    Combat *combat=NULL;
    Stats *st;
    Entity *ent=NULL;
    char **args=NULL;
    Id effect_id;
    Effect *effect=NULL;
    int target=0;

    /*this function applies the effect of id == ability->data, to the enemy specified in the last command*/
    /*last command is: sk (num) (enemy)*/
    /*player must be in a combat to use the ability*/

    if(!ability || !game) return ERROR;
    if(command_get_arguments_count(game_get_last_command(game)) != 2) return ERROR;
    if(game_get_state(game) != COMBAT) return ERROR;

    args=command_get_arguments(game_get_last_command(game));
    combat = game_get_combat(game);
    if(!args || !combat) return ERROR;
    target=atoi(args[1]);
    if((target <= 0) || (target>combat_get_enemies_count(combat))) return ERROR;
    if((st = combat_get_enemies_stats_at(combat,target-1)) == NULL) return ERROR;
    ent = st->entity;
    if(ability_get_data(ability) == NULL) return ERROR; 
    effect_id = atoi(ability_get_data(ability));
    effect = game_get_effect_by_id(game, effect_id);

    return effect_add_affected(effect, ent);
}

Status ability_effect_ally(Ability *ability, Game *game){
    Combat *combat=NULL;
    Stats *st;
    Entity *ent=NULL;
    char **args=NULL;
    Id effect_id;
    Effect *effect=NULL;
    int target=0;

    /*this function applies the effect of id == ability->data, to the ally specified in the last command*/
    /*last command is: sk (num) (ally)*/
    /*player must be in combat to apply the ability*/

    if(!ability || !game) return ERROR;
    if(command_get_arguments_count(game_get_last_command(game)) != 2) return ERROR;
    if(game_get_state(game) != COMBAT) return ERROR;

    args=command_get_arguments(game_get_last_command(game));
    combat = game_get_combat(game);
    if(!args || !combat) return ERROR;
    target=atoi(args[1]);
    if((target <= 0) || (target>combat_get_allies_count(combat))) return ERROR;
    if((st = combat_get_allies_stats_at(combat,target)) == NULL) return ERROR;
    ent = st->entity;
    if(ability_get_data(ability) == NULL) return ERROR; 
    effect_id = atoi(ability_get_data(ability));
    effect = game_get_effect_by_id(game, effect_id);

    return effect_add_affected(effect, ent);
}

Status ability_effect_self(Ability *ability, Game *game){
    Entity *ent=NULL;
    Id effect_id;
    Effect *effect=NULL;

    /*this function applies the effect of id == ability->data, to the enemy specified in the last command*/
    /*last command is: sk (num) */

    if(!ability || !game) return ERROR;
    if(command_get_arguments_count(game_get_last_command(game)) != 1) return ERROR;

    if(ability_get_data(ability) == NULL) return ERROR; 
    effect_id = atoi(ability_get_data(ability));
    effect = game_get_effect_by_id(game, effect_id);
    ent = player_get_entity(game_get_player(game));

    return effect_add_affected(effect, ent);
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
        case EFFECT_ENEMY:
            status = ability_effect_enemy(ability, game);
            break;
        case EFFECT_SELF:
            status = ability_effect_self(ability, game);
            break;
        case EFFECT_ALLY:
            status = ability_effect_ally(ability, game);
            break;
        case REVIVE_PLAYER:
            status = ability_revive_player(ability, game);
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

    return status;
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

Status ability_revive_player(Ability *ability, Game *game){
    Player *player_to_revive = NULL, *player = NULL, *fin_play = NULL;
    Id locationid;
    int i, size;

    Command *comm = NULL;
    char **args = NULL;
    int n_args;
    
    if(!ability || !game) return ERROR;

    comm = game_get_last_command(game);
    if(!comm) return ERROR;
    args = command_get_arguments(comm);
    if(!args) return ERROR;
    n_args = command_get_arguments_count(comm);
    if(n_args != 1) return ERROR;

    player = game_get_player_by_id(game, ability_get_entityid(ability));
    if(!player) return ERROR;

    locationid = entity_get_location(player_get_entity(player));
    if(locationid == NO_ID) return ERROR;

    size = game_get_n_players(game);
    for(i = 0; i < size && fin_play == NULL; i++){
        player_to_revive = game_get_player_at(game, i);
        if(entity_get_location(player_get_entity(player_to_revive)) == locationid && entity_is_dead(player_get_entity(player_to_revive)) == true){
            fin_play = player_to_revive;
        }
    }

    if(fin_play == NULL) return ERROR;

    entity_set_health(player_get_entity(player_to_revive), 1);

    return OK;
}