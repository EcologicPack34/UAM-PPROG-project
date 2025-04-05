#include "../../include/command.h"
#include "test.h"
#include "../../include/types.h"
#include "../../include/debug_printing.h"

#include <stdio.h>
#include <stdlib.h>

#define N_TEST 17

/*creates a command, expected result: command != NULL*/
void test1_command_create();
/*sets a code to a non existent command (command == NULL), expected result ERROR*/
void test1_command_set_code();
/*sets code MOVE to a command, expected result OK*/
void test2_command_set_code();
/*sets a status to a non existent command, expected result ERROR*/
void test1_command_set_status();
/*sets a status to a command, expected result OK*/
void test2_command_set_status();
/*sets a desc (description) to a non existent command, expected result ERROR*/
void test1_command_set_info();
/*sets a desc to a command, expected result OK*/
void test2_command_set_info();
/*sets a non existent desc to a command, expected result ERROR*/
void test3_command_set_info();
/*sets the state of a code in a non existent command, expected result ERROR*/
void test1_command_state_add_type();
/*sets the state of a code in a command, expected result OK*/
void test2_command_state_add_type();
/*gets a string from a non existent command to a destiny, expected result ERROR*/
void test1_command_get_as_string();
/*gets the string from a command to a non existent destiny, expected result ERROR*/
void test2_command_get_as_string();
/*gets the string from a command*/
void test3_command_get_as_string();
void test1_command_current_type_valid_by_state();
void test1_command_get_info();
void test1_command_get_code();
void test1_command_get_code_from_str();
void test1_command_get_arguments_count();
void test1_command_get_arguments();
void test1_command_get_user_input();
void test1_command_get_status();
void test1_command_get_list();
void test1_command_code_isEqual();

int main(int argc, char *argv[]){
    int test = 0;
    int all = 1;
  
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

    if (all || test == 1) test1_command_create();
    if (all || test == 2) test1_command_destroy();
    if (all || test == 3) test1_command_set_code();
    if (all || test == 4) test1_command_set_status();
    if (all || test == 5) test1_command_set_info();
    if (all || test == 6) test1_command_state_add_type();
    if (all || test == 7) test1_command_current_type_valid_by_state();
    if (all || test == 8) test1_command_get_as_string();
    if (all || test == 9) test1_command_get_info();
    if (all || test == 10) test1_command_get_code();
    if (all || test == 11) test1_command_get_code_from_str();
    if (all || test == 12) test1_command_get_arguments_count();
    if (all || test == 13) test1_command_get_arguments();
    if (all || test == 14) test1_command_get_user_input();
    if (all || test == 15) test1_command_get_status();
    if (all || test == 16) test1_command_get_list();
    if (all || test == 17) test1_command_code_isEqual();

    PRINT_PASSED_PERCENTAGE;

    return 0;
}



void test1_command_create(){
    Command *c = command_create();
    PRINT_TEST_RESULT(c != NULL);
    command_destroy(c);
}

void test1_command_set_code(){
    PRINT_TEST_RESULT(command_set_code(NULL,MOVE) == ERROR);
}

void test2_command_set_code(){
    Command *command=command_create;
    PRINT_TEST_RESULT(command_set_code(command,MOVE) == OK);
    command_destroy(command);
}

void test1_command_set_status(){
    PRINT_TEST_RESULT(command_set_status(NULL, OK) == ERROR);
}

void test2_command_set_status(){
    Command *c=command_create();
    PRINT_TEST_RESULT(command_set_status(c, OK) == OK);
    command_destroy(c);
}

void test1_command_set_info(){
    PRINT_TEST_RESULT(command_set_info(NULL,MOVE,"The Game") == ERROR);

}

void test2_command_set_info(){
    Command *c=command_create();

    PRINT_TEST_RESULT(command_set_info(c,MOVE,"The Game") == OK);
    command_destroy(c);
}

void test3_command_set_info(){
    Command *c=command_create();
    PRINT_TEST_RESULT(command_set_info(c,MOVE,NULL) == ERROR);
    command_destroy(c);
}

void test1_command_state_add_type(){
    PRINT_TEST_RESULT(command_state_add_type(NULL, DEFAULT, MOVE) == ERROR);
}

void test2_command_state_add_type(){
    Command *c=command_create();
    PRINT_TEST_RESULT(command_state_add_type(c, DEFAULT, MOVE) == OK);
    command_destroy(c);
}

void test1_command_get_as_string(){
    char aux[64]="";
    PRINT_TEST_RESULT(command_get_as_string(NULL,aux)==ERROR);
}

void test2_command_get_as_string(){
    Command *c=command_create();
    PRINT_TEST_RESULT(command_get_as_string(c,NULL) == ERROR);
    command_destroy(c);
}

void test3_command_get_as_string(){
    char aux[64];
    Command *c=command_create();
    PRINT_TEST_RESULT(command_get_as_string(c, aux));
    command_destroy(c);
}

void test1_command_current_type_valid_by_state(){
    
}
void test1_command_get_info(){
    
}
void test1_command_get_code(){
    
}
void test1_command_get_code_from_str(){
    
}
void test1_command_get_arguments_count(){
    
}
void test1_command_get_arguments(){
    
}
void test1_command_get_user_input(){
    
}
void test1_command_get_status(){
    
}
void test1_command_get_list(){
    
}
void test1_command_code_isEqual(){
    
}