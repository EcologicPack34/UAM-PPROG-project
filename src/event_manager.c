
#include "event_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "collection.h"

#define EVENT_MANAGER_DEFAULT_SIZE 10

struct _Event{
    Id id;
    EventType type;
    bool commandTriggers[N_CMD];
    char *data;
};

struct _EventManager{
    Collection *events;
};

Event *event_create(Id id, EventType type, bool commandTriggers[N_CMD], char *data){
    Event *event = NULL;
    int i;
    
    if(!data){
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

    event->type = type;
    event->id = id;
    for (i = 0; i < N_CMD; i++)
    {
        event->commandTriggers[i] = commandTriggers[i];
    }
    return event;
}

void event_destroy(void *event){
    if(event){
        free(((Event*)event)->data);
        free(event);
    }
}

int event_compare(void * e1, void *e2){
    if(!e1 || !e2) return -2;

    return ((Event*)e1)->id - ((Event*)e2)->id;
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
        if(((EventManager *)manager)->envents){
            collection_free_elements(((EventManager *)manager)->envents, event_destroy);
            collection_destroy(((EventManager *)manager)->envents);
        }
    }
}

Status event_manager_add_event(EventManager *manager, Event *event){
    if(!manager || !event) return ERROR;

    return collection_add(manager->events, (void *)event);
}

void event_manager_trigger_events(EventManager *manager, Game *game){
    int i;
    if(!manager || !game) return;

    for (i = 0; i < collection_length(manager->events); i++)
    {
        return;
    }
    
}