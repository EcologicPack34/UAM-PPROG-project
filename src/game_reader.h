
#ifndef GAME_READER_H
#define GAME_READER_H

#include "types.h"
#include "game.h"

/**
 * @brief Initializes game, loads the spaces in game and sets the player/object location
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param filename string with the name of the data file
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status game_reader_create_from_file(Game *game, char *filename);

#endif