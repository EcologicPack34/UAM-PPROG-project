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

#define NPC_MAX_FOLLOWERS NPC_MAX_ALLIES - 1    /*!< Maximum number of followers*/
#define NPC_MAX_ALLIES 4            /*!< Maximum number of ally NPCs in a combat and in general(relative to the player)*/
#define NPC_MAX_ENEMIES 4           /*!< Maximum number of enemy NPCs in a combat*/
#define COMBAT_MAX_ENTITIES 8       /*!< Maximum number of entities in a combat*/

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

#define LVLUP_TYPES_NUM 3                   /*!< Defines the num of stats you can level up when upgrading level*/
#define LVLUP_STRENGTH_COST 1               /*!< Determines the cost in SP for upgrading strength*/
#define LVLUP_MAGICLEVEL_COST 1             /*!< Determines the cost in SP for upgrading magic level*/
#define LVLUP_MAXHEALTH_COST 1              /*!< Determines the cost in SP for upgrading max health*/
#define LVLUP_STRENGTH_AMOUNT 1             /*!< Determines the amount of strength you gain by leveling up one time strength*/
#define LVLUP_MAGICLEVEL_AMOUNT 1           /*!< Determines the amount of magic level you gain by leveling up one time megic level*/
#define LVLUP_MAXHEALTH_AMOUNT 10           /*!< Determines the amount of max health you gain by leveling up one time max health*/
#define LVLUP_STRENGTH_TEXT "Strength, level up your muscles to destroy every enemy you face!"   /*!< Text of the stat strength on store*/
#define LVLUP_MAX_HEALTH_TEXT "Maximum health, make your enemies life harder! Make yourself harder to kill!"   /*!< Text of the Maximum health on store*/
#define LVLUP_MAGIC_LEVEL_TEXT "Magic level, level up your comprehension of magic to increase the power of your abilities!"   /*!< Text of the stat magic level on store*/
#define LVLUP_TO_ABILITY_STORE_TEXT "To the ability store! Here you can buy your abilities!"    /*!< Text to get on the ability store*/

#define MONEY_MIN 10                        /*!< Minimum amount of money an enemy can yield when defeated*/


#define N_GAME_STATES 5             /*!< Number of game states*/
#define MAX_SKILLS_ENTITY 5         /*!< Maximum number of skils per entity*/

#define LINE_LENGTH 100              /*!< Max length of a line of dialogue*/
#define LINE_MAX 4                  /*!< Maximum number of lines in a dialogue*/

#define STORETYPE_ENUM_NUM 4        /*!< Number of stores implemented including the error one*/
#define STORE_PAGE_MAX 4            /*!< Maximum number of items per page*/

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
typedef enum {ERROR_STATE ,DEFAULT, COMBAT, DIALOGUE, STORE_STATE}GameState;

/**
 * @brief Enum describing differents store available
 * 
 */
typedef enum {ERROR_STORE, ABILITY_STORE, OBJECT_STORE, STAT_STORE}StoreType;

/**
 * @brief Enum describing what type of stats can be leveled up with Skill Points
 * 
 */
typedef enum {NO_LVLUP_STAT, LVLUP_STRENGTH, LVLUP_MAXHEALTH, LVLUP_MAGICLEVEL}LevelUpTypes;

#endif
