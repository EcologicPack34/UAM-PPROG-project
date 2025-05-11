
#include "event_actions.h"

#include "debug_printing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int player_turn = -1;

/*
    * Private functions
*/

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
 * @brief Counts turns for player and changes player players when turn end
 * @author Daniel Gómez
 * 
 * @param event 
 * @param game 
 * @return true 
 * @return false 
 */
bool event_trigger_players_turn(Event *event, Game *game);

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

/**
 * @brief Updates deaths on DEFAULT game state
 * 
 * @param game game struct
 * @return true if entity
 * @return false 
 */
bool event_trigger_update_deaths(Game *game);

/**
 * @brief Applies poison effect to all entities in a space
 * @author Aaron Charameli Mair
 * 
 * @param event
 * @param game
 * @return true 
 * @return false 
 * @note data is EffectID:SpaceID
 */
bool event_trigger_effect_area(Event *event, Game *game);

/**
 * @brief This function fusions 2 objects into 1 that can open a link by using it
 * @author Aaron Charameli Mair
 * 
 * @param event 
 * @param game 
 * @return true 
 * @return false
 * @note data will be "idObj1:idObj2;obj:ID;Name;Data;Description;LinkId
 */
bool event_trigger_objects_fusion2key(Event *event, Game *game);

/**
 * @brief Event taht checks if a certain enemy has been killed and end the game if so;
 * @author Daniel Gómez
 * 
 * @param event 
 * @param game 
 * @return true 
 * @return false 
 * @note data will be "enemyID"
 */
bool event_trigger_end_on_kill_enemy(Event *event, Game *game);

/**
 * @brief this function triggers an npc's dialogue after his death
 * @author Aaron Charameli Mair
 * 
 * @param event event struct
 * @param game game struct
 * @return true 
 * @return false 
 * @note the event's data will be the npc's id
 */
bool event_trigger_death_dialogue(Event *event, Game *game);

/**
 * @brief this function triggers an npc's dialogue after entering a certain space
 * 
 * @param event event struct
 * @param game game struct
 * @return true 
 * @return false 
 * @note the event's data will be the npcID:SpaceID
 */
bool event_trigger_dialogue_in_space(Event *event, Game *game);

/*
    * Public functions
*/

void event_actions_trigger_events(Game *game){
    int i, eventCount;
    Event *event = NULL;
    EventManager *manager = NULL;
    bool triggered = false;

    if(!game) return;

    manager = game_get_event_manager(game);
    eventCount = event_manager_get_event_count(manager);

    debug_log(DEBUG, "Checking for event triggers, count %d", eventCount);

    /*Always checked events*/
    if(game_get_state(game) == COMBAT)
        triggered = event_trigger_end_combat(game);

    if(game_get_state(game) == DEFAULT)
        triggered = event_trigger_update_deaths(game);



    for (i = 0; i < eventCount; i++)
    {
        if((game_get_state(game) == STORE_STATE) || (game_get_state(game) == DIALOGUE)){
            break;
        }

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
            case EFFECT_AREA:
                triggered = event_trigger_effect_area(event, game);
                break;
            case PLAYER_TURN:
                triggered = event_trigger_players_turn(event, game);
                break;
            case OBJECTS_FUSION:
                triggered = event_trigger_objects_fusion2key(event, game);
                break;
            case END_ON_KILL_ENEMY:
                triggered = event_trigger_end_on_kill_enemy(event, game);
                break;
            case DEATH_DIALOGUE:
                triggered = event_trigger_death_dialogue(event, game);
                break;
            case DIALOGUE_IN_SPACE:
                triggered = event_trigger_dialogue_in_space(event, game);
                break;
            default:
                break;
        }
        if(triggered && event_get_removeOnTrigger(event)){
            event_manager_remove_event(manager, event);
            event_destroy(event);
        }
    }
    triggered = event_trigger_player_death(event, game);
}

/*
    * Event actions
*/

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
        if(npc_get_status(npc) == ALLY || npc_get_status(npc) == NEUTRAL || entity_get_health(npc_get_entity(npc)) <= 0){
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

bool event_trigger_update_deaths(Game *game){
    Player *player = NULL;
    NPC *npc = NULL;
    Entity *ent = NULL;
    int i, size;
    Collection *coll = NULL;

    if(!game) return false;

    /*Updates follows if NPCs or players died*/
    game_update_unfollows(game);

    coll = game_get_npcs(game);
    if(!coll) return false;
    size = collection_length(coll);
    for(i = 0; i < size; i++){
        npc = collection_get_element_at(coll, i);
        ent = npc_get_entity(npc);
        if(!npc) return false;

        game_dead_entity_drop_inv(game, ent);
    }

    size = game_get_n_players(game);
    for(i = 0; i < size; i++){
        player = game_get_player_at(game, i);
        if(!player) return false;

        ent = player_get_entity(player);
        if(!ent) return false;

        game_dead_entity_drop_inv(game, ent);
    }

    return true;
}

bool event_trigger_effect_area(Event *event, Game *game){
    char data[WORD_SIZE];
    char *toks=NULL;
    int i,n_player,n_npc;
    Effect *effect = NULL;
    Id effectId,location;
    if(!game || !event) return false;

    strcpy(data,event_get_aux_data(event));

    toks = strtok(data, ":");
    if(!toks) return false;
    effectId = atol(toks);

    toks = strtok(NULL, "\n\r");
    if(!toks) return false;
    location = atol(toks);

    effect = game_get_effect_by_id(game, effectId);
    if(!effect) return false;


    n_player = game_get_n_players(game);
    n_npc = collection_length(game_get_npcs(game));

    for(i = 0; i < n_player; i++){
        if(entity_get_location(player_get_entity(game_get_player_at(game, i))) == location)
            effect_add_affected(effect, player_get_entity(game_get_player_at(game, i)));
    }
    for(i = 0; i < n_npc; i++){
        if(entity_get_location(npc_get_entity(collection_get_element_at(game_get_npcs(game), i))) == location)
            effect_add_affected(effect, npc_get_entity(collection_get_element_at(game_get_npcs(game), i)));
    }

    return true;
}
bool event_trigger_players_turn(Event *event, Game *game){
    if(!event || !game){
        return false;
    }
    if(game_get_state(game) != DEFAULT) return false;
    if(!event_is_cmd_valid(event, game_get_last_command(game))) return false;

    if(player_turn == -1) player_turn = atoi(event_get_aux_data(event));
    player_turn--;
    if(player_turn == 0){
        player_turn = atoi(event_get_aux_data(event));
        game_switch_player(game, -1);
    }
    return true;
}

bool event_trigger_objects_fusion2key(Event *event, Game *game){
    int i, n_players=0;
    char data[WORD_SIZE]=""; /*correct format is expected*/
    char *toks = NULL;
    Player *player = NULL;
    Id player_id=NO_ID,id_obj1=NO_ID, id_obj2=NO_ID;
    Object *obj1=NULL,*obj2=NULL;
    bool has_objs = false; /*if a player has both objects to fusion*/

    Object *obj=NULL;
    Id id = NO_ID, ability_id=NO_ID;
    char name[WORD_SIZE]="";
    char description[WORD_SIZE]="";
    char link_id[WORD_SIZE]=""; /*saved as string as it will by the ability's data (the ability that unlocks the link)*/

    Ability *ability=NULL;

    /*in case the object is usable*/

    if(!event || !game) return false;

    if(!event_get_aux_data(event)) return false;
    strcpy(data,event_get_aux_data(event));

    /*gets objects to fusion id's*/

    toks = strtok(data, ":");
    if(toks)
        id_obj1=atol(toks);
    
    toks = strtok(NULL, ";");
    if(toks)
        id_obj2=atol(toks);

    /*#obj:ID;Nombre;Descripcion;abilityId;LinkId*/

    /*shred data's information*/

    toks = strtok(NULL, ":");
    toks = strtok(NULL, ";");
    if(!toks){
        debug_log(LOG_ERROR, "Toks is NULL at: event_tringger_objects_fusion2key(Event *, Game*) in event_actions.c");
        return false;
    }
        id=atol(toks);

    toks = strtok(NULL, ";");
    if(!toks){
        debug_log(LOG_ERROR, "Toks is NULL at: event_tringger_objects_fusion2key(Event *, Game*) in event_actions.c");
        return false;
    }
        strcpy(name,toks);
        
    toks = strtok(NULL, ";");
    if(!toks){
        debug_log(LOG_ERROR, "Toks is NULL at: event_tringger_objects_fusion2key(Event *, Game*) in event_actions.c");
        return false;
    }
        strcpy(description,toks);

    toks = strtok(NULL, ";");
    if(!toks){
        debug_log(LOG_ERROR, "Toks is NULL at: event_tringger_objects_fusion2key(Event *, Game*) in event_actions.c");
        return false;
    }
        ability_id=atol(toks);

    toks = strtok(NULL, ";\n\r");
    if(!toks){
        debug_log(LOG_ERROR, "Toks is NULL at: event_tringger_objects_fusion2key(Event *, Game*) in event_actions.c");
        return false;
    }
        strcpy(link_id,toks);

    
    /*gets players an checks they have the objects*/

    n_players = game_get_n_players(game);

    for(i=0; (i < n_players) && (has_objs==false); i++){
        player = game_get_player_at(game, i);
        if(player_has_object(player, id_obj1) && player_has_object(player, id_obj2)){
            has_objs=true;
        }
    }
    if(has_objs==false){
        return false;
    }

    player_id = entity_get_id(player_get_entity(player));
    
    

    obj = object_create(id, name, data, description, 0, -1, true, true, player_id, PLAYER_INVENTORY);
    if(!obj){
        debug_log(LOG_ERROR, "Error creating an object at: event_tringger_objects_fusion2key(Event *, Game*) in event_actions.c");
        return false;
    }

    /*delete two objets to fusion*/

    obj1 = game_get_object_by_id(game, id_obj1);
    obj2 = game_get_object_by_id(game, id_obj2);

    inventory_remove_object(entity_get_inventory(player_get_entity(player)), obj1);
    inventory_remove_object(entity_get_inventory(player_get_entity(player)), obj2);

    collection_remove(game_get_objects(game), obj1);
    collection_remove(game_get_objects(game), obj2);

    object_destroy(obj1);
    object_destroy(obj2);

    /*add new object with its link unlock ability*/

    if(game_add_object(game, obj) == ERROR){
        object_destroy(obj);
        debug_log(LOG_ERROR, "Error adding an object to game at: event_tringger_objects_fusion2key(Event *, Game*) in event_actions.c");
        return false;
    }

    ability = ability_create(ability_id,link_id,"unlocks_link_with_fusioned_obj",LINK_UNLOCK,id,0,1,0,0,0);
    if(!ability){
        inventory_remove_object(entity_get_inventory(player_get_entity(player)), obj);
        collection_remove(game_get_objects(game), obj);
        object_destroy(obj);
        debug_log(LOG_ERROR, "Error creating an ability at: event_tringger_objects_fusion2key(Event *, Game*) in event_actions.c");
        return false;
    }

    return game_add_ability(game, ability);
}

bool event_trigger_end_on_kill_enemy(Event *event, Game *game){
    Id id = NO_ID;
    Collection *npcs = NULL;
    NPC *npc=NULL;
    Entity *ent=NULL;
    int n_npcs=0;
    char str[WORD_SIZE];

    if(!event || !game) return false;

    if(game_get_state(game) == COMBAT) return false;

    id = atol(event_get_aux_data(event));
    if(id <= UNDEFINED_ID) return false;

    npcs = game_get_npcs(game);
    n_npcs = collection_length(npcs);

    for (int i = 0; i < n_npcs; i++)
    {
        npc = collection_get_element_at(npcs, i);
        ent = npc_get_entity(npc);
        if(entity_get_id(ent) == id && entity_is_dead(ent)){
            sprintf(str, "You killed %s, you WON!!", entity_get_name(ent));
            game_add_log_message(game, MESSAGE_LOG, str);
            game_set_finished(game, true);
            return true;
        }
    }
    
    return false;
}

bool event_trigger_death_dialogue(Event *event, Game *game){
    Id id=NO_ID;
    NPC *npc=NULL;

    if(!event || !game){
        return ERROR;
    }

    if(!event_get_aux_data(event)){
        return false;
    }

    id=atol(event_get_aux_data(event));

    npc=game_get_NPC_by_id(game, id);

    if(entity_is_dead(npc_get_entity(npc)) == false){
        return false;
    }
    return game_dialogue_init(game, npc);
}

bool event_trigger_dialogue_in_space(Event *event, Game *game){
    char data[WORD_SIZE]="";
    char *toks=NULL;
    Id id=NO_ID, location=NO_ID;
    NPC *npc=NULL;

    if(!event || !game) return ERROR;

    if(!event_get_aux_data(event)) return false;

    strcpy(data, event_get_aux_data(event));

    toks = strtok(data, ":"); 
    if(!toks) return false;
    id=atol(toks);

    toks = strtok(NULL, " \n\r"); 
    if(!toks) return false;
    location=atol(toks);

    npc = game_get_NPC_by_id(game, id);

    if(game_get_player_location(game) == location)
        return game_dialogue_init(game, npc);
    else
        return false;
}