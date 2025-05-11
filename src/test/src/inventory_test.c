/** 
 * @brief It declares the tests for the inventory module
 * 
 * @file inventory_test.h
 * @author Aaron Charameli Mair
 * @version 0.0 
 * @date 6-04-2025
 * @copyright GNU Public License
 */

 #include "../../../include/inventory.h"
 #include "../../../include/collection.h"
 #include "../../../include/object.h"
 #include "../../../include/types.h"
#include "../../../include/debug_printing.h"
 #include "test.h"
 
 #include <stdlib.h>
 #include <stdio.h>
 
 #define N_TEST 32

 /*creates an inventory, expected result != NULL*/
 void test1_inventory_create();
 /*creates an inventory with non possible Id, expected result == NULL*/
 void test2_inventory_create();
 /*gets an obj by its id from an inventory, expected result == obj*/
 void test1_inventory_get_object_by_id();
 /*gets a non existent obj by its id from an inventory, expected result == NULL*/
 void test2_inventory_get_object_by_id();
 /*gets an obj by its id from a non existent inventory, expected result == NULL*/
 void test3_inventory_get_object_by_id();
 /*gets an object by its index from an inventory, expected result == obj*/
 void test1_inventory_get_object_at();
 /*gets an object by a non possible index from an inventory, expected result == NULL*/
 void test2_inventory_get_object_at();
 /*gets an object by its index from a non existent inventory, expected result == NULL*/
 void test3_inventory_get_object_at();  
 /*gets an obj by its name from an inventory, expected result == obj*/
 void test1_inventory_get_object_by_name();
 /*gets a non existent obj by name id from an inventory, expected result == NULL*/
 void test2_inventory_get_object_by_name();
 /*gets an obj by its name from a non existent inventory, expected result == NULL*/
 void test3_inventory_get_object_by_name();
 /*gets the collection that manages an inventory, expected result != NULL*/
 void test1_inventory_get_collection();
 /*gets the collection that manages an inventoy of a non existent inventory, expected result == NULL*/
 void test2_inventory_get_collection();
 /*gets the size of an inventory, expected result == size*/
 void test1_inventory_get_size();
 /*gets the size of a non existent inventory, expected result == -1*/
 void test2_inventory_get_size();
 /*adds an object to an inventory, expected result OK*/
 void test1_inventory_add_object();
 /*adds a non existent object to an inventory, expected result ERROR*/
 void test2_inventory_add_object();
 /*adds an object to a non existent inventory, expected result ERROR*/
 void test3_inventory_add_object();
 /*adds an object to a inventory when it is already contained in the inventory, expected result OK (it won't add it twice)*/
 void test4_inventory_add_object();
 /*removes an object from an inventory, expected result OK*/
 void test1_inventory_remove_object();
 /*removes an object not contained in an inventory from an inventory, expected result OK*/
 void test2_inventory_remove_object();
 /*removes a non existent object from an inventory, expected result ERROR*/
 void test3_inventory_remove_object();
 /*removes an object from a non existent inventory, expected result ERROR*/
 void test4_inventory_remove_object();
 /*checks if an inventory contains an object, expected result true*/
 void test1_inventory_contains_object();
 /*checks if an inventory contains a non existent object, expected result false*/
 void test2_inventory_contains_object();
 /*checks if a non existent inventory contains an object, expected result false*/
 void test3_inventory_contains_object();
 /*gets an inventory object's list, expected result OK*/
 void test1_inventory_get_object_list();
 /*gets an inventory object's list from a non existent inventory, expected result ERROR*/
 void test2_inventory_get_object_list();
 /*gets an inventory object's list to a non existent string, expected result ERROR*/
 void test3_inventory_get_object_list();
 /*gets an object's descr from an inventory by its index, expected result OK*/
 void test1_inventory_get_object_str_at();
 /*gets an object's descr from a non existent inventory by its index, expected result ERROR*/
 void test2_inventory_get_object_str_at();
 /*gets an object's descr from an inventory by its index to a non existent string, expected result ERROR*/
 void test3_inventory_get_object_str_at();

 void test1_inventory_move_object();
 void test2_inventory_move_object();
 void test3_inventory_move_object();

 int main(int argc, char *argv[]){
    int test = 0;
    int all = 1;
  
    if (argc < 2) {
      printf("Running all test for module Inventory:\n");
    } else {
      test = atoi(argv[1]);
      all = 0;
      printf("Running test %d:\t", test);
      if (test < 1 && test > N_TEST) {
        printf("Error: unknown test %d\t", test);
        exit(EXIT_SUCCESS);
      }
    }

    if (all || test == 1) test1_inventory_create();
    if (all || test == 2) test2_inventory_create();
    if (all || test == 3) test1_inventory_get_object_by_id();
    if (all || test == 4) test2_inventory_get_object_by_id();
    if (all || test == 5) test3_inventory_get_object_by_id();
    if (all || test == 6) test1_inventory_get_object_at();
    if (all || test == 7) test2_inventory_get_object_at();
    if (all || test == 8) test3_inventory_get_object_at();
    if (all || test == 9) test1_inventory_get_object_by_name();
    if (all || test == 10) test2_inventory_get_object_by_name();
    if (all || test == 11) test3_inventory_get_object_by_name();
    if (all || test == 12) test1_inventory_get_collection();
    if (all || test == 13) test2_inventory_get_collection();
    if (all || test == 14) test1_inventory_get_size();
    if (all || test == 15) test2_inventory_get_size();
    if (all || test == 16) test1_inventory_add_object();
    if (all || test == 17) test2_inventory_add_object();
    if (all || test == 18) test3_inventory_add_object();
    if (all || test == 19) test4_inventory_add_object();
    if (all || test == 20) test1_inventory_remove_object();
    if (all || test == 21) test2_inventory_remove_object();
    if (all || test == 22) test3_inventory_remove_object();
    if (all || test == 23) test4_inventory_remove_object();
    if (all || test == 24) test1_inventory_contains_object();
    if (all || test == 25) test2_inventory_contains_object();
    if (all || test == 26) test3_inventory_contains_object();
    if (all || test == 27) test1_inventory_get_object_list();
    if (all || test == 28) test2_inventory_get_object_list();
    if (all || test == 29) test3_inventory_get_object_list();
    if (all || test == 30) test1_inventory_get_object_str_at();
    if (all || test == 31) test2_inventory_get_object_str_at();
    if (all || test == 32) test3_inventory_get_object_str_at();

    PRINT_PASSED_PERCENTAGE;

    return 0;
}

 void test1_inventory_create(){
    Inventory *i=NULL;
    PRINT_TEST_RESULT((i = inventory_create(SPACE_INVENTORY,1)) != NULL);
    inventory_destroy(i);
 }
 void test2_inventory_create(){
    Inventory *i=NULL;
    PRINT_TEST_RESULT((i = inventory_create(SPACE_INVENTORY,-2)) == NULL);
    inventory_destroy(i);
 }
 void test1_inventory_get_object_by_id(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    Object *o = object_create(5,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    inventory_add_object(i, o);
    PRINT_TEST_RESULT(inventory_get_object_by_id(i, 5) == o);
    inventory_destroy(i);
    object_destroy(o);
 }
 void test2_inventory_get_object_by_id(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    PRINT_TEST_RESULT(inventory_get_object_by_id(i, NO_ID) == NULL);
    inventory_destroy(i);
 }
 void test3_inventory_get_object_by_id(){
    Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    PRINT_TEST_RESULT(inventory_get_object_by_id(NULL, 5) == NULL);
    object_destroy(o);
 }
 void test1_inventory_get_object_at(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    inventory_add_object(i, o);
    PRINT_TEST_RESULT(inventory_get_object_at(i, 0) == o);
    inventory_destroy(i);
    object_destroy(o);
 }
 void test2_inventory_get_object_at(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    PRINT_TEST_RESULT(inventory_get_object_at(i, -1) == NULL);
    inventory_destroy(i);
 }
 void test3_inventory_get_object_at(){
    Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    PRINT_TEST_RESULT(inventory_get_object_at(NULL, 0) == NULL);
    object_destroy(o);
 }
 void test1_inventory_get_object_by_name(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    Object *o = object_create(1,"name","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    inventory_add_object(i, o);
    PRINT_TEST_RESULT(inventory_get_object_by_name(i, "name") == o);
    inventory_destroy(i);
    object_destroy(o);
 }
 void test2_inventory_get_object_by_name(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    PRINT_TEST_RESULT(inventory_get_object_by_name(i, NULL) == NULL);
    inventory_destroy(i);
 }
 void test3_inventory_get_object_by_name(){
    Object *o = object_create(1,"test","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    PRINT_TEST_RESULT(inventory_get_object_by_name(NULL, "test") == NULL);
    object_destroy(o);
 }
 void test1_inventory_get_collection(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    PRINT_TEST_RESULT(inventory_get_collection(i) != NULL);
    inventory_destroy(i);
 }
 void test2_inventory_get_collection(){
    PRINT_TEST_RESULT(inventory_get_collection(NULL) == NULL);
 }
 void test1_inventory_get_size(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    PRINT_TEST_RESULT(inventory_get_size(i) == 0);
    inventory_destroy(i);
 }
 void test2_inventory_get_size(){
    PRINT_TEST_RESULT(inventory_get_size(NULL) == -1);
 }
 void test1_inventory_add_object(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    PRINT_TEST_RESULT(inventory_add_object(i, o) == OK);
    inventory_destroy(i);
    object_destroy(o);
 }
 void test2_inventory_add_object(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    PRINT_TEST_RESULT(inventory_add_object(i, NULL) == ERROR);
    inventory_destroy(i);
 }
 void test3_inventory_add_object(){
    Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    PRINT_TEST_RESULT(inventory_add_object(NULL, o) == ERROR);
    object_destroy(o);
 }
 void test4_inventory_add_object(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    inventory_add_object(i, o);
    PRINT_TEST_RESULT(inventory_add_object(i, o) == OK);
    inventory_destroy(i);
    object_destroy(o);
 }
 void test1_inventory_remove_object(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    inventory_add_object(i, o);
    PRINT_TEST_RESULT(inventory_remove_object(i, o) == OK);
    inventory_destroy(i);
    object_destroy(o);
 }
 void test2_inventory_remove_object(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    PRINT_TEST_RESULT(inventory_remove_object(i, o) == OK);
    inventory_destroy(i);
    object_destroy(o);
 }
 void test3_inventory_remove_object(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    PRINT_TEST_RESULT(inventory_remove_object(i, NULL) == ERROR);
    inventory_destroy(i);
 }
 void test4_inventory_remove_object(){
    Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    PRINT_TEST_RESULT(inventory_remove_object(NULL, o) == ERROR);
    object_destroy(o);
 }
 void test1_inventory_contains_object(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    Object *o = object_create(5,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    inventory_add_object(i, o);
    PRINT_TEST_RESULT(inventory_contains_object(i, 5) == true);
    inventory_destroy(i);
    object_destroy(o);
 }
 void test2_inventory_contains_object(){
    Inventory *i = inventory_create(SPACE_INVENTORY,1);
    PRINT_TEST_RESULT(inventory_contains_object(i, NO_ID) == false);
    inventory_destroy(i);
 }
 void test3_inventory_contains_object(){
    Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
    PRINT_TEST_RESULT(inventory_contains_object(NULL, 5) == false);
    object_destroy(o);
 }

 void test1_inventory_get_object_list(){
   char list[50];
   Inventory *i = inventory_create(SPACE_INVENTORY,1);
   Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
   inventory_add_object(i, o);
   PRINT_TEST_RESULT(inventory_get_object_list(i, list, 0, 1) == OK);
   inventory_destroy(i);
   object_destroy(o);
 }
 void test2_inventory_get_object_list(){
   char list[50];
   Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
   PRINT_TEST_RESULT(inventory_get_object_list(NULL, list, 0, 1) == ERROR);
   object_destroy(o);
 }
 void test3_inventory_get_object_list(){
   Inventory *i = inventory_create(SPACE_INVENTORY,1);
   Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
   inventory_add_object(i, o);
   PRINT_TEST_RESULT(inventory_get_object_list(i, NULL, 0, 1) == ERROR);
   inventory_destroy(i);
   object_destroy(o);
 }
 void test1_inventory_get_object_str_at(){
   char descr[WORD_SIZE];
   Inventory *i = inventory_create(SPACE_INVENTORY,1);
   Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
   inventory_add_object(i, o);
   PRINT_TEST_RESULT(inventory_get_object_str_at(i, descr, 0) == OK);
   inventory_destroy(i);
   object_destroy(o);
 }
 void test2_inventory_get_object_str_at(){
   char descr[WORD_SIZE];
   Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
   PRINT_TEST_RESULT(inventory_get_object_str_at(NULL, descr, 0) == ERROR);
   object_destroy(o);
 }
 void test3_inventory_get_object_str_at(){
   Inventory *i = inventory_create(SPACE_INVENTORY,1);
   Object *o = object_create(1,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
   inventory_add_object(i, o);
   PRINT_TEST_RESULT(inventory_get_object_str_at(i, NULL, 0) == ERROR);
   inventory_destroy(i);
   object_destroy(o);
 }
 void test1_inventory_move_object(){
   Inventory *i1 = inventory_create(SPACE_INVENTORY,1);
   Inventory *i2 = inventory_create(SPACE_INVENTORY,1);
   Object *o = object_create(5,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
   inventory_add_object(i1, o);
   PRINT_TEST_RESULT(inventory_move_object(i1,i2,5) == OK);
   inventory_destroy(i1);
   inventory_destroy(i2);
   object_destroy(o);
 }
 void test2_inventory_move_object(){
   Inventory *i1 = inventory_create(SPACE_INVENTORY,1);
   Inventory *i2 = inventory_create(SPACE_INVENTORY,1);
   Object *o = object_create(5,"test1","test1","test1", 0, 1, true, true, 1, PLAYER_INVENTORY);
   PRINT_TEST_RESULT(inventory_move_object(i1,i2,5) == ERROR);
   inventory_destroy(i1);
   inventory_destroy(i2);
   object_destroy(o);
 }
 void test3_inventory_move_object(){
   Inventory *i1 = inventory_create(SPACE_INVENTORY,1);
   Object *o = object_create(5,"test1","test1","test1", 0, 1, true, true, 1, SPACE_INVENTORY);
   inventory_add_object(i1, o);
   PRINT_TEST_RESULT(inventory_move_object(i1,NULL,5) == ERROR);
   inventory_destroy(i1);
   object_destroy(o);
 }

 