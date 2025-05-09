/** 
 * @brief It declares the tests for the effect module
 * 
 * @file effect_test.h
 * @author Aaron Charameli Mair
 * @version 0.0 
 * @date 18-04-2025
 * @copyright GNU Public License
 */

#include "../../../include/effect.h"
#include "../../../include/player.h"
#include "test.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TESTS 10

void test1_effect_create();
void test1_effect_manager_create();
void test1_effect_manager_destroy();
void test1_effect_destroy();
void test1_effect_manager_add_effect();
void test1_effect_update();
void test1_effect_write_affected_save_data();
void test1_effect_get_as_str();
void test1_effect_get_effect_type();
void test1_effect_get_effect_in();
void test1_effect_get_name();
void test1_effect_get_data();
void test1_effect_get_id();
void test1_effect_cmp();
void test1_effect_print();

int main(int argc, char** argv) {

    /*int test = 0;
    int all = 1;
  
    if (argc < 2) {
      printf("Running all test for module Effect:\n");
    } else {
      test = atoi(argv[1]);
      all = 0;
      printf("Running test %d:\t", test);
      if (test < 1 && test > MAX_TESTS) {
        printf("Error: unknown test %d\t", test);
        exit(EXIT_SUCCESS);
      }
    }
  
  
    if (all || test == 1)*/ test1_effect_write_affected_save_data();
 
  
    /*PRINT_PASSED_PERCENTAGE;*/
  
    return 0;
}

void test1_effect_write_affected_save_data(){
    Effect *e = effect_create(1,"test","5",POISON,FFECT_ENEMY,false,3);
    Player *p1 = player_create("test",10,1,1,1,1,1,1);
    Player *p2 = player_create("test",11,1,1,1,1,1,1);
    Player *p3 = player_create("test",12,1,1,1,1,1,1);
    NPC *npc1 = npc_create(NEUTRAL, true, "hi", "test", 21, 1);
    effect_add_affected(e,player_get_entity(p1));
    effect_add_affected(e,player_get_entity(p2));
    effect_add_affected(e,player_get_entity(p3));
    effect_add_affected(e,npc_get_entity(npc1));
    effect_write_affected_save_data(e, "./svdatatest.txt");
    player_destroy(p1);
    player_destroy(p2);
    player_destroy(p3);
    npc_destroy(npc1);
    effect_destroy(e);
    return;
}