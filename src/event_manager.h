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
#define EVENET_MANAGER_H

#include <stdbool.h>
#include "command.h"
#include "types.h"
#include "game.h"

typedef enum {NO_EVENT, TEST}EventType;

typedef _Event Event;

typedef _EventManager EventManager;

Event *event_create(Id id, EventType type, bool commandTriggers[N_CMD], char *data);

void event_destroy(void *event);

int event_compare(void * e1, void *e2);

/*----------EVENT MANAGER----------*/

EventManager *event_manager_create();

void event_manager_destroy(void *eventManager);

Status event_manager_add_event(EventManager *manager, Event *event);

void event_manager_trigger_events(EventManager *manager, Game *game);

#endif