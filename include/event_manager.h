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
#include <stdio.h>

#define N_EVENTS 14 /*!< Number of events implemented*/

/**
 * @brief Enum storing the different types of events 
 */
typedef enum {NO_EVENT, OBJECT_ON_SPACE, TRIGGER_COMBAT, PLAYER_DEATH, NPC_RAND_MOVE, TRIGGER_EFFECTS, EFFECT_AREA, PLAYER_TURN, UNLOCK_WITH_OBJECT_INV, OBJECTS_FUSION,\
END_ON_KILL_ENEMY, DEATH_DIALOGUE, DIALOGUE_IN_SPACE, DIALOGUE_COMPLETE}EventType;

/**
 * @brief ADT containing basic info about event
 */
typedef struct _Event Event;

/**
 * @brief type that stores events
 */
typedef struct _EventManager EventManager;

/*
    * Event functions
*/

/**
 * @brief Event creation
 * @author Daniel Gómez
 * 
 * @param id id of the event
 * @param type event type
 * @param commands list of commands that are valid
 * @param cmdNum number of commands that trigger the event
 * @param data additional data necesary for event triggering
 * @param removeOnTrigger is the event removed from the list on trigger?
 * @return Event* 
 */
Event *event_create(Id id, EventType type, CommandCode *commands, int cmdNum, char *data, bool removeOnTrigger);

/**
 * @brief Frees memory used by event
 * @author Daniel Gómez
 * 
 * @param event event struct
 */
void event_destroy(void *event);

/*
    * Event getters
*/

/**
 * @brief Gets the type of an event
 * @author Daniel Gómez
 * 
 * @param event event struct
 * @return EventType 
 */
EventType event_get_type(Event *event);

/**
 * @brief Gets the id of an event
 * @author Daniel Gómez
 * 
 * @param event event struct
 * @return Id or NO_ID if error
 */
Id event_get_id(Event *event);

/**
 * @brief Gets the pointer to the list of valid commands
 * @author Daniel Gómez
 * 
 * @param event event struct
 * @return CommandCode* or NULL if error
 */
CommandCode *event_get_commands(Event *event);

/**
 * @brief gets the number of valid commands
 * @author Daniel Gómez
 * 
 * @param event event struct
 * @return int or -1 if error
 */
int event_get_cmd_count(Event *event);

/**
 * @brief gets if the event is removed from manager when triggered
 * @author Daniel Gómez
 * 
 * @param event event struct
 * @return true if the event is removed after being triggered
 * @return false if the event isn't removed ater being triggered or error
 */
bool event_get_removeOnTrigger(Event *event);

/**
 * @brief Gets pointer to auxiliary data use when a event is triggered
 * @author Daniel Gómez
 * 
 * @param event event struct
 * @return char* or NULL if error
 */
char *event_get_aux_data(Event *event);

/*
    * Event general functions
*/

/**
 * @brief Compares 2 events
 * @author Daniel Gómez
 * 
 * @param e1 event struct 1
 * @param e2 event struct 1
 * @return int 
 */
int event_compare(void * e1, void *e2);

/**
 * @brief Gets the event type from a string(see event_manager.c for the valid values)
 * @author Daniel Gómez
 * 
 * @param string string from event_to_str
 * @return EventType 
 */
EventType event_type_from_str(char *string);

/**
 * @brief Checks if a given cmd is within the ones that trigger an event
 * @author Daniel Gómez
 * 
 * @param event event struct
 * @param cmd command struct
 * @return true if valid
 * @return false if not valid or error
 */
bool event_is_cmd_valid(Event *event, Command *cmd);

/**
 * @brief Saves an event on a file stream
 * @author Maksym Polyak
 * 
 * @param event event struct
 * @param fOUT file stream output
 * @return int with num of char printed or -1 if error
 */
int event_save_on_file(Event *event, FILE *fOUT);

/**
 * @brief Creates an event struct from a file
 * @author Maksym Polyak
 * 
 * @param fIN file stream input
 * @return Event* or NULL if error
 */
Event *event_create_from_file(FILE *fIN);

/*
    * Event manager functions
*/

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
 * @param eventManager event manager struct
 */
void event_manager_destroy(void *eventManager);

/**
 * @brief Tries to add an event to the manager
 * @author Daniel Gómez
 * 
 * @param manager event manager struct
 * @param event event struct to be added
 * @return Status 
 */
Status event_manager_add_event(EventManager *manager, Event *event);

/**
 * @brief Tries to remove an event form the manager
 * @author Daniel Gómez
 * 
 * @param manager event manager struct
 * @param event event struct to remove
 * @return Status 
 */
Status event_manager_remove_event(EventManager *manager, Event *event);

/**
 * @brief Gets the number of events contained by the manager
 * @author Daniel Gómez
 * 
 * @param manager event manager struct
 * @return long or -1 if error
 */
long event_manager_get_event_count(EventManager *manager);

/**
 * @brief Gets an event at a certain index
 * @author Daniel Gómez
 * 
 * @param manager event manager struct
 * @param index index where the event is located
 * @return Event* or NULL if not found
 */
Event *event_manager_get_event(EventManager *manager, long index);

/**
 * @brief Saves an event manager struct on a file
 * @author Maksym Polyak
 * 
 * @param manager event manager struct
 * @param fOUT file stream output
 * @return int with num of char printed or -1 if error
 */
int event_manager_save_on_file(EventManager *manager, FILE *fOUT);

/**
 * @brief Reads an event manager struct from a file
 * @author Maksym Polyak
 * 
 * @param manager event manager struct
 * @param fIN file stream input
 * @return Status 
 */
Status event_manager_read_from_file(EventManager *manager, FILE *fIN);

#endif