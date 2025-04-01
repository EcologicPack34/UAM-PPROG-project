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

#define N_EVENTS 5

/**
 * @brief Enum storing the different types of events 
 */
typedef enum {NO_EVENT, OBJECT_ON_SPACE, TRIGGER_COMBAT, PLAYER_DEATH}EventType;

/**
 * @brief type containing basic info about event
 */
typedef struct _Event Event;

/**
 * @brief type that stores events
 */
typedef struct _EventManager EventManager;

/**
 * @brief Event creation
 * @author Daniel Gómez
 * 
 * @param id 
 * @param type 
 * @param commands list of commands that are valid
 * @param cmdNum 
 * @param data additional data necesary for event triggering
 * @param removeOnTrigger is the event removed from the list on trigger?
 * @return Event* 
 */
Event *event_create(Id id, EventType type, CommandCode *commands, int cmdNum, char *data, bool removeOnTrigger);

/**
 * @brief Frees memory used by event
 * @author Daniel Gómez
 * 
 * @param event 
 */
void event_destroy(void *event);

/**
 * @brief Compares 2 events
 * @author Daniel Gómez
 * 
 * @param e1 
 * @param e2 
 * @return int 
 */
int event_compare(void * e1, void *e2);

/**
 * @brief Gets the event type from a string(see event_manager.c for the valid values)
 * @author Daniel Gómez
 * 
 * @param string 
 * @return EventType 
 */
EventType event_type_from_str(char *string);

/**
 * @brief Checks if a given cmd is within the ones that trigger an event
 * @author Daniel Gómez
 * 
 * @param event 
 * @param cmd 
 * @return true 
 * @return false 
 */
bool event_is_cmd_valid(Event *event, Command *cmd);

/*----------EVENT GETTERS----------*/

/**
 * @brief Gets the type of an event
 * @author Daniel Gómez
 * 
 * @param event 
 * @return EventType 
 */
EventType event_get_type(Event *event);

/**
 * @brief Gets the id of an event
 * @author Daniel Gómez
 * 
 * @param event 
 * @return Id 
 */
Id event_get_id(Event *event);

/**
 * @brief Gets the pointer to the list of valid commands
 * @author Daniel Gómez
 * 
 * @param event 
 * @return CommandCode* 
 */
CommandCode *event_get_commands(Event *event);

/**
 * @brief gets the number of valid commands
 * @author Daniel Gómez
 * 
 * @param event 
 * @return int 
 */
int event_get_cmd_count(Event *event);

/**
 * @brief gets if the event is removed from manager when triggered
 * @author Daniel Gómez
 * 
 * @param event 
 * @return true 
 * @return false 
 */
bool event_get_removeOnTrigger(Event *event);

/**
 * @brief Gets pointer to auxiliary data use when a event is triggered
 * @author Daniel Gómez
 * 
 * @param event 
 * @return char* 
 */
char *event_get_aux_data(Event *event);

/*----------EVENT MANAGER----------*/

/**
 * @brief Creates a event manager
 * @author Daniel Gómez
 * 
 * @return EventManager* 
 */
EventManager *event_manager_create();

/**
 * @brief Frees memory in use by event manager
 * @author Daniel Gómez
 * 
 * @param eventManager 
 */
void event_manager_destroy(void *eventManager);

/**
 * @brief Tries to add an event to the manager
 * @author Daniel Gómez
 * 
 * @param manager 
 * @param event 
 * @return Status 
 */
Status event_manager_add_event(EventManager *manager, Event *event);

/**
 * @brief Tries to remove an event form the manager
 * @author Daniel Gómez
 * 
 * @param manager 
 * @param event 
 * @return Status 
 */
Status event_manager_remove_event(EventManager *manager, Event *event);

/**
 * @brief Gets the number of events contained by the manager
 * @author Daniel Gómez
 * 
 * @param manager 
 * @return long 
 */
long event_manager_get_event_count(EventManager *manager);

/**
 * @brief Gets an event at a certain index
 * @author Daniel Gómez
 * 
 * @param manager 
 * @param index 
 * @return Event* 
 */
Event *event_manager_get_event(EventManager *manager, long index);

#endif