
#include "event_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "collection.h"
#include "debug_printing.h"

#define EVENT_MANAGER_DEFAULT_SIZE 10

char *eventTags[N_EVENTS] = { "" , "object_on_space"};

struct _Event{
    Id id;
    EventType type;
    CommandCode *commands;
    int cmdNum;
    char *data;
    bool removeOnTrigger;
};

struct _EventManager{
    Collection *events;
};

/*----PUBLIC FUNCTIONS------*/
Event *event_create(Id id, EventType type, CommandCode *commands, int cmdNum, char *data, bool removeOnTrigger){
    Event *event = NULL;
    int i;
    
    if(!data || cmdNum < 0){
        debug_log(LOG_ERROR,"Invalid data or cmdNum when creating event");
        return NULL;
    }
    if(type == NO_EVENT) return NULL;

    event = (Event*)calloc(1, sizeof(Event));
    if(!event){
        debug_log(LOG_ERROR,"Couldn't allocate memory when creating event");
        return NULL;
    } 

    event->data = (char*)calloc(strlen(data) +1 , sizeof(char));
    if(!(event->data)){
        free(event);
        debug_log(LOG_ERROR,"Couldn't allocate memory when creating event");
        return NULL;
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

    if(!event || !cmd) return NULL;

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

/*-----------EVENT GETTERS---------*/

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