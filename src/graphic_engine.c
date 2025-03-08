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
#include "collection.h"

#define MAP_WIDTH 53
#define MAP_HEIGHT 29
#define DESCRIPT_WIDTH 27
#define HELP_BANNER_HEIGHT 1
#define HELP_BANNER_WIDTH 23
#define HELP_HEIGHT 3
#define CMD_HISTORY_HEIGHT 3

#define SPACE_HEIGHT 9
#define SPACE_WIDTH 17

struct _Graphic_engine {
  Area *map, *descript, *banner, *help, *feedback;
};

void graphic_engine_paint_space(Game *game, Space *space, char map[SPACE_HEIGHT + 1][MAP_WIDTH + 33], char spaceStr[SPACE_HEIGHT + 1][SPACE_WIDTH+10]);

Graphic_engine *graphic_engine_create() {
  static Graphic_engine *ge = NULL;

  if (ge) {
    return ge;
  }
  /*Initializes screen and graphics engine*/
  screen_init(MAP_HEIGHT + HELP_BANNER_HEIGHT + HELP_HEIGHT + CMD_HISTORY_HEIGHT + 4, MAP_WIDTH + DESCRIPT_WIDTH + 3);
  
  
  ge = (Graphic_engine *)malloc(sizeof(Graphic_engine));
  if (ge == NULL) {
    return NULL;
  }
  /*Initializes each area of the display*/
  ge->map = screen_area_init(1,1, MAP_WIDTH, MAP_HEIGHT);
  ge->descript = screen_area_init(1 + MAP_WIDTH + 1, 1, DESCRIPT_WIDTH, MAP_HEIGHT);
  ge->banner = screen_area_init((int)((MAP_WIDTH + DESCRIPT_WIDTH + 1 - HELP_BANNER_WIDTH)/2) , MAP_HEIGHT + 2 , HELP_BANNER_WIDTH, HELP_BANNER_HEIGHT);
  ge->help = screen_area_init(1, MAP_HEIGHT + 1 + 1  + HELP_BANNER_HEIGHT, MAP_WIDTH + DESCRIPT_WIDTH + 1, HELP_HEIGHT);
  ge->feedback = screen_area_init(1, MAP_HEIGHT + 1 + 1+ HELP_BANNER_HEIGHT + 1 + HELP_HEIGHT, MAP_WIDTH + DESCRIPT_WIDTH + 1, CMD_HISTORY_HEIGHT);

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

void graphic_engine_paint_game_v1(Graphic_engine *ge, Game *game){
  Id id_act = NO_ID;
  Space *space_act = NULL;
  Link *south = NULL, *north = NULL, *east = NULL, *west = NULL;
  Inventory *spaceInventory = NULL, *playerInventory = NULL;

  char obj = '\0';
  char locked = '\0';
  char str[WORD_SIZE], straux[WORD_SIZE] = "\00";
  CommandCode last_cmd = UNKNOWN;
  extern char *cmd_to_str[N_CMD][N_CMDT];
  int inventorysize, i, size;

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
    /*Gets the actual spaceinventory */
    spaceInventory = space_get_inventory(game_get_space(game, id_act));
    if (inventory_get_size(spaceInventory) >= 1) /* TEMPORAL IMPLEMENTATION TO SEE IF AT LEAST THERE IS AN OBJECT OR NOT*/
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

  playerInventory = entity_get_inventory(player_get_entity(game_get_player(game)));
  inventorysize = inventory_get_size(playerInventory);

  strcpy(str, "Player inventory:");
  screen_area_puts(ge->descript, str);
  for(i = 0; i < inventorysize && i < 5; i++){
    inventory_get_object_str_at(playerInventory, str, i);
    screen_area_puts(ge->descript, str);
  }

  inventorysize = inventory_get_size(spaceInventory);
  strcpy(str, "Space inventory:");
  screen_area_puts(ge->descript, str);
  for(i = 0; i < inventorysize && i < 5; i++){
    inventory_get_object_str_at(spaceInventory, str, i);
    screen_area_puts(ge->descript, str);
  }

  size = space_get_npc_count(space_act);
  strcpy(str, "NPCs:");
  screen_area_puts(ge->descript, str);
  for(i = 0; i < size && i < 5; i++){
    npc_get_str_descr(space_get_NPC_at(space_act, i), str, i + 1);
    screen_area_puts(ge->descript, str);
  }

  
  
  /*if (((obj_loc = game_get_object_location(game)) != NO_ID) && (obj_loc != INSIDE_INVENTORY)) { TEMPORARILY UNAVAILABLE DUE TO OBJECT REWORK*/
  /*
    sprintf(str, "  Object location:%d", (int)obj_loc);
    screen_area_puts(ge->descript, str);
  } else if(obj_loc == INSIDE_INVENTORY){
    sprintf(str, "  Object location: Player");
    screen_area_puts(ge->descript, str);
  }*/
 
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
  for(i = 0; i < MAX_CMD_ARGS_NUM; i++){
    sprintf(straux, " %s", *(command_get_arguments(game_get_last_command(game)) + i));
    strcat(str, straux);
  }
  screen_area_puts(ge->feedback, str);
 
  /* Dump to the terminal */
  screen_paint();
  printf("input:> ");
}

void graphic_engine_paint_game(Graphic_engine *ge, Game *game){
  Space *currentSpace = NULL;
  Space *directionSpace = NULL;
  GameState gameState;

  char str[WORD_SIZE];
  char space[SPACE_HEIGHT + 1][SPACE_WIDTH+10];
  char map[SPACE_HEIGHT + 1][MAP_WIDTH + 33];

  int i;

  if(!game || !ge) return;

  
  strcpy(space[0], "+------%c%c%c-----+");
  strcpy(space[1], "|              |");
  strcpy(space[2], "|              |");
  strcpy(space[3], "%c              %c");
  strcpy(space[4], "%c              %c");
  strcpy(space[5], "%c              %c");
  strcpy(space[6], "|              |");
  strcpy(space[7], "|              |");
  strcpy(space[8], "+------%c%c%c-----+");
  
  
  screen_area_clear(ge->map);
  
  gameState = game_get_state(game);
  
  currentSpace = game_get_space(game, game_get_player_location(game));
  if(!currentSpace) return;
  
  
  /*Prints top spaces*/
  for (i = 0; i < SPACE_HEIGHT; i++)
  {
    map[i][0] = '\00';
  }
  directionSpace = space_get_neighbour(currentSpace, NW);
  graphic_engine_paint_space(game, directionSpace, map, space);
  directionSpace = space_get_neighbour(currentSpace, N);
  graphic_engine_paint_space(game, directionSpace, map, space);
  directionSpace = space_get_neighbour(currentSpace, NE);
  graphic_engine_paint_space(game, directionSpace, map, space);
  
  for(i = 0; i < SPACE_HEIGHT; i++){
    screen_area_puts(ge->map, map[i]);
  }

  /*Prints Middle spaces*/
  for (i = 0; i < SPACE_HEIGHT; i++)
  {
    map[i][0] = '\00';
  }
  directionSpace = space_get_neighbour(currentSpace, W);
  graphic_engine_paint_space(game, directionSpace, map, space);
  graphic_engine_paint_space(game, currentSpace, map, space);
  directionSpace = space_get_neighbour(currentSpace, E);
  graphic_engine_paint_space(game, directionSpace, map, space);
  for(i = 0; i < SPACE_HEIGHT; i++){
    screen_area_puts(ge->map, map[i]);
  }

  /*Prints Bottom spaces*/
  for (i = 0; i < SPACE_HEIGHT; i++)
  {
    map[i][0] = '\00';
  }
  directionSpace = space_get_neighbour(currentSpace, SW);
  graphic_engine_paint_space(game, directionSpace, map, space);
  directionSpace = space_get_neighbour(currentSpace, S);
  graphic_engine_paint_space(game, directionSpace, map, space);
  directionSpace = space_get_neighbour(currentSpace, SE);
  graphic_engine_paint_space(game, directionSpace, map, space);
  for(i = 0; i < SPACE_HEIGHT; i++){
    screen_area_puts(ge->map, map[i]);
  }


  screen_paint();
  return;
}

void graphic_engine_paint_space(Game *game, Space *space, char map[SPACE_HEIGHT + 1][MAP_WIDTH + 33], char spaceStr[SPACE_HEIGHT + 1][SPACE_WIDTH+10]){
  int i;
  char str[WORD_SIZE];
  
  if(space){

    Vector2 *pos = space_get_position(space);
    printf("Printing %f %f\n", pos->x, pos->y);

    sprintf(str, spaceStr[0], '-', '-', '-');
    strcat(map[0],str);

    strcat(map[1],spaceStr[1]);
    strcat(map[2],spaceStr[2]);

    sprintf(str, spaceStr[3], '|', '|');
    strcat(map[3],str);
    sprintf(str, spaceStr[4], '|', '|');
    strcat(map[4],str);
    sprintf(str, spaceStr[5], '|', '|');
    strcat(map[5],str);


    strcat(map[6],spaceStr[6]);
    strcat(map[7],spaceStr[7]);


    sprintf(str, spaceStr[8], '-', '-', '-');
    strcat(map[8],str);
  }else{
    for (i = 0; i < SPACE_HEIGHT; i++)
    {
      strcat(map[i], "                ");
    }
  }
}
