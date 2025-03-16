#include "../entity.h"
#include "entity_test.h"
#include "test.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TESTS 48

int main(int argc, char** argv) {

    int test = 0;
    int all = 1;
  
    if (argc < 2) {
      printf("Running all test for module Entity:\n");
    } else {
      test = atoi(argv[1]);
      all = 0;
      printf("Running test %d:\t", test);
      if (test < 1 && test > MAX_TESTS) {
        printf("Error: unknown test %d\t", test);
        exit(EXIT_SUCCESS);
      }
    }
  
  
    if (all || test == 1) test1_entity_create();
    if (all || test == 2) test2_entity_create();
    if (all || test == 3) test1_entity_set_name();
    if (all || test == 4) test2_entity_set_name();
    if (all || test == 5) test1_entity_set_location();
    if (all || test == 6) test2_entity_set_location();
    if (all || test == 7) test1_entity_set_id();
    if (all || test == 8) test2_entity_set_id();
    if (all || test == 9) test1_entity_set_entityType();
    if (all || test == 10) test2_entity_set_entityType();
    if (all || test == 11) test1_entity_set_max_health();
    if (all || test == 12) test2_entity_set_max_health();
    if (all || test == 13) test1_entity_set_health();
    if (all || test == 14) test2_entity_set_health();
    if (all || test == 15) test1_entity_set_baseDamage();
    if (all || test == 16) test2_entity_set_baseDamage();
    if (all || test == 17) test1_entity_set_strength();
    if (all || test == 18) test2_entity_set_strength();
    if (all || test == 19) test1_entity_set_defense();
    if (all || test == 20) test2_entity_set_defense();
    if (all || test == 21) test1_entity_set_magicLevel();
    if (all || test == 22) test2_entity_set_magicLevel();
    if (all || test == 23) test1_entity_set_graphic_description();
    if (all || test == 24) test2_entity_set_graphic_description();
    if (all || test == 25) test1_entity_get_graphic_description();
    if (all || test == 26) test2_entity_get_graphic_description();
    if (all || test == 27) test1_entity_get_name();
    if (all || test == 28) test2_entity_get_name();
    if (all || test == 29) test1_entity_get_location();
    if (all || test == 30) test2_entity_get_location();
    if (all || test == 31) test1_entity_get_inventory();
    if (all || test == 32) test2_entity_get_inventory();
    if (all || test == 33) test1_entity_get_id();
    if (all || test == 34) test2_entity_get_id();
    if (all || test == 35) test1_entity_get_entityType();
    if (all || test == 36) test2_entity_get_entityType();
    if (all || test == 37) test1_entity_get_max_health();
    if (all || test == 38) test2_entity_get_max_health();
    if (all || test == 39) test1_entity_get_health();
    if (all || test == 40) test2_entity_get_health();
    if (all || test == 41) test1_entity_get_baseDamage();
    if (all || test == 42) test2_entity_get_baseDamage();
    if (all || test == 43) test1_entity_get_strength();
    if (all || test == 44) test2_entity_get_strength();
    if (all || test == 45) test1_entity_get_defense();
    if (all || test == 46) test2_entity_get_defense();
    if (all || test == 47) test1_entity_get_magicLevel();
    if (all || test == 48) test2_entity_get_magicLevel();
  
    PRINT_PASSED_PERCENTAGE;
  
    return 1;
}

void test1_entity_create(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity != NULL);
    entity_destroy(entity);
}

void test2_entity_create(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, -1, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity == NULL);
}

void test1_entity_set_name(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_set_name(entity, "TRYING") == OK);
    entity_destroy(entity);
}

void test2_entity_set_name(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_set_name(entity, "") == ERROR);
}

void test1_entity_set_location(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_set_location(entity, 470) == OK);
    entity_destroy(entity);
}

void test2_entity_set_location(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_set_location(entity, -1) == ERROR);
}

void test1_entity_set_id(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_set_id(entity, -1) == OK);
    entity_destroy(entity);
}

void test2_entity_set_id(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_set_id(entity, -1) == ERROR);
}

void test1_entity_set_entityType(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_set_entityType(entity, 2) == OK);
    entity_destroy(entity);
}

void test2_entity_set_entityType(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_set_entityType(entity, -1) == ERROR);
}

void test1_entity_set_max_health(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_set_max_health(entity, 10) == OK);
    entity_destroy(entity);
}

void test2_entity_set_max_health(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_set_max_health(entity, -1) == ERROR);
}

void test1_entity_set_health(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_set_health(entity, 11) == OK);
    entity_destroy(entity);
}

void test2_entity_set_health(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_set_health(entity, -1) == ERROR);
}

void test1_entity_set_baseDamage(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_set_baseDamage(entity, -1) == OK);
    entity_destroy(entity);
}

void test2_entity_set_baseDamage(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_set_baseDamage(entity, -1) == ERROR);
}

void test1_entity_set_strength(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_set_strength(entity, -1) == OK);
    entity_destroy(entity);
}

void test2_entity_set_strength(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_set_strength(entity, -1) == ERROR);
}

void test1_entity_set_defense(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_set_defense(entity, -1) == OK);
    entity_destroy(entity);
}

void test2_entity_set_defense(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_set_defense(entity, -1) == ERROR);
}

void test1_entity_set_magicLevel(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_set_magicLevel(entity, -1) == OK);
    entity_destroy(entity);
}

void test2_entity_set_magicLevel(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_set_magicLevel(entity, -1) == ERROR);
}

void test1_entity_set_graphic_description(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_set_id(entity, 2) == OK);
    entity_destroy(entity);
}

void test2_entity_set_graphic_description(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_set_graphic_description(entity, "TEST") == ERROR);
}

void test1_entity_get_graphic_description(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    entity_set_graphic_description(entity, "TAS");

    PRINT_TEST_RESULT(strcmp(entity_get_graphic_description(entity), "TAS") == 0);
    entity_destroy(entity);
}

void test2_entity_get_graphic_description(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(strcmp(entity_get_graphic_description(entity), "ERR") == 0);
    entity_destroy(entity);
}

void test1_entity_get_name(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(strcmp(entity_get_name(entity), "TEST") == 0);
    entity_destroy(entity);
}

void test2_entity_get_name(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    entity_set_name(entity, "CHANGED");

    PRINT_TEST_RESULT(strcmp(entity_get_name(entity), "CHANGED") == 0);
    entity_destroy(entity);
}

void test1_entity_get_location(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    entity_set_location(entity, 100);

    PRINT_TEST_RESULT(entity_get_location(entity) == 100);
    entity_destroy(entity);
}

void test2_entity_get_location(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_get_location(entity) == 2);
    entity_destroy(entity);
}

void test1_entity_get_inventory(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_get_inventory(entity) != NULL);
    entity_destroy(entity);
}

void test2_entity_get_inventory(){
    Entity *entity = NULL;

    PRINT_TEST_RESULT(entity_get_inventory(entity) == NULL);
    entity_destroy(entity);
}

void test1_entity_get_id(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    entity_set_id(entity, 100);

    PRINT_TEST_RESULT(entity_get_id(entity) == 100);
    entity_destroy(entity);
}

void test2_entity_get_id(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_get_location(entity) == 2);
    entity_destroy(entity);
}

void test1_entity_get_entityType(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    entity_set_entityType(entity, 1);

    PRINT_TEST_RESULT(entity_get_location(entity) == 2);
    entity_destroy(entity);
}

void test2_entity_get_entityType(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_get_entityType(entity) == UNKNOWN_ENTITY);
    entity_destroy(entity);
}

void test1_entity_get_max_health(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    entity_set_max_health(entity, 1);

    PRINT_TEST_RESULT(entity_get_max_health(entity) == 1);
    entity_destroy(entity);
}

void test2_entity_get_max_health(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_get_max_health(entity) == 4);
    entity_destroy(entity);
}

void test1_entity_get_health(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    entity_set_health(entity, 1);

    PRINT_TEST_RESULT(entity_get_health(entity) == 1);
    entity_destroy(entity);
}

void test2_entity_get_health(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_get_health(entity) == 5);
    entity_destroy(entity);
}

void test1_entity_get_baseDamage(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    entity_set_baseDamage(entity, 1);

    PRINT_TEST_RESULT(entity_get_baseDamage(entity) == 1);
    entity_destroy(entity);
}

void test2_entity_get_baseDamage(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_get_baseDamage(entity) == 6);
    entity_destroy(entity);
}

void test1_entity_get_strength(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    entity_set_strength(entity, 1);

    PRINT_TEST_RESULT(entity_get_strength(entity) == 1);
    entity_destroy(entity);
}

void test2_entity_get_strength(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_get_strength(entity) == 7);
    entity_destroy(entity);
}

void test1_entity_get_defense(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    entity_set_defense(entity, 1);

    PRINT_TEST_RESULT(entity_get_defense(entity) == 1);
    entity_destroy(entity);
}

void test2_entity_get_defense(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_get_defense(entity) == 8);
    entity_destroy(entity);
}

void test1_entity_get_magicLevel(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    entity_set_magicLevel(entity, 1);

    PRINT_TEST_RESULT(entity_get_magicLevel(entity) == 1);
    entity_destroy(entity);
}

void test2_entity_get_magicLevel(){
    Entity *entity = NULL;
    entity = entity_create("TEST", 1, 2, 3, 4, 5, 6, 7, 8, 9);

    PRINT_TEST_RESULT(entity_get_magicLevel(entity) == 9);
    entity_destroy(entity);
}
