/**
 * @brief It defines the game loop
 *
 * @file game_loop.c
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>

#include "debug_printing.h"

#include "command.h"
#include "game.h"
#include "game_reader.h"
#include "game_actions.h"
#include "graphic_engine.h"

#define DEBUG_FILE_PATH "./debug.log"

int game_loop_init(Game *game, Graphic_engine **gengine, char *file_name);

void game_loop_run(Game game, Graphic_engine *gengine);

void game_loop_cleanup(Game game, Graphic_engine *gengine);

/**
 * @brief Checks the arguments and initializes the game.
 * @author Profesores PPROG
 *
 * @param argc
 * @param argv
 * @return 0 if game execution went well or 1 if arguments check failed
 */
int main(int argc, char *argv[]){
  Game game;
  Graphic_engine *gengine;
  Debug *debugLog;

  /*Checks if num of arguments if correct, if not stops the programm*/
  if (argc < 2)
  {
    fprintf(stderr, "Use: %s <game_data_file>\n", argv[0]);
    return 1;
  }
  /*Initialize the global variable for debug*/
  debugLog = debug_create(DEBUG_FILE_PATH, 1);
  debug_log(DEBUG, "Debug global variable");

  /*Initializes and runs the game */
  if (!game_loop_init(&game, &gengine, argv[1]))
  {
    debug_log(DEBUG, "Game Initialized correctly");

    game_loop_run(game, gengine);
    game_loop_cleanup(game, gengine);

    debug_log(DEBUG, "Game ended");
  }

  /*Frees Debug Log memory*/
  debug_log(DEBUG, "Debug global variable destruction");
  debug_destroy(debugLog);

  return 0;
}

/**
 * @brief Initializes game reading from the data file and starts the graphic engine
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param gengine struct that saves all information related to the graphic engine
 * @return 0 if everything goes well or 1 if there was some mistake
 */
int game_loop_init(Game *game, Graphic_engine **gengine, char *file_name){
  if (game_reader_create_from_file(game, file_name) == ERROR)
  {
    fprintf(stderr, "Error while initializing game.\n");
    return 1;
  }

  if ((*gengine = graphic_engine_create()) == NULL)
  {
    fprintf(stderr, "Error while initializing graphic engine.\n");
    game_destroy(game);
    return 1;
  }

  return 0;
}

/**
 * @brief Essential function, receives last command, while the command isn't EXIT
 * or the game has not finished, paints the game, receives user input and updates last_cmd
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param gengine struct that saves all information related to the graphic engine
 */
void game_loop_run(Game game, Graphic_engine *gengine){
  Command *last_cmd;

  if (!gengine)
  {
    return;
  }

  last_cmd = game_get_last_command(&game);

  while ((command_get_code(last_cmd) != EXIT) && (game_get_finished(&game) == false))
  {
    graphic_engine_paint_game(gengine, &game);
    command_get_user_input(last_cmd);
    game_actions_update(&game, last_cmd);
  }
}

/**
 * @brief Frees game memory and destroys graphic engine
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param gengine struct that saves all information related to the graphic engine
 */
void game_loop_cleanup(Game game, Graphic_engine *gengine){
  game_destroy(&game);
  graphic_engine_destroy(gengine);
}
