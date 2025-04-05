/** 
 * @brief It declares the tests for the link module
 * 
 * @file object_test.h
 * @author Aaron Charameli Mair
 * @version 0.0 
 * @date 5-04-2025
 * @copyright GNU Public License
 */

 #include "../../include/object.h"
 #include "../../include/types.h"
 #include "../../include/debug_printing.h"
 #include "test.h"
 
 #include <stdlib.h>
 #include <stdio.h>
 
 #define N_TEST 32

/*it creates an object, expected result != NULL*/
void test1_object_create();
/*it creates an object with wrong values, expected result == NULL*/
void test2_object_create();
/*tests if to equal objects are equal, expected result == true*/
void test1_object_isEqual();
/*tests if to different objects are equal, expected result == false*/
void test2_object_isEqual();
/*tests if a non existent object and another are equal, expected result == false*/
void test3_object_isEqual();
/*sets an id to an object, expected result == OK*/
void test1_object_set_id();
/*sets an id to an non existent object, expected result == ERROR*/
void test2_object_set_id();
/*sets an non permitted id to an object, expected result == ERROR*/
void test3_object_set_id();
/*sets a name to an object, expected result OK*/
void test1_object_set_name();
/*sets a non existent name to an object, expected result ERROR*/
void test2_object_set_name();
/*sets a name to a non existent object, expected result ERROR*/
void test3_object_set_name();
/*sets a location to an object, expected result OK*/
void test1_object_set_location();
/*sets a non existent location to an object, expected result ERROR*/
void test2_object_set_location();
/*sets a location to a non existent object, expected result ERROR*/
void test3_object_set_location();
/*sets the inventory type of an object, expected result OK*/
void test1_object_set_type();
/*sets the inventory type of a non existent object, expected result ERROR*/
void test2_object_set_type();
/*sets an unknown inventory type to an object, expected result ERROR*/
void test3_object_set_type();
/*sets an object description, expected result OK*/
void test1_object_set_descr();
/*sets an object a non existent description, expected result ERROR*/
void test2_object_set_descr();
/*sets a non existent object a description, expected result ERROR*/
void test3_object_set_descr();
/*gets the id of an object, expected result id*/
void test1_object_get_id();
/*gets the id of a non existent object, expected result NO_ID*/
void test2_object_get_id();
/*gets the name of an object, expected result name*/
void test1_object_get_name();
/*gets the name of a non existent object, expected result NULL*/
void test2_object_get_name();
/*gets the location of an object, expected result location*/
void test1_object_get_location();
/*gets the location of an non existent object, expected result NO_ID*/
void test2_object_get_location();
/*gets the InventoryType of an object, expected result InventoryType*/
void test1_object_get_type();
/*gets the InventoryType of a non existent object, expected result UNKNOWN_INVENTORY*/
void test2_object_get_type();
/*gets if a consumable object is consumable, expected result true*/
void test1_object_is_consumable();
/*gets if a non existent object is consumable, expected result false*/
void test2_object_is_consumable();
/*gets the description of an object, expected result "description"*/
void test1_object_get_descr();
/*gets the description of a non existent object, expected result NULL*/
void test2_object_get_descr();
/*gets the ObjectEffect of an object, expected result != NULL*/
void test1_object_get_object_effect();
/*gets the ObjectEffect of a non existent object, expected result == NULL*/
void test2_object_get_object_effect();
/*gets the data of an object, expected result "data"*/
void test1_object_get_data();
/*gets the data of a non existent object, expected result NULL*/
void test2_object_get_data();
/*adds an ObjectEffect to an object, expected result OK*/
void test1_object_add_object_effect();
/*adds an ObjectEffect to a non existent object, expected result ERROR*/
void test2_object_add_object_effect();
/*adds a non existent ObjectEffect to an object, expected result ERROR*/
void test3_object_add_object_effect();

int main(int argc, char** argv) {

    int test = 0;
    int all = 1;
    Debug *debugLog = NULL;
  
    if (argc < 2) {
      printf("Running all test for module Object:\n");
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
  
    if (all || test == 1) test1_object_create();
    if (all || test == 2) test2_object_create();
    if (all || test == 3) test1_object_isEqual();
    if (all || test == 4) test2_object_isEqual();
    if (all || test == 5) test3_object_isEqual();
    if (all || test == 6) test1_object_set_id();
    if (all || test == 7) test2_object_set_id();
    if (all || test == 8) test3_object_set_id();
    if (all || test == 9) test1_object_set_name();
    if (all || test == 10) test2_object_set_name();
    if (all || test == 11) test3_object_set_name();
    if (all || test == 12) test1_object_set_location();
    if (all || test == 13) test2_object_set_location();
    if (all || test == 14) test3_object_set_location();
    if (all || test == 15) test1_object_set_type();
    if (all || test == 16) test2_object_set_type();
    if (all || test == 17) test3_object_set_type();
    if (all || test == 18) test1_object_set_descr();
    if (all || test == 19) test2_object_set_descr();
    if (all || test == 20) test3_object_set_descr();
    if (all || test == 21) test1_object_get_id();
    if (all || test == 22) test2_object_get_id();
    if (all || test == 23) test1_object_get_name();
    if (all || test == 24) test2_object_get_name();
    if (all || test == 25) test1_object_get_location();
    if (all || test == 26) test2_object_get_location();
    if (all || test == 27) test1_object_get_type();
    if (all || test == 28) test2_object_get_type();
    if (all || test == 29) test1_object_is_consumable();
    if (all || test == 30) test2_object_is_consumable();
    if (all || test == 31) test1_object_get_descr();
    if (all || test == 32) test2_object_get_descr();
    if (all || test == 33) test1_object_get_object_effect();
    if (all || test == 34) test2_object_get_object_effect();
    if (all || test == 35) test1_object_get_data();
    if (all || test == 36) test2_object_get_data();
    if (all || test == 37) test1_object_add_object_effect();
    if (all || test == 38) test2_object_add_object_effect();
    if (all || test == 39) test3_object_add_object_effect();
    
    PRINT_PASSED_PERCENTAGE;
  
    debug_destroy(debugLog);
    return 0;
}


void test1_object_create(){
    Object *obj= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(obj != NULL);
    object_destroy(obj);
}
void test2_object_create(){
    Object *obj= object_create(1,NULL,"test1","test1",true,NO_ID,SPACE_INVENTORY);
    PRINT_TEST_RESULT(obj == NULL);
    object_destroy(obj);
}
void test1_object_isEqual(){
    Object *obj1= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    Object *obj2= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_isEqual(obj1,obj2) == true);
    object_destroy(obj1);
    object_destroy(obj2);
}
void test2_object_isEqual(){
    Object *obj1= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    Object *obj2= object_create(2,"test2","test2","test2",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_isEqual(obj1,obj2) == false);
    object_destroy(obj1);
    object_destroy(obj2);
}
void test3_object_isEqual(){
    Object *obj1= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_isEqual(obj1,NULL) == false);
    object_destroy(obj1);
}
void test1_object_set_id(){
    Object *obj= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_set_id(obj, 5) == OK);
    object_destroy(obj);
}
void test2_object_set_id(){
    PRINT_TEST_RESULT(object_set_id(NULL, 5) == OK);
}
void test3_object_set_id(){
    Object *obj= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_isEqual(obj, NO_ID) == OK);
    object_destroy(obj);
}
void test1_object_set_name(){
    Object *obj= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_set_name(obj, "The Game") == OK);
    object_destroy(obj);
}
void test2_object_set_name(){
    Object *obj= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_set_name(obj, NULL) == ERROR);
    object_destroy(obj);
}
void test3_object_set_name(){
    PRINT_TEST_RESULT(object_set_name(NULL, "The Game") == ERROR);
}
void test1_object_set_location(){
    Object *obj= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_set_location(obj, 20) == OK);
    object_destroy(obj);
}
void test2_object_set_location(){
    Object *obj= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_set_location(obj, NO_ID) == ERROR);
    object_destroy(obj);
}
void test3_object_set_location(){
    PRINT_TEST_RESULT(object_set_location(NULL, 20) == ERROR);

}
void test1_object_set_type(){
    Object *obj= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_set_type(obj, PLAYER_INVENTORY) == OK);
    object_destroy(obj);
}
void test2_object_set_type(){
    PRINT_TEST_RESULT(object_set_type(NULL, PLAYER_INVENTORY) == OK);
}
void test1_object_set_descr(){
    Object *obj= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_set_descr(obj, "test") == OK);
    object_destroy(obj);
}
void test2_object_set_descr(){
    Object *obj= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_set_descr(obj, NULL) == ERROR);
    object_destroy(obj);
}
void test3_object_set_descr(){
    PRINT_TEST_RESULT(object_set_descr(NULL, "test") == ERROR);
}
void test1_object_get_id(){
    Object *obj= object_create(1,"test1","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_get_id(obj) == 1);
    object_destroy(obj);
}
void test2_object_get_id(){
    PRINT_TEST_RESULT(object_get_id(NULL) == 1);
}
void test1_object_get_name(){
    Object *obj= object_create(1,"name","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT((strcmp(object_get_name(obj)),"name") == 0);
    object_destroy(obj);
}
void test2_object_get_name(){
    PRINT_TEST_RESULT(object_get_name(NULL) == NULL);
}
void test1_object_get_location(){
    Object *obj= object_create(1,"name","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_get_location(obj) == 10);
    object_destroy(obj);
}
void test2_object_get_location(){
    PRINT_TEST_RESULT(object_get_location(NULL) == NO_ID);
}
void test1_object_get_type(){
    Object *obj= object_create(1,"name","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_get_type(obj) == SPACE_INVENTORY);
    object_destroy(obj);
}
void test2_object_get_type(){
    PRINT_TEST_RESULT(object_get_type(NULL) == UNKNOWN_INVENTORY);
}
void test1_object_is_consumable(){
    Object *obj= object_create(1,"name","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_get_is_consumable(obj) == true);
    object_destroy(obj);
}
void test2_object_is_consumable(){
    PRINT_TEST_RESULT(object_get_is_consumable(NULL) == false);
}
void test1_object_get_descr(){
    Object *obj= object_create(1,"name","test1","descr",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT((strcmp(object_get_descr(obj)),"descr") == 0);
    object_destroy(obj);
}
void test2_object_get_descr(){
    PRINT_TEST_RESULT(object_get_name(NULL) == NULL);
}
void test1_object_get_object_effect(){
    Ability *a = ability_create(1, "data", "name", LINK_UNLOCK, 1, false, true, 1, 1);
    Object *obj= object_create(1,"name","test1","test1",true,10,SPACE_INVENTORY);
    object_add_object_effect(obj, a);
    PRINT_TEST_RESULT(object_get_object_effect(obj) == a);
    object_destroy(obj);
    ability_destroy(a);
}
void test2_object_get_object_effect(){
    PRINT_TEST_RESULT(object_get_object_effect(NULL) == NULL);
}
void test1_object_get_data(){
    Object *obj= object_create(1,"name","data","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT((strcmp(object_get_data(obj)),"data") == 0);
    object_destroy(obj);
}
void test2_object_get_data(){
    PRINT_TEST_RESULT(object_get_data(NULL) == NULL);
}
void test1_object_add_object_effect(){
    Ability *a = ability_create(1, "data", "name", LINK_UNLOCK, 1, false, true, 1, 1);
    Object *obj= object_create(1,"name","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_add_object_effect(obj, a) == OK);
    object_destroy(obj);
    ability_destroy(a);
}
void test2_object_add_object_effect(){
    Ability *a = ability_create(1, "data", "name", LINK_UNLOCK, 1, false, true, 1, 1);
    PRINT_TEST_RESULT(object_add_object_effect(NULL, a) == ERROR);
    ability_destroy(a);
}
void test3_object_add_object_effect(){
    Object *obj= object_create(1,"name","test1","test1",true,10,SPACE_INVENTORY);
    PRINT_TEST_RESULT(object_add_object_effect(obj, NULL) == ERROR);
    object_destroy(obj);
}
