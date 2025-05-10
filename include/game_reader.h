/**
 * @brief It defines the game reader public interface
 *
 * It is the module that interacts with data files to get the information of the game running.
 * 
 * @file game_reader.h
 * @author Daniel Gómez Rodríguez
 * @version 0
 * @date 8-02-2025
 * @copyright GNU Public License
 */

#ifndef GAME_READER_H
#define GAME_READER_H

#include "types.h"
#include "game.h"

#define SETTINGS_FILE_PATH "./settings.dat" /*!< constant path to the base settings of the game*/

/**
 * @brief Initializes game, loads the spaces in game and sets the player/object location
 * @author Daniel Gómez Rodríguez and Maksym Polyak
 *
 * @param game struct that saves all information related to the game
 * @param filename string with the name of the data file
 * @param procedural true if game is generated procedurally or false if game is totally
 * loaded from a file
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status game_reader_create_from_file(Game **game, char *filename, bool procedural);

/**
 * @brief Initializes a game from a save file
 * 
 * @param game game struct
 * @param filename filename of the file to load the game
 * @return Status 
 */
Status game_reader_create_from_save_file(Game **game, char *filename);

/**
 * @brief Saves all the data from a game to a file
 * 
 * @param save_file name of the save file to create
 * @param game game struct
 * @return Status 
 */
Status game_reader_create_save_file(char *save_file, Game *game);

#endif