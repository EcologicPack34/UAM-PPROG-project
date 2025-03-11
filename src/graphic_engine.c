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
#define DESCRIPT_WIDTH 30
#define HELP_BANNER_HEIGHT 1
#define HELP_BANNER_WIDTH 23
#define HELP_HEIGHT 3
#define CMD_HISTORY_HEIGHT 3

#define SPACE_HEIGHT 9
#define SPACE_WIDTH 17

struct _Graphic_engine {
  Area *map, *descript, *banner, *help, *feedback;
};

void graphic_engine_paint_space(Game *game, Space *space, Direction direction,char map[SPACE_HEIGHT + 1][MAP_WIDTH + 33], char spaceStr[SPACE_HEIGHT + 1][SPACE_WIDTH+10]);

void graphic_engine_paint_map(Graphic_engine *ge, Game *game);

void graphic_engine_paint_generalDesc(Graphic_engine *ge, Game *game);

void graphic_engine_paint_commandInfo(Graphic_engine *ge, Game *game);

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

void graphic_engine_paint_game(Graphic_engine *ge, Game *game){
  GameState gameState;

  gameState = game_get_state(game);
  if(!game || !ge) return;

  if(gameState == DEFAULT){
    graphic_engine_paint_map(ge, game);
    graphic_engine_paint_generalDesc(ge, game);
  }

  /* Paint in the banner area */
  screen_area_puts(ge->banner, "    The anthill game ");

  graphic_engine_paint_commandInfo(ge, game);
  
  return;
}

void graphic_engine_paint_map(Graphic_engine *ge, Game *game){
  Space *currentSpace = NULL;
  Space *directionSpace = NULL;
  char space[SPACE_HEIGHT + 1][SPACE_WIDTH+10];
  char map[SPACE_HEIGHT + 1][MAP_WIDTH + 33];

  int i;
  
  strcpy(space[0], "+------%c%c%c------+");
  strcpy(space[1], "|%-3s %6s %4ld|");/*player npcs id*/
  strcpy(space[2], "|%-9s      |");
  strcpy(space[3], "%c%-9s      %c");
  strcpy(space[4], "%c%-9s      %c");
  strcpy(space[5], "%c%-9s      %c");
  strcpy(space[6], "|%-9s      |");
  strcpy(space[7], "|%-15s|");
  strcpy(space[8], "+------%c%c%c------+");
  
  
  screen_area_clear(ge->map);
  
  currentSpace = game_get_space(game, game_get_player_location(game));
  if(!currentSpace) return;
  
  /*Introduces a blanck line at top for simetry*/
  screen_area_puts(ge->map, " ");
  
  /*Prints top spaces*/
  for (i = 0; i < SPACE_HEIGHT; i++)
  {
    map[i][0] = '\00';
  }

  directionSpace = space_get_neighbour(currentSpace, NW);
  graphic_engine_paint_space(game, directionSpace, NW,map, space);

  directionSpace = space_get_neighbour(currentSpace, N);
  graphic_engine_paint_space(game, directionSpace, N, map, space);

  directionSpace = space_get_neighbour(currentSpace, NE);
  graphic_engine_paint_space(game, directionSpace, NE, map, space);
  
  for(i = 0; i < SPACE_HEIGHT; i++){
    screen_area_puts(ge->map, map[i]);
  }

  /*Prints Middle spaces*/
  for (i = 0; i < SPACE_HEIGHT; i++)
  {
    map[i][0] = '\00';
  }
  directionSpace = space_get_neighbour(currentSpace, W);
  graphic_engine_paint_space(game, directionSpace, W,map, space);
  
  graphic_engine_paint_space(game, currentSpace, NO_DIR, map, space);

  directionSpace = space_get_neighbour(currentSpace, E);
  graphic_engine_paint_space(game, directionSpace, E, map, space);
  for(i = 0; i < SPACE_HEIGHT; i++){
    screen_area_puts(ge->map, map[i]);
  }

  /*Prints Bottom spaces*/
  for (i = 0; i < SPACE_HEIGHT; i++)
  {
    map[i][0] = '\00';
  }
  directionSpace = space_get_neighbour(currentSpace, SW);
  graphic_engine_paint_space(game, directionSpace, SW, map, space);

  directionSpace = space_get_neighbour(currentSpace, S);
  graphic_engine_paint_space(game, directionSpace, S, map, space);

  directionSpace = space_get_neighbour(currentSpace, SE);
  graphic_engine_paint_space(game, directionSpace, SE, map, space);
  
  for(i = 0; i < SPACE_HEIGHT; i++){
    screen_area_puts(ge->map, map[i]);
  }

}

void graphic_engine_paint_generalDesc(Graphic_engine *ge, Game *game){
  Inventory *spaceInventory = NULL, *playerInventory = NULL;
  int inventorysize, size;

  int i;

  char str[WORD_SIZE];
  char strAux[WORD_SIZE];
  Space *currentSpace;
  
  screen_area_clear(ge->descript);

  currentSpace = game_get_space(game, game_get_player_location(game));
  if(!currentSpace) return;
  

  playerInventory = entity_get_inventory(player_get_entity(game_get_player(game)));
  inventorysize = inventory_get_size(playerInventory);
  spaceInventory = space_get_inventory(currentSpace);
  
  /*Paints player info*/
  strcpy(str, "Player inventory:");
  screen_area_puts(ge->descript, str);
  if(inventorysize == 0){
    screen_area_puts(ge->descript, "No Objects in Player Inventory");
  }else{
    for(i = 0; i < inventorysize && i < 5; i++){
      inventory_get_object_str_at(playerInventory, str, i);
      screen_area_puts(ge->descript, str);
    }
  }
  screen_area_puts(ge->descript, " ");
  
  /*Paints space inventory info*/
  inventorysize = inventory_get_size(spaceInventory);
  strcpy(str, "Space inventory:");
  screen_area_puts(ge->descript, str);
  if(inventorysize == 0){
    screen_area_puts(ge->descript, "No Objects in Space Inventory");
  }else{
    for(i = 0; i < inventorysize && i < 5; i++){
      inventory_get_object_str_at(spaceInventory, str, i);
      screen_area_puts(ge->descript, str);
    }
  }
  screen_area_puts(ge->descript, " ");

  /*Paints space npcs info*/
  size = space_get_npc_count(currentSpace);
  strcpy(str, "NPCs:");
  screen_area_puts(ge->descript, str);
  for(i = 0; i < size && i < 5; i++){
    strcpy(str, "   ");
    npc_get_str_descr(space_get_NPC_at(currentSpace, i), strAux, i + 1);
    strcat(str,strAux);
    screen_area_puts(ge->descript, str);
  }
  screen_area_puts(ge->descript, " ");

  /*Paints game messages*/
  strcpy(str, "Messages:");
  while(game_log_hasMessage(game)){
    game_get_log_message(game, str);
    screen_area_puts(ge->descript, str);
  }

  
}

void graphic_engine_paint_space(Game *game, Space *space, Direction direction,char map[SPACE_HEIGHT + 1][MAP_WIDTH + 33], char spaceStr[SPACE_HEIGHT + 1][SPACE_WIDTH+10]){
  int i;
  char str[WORD_SIZE];
  char strAux[WORD_SIZE];
  char player[WORD_SIZE];
  Link *link1 = NULL, *link2 = NULL;
  char link1Char, link2Char;

  char** gdesc = NULL;

  if(space){
    
    gdesc = space_get_graphic_description(space);

    /*Determines the state of the top conexion*/
    link1 = space_get_north(space);
    link1Char = '-';
    if(link1){
      if(link_is_locked(link1)) link1Char = '-';
      else if(link_is_adjacent(link1) == false) link1Char = 'o';
      else link1Char = ' ';
    }
    sprintf(str, spaceStr[0], (link1) ? '|' : '-', link1Char, (link1) ? '|' : '-');
    strcat(map[0],str);

    /*Prints Id of space + player + npcs*/
    space_get_NPC_list(space, strAux, 6);
    strcpy(player, entity_get_graphic_description(player_get_entity(game_get_player(game))));
    sprintf(str, spaceStr[1], (direction == NO_DIR) ? player : " ", strAux ,space_get_id(space));
    strcat(map[1],str);

    sprintf(str, spaceStr[2], gdesc[0]);
    strcat(map[2],str);

    /*Determines the state of the lateral conexion between spaces*/
    link1 = space_get_east(space);
    link2 = space_get_west(space);
    link1Char = '|';
    if(link1){
      if(link_is_locked(link1)) link1Char = '|';
      else if(link_is_adjacent(link1) == false) link1Char = 'o';
      else link1Char = ' ';
    }
    link2Char = '|';
    if(link2){
      if(link_is_locked(link2)) link2Char = '|';
      else if(link_is_adjacent(link2) == false) link2Char = 'o';
      else link2Char = ' ';
    }

    sprintf(str, spaceStr[3], (link2) ? '-' : '|', gdesc[1], (link1) ? '-' : '|');
    strcat(map[3],str);
    sprintf(str, spaceStr[4], link2Char, gdesc[2], link1Char);
    strcat(map[4],str);
    sprintf(str, spaceStr[5], (link2) ? '-' : '|', gdesc[3], (link1) ? '-' : '|');
    strcat(map[5],str);


    sprintf(str, spaceStr[6], gdesc[4]);
    strcat(map[6],str);

    /*Prints the list of items in the last row of the space*/
    inventory_get_object_list(space_get_inventory(space), strAux, 1, SPACE_WIDTH - 2);
    sprintf(str, spaceStr[7], strAux);
    strcat(map[7],str);

    /*Determines the state of the top conexion*/
    link1 = space_get_south(space);
    link1Char = '-';
    if (link1){
      if (link_is_locked(link1))
        link1Char = '-';
      else if (link_is_adjacent(link1) == false)
        link1Char = 'o';
      else
        link1Char = ' ';
    }
    sprintf(str, spaceStr[8], (link1) ? '|' : '-', link1Char, (link1) ? '|' : '-');
    strcat(map[8],str);
  }else{
    for (i = 0; i < SPACE_HEIGHT; i++)
    {
      strcat(map[i], "                ");
    }
  }
}


void graphic_engine_paint_commandInfo(Graphic_engine *ge, Game *game){
  char str[WORD_SIZE], straux[WORD_SIZE];
  CommandCode last_cmd = UNKNOWN;
  extern char *cmd_to_str[N_CMD][N_CMDT];

  int i;

  /*Paint command help*/
  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  command_get_list(str);
  screen_area_puts(ge->help, str);

  /*Paints commands*/
  last_cmd = command_get_code(game_get_last_command(game));
  sprintf(str, " %s (%s)", cmd_to_str[last_cmd - NO_CMD][CMDL], cmd_to_str[last_cmd - NO_CMD][CMDS]);
  for(i = 0; i < MAX_CMD_ARGS_NUM; i++){
    sprintf(straux, " %s", *(command_get_arguments(game_get_last_command(game)) + i));
    strcat(str, straux);
  }
  strcat(str, (command_get_status(game_get_last_command(game)) == OK) ? " : OK" : " : ERROR");
  screen_area_puts(ge->feedback, str);

  screen_paint();
  printf("input:> ");
}
