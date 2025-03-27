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

#define N_SKILLS 3

/**
 * @brief Enum storing the different types of events 
 */
typedef enum {NO_SKILL, HEAL_SELF, HEAL_ALLY}AbilityType;

typedef struct _Ability Ability;

typedef struct _AbilityManager AbilityManager;

/**
 * @brief Creates a ability struct by its parameters
 * @author Maksym Polyak
 * 
 * @param id 
 * @param data 
 * @param cd_count 
 * @param cd_length 
 * @return Ability* or NULL if error
 */
Ability *ability_create(Id id, char *data, AbilityType type, Id entityid, bool is_player_ability, int cd_count, int cd_length);

/**
 * @brief Destroys a ability struct
 * @author Maksym Polyak
 * 
 * @param ability
 */
void ability_destroy(void *ability);

/*GETTERS*/
/**
 * @brief Gets the id of the ability
 * @author Maksym Polyak
 * 
 * @param ability 
 * @return Id or NO_ID if error
 */
Id ability_get_id(Ability *ability);

/**
 * @brief Gets the ability string with all the information needed for the action
 * @author Maksym Polyak
 * 
 * @param ability 
 * @return char*  or NULL if error
 */
char *ability_get_data(Ability *ability);

/**
 * @brief Gets the ability type code
 * @author Maksym Polyak
 * 
 * @param ability 
 * @return AbilityType 
 */
AbilityType ability_get_type(Ability *ability);

/**
 * @brief Gets the length of the current cooldown of the ability
 * 
 * @param ability 
 * @return int or -1 if error
 */
int ability_get_cooldown_count(Ability *ability);

/**
 * @brief Gets the maximum length of the cooldown in turns of a ability
 * @author Maksym Polyak
 * 
 * @param ability 
 * @return int or -1 if error;
 */
int ability_get_cooldown_length(Ability *ability);

/**
 * @brief Gets wheter if a ability is from a player or not
 * @author Maksym Polyak
 * 
 * @param ability 
 * @return true if a player has this ability
 * @return false if players don't have this ability
 */
bool ability_get_is_player_ability(Ability *ability);

/**
 * @brief Gets the entity id associated to the ability,
 * if it is from a player the bool is_player_ability helps find
 * the player, if not then it can be used to look in npcs
 * 
 * @param ability 
 * @return Id or -1 if error
 */
Id ability_get_entityid(Ability *ability);



/*SETTERS*/

/**
 * @brief Sets a ability cooldown count to 0;
 * @author Maksym Polyak
 * 
 * @param ability 
 * @return Status 
 */
Status ability_set_cooldown_to_0(Ability *ability);

/**
 * @brief Sets the cooldown to its length
 * @author Maksym Polyak
 * 
 * @param ability 
 * @return Status 
 */
Status ability_set_cooldown_to_length(Ability *ability);

/**
 * @brief Reduces the cooldown count of a ability by 1
 * @author Maksym Polyak
 * 
 * @param ability 
 * @return Status 
 */
Status ability_reduce_cooldown(Ability *ability);

/**
 * @brief Augments the cooldown count of a ability by 1
 * @author Maksym Polyak
 * 
 * @param ability 
 * @return Status 
 */
Status ability_augment_cooldown(Ability *ability);

/**
 * @brief Gets the ability type from a str using the skilsTags
 * @author Maksym Polyak
 * 
 * @param string 
 * @return AbilityType 
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
 * @param sm 
 */
void ability_manager_destroy(AbilityManager *sm);

/**
 * @brief Adds a ability to the ability manager collection
 * @author Maksym Polyak
 * 
 * @param sm 
 * @param ability 
 * @return Status 
 */
Status ability_manager_add_ability(AbilityManager *sm, Ability *ability);

/**
 * @brief Removes a ability from the ability manager collection
 * @author Maksym Polyak
 * 
 * @param sm 
 * @param ability 
 * @return Status 
 */
Status ability_manager_remove_ability(AbilityManager *sm, Ability *ability);

/**
 * @brief Gets the count of ability in the collection
 * @author Maksym Polyak
 * 
 * @param sm 
 * @return long or -1 if error
 */
long ability_manager_get_ability_count(AbilityManager *sm);

/**
 * @brief Gets the ability on the index received of the collection on ability manager struct
 * @author Maksym Polyak
 * 
 * @param sm 
 * @param index 
 * @return Ability* or NULL if error
 */
Ability *ability_manager_get_ability_at(AbilityManager *sm, long index);

/**
 * @brief IN COMBAT STATE Reduces the cooldown of all the ability by one if they have been used 
 * @author Maksym Polyak
 * 
 * @param sm 
 * @return Status 
 */
Status ability_manager_update_cooldowns(AbilityManager *sm);

/**
 * @brief Returns all the ability cooldown to 0
 * @author Maksym Polyak
 * 
 * @param sm 
 * @return Status 
 */
Status ability_manager_reset_cooldowns(AbilityManager *sm);

/**
 * @brief Adds a ability to the queue of ability on ability manager
 * DOES NOT COPY THE SKILL STRUCT
 * @author Maksym Polyak
 * 
 * @param sm 
 * @param ability 
 * @return Status
 */
Status ability_manager_use_ability(AbilityManager *sm, Ability *ability);

/**
 * @brief Gets the queue of ability to be used of the ability manager
 * @author Maksym Polyak
 * 
 * @param sm 
 * @return Queue * or NULL if error
 */
Queue *ability_manager_get_queue(AbilityManager *sm);

#endif