/**
 * @brief It defines common types for the whole project
 *
 * @file types.h
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef TYPES_H
#define TYPES_H

#define NO_NAME ""                  /*!< String that represents an empty name string*/

#define WORD_SIZE 700               /*!< Maximum word size */
#define NO_ID -1                    /*!< ERROR ID value */
#define UNDEFINED_ID 0              /*!< Value of a non assigned ID, usefull to not confuse errors with intended behaviour*/
#define DIRECTION_NUMBER 8          /*!< Number of directions implemented on the game*/

#define NPC_MAX_FOLLOWERS (NPC_MAX_ALLIES - 1)    /*!< Maximum number of followers*/
#define NPC_MAX_ALLIES 4            /*!< Maximum number of ally NPCs in a combat and in general(relative to the player)*/
#define NPC_MAX_ENEMIES 4           /*!< Maximum number of enemy NPCs in a combat*/
#define COMBAT_MAX_ENTITIES 10       /*!< Maximum number of entities in a combat*/

#define BASE_DAMAGE_MINIMUM 10      /*!< Minimum damage an entity can do if attack is succesful*/
#define STRENGTH_DAMAGE_MULTIPLIER 1.5  /*!< Rate at which the strength affects the base damage*/
#define DEFENSE_REDUCED_DAMAGE_PERCENTAGE 1 /*!< Rate at which the defense reduces the damage of an incoming attack*/

#define MAX_STRENGTH 30             /*!< Maximum level where the strength takes effect*/
#define MAX_DEFENSE 40              /*!< Maximum level where the defense takes effect*/

#define LEVELING_DIFFICULTY_MULTIPLIER 2    /*!< Defines the rate at which the leveling difficulty increases per level up*/
#define BASE_XP_QUANTITY 100                /*!< Base XP on level 1*/
#define SKILL_POINTS_PER_LEVEL 1            /*!< Number of skill points an entity gets per level up*/
#define XP_MINIMUM 1                        /*!< Minimum XP a dead enemy gives*/
#define XP_MULT 10                          /*!< Multiplier for the stats to add XP when an enemy dies*/

#define MONEY_MIN 10                        /*!< Minimum amount of money an enemy can yield when defeated*/


#define N_GAME_STATES 6             /*!< Number of game states*/
#define MAX_SKILLS_ENTITY 5         /*!< Maximum number of skils per entity*/

#define LINE_LENGTH 100              /*!< Max length of a line of dialogue*/
#define LINE_MAX 4                  /*!< Maximum number of lines in a dialogue*/

/**
 * @brief Id typedef
 */
typedef long Id;

/**
 * @brief Status enum that admits ERROR or OK
 */
typedef enum { ERROR, OK } Status;

/**
 * @brief Enum that says if a turn was valid or not
 */
typedef enum { NOT_VALID, VALID } TurnValidation;

/**
 * @brief Directions of the map: North, South, East, West
 */
typedef enum { N, NE, E, SE, S , SW, W , NW, NO_DIR} Direction;

/**
 * @brief Enum describing the different types of inventory
 */
typedef enum {UNKNOWN_INVENTORY, PLAYER_INVENTORY, NPC_INVENTORY, SPACE_INVENTORY} InventoryType;

/**
 * @brief Enum describing the different game states
 */
typedef enum {ERROR_STATE ,DEFAULT, COMBAT, DIALOGUE, LEVEL_UP_STATE, STORE_STATE}GameState;

#endif
