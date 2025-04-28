/**
 * @file utils.c
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-04-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "utils.h"
#include <stdio.h>
#include <string.h>

Status string_remove_newline_escape_sequence_on_end(char *str){
    char *toks = NULL;
    int size = 0;
    
    if(!str) return ERROR;

    toks = strtok(str, "\n");
    size = strlen(toks);

    if(str[size] == '\n')
        str[size] = '\0';

    return OK;
}