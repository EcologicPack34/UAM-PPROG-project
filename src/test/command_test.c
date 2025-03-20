#include "../command.h"

#include <stdio.h>
#include <stdlib.h>


int main(){

    Command *cmd = NULL;

    cmd = command_create();
    if(!cmd){
        printf("Error intializing cmd\n");
        return -1;
    }
    printf("Correctly initialized cmd\n");

    printf("Printing intial values: ");
    printf("Code: %d, Num Args: %d\n", command_get_code(cmd), command_get_arguments_count(cmd));

    printf("Testing setters:");

    if(!command_set_code(cmd,UNKNOWN))
        printf("Error setting command code\n");
    else
        printf("command code setted correctly\n");

    printf("\nTesting Getters\n");



    return 0;
}

