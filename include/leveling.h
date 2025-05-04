/**
 * @file leveling.h
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-05-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef LEVELING_H
#define LEVELING_H

#include "types.h"
#include <stdbool.h>

/**
 * @brief Leveling ADT definition
 * 
 */
typedef struct _Leveling Leveling;

/**
 * @brief Creates a leveling struct
 * @author Maksym Polyak
 * 
 * @param xp actual xp
 * @param next_xp xp needed for level up
 * @param level level
 * @param skill_points skill points available
 * @return Leveling* or NULL if error
 */
Leveling *leveling_create(int xp, int next_xp, int level, int skill_points);

/**
 * @brief Frees a leveling struct
 * @author Maksym Polyak
 * 
 * @param leveling leveling struct
 */
void leveling_destroy(Leveling *leveling);

/**
 * @brief Gets the actual XP
 * @author Maksym Polyak
 * 
 * @param leveling leveling struct
 * @return int or -1 if error
 */
int leveling_get_XP(Leveling *leveling);

/**
 * @brief Gets the total of XP needed for the next level
 * @author Maksym Polyak
 * 
 * @param leveling leveling struct
 * @return int or -1
 */
int leveling_get_next_XP(Leveling *leveling);

/**
 * @brief Gets the actual level
 * @author Maksym Polyak
 * 
 * @param leveling leveling struct
 * @return int or -1 if error
 */
int leveling_get_level(Leveling *leveling);

/**
 * @brief Gets available skill points
 * @author Maksym Polyak
 * 
 * @param leveling leveling struct
 * @return int or -1 if error
 */
int leveling_get_skill_points(Leveling *leveling);

/**
 * @brief Sets the XP of a leveling struct
 * @author Maksym Polyak
 * 
 * @param leveling leveling struct
 * @param XP xp to set
 * @return Status 
 */
Status leveling_set_XP(Leveling *leveling, int XP);

/**
 * @brief Sets the XP needed for the next level
 * @author Maksym Polyak
 * 
 * @param leveling leveling struct
 * @param next_XP next xp cap to set
 * @return Status 
 */
Status leveling_set_next_XP(Leveling *leveling, int next_XP);

/**
 * @brief Sets the level
 * @author Maksym Polyak
 * 
 * @param leveling leveling struct
 * @param level level to set
 * @return Status 
 */
Status leveling_set_level(Leveling *leveling, int level);

/**
 * @brief Sets the skill points
 * @author Maksym Polyak
 * 
 * @param leveling leveling struct
 * @param skill_points skill points to set
 * @return Status 
 */
Status leveling_set_skill_points(Leveling *leveling, int skill_points);

/**
 * @brief Checks if the struct has to level up
 * @author Maksym Polyak
 * 
 * @param leveling leveling struct
 * @return true if it has to level up actualXP>nextXP
 * @return false if it doesn't have to level up actualXP<nextXP
 */
bool leveling_check_level_up(Leveling *leveling);

/**
 * @brief Updates the next_XP, sets to 0 actual XP and increases level
 * @author Maksym Polyak
 * 
 * @param leveling leveling struct
 * @return Status 
 */
Status leveling_update_level_up(Leveling *leveling);

#endif