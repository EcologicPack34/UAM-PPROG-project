#include "../../../include/space.h"
#include "../../../include/graphic_description.h"
#include "space_test.h"
#include "test.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TESTS 58

int main(int argc, char** argv) {

    int test = 0;
    int all = 1;
    Debug *debugLog = NULL;
  
    if (argc < 2) {
      printf("Running all test for module Space:\n");
    } else {
      test = atoi(argv[1]);
      all = 0;
      printf("Running test %d:\t", test);
      if (test < 1 && test > MAX_TESTS) {
        printf("Error: unknown test %d\t", test);
        exit(EXIT_SUCCESS);
      }
    }
  
    debugLog = debug_create("./test/debug.log", 1);
  
    if (all || test == 1) test1_space_create();
    if (all || test == 2) test2_space_create();
    if (all || test == 3) test1_space_set_name();
    if (all || test == 4) test2_space_set_name();
    if (all || test == 5) test1_space_set_north();
    if (all || test == 6) test2_space_set_north();
    if (all || test == 7) test1_space_set_south();
    if (all || test == 8) test2_space_set_south();
    if (all || test == 9) test1_space_set_east();
    if (all || test == 10) test2_space_set_east();
    if (all || test == 11) test1_space_set_west();
    if (all || test == 12) test2_space_set_west();
    if (all || test == 13) test1_space_set_position();
    if (all || test == 14) test2_space_set_position();
    if (all || test == 15) test1_space_set_isMapped();
    if (all || test == 16) test2_space_set_isMapped();
    if (all || test == 17) test1_space_set_neighbour();
    if (all || test == 18) test2_space_set_neighbour();
    if (all || test == 19) test1_space_set_graphic_description();
    if (all || test == 20) test2_space_set_graphic_description();
    if (all || test == 21) test1_space_get_graphic_description();
    if (all || test == 22) test2_space_get_graphic_description();
    if (all || test == 23) test1_space_get_position();
    if (all || test == 24) test2_space_get_position();
    if (all || test == 25) test1_space_get_isMapped();
    if (all || test == 26) test2_space_get_isMapped();
    if (all || test == 27) test1_space_get_neighbour();
    if (all || test == 28) test2_space_get_neighbour();
    if (all || test == 29) test1_space_get_id();
    if (all || test == 30) test2_space_get_id();
    if (all || test == 31) test1_space_get_name();
    if (all || test == 32) test2_space_get_name();
    if (all || test == 33) test1_space_get_north();
    if (all || test == 34) test2_space_get_north();
    if (all || test == 35) test1_space_get_south();
    if (all || test == 36) test2_space_get_south();
    if (all || test == 37) test1_space_get_east();
    if (all || test == 38) test2_space_get_east();
    if (all || test == 39) test1_space_get_west();
    if (all || test == 40) test2_space_get_west();
    if (all || test == 41) test1_space_get_inventory();
    if (all || test == 42) test2_space_get_inventory();
    if (all || test == 43) test1_space_get_npc_count();
    if (all || test == 44) test2_space_get_npc_count();
    if (all || test == 45) test1_space_print();
    if (all || test == 46) test2_space_print();
    if (all || test == 47) test1_space_add_NPC();
    if (all || test == 48) test2_space_add_NPC();
    if (all || test == 49) test1_space_remove_NPC();
    if (all || test == 50) test2_space_remove_NPC();
    if (all || test == 51) test1_space_move_NPC();
    if (all || test == 52) test2_space_move_NPC();
    if (all || test == 53) test1_space_get_NPC_at();
    if (all || test == 54) test2_space_get_NPC_at();
    if (all || test == 55) test1_space_get_NPC_by_name();
    if (all || test == 56) test2_space_get_NPC_by_name();
    if (all || test == 57) test1_space_get_NPC_list();
    if (all || test == 58) test2_space_get_NPC_list();
  
    PRINT_PASSED_PERCENTAGE;
  
    debug_destroy(debugLog);
    return 0;
}

void test1_space_create(){
    Space *space = NULL;

    space = space_create(NO_ID);

    PRINT_TEST_RESULT(space == NULL);
}

void test2_space_create(){
    Space *space = NULL;

    space = space_create(1);

    PRINT_TEST_RESULT(space != NULL);

    space_destroy(space);
}

void test1_space_set_name(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_set_name(space, "TEST")== ERROR);
}

void test2_space_set_name(){
    Space *space = NULL;

    space = space_create(1);

    PRINT_TEST_RESULT(space_set_name(space, "TEST") == OK);

    space_destroy(space);
}

void test1_space_set_north(){
    Space *space = NULL;
    Link *link = NULL;

    PRINT_TEST_RESULT(space_set_north(space, link)== ERROR);
}

void test2_space_set_north(){
    Space *space1 = NULL, *space2 = NULL;
    Link *link = NULL;

    space1 = space_create(1);
    space2 = space_create(2);
    link = link_create(1, 1, 2, true, false);

    PRINT_TEST_RESULT(space_set_north(space1, link) == OK);

    space_destroy(space1);
    space_destroy(space2);
    link_destroy(link);
}

void test1_space_set_south(){
    Space *space = NULL;
    Link *link = NULL;

    PRINT_TEST_RESULT(space_set_south(space, link) == ERROR);
}

void test2_space_set_south(){
    Space *space1 = NULL, *space2 = NULL;
    Link *link = NULL;

    space1 = space_create(1);
    space2 = space_create(2);
    link = link_create(1, 1, 2, true, false);

    PRINT_TEST_RESULT(space_set_south(space1, link) == OK);

    space_destroy(space1);
    space_destroy(space2);
    link_destroy(link);
}

void test1_space_set_east(){
    Space *space = NULL;
    Link *link = NULL;

    PRINT_TEST_RESULT(space_set_east(space, link)== ERROR);
}

void test2_space_set_east(){
    Space *space1 = NULL, *space2 = NULL;
    Link *link = NULL;

    space1 = space_create(1);
    space2 = space_create(2);
    link = link_create(1, 1, 2, true, false);

    PRINT_TEST_RESULT(space_set_east(space1, link) == OK);

    space_destroy(space1);
    space_destroy(space2);
    link_destroy(link);
}

void test1_space_set_west(){
    Space *space = NULL;
    Link *link = NULL;

    PRINT_TEST_RESULT(space_set_west(space, link)== ERROR);
}

void test2_space_set_west(){
    Space *space1 = NULL, *space2 = NULL;
    Link *link = NULL;

    space1 = space_create(1);
    space2 = space_create(2);
    link = link_create(1, 1, 2, true, false);

    PRINT_TEST_RESULT(space_set_west(space1, link) == OK);

    space_destroy(space1);
    space_destroy(space2);
    link_destroy(link);
}

void test1_space_set_position(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_set_position(space, 1, 1) == ERROR);
}

void test2_space_set_position(){
    Space *space = NULL;

    space = space_create(1);

    PRINT_TEST_RESULT(space_set_position(space, 1.0f, 1.0f) == OK);

    space_destroy(space);
}

void test1_space_set_isMapped(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_set_isMapped(space, true) == ERROR);
}

void test2_space_set_isMapped(){
    Space *space = NULL;

    space = space_create(1);

    PRINT_TEST_RESULT(space_set_isMapped(space, true) == OK);

    space_destroy(space);
}

void test1_space_set_neighbour(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_set_neighbour(space, NULL, N) == ERROR);
}

void test2_space_set_neighbour(){
    Space *space = NULL, *spaceNeighbour = NULL;

    space = space_create(1);
    spaceNeighbour = space_create(2);

    PRINT_TEST_RESULT(space_set_neighbour(space, spaceNeighbour, E) == OK);

    space_destroy(space);
    space_destroy(spaceNeighbour);
}

void test1_space_set_graphic_description(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_set_graphic_description(space, NULL)== ERROR);
}

void test2_space_set_graphic_description(){
    Space *space = NULL;
    GDesc *gdesc = gdesc_create(1,5,9, SPACE_DESC);
    
    if(gdesc_set_line(gdesc, 0, "hola") == ERROR){
        PRINT_TEST_RESULT(0);
    }

    space = space_create(1);

    PRINT_TEST_RESULT(space_set_graphic_description(space, gdesc) == OK);

    space_destroy(space);
    gdesc_destroy(gdesc);
}

void test1_space_get_graphic_description(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_graphic_description(space) == NULL);
}

void test2_space_get_graphic_description(){
    Space *space = NULL;
    GDesc *gdesc = gdesc_create(1,5,9, SPACE_DESC);
    
    if(gdesc_set_line(gdesc, 0, "hola") == ERROR){
        PRINT_TEST_RESULT(0);
    }
    space = space_create(1);
    space_set_graphic_description(space, gdesc);

    PRINT_TEST_RESULT(space_get_graphic_description(space) == gdesc);

    space_destroy(space);
    gdesc_destroy(gdesc);
}

void test1_space_get_position(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_position(space) == NULL);
}

void test2_space_get_position(){
    Space *space = NULL;
    Vector2 vector = {1.0f, 1.0f};

    space = space_create(1);
    space_set_position(space, 1.0f, 1.0f);

    PRINT_TEST_RESULT(vector2_isEqual(&vector, space_get_position(space)) == 0);

    space_destroy(space);
}

void test1_space_get_isMapped(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_isMapped(space) == true);
}

void test2_space_get_isMapped(){
    Space *space = NULL;

    space = space_create(1);
    space_set_isMapped(space, false);

    PRINT_TEST_RESULT(space_get_isMapped(space) == false);

    space_destroy(space);
}

void test1_space_get_neighbour(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_neighbour(space, N)== NULL);
}

void test2_space_get_neighbour(){
    Space *space = NULL, *spaceNeighbour = NULL;

    space = space_create(1);
    spaceNeighbour = space_create(2);
    space_set_neighbour(space, spaceNeighbour, E);

    PRINT_TEST_RESULT(space_get_neighbour(space, E) == spaceNeighbour);

    space_destroy(space);
    space_destroy(spaceNeighbour);
}

void test1_space_get_id(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_id(space) == NO_ID);
}

void test2_space_get_id(){
    Space *space = NULL;

    space = space_create(1);

    PRINT_TEST_RESULT(space_get_id(space) == 1);

    space_destroy(space);
}

void test1_space_get_name(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_name(space) == NULL);
}

void test2_space_get_name(){
    Space *space = NULL;

    space = space_create(1);
    space_set_name(space, "TEST");

    PRINT_TEST_RESULT(strcmp(space_get_name(space), "TEST") == 0);

    space_destroy(space);
}

void test1_space_get_north(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_north(space) == NULL);
}

void test2_space_get_north(){
    Space *space1 = NULL, *space2 = NULL;
    Link *link = NULL;

    space1 = space_create(1);
    space2 = space_create(2);
    link = link_create(1, 1, 2, true, false);

    space_set_north(space1, link);

    PRINT_TEST_RESULT(space_get_north(space1) == link);

    space_destroy(space1);
    space_destroy(space2);
    link_destroy(link);
}

void test1_space_get_south(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_south(space) == NULL);
}

void test2_space_get_south(){
    Space *space1 = NULL, *space2 = NULL;
    Link *link = NULL;

    space1 = space_create(1);
    space2 = space_create(2);
    link = link_create(1, 1, 2, true, false);

    space_set_south(space1, link);

    PRINT_TEST_RESULT(space_get_south(space1) == link);

    space_destroy(space1);
    space_destroy(space2);
    link_destroy(link);
}

void test1_space_get_east(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_east(space) == NULL);
}

void test2_space_get_east(){
    Space *space1 = NULL, *space2 = NULL;
    Link *link = NULL;

    space1 = space_create(1);
    space2 = space_create(2);
    link = link_create(1, 1, 2, true, false);

    space_set_east(space1, link);

    PRINT_TEST_RESULT(space_get_east(space1) == link);

    space_destroy(space1);
    space_destroy(space2);
    link_destroy(link);
}

void test1_space_get_west(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_west(space) == NULL);
}

void test2_space_get_west(){
    Space *space1 = NULL, *space2 = NULL;
    Link *link = NULL;

    space1 = space_create(1);
    space2 = space_create(2);
    link = link_create(1, 1, 2, true, false);

    space_set_west(space1, link);

    PRINT_TEST_RESULT(space_get_west(space1) == link);

    space_destroy(space1);
    space_destroy(space2);
    link_destroy(link);
}

void test1_space_get_inventory(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_inventory(space) == NULL);
}

void test2_space_get_inventory(){
    Space *space = NULL;

    space = space_create(1);

    PRINT_TEST_RESULT(space_get_inventory(space) != NULL);

    space_destroy(space);
}

void test1_space_get_npc_count(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_npc_count(space) == -1);
}

void test2_space_get_npc_count(){
    Space *space = NULL;

    space = space_create(1);

    PRINT_TEST_RESULT(space_get_npc_count(space) != -1);

    space_destroy(space);
}

void test1_space_print(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_print(space) == ERROR);
}

void test2_space_print(){
    Space *space = NULL;

    space = space_create(1);

    PRINT_TEST_RESULT(space_print(space) == OK);

    space_destroy(space);
}

void test1_space_add_NPC(){
    Space *space = NULL;
    NPC *npc = NULL;

    PRINT_TEST_RESULT(space_add_NPC(space, npc)== ERROR);
}

void test2_space_add_NPC(){
    Space *space = NULL;
    NPC *npc = NULL;

    space = space_create(1);
    npc = npc_create(NEUTRAL, false, 0, "TEST1", 1, 1);

    PRINT_TEST_RESULT(space_add_NPC(space, npc) == OK);

    space_destroy(space);
    npc_destroy(npc);
}

void test1_space_remove_NPC(){
    Space *space = NULL;
    NPC *npc = NULL;

    PRINT_TEST_RESULT(space_remove_NPC(space, npc)== ERROR);
}

void test2_space_remove_NPC(){
    Space *space = NULL;
    NPC *npc = NULL;

    space = space_create(1);
    npc = npc_create(NEUTRAL, false, 0, "TEST1", 1, 1);
    space_add_NPC(space, npc);

    PRINT_TEST_RESULT(space_remove_NPC(space, npc) == OK);

    space_destroy(space);
    npc_destroy(npc);
}

void test1_space_move_NPC(){
    Space *spaceOUT = NULL, *spaceIN = NULL;
    NPC *npc = NULL;

    PRINT_TEST_RESULT(space_move_NPC(spaceOUT, spaceIN, npc)== ERROR);
}

void test2_space_move_NPC(){
    Space *space = NULL, *spaceIN = NULL;
    NPC *npc = NULL;

    space = space_create(1);
    spaceIN = space_create(2);
    npc = npc_create(NEUTRAL, false, 0, "TEST1", 1, 1);

    space_add_NPC(space, npc);

    PRINT_TEST_RESULT(space_move_NPC(space, spaceIN, npc) == OK);

    space_destroy(space);
    space_destroy(spaceIN);
    npc_destroy(npc);
}

void test1_space_get_NPC_at(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_NPC_at(space, 0) == NULL);
}

void test2_space_get_NPC_at(){
    Space *space = NULL;
    NPC *npc = NULL;

    space = space_create(1);
    npc = npc_create(NEUTRAL, false, 0, "TEST1", 1, 1);

    space_add_NPC(space, npc);

    PRINT_TEST_RESULT(space_get_NPC_at(space, 0) == npc);

    space_destroy(space);
    npc_destroy(npc);
}

void test1_space_get_NPC_by_name(){
    Space *space = NULL;

    PRINT_TEST_RESULT(space_get_NPC_by_name(space, "TEST") == NULL);
}

void test2_space_get_NPC_by_name(){
    Space *space = NULL;
    NPC *npc = NULL;

    space = space_create(1);
    npc = npc_create(NEUTRAL, false, 0, "TEST1", 1, 1);

    space_add_NPC(space, npc);

    PRINT_TEST_RESULT(space_get_NPC_by_name(space, "TEST1") == npc);

    space_destroy(space);
    npc_destroy(npc);
}

void test1_space_get_NPC_list(){
    Space *space = NULL;
    char str[20];

    PRINT_TEST_RESULT(space_get_NPC_list(space, str, 10) == ERROR);
}

void test2_space_get_NPC_list(){
    Space *space = NULL;
    NPC *npc = NULL;
    char str[100];

    space = space_create(1);
    npc = npc_create(NEUTRAL, false, 0, "TEST1", 1, 1);

    space_add_NPC(space, npc);

    PRINT_TEST_RESULT(space_get_NPC_list(space, str, 90) == OK);

    space_destroy(space);
    npc_destroy(npc);
}
void test1_space_is_discovered(){
    Space *s = NULL;
    s = space_create(1);
    space_set_isDiscovered(s, true);
    PRINT_TEST_RESULT(space_is_discovered(s) == true);
    space_destroy(s);
}
void test2_space_is_discovered(){
    PRINT_TEST_RESULT(space_is_discovered(NULL) == false);
}
void test1_space_set_discovered(){
    Space *s = NULL;
    s = space_create(1);
    PRINT_TEST_RESULT(space_set_isDiscovered(s, true) == OK);
    space_destroy(s);
}
void test2_space_set_discovered(){
    Space *s = NULL;
    s = space_create(NO_ID);
    PRINT_TEST_RESULT(space_set_isDiscovered(s, true) == ERROR);
    space_destroy(s);
}
void test1_space_cmp(){
    Space *s1,*s2 = NULL;
    s1 = space_create(1);
    s2 = space_create(1);
    PRINT_TEST_RESULT(space_cmp(s1, s2) == 0);
    space_destroy(s1);
    space_destroy(s2);
}
void test2_space_cmp(){
    Space *s1,*s2 = NULL;
    s1 = space_create(NO_ID);
    s2 = space_create(1);
    PRINT_TEST_RESULT(space_cmp(s1, s2) != 0);
    space_destroy(s1);
    space_destroy(s2);
}
