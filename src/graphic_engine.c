/**
 * @brief It implements a textual graphic engine
 *
 * @file graphic_engine.c
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "graphic_engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug_printing.h"
#include "command.h"
#include "libscreen.h"
#include "space.h"
#include "types.h"

#define WIDTH_MAP 49   /*!< Total width of the map */
#define WIDTH_DESCRIPTION 29   /*!< Total width of the description */
#define WIDTH_BANNER 23   /*!< Total width of the banner */
#define HEIGHT_MAP 13  /*!< Total height of the map */
#define HEIGHT_HELP_BANNER 1   /*!< Total height of the banner */
#define HEIGHT_HELP 3   /*!< Total height of the help */
#define HEIGHT_FDB 3   /*!< Total height of the cmd history */

struct _Graphic_engine {
  Area *map, *descript, *banner, *help, *feedback;
};

Graphic_engine *graphic_engine_create() {
  static Graphic_engine *ge = NULL;

  if (ge) {
    return ge;
  }
  /*Initializes screen and graphics engine*/
  screen_init(HEIGHT_MAP + HEIGHT_HELP_BANNER + HEIGHT_HELP + HEIGHT_FDB + 4, WIDTH_MAP + WIDTH_DESCRIPTION + 3);
  ge = (Graphic_engine *)malloc(sizeof(Graphic_engine));
  if (ge == NULL) {
    return NULL;
  }
  /*Initializes each area of the display*/
  ge->map = screen_area_init(1, 1, WIDTH_MAP, HEIGHT_MAP);
  ge->descript = screen_area_init(WIDTH_MAP + 2, 1, WIDTH_DESCRIPTION, HEIGHT_MAP);
  ge->banner = screen_area_init((int)((WIDTH_MAP + WIDTH_DESCRIPTION + 1 - WIDTH_BANNER) / 2), HEIGHT_MAP + 2, WIDTH_BANNER, HEIGHT_HELP_BANNER);
  ge->help = screen_area_init(1, HEIGHT_MAP + HEIGHT_HELP_BANNER + 2, WIDTH_MAP + WIDTH_DESCRIPTION + 1, HEIGHT_HELP);
  ge->feedback = screen_area_init(1, HEIGHT_MAP + HEIGHT_HELP_BANNER + HEIGHT_HELP + 3, WIDTH_MAP + WIDTH_DESCRIPTION + 1, HEIGHT_FDB);

  return ge;
}

void graphic_engine_destroy(Graphic_engine *ge) {
  if (!ge) return;

  screen_area_destroy(ge->map);
  screen_area_destroy(ge->descript);
  screen_area_destroy(ge->banner);
  screen_area_destroy(ge->help);
  screen_area_destroy(ge->feedback);

  screen_destroy();
  free(ge);
}

void graphic_engine_paint_game(Graphic_engine *ge, Game *game){
  Id id_act = NO_ID, obj_loc = NO_ID;
  Space *space_act = NULL;
  Link *south = NULL, *north = NULL, *east = NULL, *west = NULL;

  char obj = '\0';
  char locked = '\0';
  char str[WORD_SIZE];
  CommandCode last_cmd = UNKNOWN;
  extern char *cmd_to_str[N_CMD][N_CMDT];

  /* Paint the in the map area */
  screen_area_clear(ge->map);

  id_act = game_get_player_location(game);
  if(id_act != NO_ID){
    space_act = game_get_space(game, id_act);

    north = space_get_north(space_act);
    south = space_get_south(space_act);
    west = space_get_west(space_act);
    east = space_get_east(space_act);

    /* prints space to north*/
    if(north != NULL){

      if(link_is_locked(north))
        locked = '-';
      else
        locked = ' ';

      sprintf(str, "                 | %c |     ", locked);
      screen_area_puts(ge->map, str);
      sprintf(str, "                 |   |     ");
      screen_area_puts(ge->map, str);
      sprintf(str, "                   ^      ");
      screen_area_puts(ge->map, str);
    }
    else{/*Print 3 empty lines so the rendering doesnt move in case there's nothing to the north*/
      sprintf(str, " ");
      screen_area_puts(ge->map, str);
      sprintf(str, " ");
      screen_area_puts(ge->map, str);
      sprintf(str, " ");
      screen_area_puts(ge->map, str);
    }

    /*Prints current space*/
    if (game_get_object_location(game) == id_act)
      obj = '*';
    else
      obj = ' ';

    if (id_act != NO_ID) {

      sprintf(str,"             +-----------+             ");
      screen_area_puts(ge->map, str);

      if(west != NULL){
        if(east != NULL){/*west and east*/
          sprintf(str,"       ----- | m0^   %4d| -----", (int)id_act);
          screen_area_puts(ge->map, str);
          sprintf(str,"         %c  <|     %c     |>  %c ", (link_is_locked(west)) ? '|': ' ', obj, (link_is_locked(east)) ? '|': ' ');
          screen_area_puts(ge->map, str);
          sprintf(str,"       ----- |           | -----");
          screen_area_puts(ge->map, str);
        }
        else{/*west and not east*/
          sprintf(str,"       ----- | m0^   %4d|", (int)id_act);
          screen_area_puts(ge->map, str);
          sprintf(str,"         %c  <|     %c     |", (link_is_locked(west)) ? '|': ' ', obj);
          screen_area_puts(ge->map, str);
          sprintf(str,"       ----- |           |");
          screen_area_puts(ge->map, str);
        }
      }
      else{
        if(east != NULL){/*not west and east*/
          sprintf(str,"             | m0^   %4d| -----", (int)id_act);
          screen_area_puts(ge->map, str);
          sprintf(str,"             |     %c     |>  %c ", obj, (link_is_locked(east)) ? '|': ' ');
          screen_area_puts(ge->map, str);
          sprintf(str,"             |           | -----");
          screen_area_puts(ge->map, str);
        }else{/*not west and not east*/
          sprintf(str,"             | m0^   %4d|", (int)id_act);
          screen_area_puts(ge->map, str);
          sprintf(str,"             |     %c     |", obj);
          screen_area_puts(ge->map, str);
          sprintf(str,"             |           |");
          screen_area_puts(ge->map, str);
        }
      }

      sprintf(str,"             +-----------+             ");
      screen_area_puts(ge->map, str);


      /*sprintf(str, "  +-----------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "  | m0^     %2d|", (int)id_act);
      screen_area_puts(ge->map, str);
      sprintf(str, "  |     %c     |", obj);
      screen_area_puts(ge->map, str);
      sprintf(str, "  |           |");
      screen_area_puts(ge->map, str);
      sprintf(str, "  +-----------+");
      screen_area_puts(ge->map, str);*/
    }

    /*Prints space to south */
    if(south != NULL){

      if(link_is_locked(south))
        locked = '-';
      else
        locked = ' ';

      sprintf(str, "                   v      ");
      screen_area_puts(ge->map, str);
      sprintf(str, "                 |   |     ");
      screen_area_puts(ge->map, str);
      sprintf(str, "                 | %c |     ", locked);
      screen_area_puts(ge->map, str);
    }
  }
  /* Paint in the description area */
  screen_area_clear(ge->descript);
  if (((obj_loc = game_get_object_location(game)) != NO_ID) && (obj_loc != INSIDE_INVENTORY)) {
    sprintf(str, "  Object location:%d", (int)obj_loc);
    screen_area_puts(ge->descript, str);
  } else if(obj_loc == INSIDE_INVENTORY){
    sprintf(str, "  Object location: Player");
    screen_area_puts(ge->descript, str);
  }
 
  /* Paint in the banner area */
  screen_area_puts(ge->banner, "    The anthill game ");
 
   /* Paint in the help area */
  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  command_get_list(str);
  screen_area_puts(ge->help, str);
 
  /* Paint in the feedback area */
  last_cmd = command_get_code(game_get_last_command(game));
  sprintf(str, " %s (%s)", cmd_to_str[last_cmd - NO_CMD][CMDL], cmd_to_str[last_cmd - NO_CMD][CMDS]);
  screen_area_puts(ge->feedback, str);
 
  /* Dump to the terminal */
  screen_paint();
  printf("input:> ");
}

/*
void graphic_engine_paint_game(Graphic_engine *ge, Game *game) {
  Id id_act = NO_ID, id_back = NO_ID, id_next = NO_ID, obj_loc = NO_ID;
  Space *space_act = NULL;
  char obj = '\0';
  char str[WORD_SIZE];
  CommandCode last_cmd = UNKNOWN;
  extern char *cmd_to_str[N_CMD][N_CMDT];

  
  screen_area_clear(ge->map);
  if ((id_act = game_get_player_location(game)) != NO_ID) {
    space_act = game_get_space(game, id_act);
    id_back = space_get_north(space_act);
    id_next = space_get_south(space_act);

    if (game_get_object_location(game) == id_back)
      obj = '*';
    else
      obj = ' ';

    if (id_back != NO_ID) {
      sprintf(str, "  |         %2d|", (int)id_back);
      screen_area_puts(ge->map, str);
      sprintf(str, "  |     %c     |", obj);
      screen_area_puts(ge->map, str);
      sprintf(str, "  +-----------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "        ^");
      screen_area_puts(ge->map, str);
    }

    if (game_get_object_location(game) == id_act)
      obj = '*';
    else
      obj = ' ';

    if (id_act != NO_ID) {
      sprintf(str, "  +-----------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "  | m0^     %2d|", (int)id_act);
      screen_area_puts(ge->map, str);
      sprintf(str, "  |     %c     |", obj);
      screen_area_puts(ge->map, str);
      sprintf(str, "  +-----------+");
      screen_area_puts(ge->map, str);
    }

    if (game_get_object_location(game) == id_next)
      obj = '*';
    else
      obj = ' ';

    if (id_next != NO_ID) {
      sprintf(str, "        v");
      screen_area_puts(ge->map, str);
      sprintf(str, "  +-----------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "  |         %2d|", (int)id_next);
      screen_area_puts(ge->map, str);
      sprintf(str, "  |     %c     |", obj);
      screen_area_puts(ge->map, str);
    }
  }

  
  screen_area_clear(ge->descript);
  if ((obj_loc = game_get_object_location(game)) != NO_ID) {
    sprintf(str, "  Object location:%d", (int)obj_loc);
    screen_area_puts(ge->descript, str);
  }

  screen_area_puts(ge->banner, "    The anthill game ");

  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  command_get_list(str);
  screen_area_puts(ge->help, str);

  last_cmd = command_get_code(game_get_last_command(game));
  sprintf(str, " %s (%s)", cmd_to_str[last_cmd - NO_CMD][CMDL], cmd_to_str[last_cmd - NO_CMD][CMDS]);
  screen_area_puts(ge->feedback, str);
 
  screen_paint();
  printf("input:> ");
}*/

