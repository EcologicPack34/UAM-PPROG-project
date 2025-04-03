
#include "event_actions.h"

#include "debug_printing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------PRIVATE DECLARATION---------*/

/**
 * @brief Action for NO EVENT
 * @author Daniel Gómez
 * 
 * @param event 
 * @param game 
 * @return true 
 * @return false 
 */
bool event_trigger_none(Event *event, Game *game);

/**
 * @brief Action for Object on Space, it ends the game if the object is in certain space
 *      aux data info: ...|objectId:spaceId
 *  
 * @author Daniel Gómez
 * 
 * @param event 
 * @param game 
 * @return true 
 * @return false 
 */
bool event_trigger_object_on_space(Event *event, Game *game);

/**
 * @brief Triggers Combat mode
 *      aux data info: ...|probability
 * @author Daniel Gómez
 * 
 * @param event 
 * @param game 
 * @return true 
 * @return false 
 */
bool event_trigger_combat(Event *event, Game *game);

/**
 * @brief Triggers end of game if player is dead
 * @author Daniel Gómez
 * 
 * @param event 
 * @param game 
 * @return true 
 * @return false 
 */
bool event_trigger_player_death(Event *event, Game *game);

/**
 * @brief 
 * @author Daniel Gómez
 * 
 * @param game 
 * @return true 
 * @return false 
 */
bool event_trigger_end_combat(Game *game);

/*---------PUBLIC FUNCTIONS----------*/
void event_actions_trigger_events(Game *game){
    int i, eventCount;
    Event *event = NULL;
    EventManager *manager = NULL;
    bool triggered = false;

    if(!game) return;

    manager = game_get_event_manager(game);
    eventCount = event_manager_get_event_count(manager);

    debug_log(DEBUG, "Checking for event triggers, count %d", eventCount);

    for (i = 0; i < eventCount; i++)
    {
        event = event_manager_get_event(manager, i);
        triggered = false;

        switch (event_get_type(event))
        {
            case NO_EVENT:
                event_trigger_none(event, game);
                break;
            case OBJECT_ON_SPACE:
                triggered = event_trigger_object_on_space(event, game);
                break;
            case TRIGGER_COMBAT:
                triggered = event_trigger_combat(event, game);
                break;
            default:
                break;
            case DEAD:
                triggered = event_trigger_player_death(event, game);
                break;
        }
        if(triggered && event_get_removeOnTrigger(event))
            event_manager_remove_event(manager, event);
    }
    /*Always checked events*/
    if(game_get_state(game) == COMBAT)
        triggered = event_trigger_end_combat(game);

    triggered = event_trigger_player_death(event, game);
}

/*-------------EVENT ACTIONS------------*/

bool event_trigger_none(Event *event, Game *game){
    debug_log(DEBUG, "No event Assigned");
    return false;
}

bool event_trigger_end_combat(Game *game){

    if(combat_get_isFinished(game_get_combat(game))){
        if(game_combat_end(game) == ERROR) return false;

        game_add_log_message(game, MESSAGE_LOG, "Combat ended");

        return true;
    }
    return false;
}

bool event_trigger_object_on_space(Event *event, Game *game){
    char data[WORD_SIZE];
    char *toks = NULL;

    Id objectId = NO_ID;
    Id spaceId = NO_ID; 

    Space *space = NULL;

    if(!event || !game) return false;

    if(!event_is_cmd_valid(event, game_get_last_command(game))) return false;
    
    
    strcpy(data, event_get_aux_data(event));
    
    /*Reads the object*/
    toks = strtok(data, ":");
    if(toks == NULL){
        debug_log(LOG_ERROR, "Invalid number of tag for object_on_space event");
        return false;
    }
    objectId = atoi(toks);
    
    /*Reads the space*/
    toks = strtok(NULL, ":");
    if(toks == NULL){
        debug_log(LOG_ERROR, "Invalid number of tag for object_on_space event");
        return false;
    }
    spaceId = atoi(toks);
    
    /*Gets the space from game if posible*/
    space = game_get_space(game, spaceId);
    if(!space){
        debug_log(LOG_ERROR, "Invalid space %ld not found for object_on_space event", spaceId);
        return false;
    }
    
    /*Checks if space contains the given object*/
    if(inventory_contains_object(space_get_inventory(space), objectId)){
        game_set_finished(game, true);
        debug_log(PRINT,"Object On Space Trigger");
        return true;/*Return true because if found the space is triggered*/
    }

    return false;/*Return false because event hasnt been triggered*/
}

bool event_trigger_combat(Event *event, Game *game){
    int random = 0;
    char str[WORD_SIZE] = "";
    char strAux[WORD_SIZE * 2] = "";

    if(!event || !game) return false;

    random = rand() % 100;

    if(random > atoi(event_get_aux_data(event))) return false;

    if(!event_is_cmd_valid(event, game_get_last_command(game))) return false;
    debug_log(PRINT,"Combat trigger");
    command_get_as_string(game_get_last_command(game), str);

    sprintf(strAux, "You were attack while doing %s", str);
    game_add_log_message(game, MESSAGE_LOG, strAux);
    game_combat_start(game);
    return true;
}
bool event_trigger_player_death(Event *event, Game *game){

    if(!event || !game) return false;


    if(entity_get_health(player_get_entity(game_get_player(game))) <= 0){

        debug_log(PRINT,"Player died, finishing game");
        return true;
    }
    return false;
}