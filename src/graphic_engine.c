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

#define MAP_WIDTH 53                  /*!< Width of the map part*/
#define MAP_HEIGHT 29                 /*!< Height of the map part*/
#define DESCRIPT_WIDTH 67            /*!< Width of the description part*/
#define HELP_BANNER_HEIGHT 1          /*!< Height of the help banner part*/
#define HELP_BANNER_WIDTH 23          /*!< Width of the help banner part*/
#define HELP_HEIGHT 4                 /*!< Height of the help banner*/
#define CMD_HISTORY_HEIGHT 3          /*!< Height of the command history*/

#define SPACE_HEIGHT 9                /*!< Height of a space*/
#define SPACE_WIDTH 17                /*!< Width of a space*/

#define COMBAT_OFFSET 10              /*!< Offset needed on combat mode*/
#define HEALTH_BAR_WIDTH 5            /*!< Length of the health bar*/

#define MAX_PRINT_PLAYER_INVENTORY 7  /*!< Maximum number of items printed from the player inventory*/
#define MAX_PRINT_INVENTORY 7         /*!< Maximum number of items printed from the space inventory*/
#define MAX_PRINT_SPACE_NPCS 5        /*!< Maximum number of NPCs printed from the space*/

/**
 * @brief Graphic engine internal struct
 */
struct _Graphic_engine {
  Area *map;        /*!< Area with the spaces and general info*/
  Area *descript;   /*!< Area with descriptions*/
  Area *banner;     /*!< Banner of the game*/
  Area *help;       /*!< Help descriptions*/
  Area *feedback;   /*!< Feedback of the graphic engine*/
};

/**
 * @brief Paints a string with newlines in a designated screen area
 * @author Daniel Gómez
 * 
 * @param area where the print is done
 * @param string string to be printed
 */
void graphic_engine_newline_print(Area *area, char *string);

/**
 * @brief Paints a space in map area
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @param space space to print
 * @param direction direction where the space is located from the center
 * @param map char array with the map
 * @param spaceStr graphical description of the space
 */
void graphic_engine_paint_space(Game *game, Space *space, Direction direction,char map[SPACE_HEIGHT + 1][MAP_WIDTH + 33], char spaceStr[SPACE_HEIGHT + 1][SPACE_WIDTH+10]);

/**
 * @brief Paints map in map area
 * @author Daniel Gómez
 * 
 * @param ge graphic engine struct
 * @param game game struct
 */
void graphic_engine_paint_map(Graphic_engine *ge, Game *game);

/**
 * @brief Paints combat during combat mode
 * @author Daniel Gómez
 * 
 * @param ge graphic engine struct
 * @param game game struct
 */
void graphic_engine_paint_combat(Graphic_engine *ge, Game *game);

/**
 * @brief Paints a general description of the game in description area
 * @author Daniel Gómez
 * 
 * @param ge graphic engine struct
 * @param game game struct
 */
void graphic_engine_paint_generalDesc(Graphic_engine *ge, Game *game);

/**
 * @brief Paints info related to commands in the command area
 * @author Daniel Gómez
 * 
 * @param ge graphic engine struct
 * @param game game struct
 */
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


/*------PUBLIC IMPLEMENTATIONS--------*/

void graphic_engine_paint_game(Graphic_engine *ge, Game *game){
  GameState gameState;
  
  if(!game || !ge) return;
  gameState = game_get_state(game);
  
  if(gameState == DEFAULT){
    graphic_engine_paint_map(ge, game);
    graphic_engine_paint_generalDesc(ge, game);
  }
  else if(gameState == COMBAT){
    graphic_engine_paint_combat(ge, game);
  }
  
  /* Paint in the banner area */
  graphic_engine_paint_commandInfo(ge, game);
  
  return;
}

/*--------PRIVATE IMPLENTATIONS--------*/

void graphic_engine_newline_print(Area *area, char *string){
  char str[WORD_SIZE];
  char *toks = NULL;

  if(!area || !string) return;

  strcpy(str, string);

  toks = strtok(str,"\n");

  while(toks){
    screen_area_puts(area, toks);
    toks = strtok(NULL,"\n");
  }
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
  Entity *entityplayer = NULL;
  

  int i, printedNPCs = 0;

  char str[WORD_SIZE] = "";
  char strAux[WORD_SIZE] = "";
  Space *currentSpace;
  bool spaceDiscovered = false;

  int ability_count = 0;



  screen_area_clear(ge->descript);

  currentSpace = game_get_space(game, game_get_player_location(game));
  if(!currentSpace) return;
  
  spaceDiscovered = space_get_isDiscovered(currentSpace);

  playerInventory = entity_get_inventory(player_get_entity(game_get_player(game)));
  inventorysize = inventory_get_size(playerInventory);
  spaceInventory = space_get_inventory(currentSpace);
  
  /*Paints player info*/
  strcpy(str, "Player:");
  screen_area_puts(ge->descript, str);

  player_get_str_desc(game_get_player(game), strAux);
  strcpy(str, "   ");
  strcat(str, strAux);
  screen_area_puts(ge->descript, str);

  strcpy(str, "Player inventory:");
  screen_area_puts(ge->descript, str);

  if(inventorysize == 0){
    screen_area_puts(ge->descript, "    No Objects in Player Inventory");
  }
  else{
    for(i = 0; i < inventorysize && i < MAX_PRINT_PLAYER_INVENTORY; i++){
      inventory_get_object_str_at(playerInventory, strAux, i);
      
      strcpy(str,"    ");
      strcat(str, strAux);

      screen_area_puts(ge->descript, str);
    }
  }
  screen_area_puts(ge->descript, " ");

  entityplayer = player_get_entity(game_get_player(game));
  ability_count = entity_get_n_abilities(entityplayer);
  screen_area_puts(ge->descript, "Player abilities:");
  if(ability_count <= 0) 
    screen_area_puts(ge->descript, "Player does not have abilities");

  for(i = 0; i < ability_count; i++){
    sprintf(str, "%d. ", i + 1);
    strcat(str, entity_get_ability_name_at(entityplayer, i));
    sprintf(strAux, " | Cd: %d", ability_get_cooldown_count(entity_get_ability_at(entityplayer, i)));
    strcat(str, strAux);
    screen_area_puts(ge->descript, str);
  }

  screen_area_puts(ge->descript, " ");

  
  /*Paints space inventory info*/
  inventorysize = inventory_get_size(spaceInventory);
  strcpy(str, "Space inventory:");
  screen_area_puts(ge->descript, str);

  if(!spaceDiscovered){
    screen_area_puts(ge->descript, "Space hasn't been explored");
  }else{
    if(inventorysize == 0){
      screen_area_puts(ge->descript, "No Objects in Space Inventory");
    }else{
      for(i = 0; i < inventorysize && i < MAX_PRINT_INVENTORY; i++){
        inventory_get_object_str_at(spaceInventory, str, i);
        screen_area_puts(ge->descript, str);
      }
    }
  }

  screen_area_puts(ge->descript, " ");

  /*Paints space npcs info*/
  size = space_get_npc_count(currentSpace);
  screen_area_puts(ge->descript, "NPCs:");

  if(!spaceDiscovered){
    screen_area_puts(ge->descript, "Space hasn't been explored");
  }else{
    for(i = 0; i < size && printedNPCs < MAX_PRINT_SPACE_NPCS; i++){
      if(entity_is_dead(npc_get_entity(space_get_NPC_at(currentSpace, i)))){
        continue;
      }
  
      strcpy(str, "   ");
      npc_get_str_descr(space_get_NPC_at(currentSpace, i), strAux, i + 1);
      strcat(str,strAux);
      screen_area_puts(ge->descript, str);
      printedNPCs++;
    }
  }

  screen_area_puts(ge->descript, " ");

  /*Paints game messages*/
  if(game_log_hasMessage(game)){
    strcpy(str, "Messages:");
    screen_area_puts(ge->descript, str);
  }
  while(game_log_hasMessage(game)){
    game_get_log_message(game, str);
    //screen_area_puts(ge->descript, str);
    graphic_engine_newline_print(ge->descript,str);
  }

  
}

void graphic_engine_paint_commandInfo(Graphic_engine *ge, Game *game){
  char str[WORD_SIZE] = "";
  Command *last_cmd = UNKNOWN;

  Entity *player;

  /*Temporal player active*/
  player = player_get_entity(game_get_player(game));
  sprintf(str, "   Player: %ld,%s", entity_get_id(player), entity_get_name(player));
  screen_area_puts(ge->banner, str);

  /*Paint command help*/
  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  /*Gets command list*/
  command_get_list(game_get_last_command(game), str, game_get_state(game), false);
  screen_area_puts(ge->help, str);
  
  /*Paints commands*/
  last_cmd = game_get_last_command(game);
  str[0] = 0;
  command_get_as_string(last_cmd, str);
  screen_area_puts(ge->feedback, str);
  
  screen_paint(game_get_player_color(game));
  printf("input:> ");
}

void graphic_engine_paint_combat(Graphic_engine *ge, Game *game){
  char str[WORD_SIZE] = "";
  char strAux[WORD_SIZE] = "";
  char spacing[WORD_SIZE] = "";
  Combat *combat = NULL;
  int i, j;
  int div;
  int bar;
  char hbarChar;

  int heightDiv;

  int enemy_count = 0, ally_count = 0;
  Stats *stats = NULL;
  Stats *playerStats = NULL;

  Inventory *playerInventory = NULL;
  int inventorysize;

  Entity *entityplayer;
  int ability_count = 0;

  heightDiv = (MAP_HEIGHT - 4 + 1)/3;

  combat = game_get_combat(game);
  screen_area_clear(ge->map);
  if(!combat) return;

  enemy_count = combat_get_enemies_count(combat);
  ally_count = combat_get_allies_count(combat);

  /*Paints some blanck lines so it isnt at the top*/
  for (i = 0; i < heightDiv; i++)
  {
    screen_area_puts(ge->map, " ");
  }
  
  stats = combat_get_enemies_stats(combat);

  div = (MAP_WIDTH - COMBAT_OFFSET) / (enemy_count + 1);
  /*spacing for gdesc*/
  for (i = 0; i < div - (ENTITY_GRAPHIC_LENGTH)/2; i++)
  {
    strcat(spacing, " ");
  }

  /*prints gdesc of enemies*/
  strcat(str, spacing);
  if(enemy_count == 1) strcat(str, "  ");
  strcat(spacing, "  ");/*fixes health bars not centered, i dont know why*/
  for (i = 0; i < enemy_count; i++)
  {
    strcat(str, entity_get_graphic_description(stats[i].entity));
    if(i != enemy_count -1) strcat(str, spacing);
  }
  screen_area_puts(ge->map, str);

  /*spacing for health*/
  spacing[0] = 0;
  str[0] = 0;
  div = (MAP_WIDTH - COMBAT_OFFSET - (HEALTH_BAR_WIDTH + 2)*enemy_count) / (enemy_count + 1);
  for (i = 0; i < div; i++) /*Add 2 spaces for number + '.'*/
  {
    strcat(spacing, " ");
  }
  strcat(str, spacing);
  if(enemy_count == 1) strcat(str, "  ");
  for (i = 0; i < enemy_count; i++)
  {
    //sprintf(strAux, "[%.2lf]", stats[i].stats.health);
    sprintf(strAux, "%d.", i+1);
    strcat(str, strAux);
    strcat(str, "[");
    bar = stats[i].stats.health / entity_get_max_health(stats[i].entity) * HEALTH_BAR_WIDTH;
    if(bar == 0){
      hbarChar = '_';
    }else{
      hbarChar = '=';
    }
    for (j = 0; j < HEALTH_BAR_WIDTH; j++)
    {
      if(j <= bar) strncat(str, &hbarChar, 1);
      else strcat(str, "_");
    }
    strcat(str, "]");
    
    if(i != enemy_count -1) strcat(str, spacing);
  }
  screen_area_puts(ge->map, str);

  /*paints some more spacing*/
  for (i = 0; i < heightDiv; i++)
  {
    screen_area_puts(ge->map, " ");
  }

  spacing[0] = 0;
  str[0] = 0;

  div = (MAP_WIDTH - COMBAT_OFFSET) / (ally_count + 1);

  stats = combat_get_allies_stats(combat);
  playerStats = combat_get_player_stats(combat);

  for (i = 0; i < div - (ENTITY_GRAPHIC_LENGTH)/2; i++)
  {
    strcat(spacing, " ");
  }
  strcat(str, spacing);
  strcat(spacing, "  ");/*fixes health bars not centered, i dont know why*/

  /*prints player gdesc*/
  if(ally_count == 1) strcat(str, "  ");
  strcat(str, entity_get_graphic_description(playerStats->entity));
  strcat(str, spacing);

  /*prints allies desc*/
  for (i = 1; i < ally_count; i++)
  {
    strcat(str, entity_get_graphic_description(stats[i].entity));
    if(i != ally_count - 1) strcat(str, spacing);
  }
  screen_area_puts(ge->map, str);

  /*health bars*/
  spacing[0] = 0;
  str[0] = 0;
  for (i = 0; i < div - (HEALTH_BAR_WIDTH + 1)/2; i++)
  {
    strcat(spacing, " ");
  }
  strcat(str, spacing);
  if(ally_count == 1) strcat(str, "  ");

  /*player health bar*/
  strcat(str, "[");
  bar = playerStats->stats.health / entity_get_max_health(playerStats->entity) * HEALTH_BAR_WIDTH;
  if(bar == 0){
    hbarChar = '_';
  }else{
    hbarChar = '=';
  }
  for (j = 0; j < HEALTH_BAR_WIDTH; j++)
  {
    if(j <= bar) strncat(str, &hbarChar, 1);
    else strcat(str, "_");
  }
  strcat(str, "]");
  
  strcat(str, spacing);
  /*allies health bar*/
  for (i = 1; i < ally_count; i++)
  {
    //sprintf(strAux, "[%.2lf]", stats[i].stats.health);
    strcat(str, "[");
    bar = stats[i].stats.health / entity_get_max_health(stats[i].entity) * HEALTH_BAR_WIDTH;
    if(bar == 0){
      hbarChar = '_';
    }else{
      hbarChar = '=';
    }
    for (j = 0; j < HEALTH_BAR_WIDTH; j++)
    {
      if(j <= bar) strncat(str, &hbarChar, 1);
      else strcat(str, "_");
    }
    strcat(str, "]");
    
    if(i != ally_count - 1) strcat(str, spacing);
  }
  screen_area_puts(ge->map, str);

  /*paints description*/
  screen_area_clear(ge->descript);
  strcpy(str, "Player:");
  screen_area_puts(ge->descript, str);

  strcpy(str, "   ");
  strcat(str, entity_get_graphic_description(playerStats->entity));
  sprintf(strAux, ": Health: %.1lf/%.1lf", playerStats->stats.health, playerStats->stats.maxhealth);
  strcat(str, strAux);
  screen_area_puts(ge->descript, str);

  if(ally_count > 1){
    strcpy(str, "Allies:");
    screen_area_puts(ge->descript, str);
    stats = combat_get_allies_stats(combat);
    for (i = 1; i < ally_count; i++)
    {
      strcpy(str, "   ");
      strcat(str, entity_get_graphic_description(stats[i].entity));
      sprintf(strAux, ": Health: %.1lf/%.1lf", stats[i].stats.health, stats[i].stats.maxhealth);
      strcat(str, strAux);
      screen_area_puts(ge->descript, str);
    }
  }
  
  strcpy(str, "Enemies:");
  screen_area_puts(ge->descript, str);
  stats = combat_get_enemies_stats(combat);
  for (i = 0; i < enemy_count; i++)
  {
    strcpy(str, "   ");
    strcat(str, entity_get_graphic_description(stats[i].entity));
    sprintf(strAux, ": Health: %.1lf/%.1lf", stats[i].stats.health, stats[i].stats.maxhealth);
    strcat(str, strAux);
    screen_area_puts(ge->descript, str);
  }

  entityplayer = player_get_entity(game_get_player(game));
  ability_count = entity_get_n_abilities(entityplayer);
  screen_area_puts(ge->descript, "Player abilities:");
  if(ability_count <= 0) 
    screen_area_puts(ge->descript, "Player does not have abilities");

  for(i = 0; i < ability_count; i++){
    sprintf(str, "%d. ", i + 1);
    strcat(str, entity_get_ability_name_at(entityplayer, i));
    sprintf(strAux, " | Cd: %d", ability_get_cooldown_count(entity_get_ability_at(entityplayer, i)));
    strcat(str, strAux);
    screen_area_puts(ge->descript, str);
  }

  playerInventory = entity_get_inventory(player_get_entity(game_get_player(game)));
  inventorysize = inventory_get_size(playerInventory);

  strcpy(str, "Player inventory:");
  screen_area_puts(ge->descript, str);

  if(inventorysize == 0){
    screen_area_puts(ge->descript, "    No Objects in Player Inventory");
  }
  else{
    for(i = 0; i < inventorysize && i < MAX_PRINT_PLAYER_INVENTORY; i++){
      inventory_get_object_str_at(playerInventory, strAux, i);
      
      strcpy(str,"    ");
      strcat(str, strAux);

      screen_area_puts(ge->descript, str);
    }
  }
  screen_area_puts(ge->descript, " ");
  /*Paints game messages*/
  if(game_log_hasMessage(game)){
    strcpy(str, "Messages:");
    screen_area_puts(ge->descript, str);
  }
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

  bool spaceDiscovered = false;

  if(space){
    
    gdesc = space_get_graphic_description(space);
    spaceDiscovered = space_get_isDiscovered(space);


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
    sprintf(str, spaceStr[1], (direction == NO_DIR) ? player : " ", (spaceDiscovered == true) ? strAux : " ", space_get_id(space));
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
    sprintf(str, spaceStr[7], (spaceDiscovered == true) ? strAux : " ");
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