#include "../../../include/player.h"
#include "test.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TESTS 48

/*creates a player, expected result != NULL*/
void test1_player_create();
/*creates a player without a name, expected result == NULL*/
void test2_player_create();
/*creates a player with a non valid Id, expected result == NULL*/
void test3_player_create();
/*creates a player with a non valid Location, expected result == NULL*/
void test4_player_create();
/*gets a player's entity, expected result != NULL*/
void test1_player_get_entity();
/*gets a non existent player's entity, expected result == NULL*/
void test2_player_get_entity();
/*gets a player's equipment, expected result != NULL*/
void test1_player_get_equipment();
/*gets a non existent player's equipment, expected result == NULL*/
void test2_player_get_equipment();
/*gets a player's command data, expected result != NULL*/
void test1_player_get_cmData();
/*gets a non existent player's command data, expected result NULL*/
void test1_player_get_cmData();
/*sets a player's stats, expected result OK*/
void test1_player_set_stats();
/*sets a non existent player's stats, expected result ERROR*/
void test2_player_set_stats();
/*gets a players description to a string, expected result OK*/
void test1_player_get_str_desc();
/*gets a non existent player's description to a string, expected result ERROR*/
void test2_player_get_str_desc();
/*gets a player's description to a non existent string, expected result ERROR*/
void test3_player_get_str_desc();
/*gets a player's money, expected result >= 0*/
void test1_player_get_money();
/*gets a non existent player's money, expected result == -1*/
void test2_player_get_money();
/*adds a value of money to a player, expected result == OK*/
void test1_player_add_money();
/*adds a value of money to a non existent player, expected result == ERROR*/
void test2_player_add_money();
/*equips a piece to a player, expected result == OK*/
void test1_player_equip_piece();
/*equips a piece to a non existent player, expected result == ERROR*/
void test2_player_equip_piece();
/*equips a non existent piece to a player, expected result == ERROR*/
void test3_player_equip_piece();
void test1_player_unequip_piece();
void test1_player_add_follower();
void test1_player_remove_follower();
void test1_player_get_followers();


int main(int argc, char** argv) {

    int test = 0;
    int all = 1;
  
    if (argc < 2) {
      printf("Running all test for module Player:\n");
    } else {
      test = atoi(argv[1]);
      all = 0;
      printf("Running test %d:\t", test);
      if (test < 1 && test > MAX_TESTS) {
        printf("Error: unknown test %d\t", test);
        exit(EXIT_SUCCESS);
      }
    }
  
  
    if (all || test == 1) test1_player_create();
    
  
    PRINT_PASSED_PERCENTAGE;
  
    return 0;
}


void test1_player_create(){
  Player *p=NULL;
  PRINT_TEST_RESULT((p = player_create("test",1,1)) != NULL);
  player_destroy(p);
}
void test2_player_create(){
  PRINT_TEST_RESULT(player_create(NULL, 1, 1) == NULL);
}
void test3_player_create(){
  PRINT_TEST_RESULT(player_create("test", -1, 1) == NULL);
}
void test4_player_create(){
  PRINT_TEST_RESULT(player_create("test", 1, -1) == NULL);
}
void test1_player_get_entity(){
  Player *p = player_create("test",1,1);
  PRINT_TEST_RESULT(player_get_entity(p) != NULL);
  player_destroy(p);
}
void test2_player_get_entity(){
  PRINT_TEST_RESULT(player_get_entity(NULL) == NULL);
}
void test1_player_get_equipment(){
  Player *p = player_create("test",1,1);
  PRINT_TEST_RESULT(player_get_equipment(p) != NULL);
  player_destroy(p);
}
void test2_player_get_equipment(){
  PRINT_TEST_RESULT(player_get_equipment(NULL) == NULL);
}
void test1_player_get_cmData(){
  Player *p = player_create("test",1,1);
  PRINT_TEST_RESULT(player_get_cmdData(p) != NULL);
  player_destroy(p);
}
void test1_player_get_cmData(){
  PRINT_TEST_RESULT(player_get_cmdData(NULL) == NULL);
}
void test1_player_set_stats(){
  Player *p = player_create("test",1,1);
  PRINT_TEST_RESULT(player_set_stats(p,1,1,1,1,1,1) == OK);
  player_destroy(p);
}
void test2_player_set_stats(){
  PRINT_TEST_RESULT(player_set_stats(NULL,1,1,1,1,1,1) == ERROR);
}
void test1_player_get_str_desc(){
  char dest[50];
  Player *p = player_create("test",1,1);
  PRINT_TEST_RESULT(player_get_str_desc(p,dest) == OK);
  player_destroy(p);
}
void test2_player_get_str_desc(){
  char dest[50];
  PRINT_TEST_RESULT(player_get_str_desc(NULL,dest) == ERROR);
}
void test3_player_get_str_desc(){
  Player *p = player_create("test",1,1);
  PRINT_TEST_RESULT(player_get_str_desc(p,NULL) == ERROR);
  player_destroy(p);
}
void test1_player_get_money(){
  Player *p = player_create("test",1,1);
  PRINT_TEST_RESULT(player_get_money(p) >= 0);
  player_destroy(p);
}
void test2_player_get_money(){
  PRINT_TEST_RESULT(player_get_money(NULL) >= 0);
}
void test1_player_add_money(){
  Player *p = player_create("test",1,1);
  PRINT_TEST_RESULT(player_add_money(p, 100) == OK);
  player_destroy(p);
}
void test1_player_add_money(){
  PRINT_TEST_RESULT(player_add_money(NULL, 100) == ERROR);
}
void test1_player_equip_piece(){
  Player *p = player_create("test",1,1);
  Object *o = object_create(1,"test","test","test",false,1,PLAYER_INVENTORY);
  PRINT_TEST_RESULT(player_equip_piece(p, o) == OK);
  player_destroy(p);
  object_destroy(o);
}
void test2_player_equip_piece(){
  Object *o = object_create(1,"test","test","test",false,1,PLAYER_INVENTORY);
  PRINT_TEST_RESULT(player_equip_piece(NULL, o) == ERROR);
  object_destroy(o);
}
void test3_player_equip_piece(){
  Player *p = player_create("test",1,1);
  PRINT_TEST_RESULT(player_equip_piece(p, NULL) == ERROR);
  player_destroy(p);
}
void test1_player_unequip_piece(){

}
void test1_player_add_follower(){

}
void test1_player_remove_follower(){

}
void test1_player_get_followers(){

}