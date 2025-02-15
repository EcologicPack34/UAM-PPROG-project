/**
 * @brief It implements the command interpreter
 * 
 * Commands are defined by the global variable cmd_to_str, which stores each command and their fast access.
 *
 * @file command.c
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define CMD_LENGTH 30 /*!< Maximum length of a command */

/**
 * @brief Global variable that stores all the commands and their shortucts.
 */
char *cmd_to_str[N_CMD][N_CMDT] = {{"", "No command"}, {"", "Unknown"}, {"ex", "Exit"}, {"s", "South"}, {"n", "North"}, {"e", "East"}, {"w", "West"},{"t", "Take"}, {"d", "Drop"}};

/**
 * @brief Command
 *
 * This struct stores all the information related to a command.
 */
struct _Command {
  CommandCode code; /*!< Name of the command */
};

Command* command_create() {
  Command* newCommand = NULL;

  newCommand = (Command*)malloc(sizeof(Command));
  if (newCommand == NULL) {
    return NULL;
  }

  /* Initialization of an empty command*/
  newCommand->code = NO_CMD;

  return newCommand;
}

Status command_destroy(Command* command) {
  if (!command) {
    return ERROR;
  }

  free(command);
  command = NULL;
  return OK;
}

Status command_set_code(Command* command, CommandCode code) {
  if (!command) {
    return ERROR;
  }

  command->code=code;

  return OK;
}

CommandCode command_get_code(Command* command) {
  if (!command) {
    return NO_CMD;
  }
  return command->code;
}

Status command_get_user_input(Command* command) {
  char input[CMD_LENGTH] = "", *token = NULL;
  int i = UNKNOWN - NO_CMD + 1;
  CommandCode cmd;

  if (!command) {
    return ERROR;
  }

  if (fgets(input, CMD_LENGTH, stdin)) {
    token = strtok(input, " \n");
    if (!token) {
      return command_set_code(command, UNKNOWN);
    }

    cmd = UNKNOWN;
    while (cmd == UNKNOWN && i < N_CMD) {
      if (!strcasecmp(token, cmd_to_str[i][CMDS]) || !strcasecmp(token, cmd_to_str[i][CMDL])) {
        cmd = i + NO_CMD;
      } else {
        i++;
      }
    }
    return command_set_code(command, cmd);
  }
  else
    return command_set_code(command, EXIT);
  
}

Status command_get_list(char *destination){
  char *aux;
  int i,j;

  if(destination == NULL)
    return ERROR;

  aux = (char*)calloc((N_CMD-2)*(N_CMDT) * (CMD_LENGTH) + WORD_SIZE, sizeof(char));
  if(aux == NULL){
    return ERROR;
  }
  
  for (i = 2; i < N_CMD; i++){
    for (j = 0; j < N_CMDT; j++){
      strcat(aux,cmd_to_str[i][j]);
      if(j < N_CMDT -1)
        strcat(aux," or ");
    }
    if(i < N_CMD-1)
      strcat(aux,", ");
  }
  
  strcpy(destination,aux);
  free(aux);
  return OK;
}


