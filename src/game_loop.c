/**
 * @brief It defines the game loop
 *
 * It is the main module, takes care of how the game starts and ends, but also it controls
 * the main game loops, the ones that update and control the game struct in order for the
 * game to work correctly. It is the highest level module in the proyect.
 * 
 * 
 * @file game_loop.c
 * @author Original: Profesores PPROG, Modified By: Daniel Gómez and Maksym Polyak
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "debug_printing.h"

#include "command.h"
#include "game.h"
#include "game_reader.h"
#include "game_actions.h"
#include "graphic_engine.h"
#include "collection.h"
#include "event_actions.h"
#include "ability_actions.h"

/**
 * @brief Initializes game reading from the data file and starts the graphic engine
 * @author Original: Profesores PPROG, Modified By: Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 * @param gengine struct that saves all information related to the graphic engine
 * @param file_name string with the name of the filename with the game information
 * @return 0 if everything goes well or 1 if there was some mistake
 */
int game_loop_init(Game **game, Graphic_engine **gengine, char *file_name, int seed);

/**
 * @brief Essential function, receives last command, while the command isn't EXIT
 * or the game has not finished, paints the game, receives user input and updates last_cmd
 * @author Original: Profesores PPROG, Modified By: Maksym Polyak and Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 * @param gengine struct that saves all information related to the graphic engine
 */
void game_loop_run(Game *game, Graphic_engine *gengine);

/**
 * @brief Frees game memory and destroys graphic engine
 * @author Original: Profesores PPROG, Modified By: Maksym Polyak and Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 * @param gengine struct that saves all information related to the graphic engine
 */
void game_loop_cleanup(Game *game, Graphic_engine *gengine);


/**
 * @brief Checks the arguments and initializes the game.
 * @author Original: Profesores PPROG, Modified By: Daniel Gómez
 *
 * @param argc
 * @param argv
 * @return 0 if game execution went well or 1 if arguments check failed
 */
int main(int argc, char *argv[]){
  Game *game = NULL;
  Graphic_engine *gengine = NULL;
  Debug *debugLog = NULL;
  
  int i;
  int seed = -1;

  /*Checks if num of arguments if correct, if not stops the programm*/
  if (argc < 2)
  {
    fprintf(stderr, "Use: %s <game_data_file>\n", argv[0]);
    return 1;
  }

  for (i = 1; i < argc; i++)
  {
    if(i < (argc - 1) && strcmp(argv[i], "-l") == 0){
      if(argv[i+1][0] == '-'){
        fprintf(stderr, "Invalid format for -l argument. Use \"-l <path>\"");
        return 1;
      }
      debugLog = debug_create(argv[i+1], 1);
      debug_log(DEBUG, "Debug global variable");
    }
    if(i < (argc - 1) && strcmp(argv[i], "-s") == 0){
      if(argv[i+1][0] == '-'){
        fprintf(stderr, "Invalid format for -s argument. Use \"-s <positive number>\"");
        return 1;
      }
      seed = atoi(argv[i+1]);
      if(seed < 0){
        fprintf(stderr, "Invalid format for -s argument. Use \"-s <positive number>\"");
      }
    }
  }


  /*Initializes and runs the game */
  if (!game_loop_init(&game, &gengine, argv[1], seed))
  {
    debug_log(PRINT, "Game Initialized correctly");

    game_loop_run(game, gengine);

    game_reader_create_save_file("save1.dat", game, argv[1]);

    game_loop_cleanup(game, gengine);

    debug_log(PRINT, "Game ended");
  }

  /*Frees Debug Log memory*/
  if(debugLog){
    debug_log(PRINT, "Debug global variable destruction");
    debug_destroy(debugLog);
  }

  return 0;
}


int game_loop_init(Game **game, Graphic_engine **gengine, char *file_name, int seed){
  if(seed < 0){
    srand((unsigned) time(NULL));
  }
  else{
    srand((unsigned)seed);
  }

  if (game_reader_create_from_file(game, file_name) == ERROR)
  {
    fprintf(stderr, "Error while initializing game.\n");
    game_loop_cleanup(*game, *gengine);
    return 1;
  }

  if ((*gengine = graphic_engine_create()) == NULL)
  {
    fprintf(stderr, "Error while initializing graphic engine.\n");
    game_destroy(*game);
    return 1;
  }

  return 0;
}

void game_loop_run(Game *game, Graphic_engine *gengine){
  Command *last_cmd;

  if (!gengine)
  {
    return;
  }

  last_cmd = game_get_last_command(game);

  while ((command_get_code(last_cmd) != EXIT) )
  {

    /*Paints graphics on screen*/
    graphic_engine_paint_game(gengine, game);
    
    /*Checks if game finished before getting new input*/
    if(game_get_finished(game) == true) break;
    
    /*Gets new input and updates game*/
    command_get_user_input(last_cmd);
    game_actions_update(game, last_cmd);
    
    if(game_get_is_turn_valid(game) == VALID){
      /*Triggers event with last command actions*/
      event_actions_trigger_events(game);
      /*Manages the cooldowns of the abilities*/
      ability_actions_manage_cooldowns(game);
    }
  }
}

void game_loop_cleanup(Game *game, Graphic_engine *gengine){
  game_destroy(game);
  graphic_engine_destroy(gengine);
}