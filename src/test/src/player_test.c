#include "../../../include/player.h"
#include "../../../include/npc.h"
#include "test.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TESTS 32

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
void test2_player_get_cmData();
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
/*unequips a non-equipped slot, expected result == ERROR*/
void test1_player_unequip_piece();
/*unequips a chest piece, and returns it to the inventory, expected result == OK*/
void test2_player_unequip_piece();
/*Adds an NPC as a follower to the player, expected result == OK*/
void test1_player_add_follower();
/*Adds a non existent NPC as a follower to the player, expected result == ERROR*/
void test2_player_add_follower();
/*Removes a follower from the player, expected result == OK*/
void test1_player_remove_follower_by_entity();
/*Removes a non existent follower from the player, expected result == ERROR*/
void test2_player_remove_follower_by_entity();
/*Gets a follower's entity at a certain position, expected result != NULL*/
void test1_player_get_follower_at();
/*Tries to get followers pointer but player is NULL, expected result == NULL*/
void test2_player_get_follower_at();
/*Gets a player's followers number, expected result == 1*/
void test1_player_get_followers_num();
/*Gets non existent player's followers number, expected result == -1*/
void test2_player_get_followers_num();


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
    if (all || test == 2) test2_player_create();
    if (all || test == 3) test3_player_create();
    if (all || test == 4) test4_player_create();
    if (all || test == 5) test1_player_get_entity();
    if (all || test == 6) test2_player_get_entity();
    if (all || test == 7) test1_player_get_equipment();
    if (all || test == 8) test2_player_get_equipment();
    if (all || test == 9) test1_player_get_cmData();
    if (all || test == 10) test2_player_get_cmData();
    if (all || test == 11) test1_player_set_stats();
    if (all || test == 12) test2_player_set_stats();
    if (all || test == 13) test1_player_get_str_desc();
    if (all || test == 14) test2_player_get_str_desc();
    if (all || test == 15) test3_player_get_str_desc();
    if (all || test == 16) test1_player_get_money();
    if (all || test == 17) test2_player_get_money();
    if (all || test == 18) test1_player_add_money();
    if (all || test == 19) test2_player_add_money();
    if (all || test == 20) test1_player_equip_piece();
    if (all || test == 21) test2_player_equip_piece();
    if (all || test == 22) test3_player_equip_piece();
    if (all || test == 23) test1_player_unequip_piece();
    if (all || test == 24) test2_player_unequip_piece();
    if (all || test == 25) test1_player_add_follower();
    if (all || test == 26) test2_player_add_follower();
    if (all || test == 27) test1_player_remove_follower_by_entity();
    if (all || test == 28) test2_player_remove_follower_by_entity();
    if (all || test == 29) test1_player_get_follower_at();
    if (all || test == 30) test2_player_get_follower_at();
    if (all || test == 31) test1_player_get_followers_num();
    if (all || test == 32) test2_player_get_followers_num();
    
  
    PRINT_PASSED_PERCENTAGE;
  
    return 0;
}


void test1_player_create(){
  Player *p=NULL;
  PRINT_TEST_RESULT((p = player_create("test",1,1,1,1,1,1,1)) != NULL);
  player_destroy(p);
}
void test2_player_create(){
  PRINT_TEST_RESULT(player_create(NULL,1,1,1,1,1,1,1) == NULL);
}
void test3_player_create(){
  PRINT_TEST_RESULT(player_create("test",NO_ID,1,1,1,1,1,1) == NULL);
}
void test4_player_create(){
  PRINT_TEST_RESULT(player_create("test",1,NO_ID,1,1,1,1,1) == NULL);
}
void test1_player_get_entity(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_get_entity(p) != NULL);
  player_destroy(p);
}
void test2_player_get_entity(){
  PRINT_TEST_RESULT(player_get_entity(NULL) == NULL);
}
void test1_player_get_equipment(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_get_equipment(p) != NULL);
  player_destroy(p);
}
void test2_player_get_equipment(){
  PRINT_TEST_RESULT(player_get_equipment(NULL) == NULL);
}
void test1_player_get_cmData(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_get_cmdData(p) != NULL);
  player_destroy(p);
}
void test2_player_get_cmData(){
  PRINT_TEST_RESULT(player_get_cmdData(NULL) == NULL);
}
void test1_player_set_stats(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_set_stats(p,1,1,1,1,1,1) == OK);
  player_destroy(p);
}
void test2_player_set_stats(){
  PRINT_TEST_RESULT(player_set_stats(NULL,1,1,1,1,1,1) == ERROR);
}
void test1_player_get_str_desc(){
  char dest[50];
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_get_str_desc(p,dest) == OK);
  player_destroy(p);
}
void test2_player_get_str_desc(){
  char dest[50];
  PRINT_TEST_RESULT(player_get_str_desc(NULL,dest) == ERROR);
}
void test3_player_get_str_desc(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_get_str_desc(p,NULL) == ERROR);
  player_destroy(p);
}
void test1_player_get_money(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_get_money(p) >= 0);
  player_destroy(p);
}
void test2_player_get_money(){
  PRINT_TEST_RESULT(player_get_money(NULL) <= 0);
}
void test1_player_add_money(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_add_money(p, 100) == OK);
  player_destroy(p);
}
void test2_player_add_money(){
  PRINT_TEST_RESULT(player_add_money(NULL, 100) == ERROR);
}
void test1_player_equip_piece(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  Object *o = object_create(1,"helmet","wearable helmet base_damage:1 strength:1 max_health:20","test1", 0, 1, true, true, 1, PLAYER_INVENTORY);
  inventory_add_object(entity_get_inventory(player_get_entity(p)), o);
  PRINT_TEST_RESULT(player_equip_piece(p, o) == OK);
  player_destroy(p);
  object_destroy(o);
}
void test2_player_equip_piece(){
  Object *o = object_create(1,"test1","wearable helmet base_damage:1 strength:1 max_health:20","test1", 0, 1, true, true, 1, PLAYER_INVENTORY);
  PRINT_TEST_RESULT(player_equip_piece(NULL, o) == ERROR);
  object_destroy(o);
}
void test3_player_equip_piece(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_equip_piece(p, NULL) == ERROR);
  player_destroy(p);
}
void test1_player_unequip_piece(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_unequip_piece(p, NULL) == ERROR);
  player_destroy(p);
}
void test2_player_unequip_piece(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  Object *o = object_create(1,"helmet","wearable helmet base_damage:1 strength:1 max_health:20","test1", 0, 1, true, true, 1, PLAYER_INVENTORY);
  inventory_add_object(entity_get_inventory(player_get_entity(p)), o);
  player_equip_piece(p, o);
  /*player_unequip_piece argument "helmet" corresponds to the obj's data, "wearable <helmet> ...", not the obj's name*/
  PRINT_TEST_RESULT(player_unequip_piece(p, "helmet") == OK);
  object_destroy(o);
  player_destroy(p);
}
void test1_player_add_follower(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  NPC *npc = npc_create(NEUTRAL, false, 0, "TEST1", 1, 1);
  PRINT_TEST_RESULT(player_add_follower(p, npc_get_entity(npc)) == OK);
  player_destroy(p);
  npc_destroy(npc);
}
void test2_player_add_follower(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_add_follower(p, NULL) == ERROR);
  player_destroy(p);
}
void test1_player_remove_follower_by_entity(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  NPC *npc = npc_create(NEUTRAL, false, 0, "NPCTEST1", 1, 1);
  player_add_follower(p, npc_get_entity(npc));
  PRINT_TEST_RESULT(player_remove_follower_by_pointer(p, npc_get_entity(npc)) == OK);
  player_destroy(p);
  npc_destroy(npc);
}
void test2_player_remove_follower_by_entity(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  PRINT_TEST_RESULT(player_remove_follower_by_pointer(p, NULL) == ERROR);
  player_destroy(p);
}
void test1_player_get_follower_at(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  NPC *npc = npc_create(NEUTRAL, false, 0, "TEST1", 1, 1);
  player_add_follower(p, npc_get_entity(npc));
  PRINT_TEST_RESULT(player_get_follower_at(p, 0) == npc_get_entity(npc));
  player_destroy(p);
  npc_destroy(npc);
}
void test2_player_get_follower_at(){
  PRINT_TEST_RESULT(player_get_follower_at(NULL, 1) == NULL);
}
void test1_player_get_followers_num(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  NPC *npc = npc_create(NEUTRAL, false, 0, "TEST1", 1, 1);
  player_add_follower(p, npc_get_entity(npc));
  PRINT_TEST_RESULT(player_get_follower_num(p) == 1);
  player_destroy(p);
  npc_destroy(npc);
}
void test2_player_get_followers_num(){
  Player *p = player_create("test",1,1,1,1,1,1,1);
  NPC *npc = npc_create(NEUTRAL, false, 0, "TEST1", 1, 1);
  player_add_follower(p, npc_get_entity(npc));
  PRINT_TEST_RESULT(player_get_follower_num(NULL) == -1);
  player_destroy(p);
  npc_destroy(npc);
}