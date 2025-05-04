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
#include "utils.h"

#define CMD_LENGTH 500 /*!< Maximum length of a command */

/**
 * @brief Global variable that stores all the commands and their shortucts.
 */
char *cmd_to_str[N_CMD][N_CMDT] = {{"", "No command"}, {"", "Unknown"}, {"q", "Exit"}, {"s", "South"}, {"w", "North"}, {"d", "East"}, {"a", "West"},{"tk", "Take"}, {"dr", "Drop"},\
 {"ch", "Chat"}, {"at", "Attack"}, {"h", "Run_Away"}, {"sw", "Switch_Player"}, {"sk", "Ability"}, {"ou", "Object_Use"}, {"hp", "Help"}, {"m", "Move"}, {"sh","Search"},\
 {"eq", "Equip"}, {"ue", "Unequip"}, {"i", "Inspect"}, {"4444444444444444", "4444444444444444"}, {"up", "Up"}, {"dw", "Down"}, {"lu", "Level_Up"}, {"by","Buy"}, {"fw","Follow"}};

/**
* @brief Struct containing the info of a command
*/
struct _CommandInfo{
  CommandCode code; /*!< Name of the command */
  int argsCount;  /*!< Number of arguments in the command*/
  char *arguments[MAX_CMD_ARGS_NUM]; /*!< Arguments of the command*/
  Status cmdStatus; /*!< Status of command*/
};

/**
 * @brief Command
 *
 * This struct stores general information about command
 */
struct _Command {
  CommandInfo *cmdData; /*!<Struct that contains info about last command*/
  CommandInfo *cmdPlayerData; /*!<Struct local to player that contains info about last command*/

  char *commandInfo[N_CMD]; /*!< array of strings containing the info of the commands*/
  Collection *stateCommands[N_GAME_STATES]; /*!<array of collections containing cmd types*/
};

/*--------------Private Functions---------------*/

/**
 * @brief Copies data from data2 into data1
 * @author Daniel Gómez
 * 
 * @param data1 command info 1
 * @param data2 command info 2
 * @return Status 
 */
Status command_info_copy(CommandInfo *data1, CommandInfo *data2){
  if(!data1 || !data2) return ERROR;

  data1->argsCount = data2->argsCount;
  data1->cmdStatus = data2->cmdStatus;
  data1->code = data2->code;

  for (int i = 0; i < MAX_CMD_ARGS_NUM; i++)
  {
    strncpy(data1->arguments[i], data2->arguments[i], MAX_CMD_ARGS_LENGTH);
  }
  return OK;
}

/**
 * @brief Sets the number of readed arguments
 * @author Daniel Gómez
 * 
 * @param command command struct
 * @param num count to set
 * @return Status 
 */
Status command_set_arguments_count(Command *command, int num){
  if(!command) return ERROR;

  if(!(command->cmdData)) return ERROR;

  if(num < 0 || num > MAX_CMD_ARGS_NUM) return ERROR;

  command->cmdData->argsCount = num;
  return OK;
}

/**
 * @brief Sets the argument to a given string at a given argument
 * @author Daniel Gómez
 * 
 * @param command command struct
 * @param args string to set
 * @param index index of where it is set
 * @return Status 
 */
Status command_set_argument_at(Command *command, char args[MAX_CMD_ARGS_LENGTH], int index){

  if(!(command->cmdData)) return ERROR;
  if((!command || !args) && index >= 0 && index < MAX_CMD_ARGS_NUM) return ERROR;

  strcpy(command->cmdData->arguments[index], args);
  
  return OK;
}

/**
 * @brief Resets the arguments to avoid getting them between commands
 * @author Daniel Gómez
 * 
 * @param command command struct
 * @return Status 
 */
Status command_reset_arguments(Command *command){
  int i;
  char **arguments = NULL;

  if(!command) return ERROR;
  if(!(command->cmdData)) return ERROR;

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

  newCommand->cmdData = command_info_create();
  if(!(newCommand->cmdData)){
    free(newCommand);
    return NULL;
  }

  for (i = 0; i < N_GAME_STATES; i++)
  {
    newCommand->stateCommands[i] = collection_create(N_CMD, false, false, command_code_isEqual, NULL);
    if((newCommand->stateCommands[i]) == NULL){
      for (i--; i >= 0; i--)
      {
        collection_destroy(newCommand->stateCommands[i]);
      }
      command_info_destroy(newCommand->cmdData);
      free(newCommand);
      return NULL;
    }
  }

  return newCommand;
}

Status command_destroy(Command* command) {
  int i;
  if(command){
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
    command_info_destroy(command->cmdData);
    free(command);
    return OK;
  }
  return ERROR;
}

CommandInfo *command_info_create(){
  CommandInfo *cmdData = NULL;

  cmdData = (CommandInfo *)calloc(1, sizeof(CommandInfo));
  if(!cmdData) return NULL;

  cmdData->cmdStatus = ERROR;
  cmdData->code = NO_CMD;
  cmdData->argsCount = 0;

  for (int i = 0; i < MAX_CMD_ARGS_NUM; i++)
  {
    cmdData->arguments[i] = calloc(MAX_CMD_ARGS_LENGTH, sizeof(char));
    if(!(cmdData->arguments[i])){
      for ( i-- ; i >= 0; i--)
      {
        free(cmdData->arguments[i]);
      }
      return NULL;
    }
  }
  return cmdData;
}

Status command_info_destroy(CommandInfo *cmdData){
  if(cmdData){
    for (int i = 0; i < MAX_CMD_ARGS_NUM; i++)
    {
      if(cmdData->arguments[i]) free(cmdData->arguments[i]);
    }
    free(cmdData);
    return OK;
  }
  return ERROR;
}

/*----SETTERS----*/
Status command_set_code(Command* command, CommandCode code) {
  if (!command) {
    return ERROR;
  }
  if(!(command->cmdData)) return ERROR;
  command->cmdData->code=code;

  return OK;
}

Status command_set_status(Command *command, Status status){
  if(!command) return ERROR;
  if(!(command->cmdData)) return ERROR;
  command->cmdData->cmdStatus = status;
  return OK;
}

Status command_set_info(Command *command, CommandCode cmd, char *info){
  char *str = NULL;
  if(!command || !info) return ERROR;

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

Status command_set_player_data(Command *cmd, CommandInfo *data){
  if(!cmd || !data) return ERROR;
  cmd->cmdPlayerData = data;
  return OK;
}

Status command_update_player_data(Command *cmd){
  if(!cmd) return ERROR;
  if(!(cmd->cmdPlayerData)) return ERROR;

  cmd->cmdPlayerData->argsCount = cmd->cmdData->argsCount;
  cmd->cmdPlayerData->cmdStatus = cmd->cmdData->cmdStatus;
  cmd->cmdPlayerData->code = cmd->cmdData->code;

  for (int i = 0; i < MAX_CMD_ARGS_NUM; i++)
  {
    strncpy(cmd->cmdPlayerData->arguments[i], cmd->cmdData->arguments[i], MAX_CMD_ARGS_LENGTH);
  }
  return OK;
}

/*----GETTERS----*/

Status command_get_as_string(Command *cmd, char *dest){
  int i;

  if(!cmd || !dest) return ERROR;
  if(!(cmd->cmdPlayerData)) return ERROR;

  /*North(n) arg1 arg2 arg3 : Ok*/
  /*Take(tk) tk Grain1 : Ok*/

  strcat(dest, cmd_to_str[cmd->cmdPlayerData->code - NO_CMD][1]);
  strcat(dest, " (");
  strcat(dest, cmd_to_str[cmd->cmdPlayerData->code -NO_CMD][0]);
  strcat(dest, ")");
  strcat(dest, " ");
  for (i = 0; i < cmd->cmdPlayerData->argsCount; i++)
  {
    strcat(dest, cmd->cmdPlayerData->arguments[i]);
    if(i < cmd->cmdPlayerData->argsCount){
      strcat(dest, " ");
    }
  }
  strcat(dest, ": ");
  strcat(dest, (cmd->cmdPlayerData->cmdStatus == ERROR) ? "Error" : "Ok");
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
  if(!(command->cmdData)) return NO_CMD;
  return command->cmdData->code;
}

int command_get_arguments_count(Command *command){
  if(!command) return -1;
  if(!(command->cmdData)) return -1;
  return command->cmdData->argsCount;
}

char **command_get_arguments(Command * command){
  if(!command) return NULL;
  if(!(command->cmdData)) return NULL;
  return command->cmdData->arguments;
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
  if(!(command->cmdData)) return ERROR;

  fget = fgets(input, CMD_LENGTH, stdin);
  
  pAux = input;

  if(fget){
    while(*pAux == ' ') pAux++;

    if(strcmp(input, "\n") == 0){
      if(command->cmdPlayerData){
        command_info_copy(command->cmdData, command->cmdPlayerData);
      }
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
  int i;
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
      if(i == GM - NO_CMD) continue;

      strcat(aux, cmd_to_str[i][1]);
      strcat(aux, " (");
      strcat(aux, cmd_to_str[i][0]);
      strcat(aux, ")");
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

    strcat(aux, cmd_to_str[code -NO_CMD][1]);
    strcat(aux, " (");
    strcat(aux, cmd_to_str[code -NO_CMD][0]);
    strcat(aux, ")");

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
  if(!(command->cmdData)) return ERROR;
  return command->cmdData->cmdStatus;
}

int command_code_isEqual(void *cmd1, void *cmd2){
  if(!cmd1 || !cmd2) return -1;
  return *((CommandCode *)cmd1) != *((CommandCode *)cmd2);
}
bool command_current_type_valid_by_state(Command *command, GameState state){
  int length, i;
  if(!command || state == ERROR_STATE) return false;
  if(!(command->cmdData)) return ERROR;

  length = collection_length(command->stateCommands[state - ERROR_STATE]);

  for (i = 0; i < length; i++)
  {
    if(command->cmdData->code == *((CommandCode *)collection_get_element_at(command->stateCommands[state - ERROR_STATE], i))){
      return true;
    }
  }
  return false;
}

int command_save_on_file(Command *command, FILE *fOUT){
  int i, j, count = 0, size;
  CommandCode *code = NULL;

  if(!command || !fOUT) return -1;

  count += command_info_save_on_file(command->cmdData, fOUT);
  count += command_info_save_on_file(command->cmdPlayerData, fOUT);

  for(i = 0; i < N_CMD; i++){
    count += fprintf(fOUT, "%s\n", command->commandInfo[i]);
  }

  for(i = 0; i < N_GAME_STATES; i++){
    size = collection_length(command->stateCommands[i]);
    count += fprintf(fOUT, "%d\n", size);
    for(j = 0; j < size; j++){
      code = collection_get_element_at(command->stateCommands[i], j);
      count += fprintf(fOUT, "%d,", (int)*code);
    }
    count += fprintf(fOUT, "\n");
  }

  return count;
}

Status command_read_from_file(Command *command, FILE *fIN){
  int i, j, size;
  CommandInfo *ci = NULL;
  char str[WORD_SIZE] = "", *toks = NULL;;
  CommandCode *code = NULL;

  if(!command || !fIN) return ERROR;

  command_info_destroy(command->cmdData);
  ci = command_info_create();
  if(!ci) return ERROR;
  command_info_read_from_file(ci, fIN);
  command->cmdData = ci;

  ci = command_info_create();
  if(!ci) return ERROR;
  command_info_read_from_file(ci, fIN);
  command->cmdPlayerData = ci;

  fgets(str, WORD_SIZE, fIN);

  for(i = 0; i < N_CMD; i++){
    fgets(str, WORD_SIZE, fIN);
    string_remove_newline_escape_sequence_on_end(str);
    command_set_info(command, (CommandCode)i, str);
  }

  for(i = 0; i < N_GAME_STATES; i++){
    fscanf(fIN,"%d\n", &size);
    fgets(str,WORD_SIZE, fIN);
    if(size > 0){
      toks = strtok(str, ",");
    }
    for(j = 0; j < size; j++){
      toks = strtok(NULL,",");
      if(toks == NULL) return ERROR;
      code = (CommandCode *)malloc(sizeof(CommandCode));
      if(!code) return ERROR;
      *code = (CommandCode)atoi(toks);
    }
  }

  return OK;
}

int command_info_save_on_file(CommandInfo *cminfo, FILE *fOUT){
  int count = 0, i;

  if(!cminfo || !fOUT) return -1;

  count += fprintf(fOUT,"%d;%d;%d\n", cminfo->code, cminfo->cmdStatus, cminfo->argsCount);
  for(i = 0; i < cminfo->argsCount; i++){
    count += fprintf(fOUT,"%s\n", cminfo->arguments[i]);
  }

  return count;
}

Status command_info_read_from_file(CommandInfo *cminfo, FILE *fIN){
  int code, argsCount, i, cmdStatus;
  char str[MAX_CMD_ARGS_LENGTH];

  if(!cminfo || !fIN) return ERROR;

  fscanf(fIN, "%d;%d;%d\n", &code, &cmdStatus, &argsCount);

  cminfo->argsCount = argsCount;
  cminfo->code = (CommandCode)code;
  cminfo->cmdStatus = (Status)cmdStatus;

  for(i = 0; i < cminfo->argsCount; i++){
    fgets(str, MAX_CMD_ARGS_LENGTH, fIN);
    string_remove_newline_escape_sequence_on_end(str);
    strcpy(cminfo->arguments[i], str);
    if(!cminfo->arguments[i]) return ERROR;
  }

  return OK;
}