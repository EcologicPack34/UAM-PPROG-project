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
    char aux_str[WORD_SIZE] = "";
    
    if(!str) return ERROR;

    if(strcmp(str,"\n") == 0 || strcmp(str,"") == 0) return OK;

    strcpy(aux_str, str);
    toks = strtok(aux_str, "\n");
    size = strlen(toks);

    if(str[size] == '\n')
        str[size] = '\0';

    return OK;
}

Status string_remove_endofline_escape_sequence_on_end_to_newline(char *str){
    char *toks = NULL;
    int size = 0;
    char aux_str[WORD_SIZE] = "";
    
    if(!str) return ERROR;

    if(strcmp(str,"\n") == 0 || strcmp(str,"") == 0) return OK;

    strcpy(aux_str, str);
    toks = strtok(aux_str, "\r");
    size = strlen(toks);

    if(str[size] == '\r'){
        str[size] = '\n';
        str[size + 1] = '\0';
    }

    return OK;
}