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
#include "graphic_description.h"
#include "dialogue.h"

#define MAP_WIDTH 53                  /*!< Width of the map part*/
#define MAP_HEIGHT 29                 /*!< Height of the map part*/
#define DESCRIPT_WIDTH 53            /*!< Width of the description part*/
#define DESCRIPT_WIDTH2 53            /*!< Width of the description part*/
#define HELP_BANNER_HEIGHT 1          /*!< Height of the help banner part*/
#define HELP_BANNER_WIDTH 23          /*!< Width of the help banner part*/
#define HELP_HEIGHT 4                 /*!< Height of the help banner*/
#define CMD_HISTORY_HEIGHT 3          /*!< Height of the command history*/
#define MESSAGES_HEIGHT 4             /*!< Height of the are for displaying messages*/

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
  Area *descript2;  /*!< Area with descriptions*/
  Area *banner;     /*!< Banner of the game*/
  Area *help;       /*!< Help descriptions*/
  Area *feedback;   /*!< Feedback of the graphic engine*/
  Area *messages;   /*!< Area which displays game messages*/
};

/**
 * @brief Paints a string with newlines in a designated screen area
 * but at the start adds the extra string
 * @author Maksym Polyak
 * 
 * @param area where the print is done
 * @param string string to be printed
 */
void graphic_engine_newline_print_with_extra(Area *area, char *string, char *extra);

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
 * @brief Paints level up state
 * @author Maksym Polyak
 * 
 * @param ge graphic engine struct
 * @param game game struct
 */
void graphic_engine_paint_level_up(Graphic_engine *ge, Game *game);

/**
 * @brief Paints the dialogue scene in dialogue mode
 * @author Maksym Polyak
 * 
 * @param ge graphic engine struct
 * @param game game struct
 */
void graphic_engine_paint_dialogue(Graphic_engine *ge, Game *game);

/**
 * @brief Paints the store scene in store mode
 * @author Maksym Polyak
 * 
 * @param ge graphic engine struct
 * @param game game struct
 */
void graphic_engine_paint_store(Graphic_engine *ge, Game *game);

/**
 * @brief Paints a general description of the game in description area
 * @author Daniel Gómez
 * 
 * @param ge graphic engine struct
 * @param game game struct
 */
void graphic_engine_paint_generalDesc(Graphic_engine *ge, Game *game);

/**
 * @brief Paints a general description of the game in description area
 * @author Daniel Gómez
 * 
 * @param ge graphic engine struct
 * @param game game struct
 */
void graphic_engine_paint_playerDesc(Graphic_engine *ge, Game *game);

/**
 * @brief Paints info related to commands in the command area
 * @author Daniel Gómez
 * 
 * @param ge graphic engine struct
 * @param game game struct
 */
void graphic_engine_paint_commandInfo(Graphic_engine *ge, Game *game);

/**
 * @brief Paints game messages
 * @author Daniel Gómez
 * 
 * @param ge 
 * @param game 
 */
void graphic_engine_paint_messages(Graphic_engine *ge, Game *game);

Graphic_engine *graphic_engine_create() {
  static Graphic_engine *ge = NULL;

  if (ge) {
    return ge;
  }
  /*Initializes screen and graphics engine*/
  screen_init(MAP_HEIGHT + HELP_BANNER_HEIGHT + HELP_HEIGHT + CMD_HISTORY_HEIGHT + MESSAGES_HEIGHT + 5, MAP_WIDTH + DESCRIPT_WIDTH + DESCRIPT_WIDTH2 + 4);
  
  
  ge = (Graphic_engine *)malloc(sizeof(Graphic_engine));
  if (ge == NULL) {
    return NULL;
  }
  /*Initializes each area of the display*/
  ge->map = screen_area_init(1,1, MAP_WIDTH, MAP_HEIGHT);

  ge->descript = screen_area_init(1 + MAP_WIDTH + 1, 1, DESCRIPT_WIDTH, MAP_HEIGHT);
  ge->descript2 = screen_area_init(1 + MAP_WIDTH + 1 + DESCRIPT_WIDTH + 1, 1, DESCRIPT_WIDTH2, MAP_HEIGHT + HELP_BANNER_HEIGHT + HELP_HEIGHT + CMD_HISTORY_HEIGHT + MESSAGES_HEIGHT + 3);

  ge->banner = screen_area_init((int)((MAP_WIDTH + DESCRIPT_WIDTH + 1 - HELP_BANNER_WIDTH)/2) , MAP_HEIGHT + 2 , HELP_BANNER_WIDTH, HELP_BANNER_HEIGHT);
  ge->messages = screen_area_init(1, MAP_HEIGHT + 3, MAP_WIDTH + DESCRIPT_WIDTH + 1, MESSAGES_HEIGHT);
  ge->help = screen_area_init(1, MAP_HEIGHT + MESSAGES_HEIGHT + 3 + HELP_BANNER_HEIGHT, MAP_WIDTH + DESCRIPT_WIDTH + 1, HELP_HEIGHT);
  ge->feedback = screen_area_init(1, MAP_HEIGHT + 1 + MESSAGES_HEIGHT + 1 + HELP_BANNER_HEIGHT + 1 + HELP_HEIGHT + 1, MAP_WIDTH + DESCRIPT_WIDTH + 1, CMD_HISTORY_HEIGHT);
  
  area_set_color(ge->descript2, BLACK, WHITE);
  area_set_color(ge->descript, BLACK, WHITE);
  area_set_color(ge->map, BLACK, WHITE);
  area_set_color(ge->banner, BLACK, WHITE);
  area_set_color(ge->messages, BLACK, WHITE);
  area_set_color(ge->feedback, BLACK, WHITE);
  area_set_color(ge->help, BLACK, WHITE);
  return ge;
}

void graphic_engine_destroy(Graphic_engine *ge) {
  if (!ge) return;

  screen_area_destroy(ge->map);
  screen_area_destroy(ge->descript);
  screen_area_destroy(ge->banner);
  screen_area_destroy(ge->help);
  screen_area_destroy(ge->feedback);
  screen_area_destroy(ge->descript2);
  screen_area_destroy(ge->messages);

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
    graphic_engine_paint_playerDesc(ge, game);
  }
  else if(gameState == COMBAT){
    graphic_engine_paint_combat(ge, game);
  } 
  else if(gameState == DIALOGUE){
    graphic_engine_paint_dialogue(ge, game);
    graphic_engine_paint_playerDesc(ge, game);
  } 
  else if(gameState == STORE_STATE){
    graphic_engine_paint_store(ge, game);
    graphic_engine_paint_playerDesc(ge, game);
  }
  
  /* Paint in the banner area */
  graphic_engine_paint_messages(ge, game);
  graphic_engine_paint_commandInfo(ge, game);
  
  return;
}

/*--------PRIVATE IMPLENTATIONS--------*/

void graphic_engine_newline_print_with_extra(Area *area, char *string, char *extra){
  char str[WORD_SIZE], aux_str[WORD_SIZE];
  char *toks = NULL;

  if(!area || !string) return;

  strcpy(str, string);

  toks = strtok(str,"\n");

  while(toks){
    sprintf(aux_str,"%s%s", extra, toks);
    screen_area_puts(area, aux_str);
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
  strcpy(space[3], "%c%-9s   %c  %c");
  strcpy(space[4], "%c%-9s   %c  %c");
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

void graphic_engine_paint_playerDesc(Graphic_engine *ge, Game *game){
  Inventory *playerInventory = NULL;
  int inventorysize;
  Entity *entityplayer = NULL, *follower = NULL;
  Player *player = NULL;
  Leveling *leveling = NULL;
  Equipment *equipment = NULL;
  Object *eqObj = NULL;

  int i;
  int auxInt;

  char tab[5] = "    ";
  char str[WORD_SIZE] = "";
  char strAux[WORD_SIZE] = "";

  int ability_count = 0;
  int followeCount = 0;

  screen_area_clear(ge->descript2);

  playerInventory = entity_get_inventory(player_get_entity(game_get_player(game)));
  inventorysize = inventory_get_size(playerInventory);

  player = game_get_player(game);
  entityplayer = player_get_entity(player);
  leveling = player_get_leveling(player);

  screen_area_puts(ge->descript2, "Player: ");

  strcpy(str, tab);
  strcat(str, entity_get_graphic_description(entityplayer));
  strcat(str, " [RESET]| Name:[YELLOW] ");
  strcat(str, entity_get_name(entityplayer));

  strcat(str, " [RESET]| Loc:[YELLOW] ");
  sprintf(strAux, "%ld", entity_get_location(entityplayer));
  strcat(str, strAux);

  strcat(str, " [RESET]| Money:[YELLOW] ");
  sprintf(strAux, "%d", player_get_money(player));
  strcat(str, strAux);

  screen_area_puts(ge->descript2, str);//Gdes, name, location, money

  strcpy(str, tab);
  strcat(str, "Health: ");

  if(entity_get_health(entityplayer)/entity_get_max_health(entityplayer) <= .5){
    strcat(str, "[YELLOW]");
  }
  if(entity_get_health(entityplayer)/entity_get_max_health(entityplayer) <= .2){
    strcat(str, "[RED]");
  }else{
    strcat(str, "[GREEN]");
  }

  sprintf(strAux, "%.2lf / %.2lf", entity_get_health(entityplayer), entity_get_max_health(entityplayer));
  strcat(str, strAux);

  screen_area_puts(ge->descript2, str);// health

  strcpy(str, tab);
  sprintf(strAux, "Level: [YELLOW]%d [RESET]| Skill Points: [YELLOW]%d [RESET]| XP: [YELLOW]%d / %d", leveling_get_level(leveling), leveling_get_skill_points(leveling) ,leveling_get_XP(leveling), leveling_get_next_XP(leveling));
  strcat(str, strAux);
  
  screen_area_puts(ge->descript2, str);// level, skill points, xp
  
  strcpy(str, tab);
  sprintf(strAux, "BaseD:[YELLOW]%.1lf [RESET]| Strength:[YELLOW]%d [RESET]| Defense:[YELLOW]%d [RESET]", entity_get_baseDamage(entityplayer), entity_get_strength(entityplayer), entity_get_defense(entityplayer));
  strcat(str, strAux);

  screen_area_puts(ge->descript2, str);// damage, strength, defense


  /*INVENTORY*/
  screen_area_puts(ge->descript2, "\nInventory: ");

  if(inventorysize == 0){
    strcpy(str, tab);
    strcat(str, "[RED]No Objects in Player Inventory");
    screen_area_puts(ge->descript2, str);
  }
  else{
    for(i = 0; i < inventorysize && i < MAX_PRINT_PLAYER_INVENTORY; i++){
      inventory_get_object_str_at(playerInventory, strAux, i);
      
      strcpy(str,tab);
      strcat(str, strAux);

      screen_area_puts(ge->descript2, str);
    }
  }

  entityplayer = player_get_entity(game_get_player(game));
  ability_count = entity_get_n_abilities(entityplayer);

  /*ABILITIES*/
  screen_area_puts(ge->descript2, "\nAbilities:");

  if(ability_count <= 0) {
    sprintf(str, "%s[RED]Player has no abilities", tab);
    screen_area_puts(ge->descript2, str);
  }

  for(i = 0; i < ability_count; i++){
    sprintf(str, "%s[YELLOW]%d.[RESET] ", tab, i + 1);
    strcat(str, entity_get_ability_name_at(entityplayer, i));

    auxInt = ability_get_cooldown_length(entity_get_ability_at(entityplayer, i));
    sprintf(strAux, "\n%s%sCooldown:[BLUE]%d[RESET]", tab, tab, auxInt);
    strcat(str, strAux);

    auxInt = ability_get_cooldown_count(entity_get_ability_at(entityplayer, i));
    sprintf(strAux, " | Cooldown-Left:%s%d", (auxInt > 0)? "[RED]":"[GREEN]", auxInt);

    strcat(str, strAux);
    screen_area_puts(ge->descript2, str);
  }

  /*EQUIPMENT */
  screen_area_puts(ge->descript2, "\nEquipment:");
  equipment = player_get_equipment(player);

  auxInt = 0;
  /*equipment checks*/
  eqObj = equipment_get_piece(entityplayer, equipment, HELMET);
  if(eqObj){
    sprintf(strAux, "%sHelmet: ID:[YELLOW]%ld [RESET]| Name: [YELLOW]%s", tab, object_get_id(eqObj), object_get_name(eqObj));
    screen_area_puts(ge->descript2, strAux);
    auxInt++;
  }
  eqObj = equipment_get_piece(entityplayer, equipment, CHEST);
  if(eqObj){
    sprintf(strAux, "%sChestplate: ID:[YELLOW]%ld [RESET]| Name: [YELLOW]%s", tab, object_get_id(eqObj), object_get_name(eqObj));
    screen_area_puts(ge->descript2, strAux);
    auxInt++;
  }
  eqObj = equipment_get_piece(entityplayer, equipment, ARMS);
  if(eqObj){
    sprintf(strAux, "%sArms: ID:[YELLOW]%ld [RESET]| Name: [YELLOW]%s", tab, object_get_id(eqObj), object_get_name(eqObj));
    screen_area_puts(ge->descript2, strAux);
    auxInt++;
  }
  eqObj = equipment_get_piece(entityplayer, equipment, LEG_ARMOR);
  if(eqObj){
    sprintf(strAux, "%sLeg Armor: ID:[YELLOW]%ld [RESET]| Name: [YELLOW]%s", tab, object_get_id(eqObj), object_get_name(eqObj));
    screen_area_puts(ge->descript2, strAux);
    auxInt++;
  }
  eqObj = equipment_get_piece(entityplayer, equipment, SHOES);
  if(eqObj){
    sprintf(strAux, "%sShoes: ID:[YELLOW]%ld [RESET]| Name: [YELLOW]%s", tab, object_get_id(eqObj), object_get_name(eqObj));
    screen_area_puts(ge->descript2, strAux);
    auxInt++;
  }
  eqObj = equipment_get_piece(entityplayer, equipment, TWO_HANDED);
  if(eqObj){
    sprintf(strAux, "%sBoth Hands: ID:[YELLOW]%ld [RESET]| Name: [YELLOW]%s", tab, object_get_id(eqObj), object_get_name(eqObj));
    screen_area_puts(ge->descript2, strAux);
    auxInt++;
  }
  eqObj = equipment_get_piece(entityplayer, equipment, HANDL);
  if(eqObj){
    sprintf(strAux, "%sLeft Hand: ID:[YELLOW]%ld [RESET]| Name: [YELLOW]%s", tab, object_get_id(eqObj), object_get_name(eqObj));
    screen_area_puts(ge->descript2, strAux);
    auxInt++;
  }
  eqObj = equipment_get_piece(entityplayer, equipment, HANDR);
  if(eqObj){
    sprintf(strAux, "%sRight Hand: ID:[YELLOW]%ld [RESET]| Name: [YELLOW]%s", tab, object_get_id(eqObj), object_get_name(eqObj));
    screen_area_puts(ge->descript2, strAux);
    auxInt++;
  }
  if(auxInt == 0){
    sprintf(str, "%s[RED]Nothing equiped", tab);
    screen_area_puts(ge->descript2, str);
  }

  /*Followers*/
  screen_area_puts(ge->descript2, "\nFollowers:");

  followeCount = player_get_follower_num(player);
  if(followeCount == 0){
    sprintf(str, "%s[RED]No followers", tab);
    screen_area_puts(ge->descript2, str);
  }
  for (i = 0, auxInt = 1; i < followeCount; i++)
  {
    follower = player_get_follower_at(player, i);
    if(!follower) continue;
    if(entity_is_dead(follower)) continue;

    sprintf(str, "%s%d. ID: [YELLOW]%ld [RESET]| Name: [YELLOW]%s", tab, auxInt, entity_get_id(follower), entity_get_name(follower));
    screen_area_puts(ge->descript2, str);

    auxInt++;
  }
}

void graphic_engine_paint_generalDesc(Graphic_engine *ge, Game *game){
  Inventory *spaceInventory = NULL, *playerInventory = NULL;
  int inventorysize, size;
  Player *player = NULL;
  Entity *ent = NULL;
  int i, printedNPCs = 0;

  char tab[5] = "    ";
  char str[WORD_SIZE] = "";
  char strAux[WORD_SIZE] = "";
  Space *currentSpace;
  bool spaceDiscovered = false;

  screen_area_clear(ge->descript);

  currentSpace = game_get_space(game, game_get_player_location(game));
  if(!currentSpace) return;
  
  spaceDiscovered = space_get_isDiscovered(currentSpace);

  playerInventory = entity_get_inventory(player_get_entity(game_get_player(game)));
  inventorysize = inventory_get_size(playerInventory);
  spaceInventory = space_get_inventory(currentSpace);

  screen_area_puts(ge->descript, " ");

  
  /*Paints space inventory info*/
  inventorysize = inventory_get_size(spaceInventory);
  strcpy(str, "Space inventory:");
  screen_area_puts(ge->descript, str);

  if(!spaceDiscovered){
    sprintf(str, "%s[RED]Spaces hasn't been explored", tab);
    screen_area_puts(ge->descript, str);
  }else{
    if(inventorysize == 0){
      sprintf(str, "%s[RED]No objects", tab);
      screen_area_puts(ge->descript, str);
    }else{
      for(i = 0; i < inventorysize && i < MAX_PRINT_INVENTORY; i++){
        strcpy(str, tab);
        inventory_get_object_str_at(spaceInventory, strAux, i);
        strcat(str, strAux);
        screen_area_puts(ge->descript, str);
      }
    }
  }

  screen_area_puts(ge->descript, " ");

  /*Paints space npcs info*/
  size = space_get_npc_count(currentSpace);
  screen_area_puts(ge->descript, "NPCs:");

  if(!spaceDiscovered){
    sprintf(str, "%s[RED]Spaces hasn't been explored", tab);
    screen_area_puts(ge->descript, str);
  }else{
    for(i = 0; i < size && printedNPCs < MAX_PRINT_SPACE_NPCS; i++){
      if(entity_is_dead(npc_get_entity(space_get_NPC_at(currentSpace, i)))){
        continue;
      }
  
      strcpy(str, tab);
      npc_get_str_descr(space_get_NPC_at(currentSpace, i), strAux, i + 1);
      strcat(str,strAux);
      screen_area_puts(ge->descript, str);
      printedNPCs++;
    }
  }
  screen_area_puts(ge->descript, "Other Players:");

  size = game_get_n_players(game);
  for (i = 0, printedNPCs = 0; i < size; i++)
  {
    player = game_get_player_at(game, i);
    ent = player_get_entity(player);
    if(player == game_get_player(game) || entity_get_location(ent) != game_get_player_location(game)){
      continue;
    }

    strcpy(str, tab);
    strcat(str, entity_get_graphic_description(ent));
    strcat(str, " [RESET]| Name:[YELLOW] ");
    strcat(str, entity_get_name(ent));
    
    strcat(str, " [RESET]| Health: ");
    
    if(entity_get_health(ent)/entity_get_max_health(ent) <= .5){
      strcat(str, "[YELLOW]");
    }
    if(entity_get_health(ent)/entity_get_max_health(ent) <= .2){
      strcat(str, "[RED]");
    }else{
      strcat(str, "[GREEN]");
    }
    
    sprintf(strAux, "%.2lf / %.2lf", entity_get_health(ent), entity_get_max_health(ent));
    strcat(str, strAux);
    screen_area_puts(ge->descript, str);//Gdes, name, location, money
  
    strcpy(str, tab);
    sprintf(strAux, "BaseD:[YELLOW]%.1lf [RESET]| Strength:[YELLOW]%d [RESET]| Defense:[YELLOW]%d [RESET]", entity_get_baseDamage(ent), entity_get_strength(ent), entity_get_defense(ent));
    strcat(str, strAux);
  
    screen_area_puts(ge->descript, str);// damage, strength, defense
    printedNPCs++;
  }
  if(printedNPCs == 0){
    sprintf(str, "%s[RED]No players in space", tab);
    screen_area_puts(ge->descript, str);
  }

}

void graphic_engine_paint_messages(Graphic_engine *ge, Game *game){
  char str[WORD_SIZE] = "";

  screen_area_clear(ge->messages);
  strcpy(str, "Messages:");
  screen_area_puts(ge->messages, str);
  while(game_log_hasMessage(game)){
    game_get_log_message(game, str);
    //screen_area_puts(ge->descript, str);
    screen_area_puts(ge->messages,str);
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

  if(game_has_request_switch(game)){
    screen_area_clear(ge->feedback);
  }

  str[0] = 0;
  if(command_get_status(last_cmd) == ERROR){
    strcpy(str, "[RED]");
  }else{
    strcpy(str, "[GREEN]");
  }

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
  char tab[5] = "    ";

  int heightDiv;
  int auxInt;

  int enemy_count = 0, ally_count = 0;
  Stats *stats = NULL;
  Stats *playerStats = NULL;

  Inventory *playerInventory = NULL;
  int inventorysize;

  Entity *entityplayer;
  int ability_count = 0;

  Collection *attacks = NULL;
  Attack *attack = NULL;

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
    bar = stats[i].stats.health/stats[i].stats.maxhealth * HEALTH_BAR_WIDTH;
    if(bar < 4){
      strcat(str, "[YELLOW]");
    }
    if(bar < 2){
      strcat(str, "[RED]");
    }else{
      strcat(str, "[GREEN]");
    }

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
    strcat(str, "[RESET]]");
    
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
  strcpy(str, spacing);
  if(ally_count == 1) strcat(str, "  ");

  /*------PLAYER HBAR------*/
  strcat(str, "[");
  bar = stats[0].stats.health/stats[0].stats.maxhealth * HEALTH_BAR_WIDTH;
  if(bar < 4){
    strcat(str, "[YELLOW]");
  }
  if(bar < 2){
    strcat(str, "[RED]");
  }else{
    strcat(str, "[GREEN]");
  }
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
  strcat(str, "[RESET]]");
  
  strcat(str, spacing);


  /*--------ALLIES HBAR----------*/
  for (i = 1; i < ally_count; i++)
  {
    //sprintf(strAux, "[%.2lf]", stats[i].stats.health);
    strcat(str, "[");
    bar = stats[i].stats.health/stats[i].stats.maxhealth * HEALTH_BAR_WIDTH;
    if(bar < 4){
      strcat(str, "[YELLOW]");
    }
    if(bar < 2){
      strcat(str, "[RED]");
    }else{
      strcat(str, "[GREEN]");
    }
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
    strcat(str, "[RESET]]");
    
    if(i != ally_count - 1) strcat(str, spacing);
  }
  screen_area_puts(ge->map, str);

  /*^^^^^^^^`HEALTHBARS^^^^^^^^^`*/

  /*paints description*/
  screen_area_clear(ge->descript2);
  strcpy(str, "Player:");
  screen_area_puts(ge->descript2, str);
  for (i = 0; i < ally_count; i++)
  {
    if(entity_get_entityType(stats[i].entity) != PLAYER_TYPE) continue;
    strcpy(str, tab);
    strcat(str, entity_get_graphic_description(stats[i].entity));
    strcat(str, " [RESET]| Name:[YELLOW] ");
    strcat(str, entity_get_name(stats[i].entity));
    
    strcat(str, " [RESET]| Health: ");
    
    if(stats[0].stats.health/stats[i].stats.maxhealth <= .5){
      strcat(str, "[YELLOW]");
    }
    if(stats[0].stats.health/stats[i].stats.maxhealth <= .2){
      strcat(str, "[RED]");
    }else{
      strcat(str, "[GREEN]");
    }
    
    sprintf(strAux, "%.2lf / %.2lf", stats[i].stats.health, stats[i].stats.maxhealth);
    strcat(str, strAux);
    screen_area_puts(ge->descript2, str);//Gdes, name, location, money
  
    strcpy(str, tab);
    sprintf(strAux, "BaseD:[YELLOW]%.1lf [RESET]| Strength:[YELLOW]%d [RESET]| Defense:[YELLOW]%d [RESET]", stats[i].stats.baseDamage, stats[i].stats.strength, stats[i].stats.defense);
    strcat(str, strAux);
  
    screen_area_puts(ge->descript2, str);// damage, strength, defense
  }
  


  /*INVENTORY*/
  screen_area_puts(ge->descript2, "\nInventory: ");

  
  entityplayer = stats[0].entity;
  playerInventory = entity_get_inventory(player_get_entity(game_get_player(game)));

  inventorysize = inventory_get_size(playerInventory);

  if(inventorysize == 0){
    strcpy(str, tab);
    strcat(str, "[RED]No Objects in Player Inventory");
    screen_area_puts(ge->descript2, str);
  }
  else{
    for(i = 0; i < inventorysize && i < MAX_PRINT_PLAYER_INVENTORY; i++){
      inventory_get_object_str_at(playerInventory, strAux, i);
      
      strcpy(str,tab);
      strcat(str, strAux);

      screen_area_puts(ge->descript2, str);
    }
  }

  ability_count = entity_get_n_abilities(entityplayer);

  /*ABILITIES*/
  screen_area_puts(ge->descript2, "\nAbilities:");

  if(ability_count <= 0) {
    sprintf(str, "%s[RED]Player has no abilities", tab);
    screen_area_puts(ge->descript2, str);
  }

  for(i = 0; i < ability_count; i++){
    sprintf(str, "%s[YELLOW]%d.[RESET] ", tab, i + 1);
    strcat(str, entity_get_ability_name_at(player_get_entity(game_get_player(game)), i));

    auxInt = ability_get_cooldown_length(entity_get_ability_at(player_get_entity(game_get_player(game)), i));
    sprintf(strAux, "\n%s%sCooldown:[BLUE]%d[RESET]", tab, tab, auxInt);
    strcat(str, strAux);

    auxInt = ability_get_cooldown_count(entity_get_ability_at(player_get_entity(game_get_player(game)), i));
    sprintf(strAux, " | Cooldown-Left:%s%d", (auxInt > 0)? "[RED]":"[GREEN]", auxInt);

    strcat(str, strAux);
    screen_area_puts(ge->descript2, str);
  }

  /*^^^^^^`PLAYER INFO^^^^^^*/

  if(ally_count > 1){
    screen_area_puts(ge->descript2,  "\nAllies:");
    stats = combat_get_allies_stats(combat);
    for (i = 1; i < ally_count; i++)
    {
      if(entity_get_entityType(stats[i].entity) == PLAYER_TYPE) continue;

      strcpy(str, tab);
      strcat(str, entity_get_graphic_description(stats[i].entity));
      strcat(str, ": Health: ");
      if(stats[i].stats.health/stats[i].stats.maxhealth <= .5){
        strcat(str, "[YELLOW]");
      }
      if(stats[i].stats.health/stats[i].stats.maxhealth <= .2){
        strcat(str, "[RED]");
      }else{
        strcat(str, "[GREEN]");
      }
  
      sprintf(strAux, "%.1lf/%.1lf[RESET] | BaseD:[YELLOW]%.1lf[RESET] | St:[YELLOW]%d[RESET]", stats[i].stats.health, stats[i].stats.maxhealth, stats[i].stats.baseDamage, stats[i].stats.strength);
      strcat(str, strAux);
      screen_area_puts(ge->descript2, str);
    }
  }
  
  screen_area_puts(ge->descript2, "\nAllies:");
  stats = combat_get_allies_stats(combat);
  if(ally_count == 1){
    screen_area_puts(ge->descript2, "    [RED]No allies");
  }
  for (i = 1; i < ally_count; i++)
  {
    strcpy(str, tab);
    strcat(str, entity_get_graphic_description(stats[i].entity));
    strcat(str, ": H: ");
    if(stats[i].stats.health/stats[i].stats.maxhealth <= .5){
      strcat(str, "[YELLOW]");
    }
    if(stats[i].stats.health/stats[i].stats.maxhealth <= .2){
      strcat(str, "[RED]");
    }else{
      strcat(str, "[GREEN]");
    }

    sprintf(strAux, "%.1lf/%.1lf[RESET] | BD:[YELLOW]%.1lf[RESET] | St:[YELLOW]%d[RESET]", stats[i].stats.health, stats[i].stats.maxhealth, stats[i].stats.baseDamage, stats[i].stats.strength);
    strcat(str, strAux);
    sprintf(strAux, " | Df: [YELLOW]%d[RESET]", stats[i].stats.defense);
    strcat(str, strAux);
    screen_area_puts(ge->descript2, str);
  }

  screen_area_clear(ge->descript);

  screen_area_puts(ge->descript2, "\nEnemies:");
  stats = combat_get_enemies_stats(combat);
  for (i = 0; i < enemy_count; i++)
  {
    strcpy(str, tab);
    strcat(str, entity_get_graphic_description(stats[i].entity));
    strcat(str, ": H: ");
    if(stats[i].stats.health/stats[i].stats.maxhealth <= .5){
      strcat(str, "[YELLOW]");
    }
    if(stats[i].stats.health/stats[i].stats.maxhealth <= .2){
      strcat(str, "[RED]");
    }else{
      strcat(str, "[GREEN]");
    }

    sprintf(strAux, "%.1lf/%.1lf[RESET] | BD:[YELLOW]%.1lf[RESET] | St:[YELLOW]%d[RESET]", stats[i].stats.health, stats[i].stats.maxhealth, stats[i].stats.baseDamage, stats[i].stats.strength);
    strcat(str, strAux);
    sprintf(strAux, " |Df: [YELLOW]%d[RESET]", stats[i].stats.defense);
    strcat(str, strAux);
    screen_area_puts(ge->descript2, str);
  }

  screen_area_clear(ge->descript);

  attacks = game_get_attacks(game);
  auxInt = collection_length(attacks);

  screen_area_puts(ge->descript, "Attacks:");

  for (i = 0; i < auxInt; i++)
  {
    attack = (Attack *)collection_get_element_at(attacks, i);
    strcpy(str, "");

    sprintf(strAux, "[BLUE]%s[RESET]: D.Mult: [YELLOW]%.1lf[RESET] | Fail%%:", attack_get_name(attack), attack_get_damage_multiplicator(attack));
    strcat(str, strAux);

    if(100 - attack_get_success_chance(attack) >= 15){
      strcat(str, "[YELLOW]");
    }
    if(100 - attack_get_success_chance(attack) >= 50){
      strcat(str, "[RED]");
    }else if (100 - attack_get_success_chance(attack) < 15){
      strcat(str, "[GREEN]");
    }
    sprintf(strAux, " %.1lf[RESET] | Area: %s", 100 - attack_get_success_chance(attack), (attack_get_target_bool(attack)) ? "[BLUE]false[RESET]" : "[GREEN]true[RESET]");
    strcat(str, strAux);


    screen_area_puts(ge->descript, str);
  }

  strcpy(str, "Turn:");
  screen_area_puts(ge->descript, str);
  while(game_combat_log_hasMessage(game)){
    game_get_combat_log_message(game, str);
    screen_area_puts(ge->descript, str);
  }
}

void graphic_engine_paint_dialogue(Graphic_engine *ge, Game *game){
  char str[MAP_WIDTH] = "";
  Dialogue *dialogue = NULL;
  NPC *npc = NULL;
  char *text = NULL;
  char **player_replies = NULL;
  int count, n_player_replies;

  if(!ge || !game) return;

  dialogue = game_get_dialogue(game);
  if(!dialogue) return;

  npc = dialogue_get_NPC(dialogue);
  if(!npc) return;

  screen_area_clear(ge->map);

  /*Paints the dialogue on the map*/
  screen_area_puts(ge->map," \n \n \n \n \n \n \n");
  screen_area_puts(ge->map,"  Dialogue:\n");

  text = dialogue_get_npc_text(dialogue);
  if(!text) return;

  graphic_engine_newline_print_with_extra(ge->map, text, "   ");

  screen_area_puts(ge->map," \n \n");
  screen_area_puts(ge->map,"  Possible replies:\n");

  player_replies = dialogue_get_player_replies(dialogue);
  n_player_replies = dialogue_get_num_player_replies(dialogue);
  for(count = 0; count < n_player_replies; count++){
    sprintf(str, "   %d. %s | (%s)", count + 1, player_replies[count], dialogue_get_outcome_as_string(dialogue_get_output_at(dialogue, count)));
    screen_area_puts(ge->map, str);
  }

  graphic_engine_paint_generalDesc(ge, game);
}

void graphic_engine_paint_store(Graphic_engine *ge, Game *game){
  int i, item_index, page, cost, size;
  char str[WORD_SIZE] = "";
  char aux_str[LINE_LENGTH] = "";
  Store *st = NULL;
  StoreType type;
  void *ele = NULL;
  char *auxaux = NULL;


  if(!ge || !game) return;

  st = game_get_store(game);
  if(!st) return;

  type = store_get_type(st);
  if(type == ERROR_STORE);

  screen_area_clear(ge->map);
  screen_area_clear(ge->descript);

  graphic_engine_paint_generalDesc(ge, game);

  screen_area_puts(ge->map," \n ");
  screen_area_puts(ge->map,"  (Enter 'by e' or 'by exit' to exit the store or  'by (NUMBER OF ITEM)' to buy. You can also use i (NUMBER OF ITEM) to see description)\n Use 'by n' to move to next page\n Or use 'by b' to move to the previous page\n");
  
  size = store_get_size(st);
  page = store_get_page(st);
  item_index = (page - 1)*STORE_PAGE_MAX;

  sprintf(str, " \n  Page number: %d out of %d\n", page, (size - 1)/STORE_PAGE_MAX + 1);
  screen_area_puts(ge->map,str);

  switch(type){
    case ABILITY_STORE:
      sprintf(str, "  Skill Points available: %d\n", *store_get_money(st));
      break;
    case OBJECT_STORE:
      sprintf(str, "  Money available: %d\n", *store_get_money(st));
      break;
    case STAT_STORE:
      sprintf(str, "  Skill Points available: %d\n", *store_get_money(st));
      break;
    default:
      sprintf(str, "  ERROR:\n");
      break;
  }
  screen_area_puts(ge->map,str);

  switch(type){
    case ABILITY_STORE:
      sprintf(str, "  These are all the abilities offered:\n");
      break;
    case OBJECT_STORE:
      sprintf(str, "  This is all I can offer:\n");
      break;
    case STAT_STORE:
      sprintf(str, "  This is all you can level up!\n");
      break;
    default:
      sprintf(str, "  ERROR:\n");
      break;
  }
  screen_area_puts(ge->map,str);

  for(i = 0; i < STORE_PAGE_MAX && (i + (page - 1)*STORE_PAGE_MAX  < size); i++){
    sprintf(aux_str,"    %d - ", i + 1);
    strcpy(str,aux_str);
    ele = store_get_item_element_at(st, item_index + i);

    auxaux = game_store_get_name(type, ele);
    if(!auxaux) return;
    sprintf(aux_str, "[YELLOW]%s[RESET] |", auxaux);
    strcat(str, aux_str);

    auxaux = game_store_get_descr(type, ele);
    if(auxaux != NULL){
      sprintf(aux_str, "[YELLOW]%s[RESET]|", auxaux);
      strcat(str, aux_str);
    }

    cost = game_store_get_switch_cost(type, ele);
    sprintf(aux_str, " Cost: [GREEN]%d\n", cost);
    strcat(str, aux_str);

    screen_area_puts(ge->map,str);
  }

  graphic_engine_paint_generalDesc(ge, game);
}
                                                     
void graphic_engine_paint_space(Game *game, Space *space, Direction direction,char map[SPACE_HEIGHT + 1][MAP_WIDTH + 33], char spaceStr[SPACE_HEIGHT + 1][SPACE_WIDTH+10]){
  int i;
  char str[WORD_SIZE];
  char strAux[WORD_SIZE];
  char player[WORD_SIZE];
  Link *link1 = NULL, *link2 = NULL, *link3 = NULL;
  char link1Char, link2Char, link3Char;

  GDesc *gdesc = NULL;
  char gdescAux[2] = "";

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

    sprintf(str, spaceStr[2], (gdesc) ? gdesc_get_line(gdesc, 0) : gdescAux);
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

    link3 = space_get_up(space);
    link3Char = ' ';
    if(link3){
      if(link_is_locked(link3)) link3Char = '-';
      else if(link_is_adjacent(link3) == false) link3Char = 'o';
      else link3Char = '^';
    }
    sprintf(str, spaceStr[3], (link2) ? '-' : '|', (gdesc) ? gdesc_get_line(gdesc, 1) : gdescAux, link3Char ,(link1) ? '-' : '|');
    strcat(map[3],str);

    link3 = space_get_down(space);
    link3Char = ' ';
    if(link3){
      if(link_is_locked(link3)) link3Char = '-';
      else if(link_is_adjacent(link3) == false) link3Char = 'o';
      else link3Char = '^';
    }
    sprintf(str, spaceStr[4], link2Char, (gdesc) ? gdesc_get_line(gdesc, 2) : gdescAux, link3Char,link1Char);
    strcat(map[4],str);

    sprintf(str, spaceStr[5], (link2) ? '-' : '|', (gdesc) ? gdesc_get_line(gdesc, 3) : gdescAux, (link1) ? '-' : '|');
    strcat(map[5],str);


    sprintf(str, spaceStr[6], (gdesc) ? gdesc_get_line(gdesc, 4) : gdescAux);
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
      strcat(map[i], "                 ");
    }
  }
}