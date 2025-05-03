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

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "gl_game_interpreter.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE ("GL TESTING")

/*Proyection matrices*/
//Model matrix is stored in mesh renderer as its local to each object



/**
 * @brief Functions that sets the gl viewport size on resize
 * @author Daniel Gómez
 * 
 * @param window 
 * @param width 
 * @param height 
 */
void frame_buffer_size_callback(GLFWwindow *window, int width, int height);

/**
 * @brief Function that checks all input of a given window
 * @author Daniel Gómez
 * 
 * @param window 
 */
void process_input(GLFWwindow *window);

/**
 * @brief Renders ALL
 * @author Daniel Gómez
 * 
 * @param window 
 */
void draw_viewport(GLFWwindow * window);

/**
 * @brief Starts a context to render
 * @author Daniel Gómez
 * 
 * @return GLFWwindow* 
 */
GLFWwindow *initialize_window();

/**
 * @brief Updates opengl graphics
 * @author Daniel Gómez
 * 
 */
void gl_update_render(GLFWwindow * window, GLInterpreter* gl, Game* game);

/**
 * @brief Initializes game reading from the data file and starts the graphic engine
 * @author Original: Profesores PPROG, Modified By: Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 * @param gengine struct that saves all information related to the graphic engine
 * @param file_name string with the name of the filename with the game information
 * @return 0 if everything goes well or 1 if there was some mistake
 */
int game_loop_init(Game **game, Graphic_engine **gengine, char *file_name, int seed, GLInterpreter **gl);

/**
 * @brief Essential function, receives last command, while the command isn't EXIT
 * or the game has not finished, paints the game, receives user input and updates last_cmd
 * @author Original: Profesores PPROG, Modified By: Maksym Polyak and Daniel Gómez
 *
 * @param game struct that saves all information related to the game
 * @param gengine struct that saves all information related to the graphic engine
 */
void game_loop_run(Game *game, Graphic_engine *gengine, GLFWwindow* window, GLInterpreter *gl);

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
  GLInterpreter *gl = NULL;

  GLFWwindow * window;

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

  window = initialize_window();

  /*Initializes and runs the game */
  if (!game_loop_init(&game, &gengine, argv[1], seed, &gl))
  {
    debug_log(PRINT, "Game Initialized correctly");

    game_loop_run(game, gengine, window, gl);
    game_loop_cleanup(game, gengine);

    debug_log(PRINT, "Game ended");
  }

  /*Frees Debug Log memory*/
  if(debugLog){
    debug_log(PRINT, "Debug global variable destruction");
    debug_destroy(debugLog);
  }

  glfwTerminate();

  return 0;
}


int game_loop_init(Game **game, Graphic_engine **gengine, char *file_name, int seed, GLInterpreter **gl){
  if(seed < 0){
    srand((unsigned) time(NULL));
  }
  else{
    srand((unsigned)seed);
  }

  if((*gl = gl_inter_create()) == NULL){
    fprintf(stderr, "Error while initializing openGL graphics.\n");
    game_destroy(*game);
    return 1;
  }

  /*Load texures*/
  gl_inter_load_space_tex(*gl,"./textures/tempSpace.jpg");
  gl_inter_load_link_tex(*gl,"./textures/tempLink.png");


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

void game_loop_run(Game *game, Graphic_engine *gengine, GLFWwindow *window, GLInterpreter *gl){
  Command *last_cmd;

  if (!gengine)
  {
    return;
  }

  last_cmd = game_get_last_command(game);

  while ((command_get_code(last_cmd) != EXIT) && !glfwWindowShouldClose(window))
  {

    /*Paints graphics on screen*/
    graphic_engine_paint_game(gengine, game);
    gl_update_render(window, gl, game);
    
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

GLFWwindow *initialize_window(){
  GLFWwindow *window = NULL;

  /*Initializes glfw*/
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /*Creates window and sets it as current context*/
  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
  if(!window){
      fprintf(stderr, "ERROR CREATING WINDOW\n");
      glfwTerminate();
      return NULL;
  }
  glfwMakeContextCurrent(window);

  /*GLAD loader*/
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      fprintf(stderr, "FAILED TO INITIALIZE GLAD\n");
      glfwTerminate();
      return NULL;
  }
  /*Tells gl the size and position of window (Position of the lower left corner)*/
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
  
  //glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LESS);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return window;
}

void gl_update_render(GLFWwindow * window, GLInterpreter *gl, Game *game){
  /*Input*/
  process_input(window);

  draw_viewport(window);

  gl_inter_update_graphics(gl, game, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT);

 /*Events and buffer swap*/
  glfwSwapBuffers(window);/*Renders frame buffer*/
  glfwPollEvents();/*Checks if events are trigger(mouse, keyboard, etc) and calls corresponding functions*/
}

void frame_buffer_size_callback(GLFWwindow *window, int width, int height){
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window){
  /*Checks if escape key is pressend and if so closes window*/
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
      glfwSetWindowShouldClose(window, true);
  }
}

void draw_viewport(GLFWwindow * window){
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}