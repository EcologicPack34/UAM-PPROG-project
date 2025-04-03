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

#define WORD_SIZE 500               /*!< Maximum word size */
#define NO_ID -1                    /*!< ERROR ID value */
#define UNDEFINED_ID 0              /*!< Value of a non assigned ID, usefull to not confuse errors with intended behaviour*/
#define DIRECTION_NUMBER 8          /*!< Number of directions implemented on the game*/

#define NPC_MAX_FOLLOWERS NPC_MAX_ALLIES - 1    /*!< Maximum number of followers*/
#define NPC_MAX_ALLIES 4            /*!< Maximum number of ally NPCs in a combat and in general(relative to the player)*/
#define NPC_MAX_ENEMIES 4           /*!< Maximum number of enemy NPCs in a combat*/
#define COMBAT_MAX_ENTITIES 8       /*!< Maximum number of entities in a combat*/


#define N_GAME_STATES 5             /*!< Number of game states*/
#define MAX_SKILLS_ENTITY 5         /*!< Maximum number of skils per entity*/

/**
 * @brief Id typedef
 */
typedef long Id;

/**
 * @brief Status enum that admits ERROR or OK
 */
typedef enum { ERROR, OK } Status;

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
typedef enum {ERROR_STATE ,DEFAULT, COMBAT, INVENTORY, DIALOGUE}GameState;

#endif
