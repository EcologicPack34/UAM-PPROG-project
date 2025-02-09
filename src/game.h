/**
 * @brief It defines the game interface
 *
 * @file game.h
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef GAME_H
#define GAME_H

#include "command.h"
#include "space.h"
#include "types.h"
#include "player.h"

#include <stdbool.h>

#define MAX_SPACES 100 /*!< Maximum number of spaces on the map */

/**
 * @brief Game struct, defines all the information of the game
 */
typedef struct _Game {
  Player *player;              /*!< Contains all the information related to the player */
  Object *object;              /*!< Contains all the information related to the object */
  Space *spaces[MAX_SPACES];   /*!< Array with all the spaces of the map */
  int n_spaces;                /*!< int with the number of spaces on *spaces */
  Command *last_cmd;           /*!< string with the last command */
  bool finished;               /*!< bool that determines if the game has finished*/
} Game;

/**
 * @brief Initializes game struct to the corresponding 0
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status game_create(Game *game);

/**
 * @brief Frees corresponding memory from spaces and frees command
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status game_destroy(Game *game);

/**
 * @brief Prints the ingame screen
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 */
void game_print(Game *game);

/**
 * @brief Converts Game structure into a string that can be later used in, for example, saving the game
 * 
 * @param game Struct to be converted to string
 * @return todavia no se como implementarlo
 */
char* game_to_string(Game * game);

#pragma region GETTERS

/**
 * @brief 
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param id long that saves the location of a space
 * @return *space if everything goes well or NULL if there was some mistake
 */
Space *game_get_space(Game *game, Id id);

/**
 * @brief Checks the game->spaces[position] and returns the ID of that position on the array
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param position int that saves position in the array of spaces of the struct game
 * @return Id of the position if everything goes well or NO_ID if there was some mistake
 */
Id game_get_space_id_at(Game *game, int position);

/**
 * @brief Gets the total number of spaces on game
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return n_spaces if everything goes well or -1 if there was a mistake
 */
int game_get_n_spaces(Game *game);

/**
 * @brief Gets the player pointer of the game struct
 * @author maksym Polyak
 *
 * @param game struct that saves all information related to the game
 * @return player pointer from game or NULL if there was a mistake
 */
Player* game_get_player(Game *game);

/**
 * @brief Gets the object pointer of the game struct
 * @author Maksym Polyak
 *
 * @param game struct that saves all information related to the game
 * @return object pointer from game or NULL if there was a mistake
 */
Object* game_get_object(Game *game);

/**
 * @brief Gets the last command from the game struct
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return last command
 */
Command* game_get_last_command(Game *game);

/**
 * @brief Gets the game state
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return state of game (finished if 1, not finished if 0)
 */
bool game_get_finished(Game *game);

/**
 * @brief Gets the player location
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return player location id or -1 if there was a mistake
 */
Id game_get_player_location(Game *game);

/**
 * @brief Gets the object location
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return object location id or -1 if there was a mistake
 */
Id game_get_object_location(Game *game);

#pragma endregion

#pragma region SETTERS

/**
 * @brief Sets the received command on the game struct
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param command struct that saves all information related to the command
 * @return OK if everything goes well
 */
Status game_set_last_command(Game *game, Command *command);

/**
 * @brief Sets the game state to the finished value
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param finished bool that defines the game state
 * @return OK if everything goes well
 */
Status game_set_finished(Game *game, bool finished);

/**
 * @brief Sets the player location
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param id id with the location of the player
 * @return OK if everything went well or ERROR if there was a mistake
 */
Status game_set_player_location(Game *game, Id id);

/**
 * @brief Sets the object location
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param id id with the location of the object
 * @return OK if everything went well or ERROR if there was a mistake
 */
Status game_set_object_location(Game *game, Id id);

#pragma endregion

#pragma region OTHERS

/**
 * @brief Adds the space received to the array of spaces on game
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param space struct that saves all information related to the space
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status game_add_space(Game *game, Space *space);

#pragma endregion

#endif
