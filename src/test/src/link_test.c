/** 
 * @brief It declares the tests for the link module
 * 
 * @file link_test.h
 * @author Aaron Charameli Mair
 * @version 0.0 
 * @date 5-04-2025
 * @copyright GNU Public License
 */

#include "../../../include/link.h"
#include "../../../include/types.h"
#include "../../../include/debug_printing.h"
#include "test.h"

#include <stdlib.h>
#include <stdio.h>

#define N_TEST 35

/*creates a link with a NO_ID space id, expected result NULL*/
void test1_link_create();
/*creates a link with a negative id, expected result NULL*/
void test2_link_create();
/*creates a link with normal values, expected result != NULL*/
void test3_link_create();
/*sets a negative id to a link, expected result ERROR*/
void test1_link_set_id();
/*sets a id to a link, expected result ERROR*/
void test2_link_set_id();
/*sets a id to a non existent link, expected result ERROR*/
void test3_link_set_id();
/*sets two spaces to a link, expected result OK*/
void test1_link_set_spaces();
/*sets 1 negative space to a link, expected result ERROR*/
void test2_link_set_spaces();
/*sets 2 negative space to a link, expected result ERROR*/
void test3_link_set_spaces();
/*sets two spaces to a non existent link, expected result ERROR*/
void test4_link_set_spaces();
/*checks if an adjacent link is adjacent, expected result TRUE*/
void test1_link_is_adjacent();
/*checks if a non adjacent link is adjacent, expected result FALSE*/
void test2_link_is_adjacent();
/*checks if a non existent link is adjacent, expected result FALSE*/
void test3_link_is_adjacent();
/*checks if a locked link is locked, expected result TRUE*/
void test1_link_is_locked();
/*checks if an unlocked link is locked, expected result FALSE*/
void test2_link_is_locked();
/*checks if an non existent link is locked, expected result FALSE*/
void test3_link_is_locked();
/*gets de id of a link (10), expected result 10*/
void test1_link_get_id();
/*gets de id of a non existent link, expected result NO_ID*/
void test2_link_get_id();
/*gets the space 1 (21) of a link, expected result 21*/
void test1_link_get_space1();
/*gets the space 1 of a non existent link, expected result NO_ID*/
void test2_link_get_space1();
/*gets the space 2 (22) of a link, expected result 22*/
void test1_link_get_space2();
/*gets the space 2 of a non existent link, expected result NO_ID*/
void test2_link_get_space2();
/*given space 2, it gets space 1 of a link, expected result 21 (space 1)*/
void test1_link_get_oposite_space();
/*given a space it gets the opposite space of a non existent link, expected result NO_ID*/
void test2_link_get_oposite_space();
/*given a space non related to a link, it gets the opposite space of the link, expected result NO_ID*/
void test3_link_get_oposite_space();
/*given a non existent space, it gets the opposite space of a link, expected result NO_ID*/
void test4_link_get_oposite_space();
/*moves an entity from one space of the link to the other, expected result OK*/
void test1_link_move_entity();
/*moves a non existent entity between a link, expected result ERROR*/
void test2_link_move_entity();
/*moves an entity between a non existent link, expected result ERROR*/
void test3_link_move_entity();
/*unlocks a link if an entity is in the right condition, expected result OK*/
void test1_link_unlock();
/*unlocks a link if a non existent entity is in the right condition, expected result ERROR*/
void test2_link_unlock();
/*unlocks a non existent link if an entity is in the right condition, expected result ERROR*/
void test3_link_unlock();
/*compares to identical links (by id), expected result == 0*/
void test1_link_cmp();
/*compares to non identical links, expected result != 0*/
void test2_link_cmp();
/*compares a link with a non existent link, expected result == -1*/
void test3_link_cmp();


int main(int argc, char** argv) {

    int test = 0;
    int all = 1;
    Debug *debugLog = NULL;
  
    if (argc < 2) {
      printf("Running all test for module Link:\n");
    } else {
      test = atoi(argv[1]);
      all = 0;
      printf("Running test %d:\t", test);
      if (test < 1 && test > N_TEST) {
        printf("Error: unknown test %d\t", test);
        exit(EXIT_SUCCESS);
      }
    }
  
    debugLog = debug_create("./test/debug.log", 1);
  
    if (all || test == 1) test1_link_create();
    if (all || test == 2) test2_link_create();
    if (all || test == 3) test3_link_create();
    if (all || test == 4) test1_link_set_id();
    if (all || test == 5) test2_link_set_id();
    if (all || test == 6) test3_link_set_id();
    if (all || test == 7) test1_link_set_spaces();
    if (all || test == 8) test2_link_set_spaces();
    if (all || test == 9) test3_link_set_spaces();
    if (all || test == 10) test4_link_set_spaces();
    if (all || test == 11) test1_link_is_adjacent();
    if (all || test == 12) test2_link_is_adjacent();
    if (all || test == 13) test3_link_is_adjacent();
    if (all || test == 14) test1_link_is_locked();
    if (all || test == 15) test2_link_is_locked();
    if (all || test == 16) test3_link_is_locked();
    if (all || test == 17) test1_link_get_id();
    if (all || test == 18) test2_link_get_id();
    if (all || test == 19) test1_link_get_space1();
    if (all || test == 20) test2_link_get_space1();
    if (all || test == 21) test1_link_get_space2();
    if (all || test == 22) test2_link_get_space2();
    if (all || test == 23) test1_link_get_oposite_space();
    if (all || test == 24) test2_link_get_oposite_space();
    if (all || test == 25) test3_link_get_oposite_space();
    if (all || test == 26) test4_link_get_oposite_space();
    if (all || test == 27) test1_link_move_entity();
    if (all || test == 28) test2_link_move_entity();
    if (all || test == 29) test3_link_move_entity();
    if (all || test == 30) test1_link_unlock();
    if (all || test == 31) test2_link_unlock();
    if (all || test == 32) test3_link_unlock();
    if (all || test == 33) test1_link_cmp();
    if (all || test == 34) test2_link_cmp();
    if (all || test == 35) test3_link_cmp();
    
    PRINT_PASSED_PERCENTAGE;
  
    debug_destroy(debugLog);
    return 0;
}



void test1_link_create(){
    Link *l=link_create(1,1,NO_ID,true,false);
    PRINT_TEST_RESULT(l == NULL);
    link_destroy(l);
}
void test2_link_create(){
    Link *l=link_create(-1,1,2,true,false);
    PRINT_TEST_RESULT(l == NULL);
    link_destroy(l);
}
void test3_link_create(){
    Link *l=link_create(1,1,2,true,false);
    PRINT_TEST_RESULT(l != NULL);
    link_destroy(l);
}
void test1_link_set_id(){
    Link *l=link_create(1,1,2,true,false);
    PRINT_TEST_RESULT(link_set_id(l,NO_ID) == ERROR);
    link_destroy(l);
}
void test2_link_set_id(){
    Link *l=link_create(1,1,2,true,false);
    PRINT_TEST_RESULT(link_set_id(l,2) == OK);
    link_destroy(l);
}
void test3_link_set_id(){
    PRINT_TEST_RESULT(link_set_id(NULL,2) == ERROR);
}
void test1_link_set_spaces(){
    Link *l=link_create(1,1,2,true,false);
    PRINT_TEST_RESULT(link_set_spaces(l,2,1) == OK);
    link_destroy(l);
}
void test2_link_set_spaces(){
    Link *l=link_create(1,1,2,true,false);
    PRINT_TEST_RESULT(link_set_spaces(l,-2,1) == ERROR);
    link_destroy(l);
}
void test3_link_set_spaces(){
    Link *l=link_create(1,1,2,true,false);
    PRINT_TEST_RESULT(link_set_spaces(l,NO_ID,NO_ID) == ERROR);
    link_destroy(l);
}
void test4_link_set_spaces(){
    PRINT_TEST_RESULT(link_set_spaces(NULL,1,2) == ERROR);
}
void test1_link_is_adjacent(){
    Link *l=link_create(1,1,2,true,false);
    PRINT_TEST_RESULT(link_is_adjacent(l) == true);
    link_destroy(l);
}
void test2_link_is_adjacent(){
    Link *l=link_create(1,1,2,false,false);
    PRINT_TEST_RESULT(link_is_adjacent(l) == false);
    link_destroy(l);
}
void test3_link_is_adjacent(){
    PRINT_TEST_RESULT(link_is_adjacent(NULL) == false);
}

void test1_link_is_locked(){
    Link *l=link_create(1,1,2,false,true);
    PRINT_TEST_RESULT(link_is_locked(l) == true);
    link_destroy(l);
}
void test2_link_is_locked(){
    Link *l=link_create(1,1,2,false,false);
    PRINT_TEST_RESULT(link_is_locked(l) == false);
    link_destroy(l);
}

void test3_link_is_locked(){
    PRINT_TEST_RESULT(link_is_locked(NULL) == false);
}
void test1_link_get_id(){
    Link *l=link_create(10,1,2,false,false);
    PRINT_TEST_RESULT(link_get_id(l) == 10);
    link_destroy(l);
}
void test2_link_get_id(){
    PRINT_TEST_RESULT(link_get_id(NULL) == NO_ID);
}
void test1_link_get_space1(){
    Link *l=link_create(10,21,22,false,false);
    PRINT_TEST_RESULT(link_get_space1(l) == 21);
    link_destroy(l);
}
void test2_link_get_space1(){
    PRINT_TEST_RESULT(link_get_space1(NULL) == NO_ID);
}
void test1_link_get_space2(){
    Link *l=link_create(10,21,22,false,false);
    PRINT_TEST_RESULT(link_get_space2(l) == 22);
    link_destroy(l);
}
void test2_link_get_space2(){
    PRINT_TEST_RESULT(link_get_space2(NULL) == NO_ID);
}
void test1_link_get_oposite_space(){
    Link *l=link_create(10,21,22,false,false);
    PRINT_TEST_RESULT(link_get_oposite_space(l,22) == 21);
    link_destroy(l);
}
void test2_link_get_oposite_space(){
    Link *l=link_create(10,21,22,false,false);
    PRINT_TEST_RESULT(link_get_oposite_space(l,5) == NO_ID);
    link_destroy(l);
}
void test3_link_get_oposite_space(){
    PRINT_TEST_RESULT(link_get_oposite_space(NULL,21) == NO_ID);
}
void test4_link_get_oposite_space(){
    Link *l=link_create(10,21,22,false,false);
    PRINT_TEST_RESULT(link_get_oposite_space(l,NO_ID) == NO_ID);
    link_destroy(l);
}
void test1_link_move_entity(){
    Entity *e = entity_create("test1",1,21,PLAYER_INVENTORY);
    Link *l=link_create(10,21,22,true,false);
    PRINT_TEST_RESULT(link_move_entity(l,e) == OK);
    link_destroy(l);
    entity_destroy(e);
}
void test2_link_move_entity(){
    Link *l=link_create(10,21,22,true,false);
    PRINT_TEST_RESULT(link_move_entity(l,NULL) == ERROR);
    link_destroy(l);
}
void test3_link_move_entity(){
    Entity *e = entity_create("test3",1,21,PLAYER_INVENTORY);
    PRINT_TEST_RESULT(link_move_entity(NULL,e) == ERROR);
    entity_destroy(e);
}
void test1_link_unlock(){
    Entity *e = entity_create("test1",1,21,PLAYER_INVENTORY);
    Link *l=link_create(10,21,22,true,false);
    PRINT_TEST_RESULT(link_unlock(l,e) == OK);
    link_destroy(l);
    entity_destroy(e);
}
void test2_link_unlock(){
    Link *l=link_create(10,21,22,true,false);
    PRINT_TEST_RESULT(link_unlock(l,NULL) == ERROR);
    link_destroy(l);
}
void test3_link_unlock(){
    Entity *e = entity_create("test3",1,21,PLAYER_INVENTORY);
    PRINT_TEST_RESULT(link_unlock(NULL,e) == ERROR);
    entity_destroy(e);
}
void test1_link_cmp(){
    Link *l1=link_create(1,10,11,true,false);
    Link *l2=link_create(1,10,11,true,false);
    PRINT_TEST_RESULT(link_cmp(l1,l2) == 0);
    link_destroy(l1);
    link_destroy(l2);
}
void test2_link_cmp(){
    Link *l1=link_create(1,10,11,true,false);
    Link *l2=link_create(2,10,11,true,false);
    PRINT_TEST_RESULT(link_cmp(l1,l2) != 0);
    link_destroy(l1);
    link_destroy(l2);
}
void test3_link_cmp(){
    Link *l1=link_create(1,10,11,true,false);
    Link *l2=link_create(2,10,11,true,false);
    PRINT_TEST_RESULT(link_cmp(l1,NULL) != 0);
    link_destroy(l1);
    link_destroy(l2);
}