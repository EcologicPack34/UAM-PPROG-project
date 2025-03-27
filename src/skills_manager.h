/**
 * @file skills_manager.h
 * @author Maksym Polyak
 * @brief This module controls player special interactions with the environment,
 * specially in combat throught the use of skills.
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

typedef struct _Skill Skill;

typedef struct _SkillManager SkillManager;

/**
 * @brief Creates a skill struct by its parameters
 * @author Maksym Polyak
 * 
 * @param id 
 * @param data 
 * @param cd_count 
 * @param cd_length 
 * @return Skill* or NULL if error
 */
Skill *skill_create(Id id, char *data, AbilityType type, Id entityid, bool is_player_skill, int cd_count, int cd_length);

/**
 * @brief Destroys a skill struct
 * @author Maksym Polyak
 * 
 * @param skill
 */
void skill_destroy(void *skill);

/*GETTERS*/
/**
 * @brief Gets the id of the skill
 * @author Maksym Polyak
 * 
 * @param skill 
 * @return Id or NO_ID if error
 */
Id skill_get_id(Skill *skill);

/**
 * @brief Gets the skill string with all the information needed for the action
 * @author Maksym Polyak
 * 
 * @param skill 
 * @return char*  or NULL if error
 */
char *skill_get_data(Skill *skill);

/**
 * @brief Gets the skill type code
 * @author Maksym Polyak
 * 
 * @param skill 
 * @return AbilityType 
 */
AbilityType skill_get_type(Skill *skill);

/**
 * @brief Gets the length of the current cooldown of the skill
 * 
 * @param skill 
 * @return int or -1 if error
 */
int skill_cooldown_count(Skill *skill);

/**
 * @brief Gets the maximum length of the cooldown in turns of a skill
 * @author Maksym Polyak
 * 
 * @param skill 
 * @return int or -1 if error;
 */
int skill_get_cooldown_length(Skill *skill);

/**
 * @brief Gets wheter if a skill is from a player or not
 * @author Maksym Polyak
 * 
 * @param skill 
 * @return true if a player has this skill
 * @return false if players don't have this skill
 */
bool skill_get_is_player_skill(Skill *skill);

/**
 * @brief Gets the entity id associated to the skill,
 * if it is from a player the bool is_player_skill helps find
 * the player, if not then it can be used to look in npcs
 * 
 * @param skill 
 * @return Id or -1 if error
 */
Id skill_get_entityid(Skill *skill);



/*SETTERS*/

/**
 * @brief Sets a skill cooldown count to 0;
 * @author Maksym Polyak
 * 
 * @param skill 
 * @return Status 
 */
Status skill_set_cooldown_to_0(Skill *skill);

/**
 * @brief Sets the cooldown to its length
 * @author Maksym Polyak
 * 
 * @param skill 
 * @return Status 
 */
Status skill_set_cooldown_to_length(Skill *skill);

/**
 * @brief Reduces the cooldown count of a skill by 1
 * @author Maksym Polyak
 * 
 * @param skill 
 * @return Status 
 */
Status skill_reduce_cooldown(Skill *skill);

/**
 * @brief Augments the cooldown count of a skill by 1
 * @author Maksym Polyak
 * 
 * @param skill 
 * @return Status 
 */
Status skill_augment_cooldown(Skill *skill);

/**
 * @brief Gets the skill type from a str using the skilsTags
 * @author Maksym Polyak
 * 
 * @param string 
 * @return AbilityType 
 */
AbilityType skill_type_from_str(char *string);

/* SKILLS MANAGER */

/**
 * @brief Creates a skill manager struct to control cooldowns and skill use
 * @author Maksym Polyak
 * 
 * @return SkillManager* 
 */
SkillManager *skill_manager_create();

/**
 * @brief Destroys a skill manager and frees its skills
 * @author Maksym Polyak
 * 
 * @param sm 
 */
void skill_manager_destroy(SkillManager *sm);

/**
 * @brief Adds a skill to the skill manager collection
 * @author Maksym Polyak
 * 
 * @param sm 
 * @param skill 
 * @return Status 
 */
Status skill_manager_add_skill(SkillManager *sm, Skill *skill);

/**
 * @brief Removes a skill from the skill manager collection
 * @author Maksym Polyak
 * 
 * @param sm 
 * @param skill 
 * @return Status 
 */
Status skill_manager_remove_skill(SkillManager *sm, Skill *skill);

/**
 * @brief Gets the count of skills in the collection
 * @author Maksym Polyak
 * 
 * @param sm 
 * @return long or -1 if error
 */
long skill_manager_get_skill_count(SkillManager *sm);

/**
 * @brief Gets the skill on the index received of the collection on skill manager struct
 * @author Maksym Polyak
 * 
 * @param sm 
 * @param index 
 * @return Skill* or NULL if error
 */
Skill *skill_manager_get_skill_at(SkillManager *sm, long index);

/**
 * @brief IN COMBAT STATE Reduces the cooldown of all the skills by one if they have been used 
 * @author Maksym Polyak
 * 
 * @param sm 
 * @return Status 
 */
Status skill_manager_update_cooldowns(SkillManager *sm);

/**
 * @brief Returns all the skills cooldown to 0
 * @author Maksym Polyak
 * 
 * @param sm 
 * @return Status 
 */
Status skill_manager_reset_cooldowns(SkillManager *sm);

/**
 * @brief Adds a skill to the queue of skills on skill manager
 * DOES NOT COPY THE SKILL STRUCT
 * @author Maksym Polyak
 * 
 * @param sm 
 * @param skill 
 * @return Status
 */
Status skill_manager_use_skill(SkillManager *sm, Skill *skill);

/**
 * @brief Gets the queue of skills to be used of the skill manager
 * @author Maksym Polyak
 * 
 * @param sm 
 * @return Queue * or NULL if error
 */
Queue *skill_manager_get_queue(SkillManager *sm);

#endif