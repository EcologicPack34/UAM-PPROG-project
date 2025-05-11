
#include "event_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "collection.h"
#include "debug_printing.h"
#include "utils.h"

#define EVENT_MANAGER_DEFAULT_SIZE 20 /*!< Inital size of the event manager*/

/**
 * @brief Array of strings to relate the event types with their code
 */
char *eventTags[N_EVENTS] = { "" , "object_on_space", "trigger_combat", "player_death","npc_rand_move", "trigger_effects","effect_area", "player_turn", "unlock_with_object_inv",\
     "objects_fusion", "end_on_kill_enemy", "death_dialogue", "dialogue_in_space", "dialogue_complete", "unlock_on_kill", "space_end"};

/**
 * @brief Event struct to control special interactions or triggers within the game
 */
struct _Event{
    Id id;                  /*!< Id of the event*/
    EventType type;         /*!< Type of the event*/
    CommandCode *commands;  /*!< Commands that trigger the event*/
    int cmdNum;             /*!< Number of commands that trigger the event*/
    char *data;             /*!< Data of the event*/
    bool removeOnTrigger;   /*!< Bool true if it is removed on trigger*/
};

/**
 * @brief Event manager to control events trigger and save events
 */
struct _EventManager{
    Collection *events;     /*!< Events collection*/
};

/*
    * Public functions
*/

Event *event_create(Id id, EventType type, CommandCode *commands, int cmdNum, char *data, bool removeOnTrigger){
    Event *event = NULL;
    int i;
    
    if(cmdNum < 0){
        debug_log(LOG_ERROR,"Invalid data or cmdNum when creating event");
        return NULL;
    }
    if(type == NO_EVENT) return NULL;

    event = (Event*)calloc(1, sizeof(Event));
    if(!event){
        debug_log(LOG_ERROR,"Couldn't allocate memory when creating event");
        return NULL;
    } 

    if(data != NULL){
        event->data = (char*)calloc(strlen(data) +1 , sizeof(char));
        if(!(event->data)){
            free(event);
            debug_log(LOG_ERROR,"Couldn't allocate memory when creating event");
            return NULL;
        }
    }

    strcpy(event->data, data);
    
    if(cmdNum > 0){
        event->commands = calloc(cmdNum,sizeof(CommandCode));
        if(!(event->commands)){
            free(event->data);
            free(event);
            debug_log(LOG_ERROR,"Couldn't allocate memory when creating event");
            return NULL;
        }
        for (i = 0; i < cmdNum; i++)
        {
            event->commands[i] = commands[i];
        }
        
    }
    else{
        (event->commands) = NULL;
    }

    if(event == NULL) debug_log(LOG_WARNING, "How did we get here?");

    event->cmdNum = cmdNum;
    event->type = type;
    event->id = id;
    event->removeOnTrigger = removeOnTrigger;
    return event;
}

void event_destroy(void *event){
    if(event){
        free(((Event*)event)->data);
        free(((Event*)event)->commands);
        free(event);
    }
}

/*
    * Event getters
*/

EventType event_get_type(Event *event){
    if(!event) return NO_EVENT;
    return event->type;
}

Id event_get_id(Event *event){
    if(!event) return NO_ID;
    return event->id;
}

CommandCode *event_get_commands(Event *event){
    if(!event) return NULL;
    return event->commands;
}

int event_get_cmd_count(Event *event){
    if(!event) return -1;
    return event->cmdNum;
}

bool event_get_removeOnTrigger(Event *event){
    if(!event) return false;
    return event->removeOnTrigger;
}

char *event_get_aux_data(Event *event){
    if(!event) return NULL;
    return event->data;
}

/*
    * Event general functions
*/

int event_compare(void * e1, void *e2){
    if(!e1 || !e2) return -2;

    return ((Event*)e1)->id - ((Event*)e2)->id;
}

EventType event_type_from_str(char *string){
    int i;
    
    if(!string) return NO_EVENT;

    for (i = 0; i < N_EVENTS; i++)
    {   
        if(strcmp(string, eventTags[i]) == 0)
            return i + NO_EVENT;
    }
    return NO_EVENT;
}

bool event_is_cmd_valid(Event *event, Command *cmd){
    int i, cmdCount;
    CommandCode code;
    CommandCode *eventCodes = NULL;

    if(!event || !cmd) return false;

    if(event->cmdNum == 0) return true;

    if(command_get_status(cmd) == ERROR) return false;

    cmdCount = event_get_cmd_count(event);
    code = command_get_code(cmd);
    eventCodes = event_get_commands(event);
    for (i = 0; i < cmdCount; i++)
    {
        if(code == eventCodes[i]){
            return true;
        }
    }
    return false;
}

int event_save_on_file(Event *event, FILE *fOUT){
    int i, count = 0;

    if(!event || !fOUT) return -1;

    count += fprintf(fOUT, "%ld;%d;%d;%d.", event->id, event->type, event->removeOnTrigger, event->cmdNum);
    for(i = 0; i < event->cmdNum; i++){
        count += fprintf(fOUT, "%d,", event->commands[i]);
    }

    count += fprintf(fOUT, "%s\n", event->data);

    return count;
}

Event *event_create_from_file(FILE *fIN){
    Event *event = NULL;
    Id id;
    int i, type, removeOnTrigger, cmdNum;
    char *toks = NULL, aux_str[WORD_SIZE];
    CommandCode *commands = NULL;

    if(!fIN) return NULL;

    fgets(aux_str,WORD_SIZE,fIN);

    sscanf(aux_str, "%ld;%d;%d;%d.", &id, &type, &removeOnTrigger, &cmdNum);

    commands = (CommandCode *)malloc(cmdNum*sizeof(CommandCode));
    if(!commands) return NULL;

    toks = strtok(aux_str,".");
    if(cmdNum > 0){
        commands[0] = (CommandCode)atoi(toks);
    }
    for(i = 0; i < cmdNum && toks != NULL; i++){
        toks = strtok(NULL,",");
        commands[i] = (CommandCode)atoi(toks);
    }

    toks = strtok(NULL,"\n\r");
    string_remove_newline_escape_sequence_on_end(toks);

    event = event_create(id, type, commands, cmdNum, toks, removeOnTrigger);
    
    /*event_create allocs another _Struct CommandCode to copy this one's information*/
    free(commands);

    return event;
}



/* ==============================
   --------EVENT MANAGER---------
   ==============================*/

EventManager *event_manager_create(){
    EventManager *manager = NULL;

    manager = calloc(1, sizeof(EventManager));
    if(!manager) return NULL;

    manager->events = collection_create(EVENT_MANAGER_DEFAULT_SIZE, false, true, event_compare, NULL);
    if(!(manager->events)){
        free(manager);
        return NULL;
    }
    return manager;
}

void event_manager_destroy(void *manager){
    if(manager){
        if(((EventManager *)manager)->events){
            collection_free_elements(((EventManager *)manager)->events, event_destroy);
            collection_destroy(((EventManager *)manager)->events);
        }
        free(manager);
    }
}

/* ========================================
   --------EVENT MANAGER FUNCTIONS---------
   ========================================*/

Status event_manager_add_event(EventManager *manager, Event *event){
    if(!manager || !event) return ERROR;

    return collection_add(manager->events, (void *)event);
}

Status event_manager_remove_event(EventManager *manager, Event *event){
    if(!manager || !event) return ERROR;

    return collection_remove(manager->events, (void *)event);
}

long event_manager_get_event_count(EventManager *manager){
    if(!manager) return -1;
    return collection_length(manager->events);
}

Event *event_manager_get_event(EventManager *manager, long index){
    if(!manager || index < 0) return NULL;

    return (Event*)collection_get_element_at(manager->events, index);
}

int event_manager_save_on_file(EventManager *manager, FILE *fOUT){
    int i, count = 0, size;

    if(!manager || !fOUT) return -1;

    size = collection_length(manager->events);
    count += fprintf(fOUT, "%d\n", size);
    for(i = 0; i < size; i++){
        count += event_save_on_file(collection_get_element_at(manager->events, i), fOUT);
    }

    return count;
}

Status event_manager_read_from_file(EventManager *manager, FILE *fIN){
    int i, size;

    if(!manager || !fIN) return ERROR;

    fscanf(fIN, "%d\n", &size);
    for(i = 0; i < size; i++){
        if(event_manager_add_event(manager, event_create_from_file(fIN)) == ERROR)
            return ERROR;
    }

    return OK;
}