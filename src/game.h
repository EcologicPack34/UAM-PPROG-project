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

#include <stdbool.h>

#define MAX_SPACES 100 /*!< Maximum number of spaces on the map */

/**
 * @brief Game struct, defines all the information of the game
 */
typedef struct _Game {
  Id player_location;          /*!< Id of the player location */
  Id object_location;          /*!< Id of the object location */
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
 * @brief Gets the player_location (actual location of the player)
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return player_location from game
 */
Id game_get_player_location(Game *game);

/**
 * @brief Gets the object_location (actual object location)
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return object_location
 */
Id game_get_object_location(Game *game);

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

#pragma endregion

#pragma region SETTERS
/**
 * @brief Sets the player location to the id received as argument
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param id long that saves the location of a space
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status game_set_player_location(Game *game, Id id);


/**
 * @brief Sets the object_location to the received id
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param id long that saves the location of a space
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status game_set_object_location(Game *game, Id id);



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
