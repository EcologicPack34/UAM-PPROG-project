/**
 * @brief It defines the textual graphic engine interface
 *
 * @file graphic_engine.h
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef GRAPHIC_ENGINE_H
#define GRAPHIC_ENGINE_H

#include "game.h"


typedef struct _Graphic_engine Graphic_engine;

/**
 * @brief Creates the screen areas
 * @author Profesores PPROG
 *
 * @return ge with the screen if it went well or ge = NULL if something went wrong
 */
Graphic_engine *graphic_engine_create();

/**
 * @brief Destroys the graphic_engine struct and frees ge
 * @author Profesores PPROG
 *
 * @param ge pointer that saves all screen output
 */
void graphic_engine_destroy(Graphic_engine *ge);

/**
 * @brief Prints the map on screen
 * @author Profesores PPROG
 *
 * @param game struct that saves all the information from game
 * @param ge pointer that saves all screen output
 */
void graphic_engine_paint_game(Graphic_engine *ge, Game *game);

#endif
