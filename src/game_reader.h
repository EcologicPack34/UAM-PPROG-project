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

/**
 * @brief Initializes game, loads the spaces in game and sets the player/object location
 * @author Daniel Gómez Rodríguez and Maksym Polyak
 *
 * @param game struct that saves all information related to the game
 * @param filename string with the name of the data file
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status game_reader_create_from_file(Game *game, char *filename);

#endif