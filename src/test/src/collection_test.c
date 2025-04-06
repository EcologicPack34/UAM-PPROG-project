
#include "../../include/collection.h"
#include "../../include/types.h"
#include "test.h"
#include "../../include/debug_printing.h"

#include <stdlib.h>
#include <stdio.h>

#define N_TEST 19

/*collection destroy doesn't have a pointer because its useless to make a test for a function that doesnt give any information on what it does*/

/*creates collection with invalid length, expected result, null*/
void test1_collection_create();
/*Creates collection without comparing method, expected result null*/
void test2_collection_create();
/*adds an element to collection, expeted result: OK*/
void test1_collection_add();
/*add an element to a null collection, expected :ERROR*/
void test2_collection_add();
/*adds an element to filled fixed size collection, expected ERROR*/
void test3_collection_add();
/*try remove a null element, expected ERROR*/
void test1_collection_remove();
/*try remove a element from null colection, expected ERROR*/
void test2_collection_remove();
/*try remove element at invalid position, expected ERROR*/
void test1_collection_remove_at();
/*try remove element from null collection, expected ERROR*/
void test2_collection_remove_at();
/*try get element from invalid position, expected ERROR*/
void test1_collection_get_element_at();
/*try get element from null collection, expected ERROR*/
void test2_collection_get_element_at();
/*try find a element that is not in the collection, expected NULL*/
void test1_collection_find();
/*try find a element in a null collection, expected NULL*/
void test2_collection_find();
/*try find element that is not contained, expected -1*/
void test1_collection_contains();
/*try find element in null collection, expected -1*/
void test2_collection_contains();
/*try get length from null collection, expected -1*/
void test1_collection_length();
/*try get length from a non filled collection, expected 0*/
void test2_collection_length();
/*try free mem for elements on null collection, expected ERROR*/
void test1_collection_free_elements();
/*try free mem with null free function, expected ERROR*/
void test2_collection_free_elements();


int int_cmp(void *e1, void *e2){
    return *((int *)e1) - *((int *)e2);
}
void test_free(void *e){
    return;
}

int main(int argc, char *argv[]){
    int test = 0;
    int all = 1;
    Debug *debugLog;
  
    if (argc < 2) {
      printf("Running all test for module Collection:\n");
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

    if (all || test == 1) test1_collection_create();
    if (all || test == 2) test2_collection_create();
    if (all || test == 3) test1_collection_add();
    if (all || test == 4) test2_collection_add();
    if (all || test == 5) test3_collection_add();
    if (all || test == 6) test1_collection_remove();
    if (all || test == 7) test2_collection_remove();
    if (all || test == 8) test1_collection_remove_at();
    if (all || test == 9) test2_collection_remove_at();
    if (all || test == 10) test1_collection_get_element_at();
    if (all || test == 11) test2_collection_get_element_at();
    if (all || test == 12) test1_collection_find();
    if (all || test == 13) test2_collection_find();
    if (all || test == 14) test1_collection_contains();
    if (all || test == 15) test2_collection_contains();
    if (all || test == 16) test1_collection_length();
    if (all || test == 17) test2_collection_length();
    if (all || test == 18) test1_collection_free_elements();
    if (all || test == 19) test2_collection_free_elements();

    PRINT_PASSED_PERCENTAGE;

    debug_destroy(debugLog);
    return 0;
}

void test1_collection_create(){
    Collection *collection = NULL;

    collection = collection_create(-1, true, true, int_cmp, NULL);
    PRINT_TEST_RESULT(collection == NULL);
    collection_destroy(collection);
}

void test2_collection_create(){
    Collection *collection = NULL;

    collection = collection_create(10, true, true, NULL, NULL);
    PRINT_TEST_RESULT(collection == NULL);
    collection_destroy(collection);
}
void test1_collection_add(){
    Collection *collection = NULL;
    Status st;

    int num = 2;

    collection = collection_create(2, true, true, int_cmp, NULL);
    st = collection_add(collection, (void *)(&num));
    PRINT_TEST_RESULT(st == OK);
    collection_destroy(collection);
}
void test2_collection_add(){
    int num = 2;
    Status st;
    st = collection_add(NULL, (void *)(&num));
    PRINT_TEST_RESULT(st == ERROR);
}
void test3_collection_add(){
    Collection *collection = NULL;
    Status st;

    int num = 2;
    int num2 = 4;

    collection = collection_create(1, true, true, int_cmp, NULL);
    collection_add(collection, (void *)(&num));
    st = collection_add(collection, &num2);
    PRINT_TEST_RESULT(st == ERROR);
    collection_destroy(collection);
}
void test1_collection_remove(){
    Collection *collection = NULL;
    Status st;

    int num = 2;

    collection = collection_create(2, true, true, int_cmp, NULL);
    collection_add(collection, (void *)(&num));

    st = collection_remove(collection, NULL);

    PRINT_TEST_RESULT(st == ERROR);
    collection_destroy(collection);
}
void test2_collection_remove(){
    Status st;

    int num = 2;

    st = collection_remove(NULL, &num);

    PRINT_TEST_RESULT(st == ERROR);
}
void test1_collection_remove_at(){
    Collection *collection = NULL;
    Status st;

    int num = 2;

    collection = collection_create(2, true, true, int_cmp, NULL);
    collection_add(collection, (void *)(&num));

    st = collection_remove_at(collection, -2);

    PRINT_TEST_RESULT(st == ERROR);
    collection_destroy(collection);
}
void test2_collection_remove_at(){
    Status st;

    st = collection_remove_at(NULL, 0);

    PRINT_TEST_RESULT(st == ERROR);
}
void test1_collection_get_element_at(){
    Collection *collection = NULL;
    void *st;

    int num = 2;

    collection = collection_create(2, true, true, int_cmp, NULL);
    collection_add(collection, (void *)(&num));

    st = collection_get_element_at(collection, -2);

    PRINT_TEST_RESULT(st == NULL);
    collection_destroy(collection);
}
void test2_collection_get_element_at(){
    void * st;

    st = collection_get_element_at(NULL, 0);

    PRINT_TEST_RESULT(st == NULL);
}
void test1_collection_find(){
    Collection *collection = NULL;
    void *st;

    int num = 2;

    collection = collection_create(2, true, true, int_cmp, NULL);
    collection_add(collection, (void *)(&num));

    st = collection_find(collection, NULL);

    PRINT_TEST_RESULT(st == NULL);
    collection_destroy(collection);
}
void test2_collection_find(){
    void * st;

    int num = 2;

    st = collection_find(NULL, &num);

    PRINT_TEST_RESULT(st == NULL);
}
void test1_collection_contains(){
    Collection *collection = NULL;
    int st;

    int num = 2;
    int num2 = 3;

    collection = collection_create(2, true, true, int_cmp, NULL);
    collection_add(collection, (void *)(&num));

    st = collection_contains(collection, &num2);

    PRINT_TEST_RESULT(st == -1);
    collection_destroy(collection);
}
void test2_collection_contains(){
    int st;

    int num = 2;


    st = collection_contains(NULL, &num);

    PRINT_TEST_RESULT(st == -1);
}
void test1_collection_length(){
    int st;

    st = collection_length(NULL);

    PRINT_TEST_RESULT(st == -1);
}
void test2_collection_length(){
    Collection *collection = NULL;
    int st;

    collection = collection_create(2, true, true, int_cmp, NULL);

    st = collection_length(collection);

    PRINT_TEST_RESULT(st == 0);
    collection_destroy(collection);
}
void test1_collection_free_elements(){
    Status st;

    st = collection_free_elements(NULL, test_free);

    PRINT_TEST_RESULT(st == ERROR);
}
void test2_collection_free_elements(){
    Collection *collection = NULL;
    Status st;

    int num = 2;

    collection = collection_create(2, true, true, int_cmp, NULL);
    collection_add(collection, (void *)(&num));

    st = collection_free_elements(collection, NULL); 

    PRINT_TEST_RESULT(st == ERROR);
    collection_destroy(collection);
}