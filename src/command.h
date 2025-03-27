/**
 * @brief It defines the command interpreter interface
 * 
 * Commands are defined by a CommandCode, command regulates the interaction 
 * between the player and the game, and translates the player intention into 
 * a CommandCode for game_action.
 * 
 * This module also makes sure that till the user doesn't enter a command, 
 * the game does not keep running.
 *
 * @file command.h
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "types.h"

#define N_CMDT 2 /*!< Total number of CommandType in the enum */
#define N_CMD 14  /*!< Total number of CommandCode in the enum */

#define MAX_CMD_ARGS_NUM 5      /*!< Max number of arguments that can be passed to a command*/
#define MAX_CMD_ARGS_LENGTH 20  /*!< Max length that the arguments of a command can have */

typedef enum { CMDS, CMDL } CommandType; /* CMDS: Command Short | CMDL: Command Long */

typedef enum { NO_CMD = -1, UNKNOWN, EXIT, SOUTH, NORTH, EAST, WEST, TAKE, DROP, CHAT, ATTACK, RUN_AWAY, SWITCH, ABILITY} CommandCode; /* All values that a command can take */

/* 
 * Struct that contains all the information of the command
 */
typedef struct _Command Command;


/**
 * @brief Creates a command dynamically
 * @author Profesores PPROG
 *
 * @return command pointer, initialized to NO_CMD
 */
Command* command_create();

/**
 * @brief Frees command memory
 * @author Profesores PPROG
 *
 * @param command saves all information related to a command
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status command_destroy(Command* command);

/**
 * @brief changes name of the command on the command struct
 * @author Profesores PPROG
 *
 * @param command struct that saves all information related to a command
 * @param code command name
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status command_set_code(Command* command, CommandCode code);

/**
 * @brief Sets the status of a given command
 * @author Daniel Gómez
 * 
 * @param command 
 * @param status 
 * @return Status 
 */
Status command_set_status(Command *command, Status status);

/**
 * @brief Gets the command name
 * @author Profesores PPROG
 *
 * @param command saves all information related to a command
 * @return retrieves code from command struct
 */
CommandCode command_get_code(Command* command);

/**
 * @brief Gets the command code for a given string value
 * @author Daniel Gómez
 * 
 * @param string 
 * @return CommandCode 
 */
CommandCode command_get_code_from_str(char *string);

/**
 * @brief Gets the number of arguments in the command
 * @author Daniel Gómez
 * 
 * @param command 
 * @return returns the number of arguments or -1 if error
 */
int command_get_arguments_count(Command *command);

/**
 * @brief Gets the array of command arguments
 * @author Daniel Gómez
 * 
 * @param command 
 * @return returns the pointer to the array or null if error
 */
char **command_get_arguments(Command * command);

/**
 * @brief Receives the user input and sets the command name to that of the input
 * @author Original: Profesores Prog, Modified by: Daniel Gómez
 *
 * @param command struct that saves all information related to a command
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status command_get_user_input(Command* command);

/**
 * @brief Gets the status of a given Command
 * @author Daniel Gómez
 * 
 * @param command 
 * @return Status 
 */
Status command_get_status(Command *command);

/**
 * @brief makes a string of the current commands and copies it to a given string
 * @author Daniel Gómez
 * 
 * @param destination string to which the information is copied. Requires the user to assing this string a proper length
 * @return Status
 */
Status command_get_list(char *destination);

#endif