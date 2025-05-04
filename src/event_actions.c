
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
 * @param event event struct
 * @param game game struct
 * @return true 
 * @return false 
 */
bool event_trigger_none(Event *event, Game *game);

/**
 * @brief Action for Object on Space, it ends the game if the object is in certain space
 *      aux data info: ...|objectId:spaceId
 * @author Daniel Gómez
 * 
 * @param event event struct
 * @param game game struct
 * @return true 
 * @return false 
 */
bool event_trigger_object_on_space(Event *event, Game *game);

/**
 * @brief Triggers Combat mode
 *      aux data info: ...|probability
 * @author Daniel Gómez
 * 
 * @param event event struct
 * @param game game struct
 * @return true 
 * @return false 
 */
bool event_trigger_combat(Event *event, Game *game);

/**
 * @brief Triggers end of game if player is dead
 * @author Daniel Gómez
 * 
 * @param event event struct
 * @param game game struct
 * @return true 
 * @return false 
 */
bool event_trigger_player_death(Event *event, Game *game);

/**
 * @brief Checks if combat has ended and returns game to default state
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @return true 
 * @return false 
 */
bool event_trigger_end_combat(Game *game);

bool event_trigger_end_dialoge(Game *game);

/**
 * @brief Checks for all npcs and move them randomly
 * @author Daniel Gómez
 * 
 * @param event
 * @param game 
 * @return true 
 * @return false 
 */
bool event_trigger_npc_rand_move(Event *event, Game *game);

/**
 * @brief Triggers all the effects to each affected entity in the game
 * @author Aaron Charameli Mair
 * 
 * @param event a pointer to event
 * @param game a pointer to game
 * @return true 
 * @return false 
 */
bool event_trigger_effects(Event *event, Game *game);

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
            case NPC_RAND_MOVE:
                triggered = event_trigger_npc_rand_move(event, game);
                break;
            case TRIGGER_EFFECTS:
                triggered = event_trigger_effects(event, game);
                break;
            default:
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
    
    if(game_combat_start(game) == OK)
        game_add_log_message(game, MESSAGE_LOG, strAux);
    return true;
}

bool event_trigger_player_death(Event *event, Game *game){

    Entity *entity = NULL;
    int status = 0;
    if(!event || !game) return false;

    entity = player_get_entity(game_get_player(game));
    if(entity_is_dead(entity)){
        status = game_switch_player(game, -1);
        if(status == -2 || status == -1){
            debug_log(PRINT,"Players died, finishing game");
            return true;
        }
    }
    return false;
}

bool event_trigger_npc_rand_move(Event *event, Game *game){
    Collection *npcs = NULL;
    NPC *npc = NULL;
    Space *currentSpace = NULL;
    Space *nextSpace = NULL;
    Link *link = NULL;

    int i,j;
    int npc_length;
    int prob;
    int dir;

    if(!event || !game) return false;

    if(game_get_state(game) != DEFAULT) return false;

    if(!event_is_cmd_valid(event, game_get_last_command(game))) return false;

    prob = atoi(event_get_aux_data(event));

    npcs = game_get_npcs(game);
    npc_length = collection_length(npcs);

    /*loops through every npc*/
    for (i = 0; i < npc_length; i++)
    {
        npc = collection_get_element_at(npcs, i);

        /*if ally doesn't try to move as it follows player*/
        if(npc_get_status(npc) == ALLY || entity_get_health(npc_get_entity(npc)) <= 0){
            continue;
        } 
        /*Checks a probability, if not, it doesnt move the entity*/
        if(rand()%100 > prob){
            continue;
        } 

        currentSpace = game_get_space(game, entity_get_location(npc_get_entity(npc)));

        if(!currentSpace) continue;

        /*Set dir to rand means we start checking a random link*/
        dir = rand()%SPACE_DIRECTIONS;

        /*Loop through posible links if it doesnt exist, so those spaces with less link has same prob to move enemies as those with more links*/
        for ( j = 0; j < SPACE_DIRECTIONS; j++)
        {
            /*gets link for the direction*/
            switch (dir)
            {
                case 0: /*north*/
                    link = space_get_north(currentSpace);
                    break;
                case 1: /*east*/
                    link = space_get_east(currentSpace);
                    break;
                case 2: /*south*/
                    link = space_get_south(currentSpace);
                    break;
                case 3: /*west*/
                    link = space_get_west(currentSpace);
                    break;
                case 4: /*up*/
                    link = space_get_up(currentSpace);
                    break;
                case 5: /*down*/
                    link = space_get_south(currentSpace);
                    break;
                default:
                    break;
            }

            /*if a link is found, we break the loop*/
            if(link && !link_is_locked(link) && link_is_adjacent(link)){
                break;
            }

            /*if a link isnt found, we check the next dir*/
            dir = (dir + 1) % SPACE_DIRECTIONS;
        }
        
        if(!link) continue;

        if(!link_is_locked(link) && link_is_adjacent(link)){
            nextSpace = game_get_space(game, link_get_oposite_space(link, space_get_id(currentSpace)));
        }
        else{
            nextSpace = NULL;
        }

        if(!nextSpace || !currentSpace) continue;

        space_move_NPC(currentSpace, nextSpace, npc);

    }
    return true;
}

bool event_trigger_effects(Event *event, Game *game){
    EffectManager *em=NULL;
    Collection *effects=NULL;
    Effect *aux=NULL;
    Stats *ally_stats=NULL;
    Stats *enemy_stats=NULL;
    int ally_count, enemy_count;
    int n_effects,i;

    if(!event || !game) return false;
    if(!(em = game_get_effect_manager(game))) return false;

    effects = effect_manager_get_effects(game_get_effect_manager(game));
    n_effects = collection_length(effects);

    if(game_get_state(game) == COMBAT){
        ally_stats = combat_get_allies_stats(game_get_combat(game));
        ally_count = combat_get_allies_count(game_get_combat(game));
        enemy_stats = combat_get_enemies_stats(game_get_combat(game));
        enemy_count = combat_get_enemies_count(game_get_combat(game));
    }

    for(i=0; i<n_effects; i++){
        aux = collection_get_element_at(effects,i);
        switch (effect_get_effectAffects(aux))
        {
        case AFFECTS_ENEMY:
            effect_update(aux, enemy_stats, enemy_count);
            break;
        case AFFECTS_ALLY:
            effect_update(aux, ally_stats, ally_count);
            break;
        /*ally stats position 0 is now the player's stats*/
        case AFFECTS_PLAYER:
            effect_update(aux, ally_stats, ally_count);
            break;
        default:
            return false;
            break;
        }
    }
    return true;
}