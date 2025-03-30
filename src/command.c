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

#include "debug_printing.h"
#include "collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define CMD_LENGTH 500 /*!< Maximum length of a command */

/**
 * @brief Global variable that stores all the commands and their shortucts.
 */
char *cmd_to_str[N_CMD][N_CMDT] = {{"", "No command"}, {"", "Unknown"}, {"q", "Exit"}, {"s", "South"}, {"w", "North"}, {"d", "East"}, {"a", "West"},{"tk", "Take"}, {"dr", "Drop"},\
 {"ch", "Chat"}, {"at", "Attack"}, {"h", "Run_Away"}, {"sw", "Switch_Player"}, {"sk", "Ability"}, {"ou", "Object_Use"}, {"hp", "Help"}, {"m", "Move"}, {"sh","Search"}};

/**
 * @brief Command
 *
 * This struct stores all the information related to a command.
 */
struct _Command {
  CommandCode code; /*!< Name of the command */
  int argsCount;
  char *arguments[MAX_CMD_ARGS_NUM];
  Status cmdStatus;

  char *commandInfo[N_CMD];
  Collection *stateCommands[N_GAME_STATES]; /*array of collections containing cmd types*/
};

/*--------------Private Functions---------------*/

/**
 * @brief Sets the number of readed arguments
 * @author Daniel Gómez
 * 
 * @param command 
 * @param num 
 * @return Status 
 */
Status command_set_arguments_count(Command *command, int num){
  if(!command) return ERROR;

  if(num < 0 || num > MAX_CMD_ARGS_NUM) return ERROR;

  command->argsCount = num;
  return OK;
}

/**
 * @brief Sets the argument to a given string at a given argument
 * @author Daniel Gómez
 * 
 * @param command 
 * @param args 
 * @param index 
 * @return Status 
 */
Status command_set_argument_at(Command *command, char args[MAX_CMD_ARGS_LENGTH], int index){

  if((!command || !args) && index >= 0 && index < MAX_CMD_ARGS_NUM) return ERROR;

  strcpy(command->arguments[index], args);
  
  return OK;
}

/**
 * @brief Resets the arguments to avoid getting them between commands
 * 
 * @param command 
 * @return Status 
 */
Status command_reset_arguments(Command *command){
  int i;
  char **arguments = NULL;

  if(!command)
    return ERROR;

  arguments = command_get_arguments(command);

  /*Sets each character of each argument to \0*/
  for(i = 0; i < MAX_CMD_ARGS_NUM; i++){
      arguments[i][0] = '\0';
  }

  return OK;
}

/*--------------Public Functions---------------*/

Command* command_create() {
  int i;
  Command* newCommand = NULL;

  /*Allocates memory and intializes to 0*/
  newCommand = (Command*)calloc(1,sizeof(Command));
  if (newCommand == NULL) {
    return NULL;
  }
  for (i = 0; i < MAX_CMD_ARGS_NUM; i++)
  {
    newCommand->arguments[i] = (char*)calloc(MAX_CMD_ARGS_LENGTH, sizeof(char));
    /*In case of error we free all the previous memory*/
    if(newCommand->arguments[i] == NULL){
      for (i--; i >= 0; i--)
      {
        free(newCommand->arguments[i]);
      }
      free(newCommand);
      return NULL;
    }
  }

  for (i = 0; i < N_GAME_STATES; i++)
  {
    newCommand->stateCommands[i] = collection_create(N_CMD, false, false, command_code_isEqual, NULL);
    if((newCommand->stateCommands[i]) == NULL){
      for (i--; i >= 0; i--)
      {
        collection_destroy(newCommand->stateCommands[i]);
      }
      for (i = 0; i < MAX_CMD_ARGS_NUM; i++)
      {
        free(newCommand->arguments[i]); 
      }
      free(newCommand);
      return NULL;
    }
  }
  

  /* Initialization of an empty command*/
  newCommand->code = NO_CMD;

  return newCommand;
}

Status command_destroy(Command* command) {
  int i;
  if(command){
    for (i = 0; i < MAX_CMD_ARGS_NUM; i++)
    {
      free(command->arguments[i]);
    }
    for (i = 0; i < N_CMD; i++)
    {
      if(command->commandInfo[i])
        free(command->commandInfo[i]);
    }

    for (i = 0; i < N_GAME_STATES; i++)
    {
      collection_free_elements(command->stateCommands[i], free);
      collection_destroy(command->stateCommands[i]);
    }
    
    free(command);
    return OK;
  }
  return ERROR;
}
/*----SETTERS----*/
Status command_set_code(Command* command, CommandCode code) {
  if (!command) {
    return ERROR;
  }

  command->code=code;

  return OK;
}

Status command_set_status(Command *command, Status status){
  if(!command) return ERROR;
  command->cmdStatus = status;
  return OK;
}

Status command_set_info(Command *command, CommandCode cmd, char *info){
  char *str = NULL;
  if(!command) return ERROR;

  str = malloc((strlen(info) + 1) * sizeof(char));
  if(!str) return ERROR;
  
  strcpy(str, info);

  command->commandInfo[cmd - NO_CMD] = str;
  return OK;
}

Status command_state_add_type(Command * command, GameState state, CommandCode type){
  CommandCode *code = NULL;
  
  if(!command) return ERROR;

  code = (CommandCode *)calloc(1, sizeof(CommandCode));
  if(!code) return ERROR;

  *code = type;

  return collection_add(command->stateCommands[state-ERROR_STATE], (void *)code);
}

/*----GETTERS----*/

Status command_get_as_string(Command *cmd, char *dest){
  int i;

  if(!cmd || !dest) return ERROR;

  for (i = 0; i < N_CMDT; i++)
  {
    strcat(dest,cmd_to_str[cmd->code - NO_CMD][i]);
    if(i < N_CMDT -1){
      strcat(dest," or ");
    }
  }
  strcat(dest, " ");
  for (i = 0; i < cmd->argsCount; i++)
  {
    strcat(dest, cmd->arguments[i]);
    if(i < cmd->argsCount){
      strcat(dest, " ");
    }
  }
  strcat(dest, ": ");
  strcat(dest, (cmd->cmdStatus == ERROR) ? "Error" : "Ok");
  return OK;
}

Status command_get_info(Command *command, CommandCode cmd, char *dest){
  int i;
  
  if(!command) return ERROR;

  for (i = 0; i < N_CMDT; i++)
  {
    strcat(dest,cmd_to_str[cmd - NO_CMD][i]);
    if(i < N_CMDT -1){
      strcat(dest," or ");
    }
  }
  strcat(dest, ": ");
  strcat(dest, command->commandInfo[cmd - NO_CMD]);

  return OK;
}

CommandCode command_get_code(Command* command) {
  if (!command) {
    return NO_CMD;
  }
  return command->code;
}

int command_get_arguments_count(Command *command){
  if(!command) return -1;
  return command->argsCount;
}

char **command_get_arguments(Command * command){
  if(!command) return NULL;
  return command->arguments;
}

CommandCode command_get_code_from_str(char *string){
  int i = UNKNOWN - NO_CMD + 1;
  CommandCode cmd = UNKNOWN;

  if(!string) return NO_CMD;

  while (cmd == UNKNOWN && i < N_CMD) {
    if (!strcasecmp(string, cmd_to_str[i][CMDS]) || !strcasecmp(string, cmd_to_str[i][CMDL])) {
      cmd = i + NO_CMD;
    } else {
      i++;
    }
  }
  return cmd;
}

Status command_get_user_input(Command* command) {
  char originalInput[CMD_LENGTH] = "";
  char input[CMD_LENGTH] = "";
  char *pAux = NULL;
  char *token = NULL;
  char aux[MAX_CMD_ARGS_LENGTH];
  
  char *fget = NULL;

  int i = UNKNOWN - NO_CMD + 1;
  int wordCount = 0, argsCount = 0, counter;
  int inputLength;
  CommandCode cmd;
  
  if (!command) {
    return ERROR;
  }

  fget = fgets(input, CMD_LENGTH, stdin);
  
  pAux = input;

  if(fget){
    while(*pAux == ' ') pAux++;

    if(strcmp(input, "\n") == 0){
      return OK;
    }
  }
  
  command_reset_arguments(command);
  
  if (fget) {
    
    /*Changes the \n for a 0 and copies to an aux string so it isnt loose when using strtok*/
    //input[strlen(input)-1] = 0;
    
    
    
    strcpy(originalInput, pAux);

    /*Reads which command is executed*/
    token = strtok(input, " \n");
    if (!token) {
      command_set_arguments_count(command, argsCount);
      return command_set_code(command, UNKNOWN);
    }
    
    /*Gets the command code from the string that has been read*/
    cmd = command_get_code_from_str(token);
    
    /*Stores the arguments of the commands*/
    wordCount = strlen(token);
    inputLength = strlen(originalInput);
    if(wordCount == inputLength -1){
      command_set_arguments_count(command,0);
      return command_set_code(command, cmd);
    }
    /*Tries to read all posible arguments*/
    for (i = 0; i < MAX_CMD_ARGS_NUM; i++)
    {
      /*Ignores blank spaces*/
      while(originalInput[wordCount] == ' ' && wordCount + 1 < inputLength) wordCount++;

      counter = 0;
      if(originalInput[wordCount] == '\n' || originalInput[wordCount] == '\00' || wordCount == inputLength)
        break;
      /*Counts the amount of letters and assing it to an aux string*/
      while(originalInput[wordCount + counter] != '\n' && originalInput[wordCount + counter] != '\00' && originalInput[wordCount + counter] != ' '
          && wordCount + counter < inputLength && counter < MAX_CMD_ARGS_LENGTH){
        
        aux[counter] = originalInput[wordCount + counter];
        counter++;
      }
      if(counter == MAX_CMD_ARGS_LENGTH){
        debug_log(LOG_WARNING,"at command_get_user_input : argument count reached size limit tho it will be inclomplete");
      }
      /*We end the string by putting a null character*/
      aux[counter] = '\00';
      wordCount += counter;
      command_set_argument_at(command, aux, i);
    }
    
    command_set_arguments_count(command,i);
    return command_set_code(command, cmd);
  }
  else
    return command_set_code(command, EXIT); 
}

Status command_get_list(Command *command, char *destination, GameState state, bool getAll){
  char *aux;
  int i,j;
  int length;

  Collection *codeList = NULL;
  CommandCode code = NO_CMD;

  if(destination == NULL){
    return ERROR;
  }

  if(getAll){
    aux = (char *)calloc((N_CMD) * (N_CMDT) * (CMD_LENGTH) + WORD_SIZE, sizeof(char));
    if (aux == NULL){
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

  codeList = command->stateCommands[state - ERROR_STATE];
  length = collection_length(codeList);

  aux = (char *)calloc((length) * (N_CMDT) * (CMD_LENGTH) + WORD_SIZE, sizeof(char));
  if (aux == NULL){
    return ERROR;
  }

  for (i = 0; i < length; i++)
  {
    code = *((CommandCode *)collection_get_element_at(codeList, i));
    for (j = 0; j < N_CMDT; j++)
    {
      strcat(aux, cmd_to_str[code - NO_CMD][j]);
      if(j < N_CMDT -1){
        strcat(aux," or ");
      }

    }
    if(i < length-1){
      strcat(aux,", ");
    }
  }
  
  strcpy(destination,aux);
  free(aux);
  return OK;
}

Status command_get_status(Command *command){
  if(!command) return ERROR;
  return command->cmdStatus;
}

int command_code_isEqual(void *cmd1, void *cmd2){
  if(!cmd1 || !cmd2) return -1;
  return *((CommandCode *)cmd1) != *((CommandCode *)cmd2);
}
bool command_current_type_valid_by_state(Command *command, GameState state){
  int length, i;
  if(!command || state == ERROR_STATE) return false;

  length = collection_length(command->stateCommands[state - ERROR_STATE]);

  for (i = 0; i < length; i++)
  {
    if(command->code == *((CommandCode *)collection_get_element_at(command->stateCommands[state - ERROR_STATE], i))){
      return true;
    }
  }
  return false;
}