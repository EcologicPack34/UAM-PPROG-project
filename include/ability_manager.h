/**
 * @file ability_manager.h
 * @author Maksym Polyak
 * @brief This module controls player special interactions with the environment,
 * specially in combat throught the use of ability.
 * @version 0.1
 * @date 2025-03-25
 * 
 * @copyright Copyright (c) 2025
*/

#ifndef SKILLS_MANAGER_H
#define SKILLS_MANAGER_H

#include "types.h"
#include "stdbool.h"
#include "queue.h"

#define N_SKILLS 5 /*!< Number of skills implemented on AbilityType*/

/**
 * @brief Enum storing the different types of events 
 */
typedef enum {NO_SKILL, HEAL_SELF, HEAL_ALLY, MONEY_BAG, LINK_UNLOCK}AbilityType; /*!< Type to determine the effect of an ability*/

/**
 * @brief Ability ADT that contains all the information about the ability
 */
typedef struct _Ability Ability;

/**
 * @brief Ability Manager ADT that contains all abilities and manages their uses
 */
typedef struct _AbilityManager AbilityManager;

/**
 * @brief Creates a ability struct by its parameters
 * @author Maksym Polyak
 * 
 * @param id id of the ability
 * @param data data used by the ability
 * @param name name of the ability(must be unique)
 * @param type type of the ability enum AbilityType
 * @param entityid id of the entity/object that holds the ability
 * @param is_player_ability true if a player has the ability
 * @param is_object_use true if an object has the ability
 * @param cd_count counter to manage how many turns left the entity/object can't use the ability
 * @param cd_length maxmimum length of the cooldown
 * @return Ability* or NULL if error
 */
Ability *ability_create(Id id, char *data, char *name, AbilityType type, Id entityid, bool is_player_ability, bool is_object_use, int cd_count, int cd_length);

/**
 * @brief Destroys a ability struct
 * @author Maksym Polyak
 * 
 * @param ability ability to destroy
 */
void ability_destroy(void *ability);

/*GETTERS*/
/**
 * @brief Gets the id of the ability
 * @author Maksym Polyak
 * 
 * @param ability where the id is taken
 * @return Id or NO_ID if error
 */
Id ability_get_id(Ability *ability);

/**
 * @brief Gets the ability string with all the information needed for the action
 * @author Maksym Polyak
 * 
 * @param ability where the data is taken
 * @return char*  or NULL if error
 */
char *ability_get_data(Ability *ability);

/**
 * @brief Gets the ability type code
 * @author Maksym Polyak
 * 
 * @param ability where the type is taken
 * @return AbilityType 
 */
AbilityType ability_get_type(Ability *ability);

/**
 * @brief Gets the length of the current cooldown of the ability
 * 
 * @param ability where the cooldown count is taken
 * @return int or -1 if error
 */
int ability_get_cooldown_count(Ability *ability);

/**
 * @brief Gets the maximum length of the cooldown in turns of a ability
 * @author Maksym Polyak
 * 
 * @param ability wjere the cooldown length is taken
 * @return int or -1 if error;
 */
int ability_get_cooldown_length(Ability *ability);

/**
 * @brief Gets wheter if a ability is from a player or not
 * @author Maksym Polyak
 * 
 * @param ability where the is_player_ability is taken
 * @return true if a player has this ability
 * @return false if players don't have this ability
 */
bool ability_get_is_player_ability(Ability *ability);

/**
 * @brief Gets wheter an object has the ability or not
 * @author Maksym Polyak
 * 
 * @param ability where the is_object_use is taken
 * @return true if an object has the ability
 * @return false if an object does not have the ability
 */
bool ability_get_is_object_use(Ability *ability);

/**
 * @brief Gets the entity id associated to the ability,
 * if it is from a player the bool is_player_ability helps find
 * the player, if not then it can be used to look in npcs
 * 
 * @param ability where the entityid is taken
 * @return Id or -1 if error
 */
Id ability_get_entityid(Ability *ability);

/**
 * @brief Gets the ability name/description
 * @author Maksym Polyak
 * 
 * @param ability where the name is taken
 * @return char* or NULL if error
 */
char *ability_get_name(Ability *ability);

/*SETTERS*/

/**
 * @brief Sets a ability cooldown count to 0;
 * @author Maksym Polyak
 * 
 * @param ability where cooldown is set to 0
 * @return Status 
 */
Status ability_set_cooldown_to_0(Ability *ability);

/**
 * @brief Sets the cooldown to its length
 * @author Maksym Polyak
 * 
 * @param ability where cooldown is set to max length(inside ability)
 * @return Status 
 */
Status ability_set_cooldown_to_length(Ability *ability);

/**
 * @brief Reduces the cooldown count of a ability by 1
 * @author Maksym Polyak
 * 
 * @param ability where cooldown count is reduced by 1
 * @return Status 
 */
Status ability_reduce_cooldown(Ability *ability);

/**
 * @brief Augments the cooldown count of a ability by 1
 * @author Maksym Polyak
 * 
 * @param ability where cooldown count is augmented by 1
 * @return Status 
 */
Status ability_augment_cooldown(Ability *ability);

/**
 * @brief Compares two abilities, returns 0 if they are equal, >0
 * if e1 > e2 or <0 if e1 < e2 by name
 * @author Maksym Polyak
 * 
 * @param e1 first ability
 * @param e2 second ability
 * @return int 
 */
int ability_compare(void * e1, void *e2);

/**
 * @brief Gets the ability type from a str using the skilsTags
 * @author Maksym Polyak
 * 
 * @param string string with an abilityTags string
 * @return AbilityType or NO_SKILL if error or not found
 */
AbilityType ability_type_from_str(char *string);

/* SKILLS MANAGER */

/**
 * @brief Creates a ability manager struct to control cooldowns and ability use
 * @author Maksym Polyak
 * 
 * @return AbilityManager* 
 */
AbilityManager *ability_manager_create();

/**
 * @brief Destroys a ability manager and frees its ability
 * @author Maksym Polyak
 * 
 * @param sm ability manager to destroy
 */
void ability_manager_destroy(AbilityManager *sm);

/**
 * @brief Adds an ability to be evaluated by the ability_manager
 * @author Maksym Polyak    
 * 
 * @param sm ability manager struct
 * @param ability ability to be added
 * @return Status 
 */
Status ability_manager_add_evaluated_ability(AbilityManager *sm, Ability *ability);

/**
 * @brief Gets the evaluated ability
 * @author Maksym Polyak    
 * 
 * @param sm ability manager struct
 * @return Ability * or NULL if error
 */
Ability *ability_manager_get_evaluated_ability(AbilityManager *sm);

/**
 * @brief Adds a ability to the ability manager collection
 * @author Maksym Polyak
 * 
 * @param sm ability manager
 * @param ability to be added to ability manager
 * @return Status 
 */
Status ability_manager_add_ability_to_cd(AbilityManager *sm, Ability *ability);

/**
 * @brief Removes a ability from the ability manager collection
 * @author Maksym Polyak
 * 
 * @param sm ability manager
 * @param ability to be removed from ability manager
 * @return Status 
 */
Status ability_manager_remove_ability_from_cd(AbilityManager *sm, Ability *ability);

/**
 * @brief Gets the count of ability in the collection
 * @author Maksym Polyak
 * 
 * @param sm ability manager
 * @return long or -1 if error
 */
long ability_manager_get_ability_count(AbilityManager *sm);

/**
 * @brief Gets the ability on the index received of the collection on ability manager struct
 * @author Maksym Polyak
 * 
 * @param sm ability manager
 * @param index where the ability is taken
 * @return Ability* or NULL if error
 */
Ability *ability_manager_get_ability_at(AbilityManager *sm, long index);

/**
 * @brief Tries to use an ability, if succesfull, adds it to the queue of abilities
 * DOES NOT COPY THE ABILITY STRUCT
 * @author Maksym Polyak
 * 
 * @param sm ability manager
 * @param ability to use and add to the queue
 * @return Status
 */
Status ability_manager_use_ability(AbilityManager *sm, Ability *ability);

/**
 * @brief Gets the queue of ability to be used of the ability manager
 * @author Maksym Polyak
 * 
 * @param sm ability manager
 * @return Queue * or NULL if error
 */
Queue *ability_manager_get_queue(AbilityManager *sm);

#endif