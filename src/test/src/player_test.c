#include "../../../include/player.h"
#include "test.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TESTS 48


void test1_player_create();
void test1_player_get_entity();
void test1_player_get_equipment();
void test1_player_get_cmData();
void test1_player_set_stats();
void test1_player_get_str_desc();
void test1_player_get_money();
void test1_player_add_money();
void test1_player_equip_piece();
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
    
}
void test1_player_get_entity(){

}
void test1_player_get_equipment(){

}
void test1_player_get_cmData(){

}
void test1_player_set_stats(){

}
void test1_player_get_str_desc(){

}
void test1_player_get_money(){

}
void test1_player_add_money(){

}
void test1_player_equip_piece(){

}
void test1_player_unequip_piece(){

}
void test1_player_add_follower(){

}
void test1_player_remove_follower(){

}
void test1_player_get_followers(){

}