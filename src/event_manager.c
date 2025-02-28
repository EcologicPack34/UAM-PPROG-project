
#include "event_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "collection.h"
#include "debug_printing.h"

#define EVENT_MANAGER_DEFAULT_SIZE 10

char *eventTags[N_EVENTS] = { "" , "test"};

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
        return NULL;
    }
    if(type == NO_EVENT) return NULL;

    event = (Event*)calloc(1, sizeof(Event));
    if(!event) return NULL;

    event->data = (char*)calloc(strlen(data) +1 , sizeof(char));
    if(!(event->data)){
        free(event);
        return NULL;
    }

    strcpy(event->data, data);

    if(cmdNum > 0){
        event->commands = calloc(cmdNum,sizeof(CommandCode));
        if(!(event->commands)){
            free(event->data);
            free(event);
            return NULL;
        }
        for (i = 0; i < cmdNum; i++)
        {
            event->commands[i] = commands[i];
        }
        
    }
    else{
        event->commands = NULL;
    }

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

/*----------EVENT MANAGER----------*/

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
 /*
void event_manager_trigger_events(EventManager *manager, Game *game){
    int i;
    Event *event = NULL;
    if(!manager || !game) return;

    for (i = 0; i < collection_length(manager->events); i++)
    {
        event = (Event *)collection_get_element_at(manager->events, i);

        switch (event->type)
        {
            case NO_EVENT:
                event_trigger_none(game);
                break;
            case TEST:
                event_trigger_test(game);
                break;
        
            default:
                break;
        }
    }
    
}*/

/*-------------EVENT ACTIONS------------*/
/*
void event_trigger_none(Game *game){
    debug_log(DEBUG, "No event Assigned");
}
void event_trigger_test(Game *game){
    debug_log(DEBUG, "Test event triggered");
}*/