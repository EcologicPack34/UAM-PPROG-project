/**
 * @file event_manager.h
 * @author Daniel Gómez
 * @brief   Small module that aims to implement a way to have generic events in game
 * @version 0.1
 * @date 2025-02-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <stdbool.h>
#include "command.h"
#include "types.h"

#define N_EVENTS 2

typedef enum {NO_EVENT, TEST}EventType;

typedef struct _Event Event;

typedef struct _EventManager EventManager;

Event *event_create(Id id, EventType type, CommandCode *commands, int cmdNum, char *data, bool removeOnTrigger);

void event_destroy(void *event);

int event_compare(void * e1, void *e2);

EventType event_type_from_str(char *string);

/*----------EVENT MANAGER----------*/

EventManager *event_manager_create();

void event_manager_destroy(void *eventManager);

Status event_manager_add_event(EventManager *manager, Event *event);

//void event_manager_trigger_events(EventManager *manager, Game *game);

#endif