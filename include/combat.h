/**
 * @file combat.h
 * @author Maksym Polyak && Daniel Gómez
 * @brief The combat module takes care of the game state COMBAT, checks stats of all
 * entities involved and then creates all the logic of the combat.
 * 
 * Every time a combat starts, the combat struct is created and takes all the information
 * from the space and the player.
 * 
 * Combat is based on turns, player turn and enemies turn, player can choose to run away
 * or attack in their turn.
 * 
 * @version 0.1
 * @date 2025-03-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef COMBAT_H
#define COMBAT_H

#include "collection.h"
#include "debug_printing.h"
#include "queue.h"
#include "entity.h"
#include "npc.h"
#include "space.h"
#include "player.h"
#include "command.h"
#include "attack.h"

/**
 * @brief Struct of the combat module to save the stats of an NPC, public for the graphic_engine
 */
typedef struct {
    EntityStats stats;             /*!< Copy of the stats in order to buff/debuff and not modify the original stats*/
    Entity *entity;                 /*!< Entity from which the stats are saved*/
}Stats;

/**
 * @brief Struct with all the information related to a combat
 */
typedef struct _Combat Combat;

/**
 * @brief Initializes the combat struct
 * @author Maksym Polyak
 * 
 * @param space space where the combat is located
 * @param player player that is involved in the combat
 * @param code last cmd 
 * @return Combat* or NULL if ERROR
 */
Combat *combat_initialize(Space *space, Player *pl1, Player *pl2, CommandCode code, Collection *attacks, int num_players);

/**
 * @brief Ends the combat and frees all the memory related to it
 * @author Maksym Polyak
 * 
 * @param combat combat struct to free
 */
void combat_free(Combat *combat);

/**
 * @brief Updates the combat turns
 * @author Sofía Calvo && Maksym Polyak && Daniel Gómez
 * 
 * @param combat combat to update
 * @param last_cmd last command of the player in the combat
 * @return Status 
 */
Status combat_update(Combat *combat, Command *last_cmd);

/**
 * @brief Creats and reserves memory for the attacks
 * @author Sofía Calvo
 * 
 * @param combat combat to reserve memory for the array of attacks
 * @param num number of attacks needed
 * @return Status 
 */
Status combat_set_num_attacks(Combat *combat, int num);

/**
 * @brief Finalizes the combat and returns the game state to DEFAULT
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat combat to modify
 * @return Status 
 */
Status combat_runaway(Combat *combat);

/**
 * @brief Gets the number of enemies on the combat
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat combat struct
 * @return int or 0 if error;
 */
int combat_get_enemies_count(Combat *combat);

/**
 * @brief Gets the number of allies on the combat - NON IMPLEMENTED THE FUNCTIONALITY
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat combat struct
 * @return int or 0 if error;
 */
int combat_get_allies_count(Combat *combat);

/**
 * @brief Gets the stats pointer of an enemy at the index
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat combat struct
 * @param index index of the enemy
 * @return int or 0 if error;
 */
Stats *combat_get_enemies_stats_at(Combat *combat, int index);

/**
 * @brief Gets the stats pointer of all the enemies
 * @author Maksym Polyak
 * 
 * @param combat combat struct
 * @return Stats* or NULL if error
 */
Stats *combat_get_enemies_stats(Combat *combat);

/**
 * @brief Gets the stats pointer of an ally at the index
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat combat struct
 * @param index index of the ally
 * @return int or 0 if error;
 */
Stats *combat_get_allies_stats_at(Combat *combat, int index);

/**
 * @brief Gets the stats pointer of all the allies
 * @author Maksym Polyak
 * 
 * @param combat combat struct
 * @return Stats* or NULL if error
 */
Stats *combat_get_allies_stats(Combat *combat);

/**
 * @brief Gets the bool with the information of the combat end
 * @author Maksym Polyak && Daniel Gómez
 * 
 * @param combat combat struct
 * @return int or 0 if error;
 */
bool combat_get_isFinished(Combat *combat);

/**
 * @brief Gets the player stats
 * @author Maksym
 * 
 * @param combat combat struct
 * @return Stats* 
 */
Stats *combat_get_player_stats(Combat *combat);

/**
 * @brief Gets the attack in a certain position
 * @author Sofía Calvo
 * 
 * @param combat combat struct
 * @return Attack* 
 */
Attack *combat_get_attack_in_position(Combat *combat, int pos);

/**
 * @brief Gets the attack in a certain position
 * @author Sofía Calvo
 * 
 * @param combat combat struct
 * @param attack attack struct
 * @param pos position in the array
 * @return Status
 */
Status combat_set_attack_in_position(Combat *combat, Attack *attack, int pos);


/**
 * @brief Finds the attack in certain position of an array
 * @author Sofía Calvo
 * 
 * @param combat combat struct
 * @param pos position in the array
 * @return Attack structure
 */
Attack *combat_find_attack_by_name(Combat *cmb, char *name);

/**
 * @brief Gets the number of dead entities of a combat
 * @author Maksym Polyak
 * 
 * @param combat combat struct
 * @return int or -1 if error
 */
int combat_get_dead_entities_num(Combat *combat);

/**
 * @brief Gets the dead entity at the index
 * 
 * @param combat combat struct
 * @param i index
 * @return Entity* or NULL if error or not valid index
 */
Entity *combat_get_dead_entity_at(Combat *combat, int i);

/**
 * @brief Gets the queue with the messages of the combat
 * @author Maksym Polyak
 * 
 * @param combat combat struct
 * @return Queue* or NULL if error
 */
Queue *combat_get_messages(Combat *combat);

int combat_get_turn(Combat *cmb);

Status combat_set_turn(Combat *cmb, int i);

int combat_get_n_players(Combat *cmb);

#endif