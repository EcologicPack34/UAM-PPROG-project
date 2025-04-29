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
#include <stdbool.h>

#define N_CMDT 2    /*!< Total number of CommandType in the enum */
#define N_CMD 24    /*!< Total number of CommandCode in the enum */

#define MAX_CMD_ARGS_NUM 5      /*!< Max number of arguments that can be passed to a command*/
#define MAX_CMD_ARGS_LENGTH 20  /*!< Max length that the arguments of a command can have */

/**
 * @brief Enum with the type of expression of a command
 */
typedef enum { CMDS, CMDL } CommandType; /* CMDS: Command Short | CMDL: Command Long */

/**
 * @brief Enum with the codes of all the commands in the game
 */
typedef enum { NO_CMD = -1, UNKNOWN, EXIT, SOUTH, NORTH, EAST, WEST, TAKE, DROP, CHAT,\
     ATTACK, RUN_AWAY, SWITCH, ABILITY, OBJECT_USE, HELP, MOVE, SEARCH, EQUIP, UNEQUIP, INSPECT, GM,\
    UP, DOWN} CommandCode; /* All values that a command can take */

/**
 * @brief ADT with player related information
 */
typedef struct _CommandInfo CommandInfo;

/**
 * @brief ADT with game related information to command
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
 * @brief Creates a struct of command info dynamically
 * @author Daniel Gómez
 * 
 * @return CommandInfo* 
 */
CommandInfo *command_info_create();

/**
 * @brief Destroys a struct of command info
 * @author Daniel Gómez
 * 
 * @param cmdData command info to destroy
 * @return Status 
 */
Status command_info_destroy(CommandInfo *cmdData);


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
 * @param command command to modify
 * @param status status to set
 * @return Status 
 */
Status command_set_status(Command *command, Status status);

/**
 * @brief Sets the description of a command
 * @author Daniel Gómez
 * 
 * @param command command to modify
 * @param cmd command code where info is set
 * @param info info to set
 * @return Status 
 */
Status command_set_info(Command *command, CommandCode cmd, char *info);

/**
 * @brief Adds a command code to a game state valid commands
 * @author Daniel Gómez
 * 
 * @param command command to modify
 * @param state state where the command code type is added
 * @param type command code type to add
 * @return Status 
 */
Status command_state_add_type(Command * command, GameState state, CommandCode type);

/**
 * @brief Sets the data struct which will be used to store data
 * @author Daniel Gómez
 * 
 * @param cmd command struct
 * @param data command info
 * @return Status 
 */
Status command_set_player_data(Command *cmd, CommandInfo *data);

/**
 * @brief Updates the data of cmd stored by player
 * 
 * @param cmd command struct
 * @return Status 
 */
Status command_update_player_data(Command *cmd);

/*-----------GETTERS--------------*/

/**
 * @brief Copies into dest the info of the current command.
 * @author Daniel Gómez
 * 
 * @param cmd command where the info is taken
 * @param dest string where the info is copied
 * @return Status 
 */
Status command_get_as_string(Command *cmd, char *dest);

/**
 * @brief Gets if the current command is valid by checking the game state
 * @author Daniel Gómoez
 * 
 * @param command command struct
 * @param state state to check if its valid
 * @return true if valid
 * @return false if not valid
 */
bool command_current_type_valid_by_state(Command *command, GameState state);

/**
 * @brief Gets the pointer to the string containing the info of a command
 * @author Daniel Gómez
 * 
 * @param command command struct
 * @param cmd command code where info is taken
 * @param dest string where info is copied
 * @return Status 
 */
Status command_get_info(Command *command, CommandCode cmd, char *dest);

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
 * @param string string of a type cmd_to_str
 * @return CommandCode 
 */
CommandCode command_get_code_from_str(char *string);

/**
 * @brief Gets the number of arguments in the command
 * @author Daniel Gómez
 * 
 * @param command command struct
 * @return returns the number of arguments or -1 if error
 */
int command_get_arguments_count(Command *command);

/**
 * @brief Gets the array of command arguments
 * @author Daniel Gómez
 * 
 * @param command command struct
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
 * @param command command struct
 * @return Status 
 */
Status command_get_status(Command *command);

/**
 * @brief Makes a string of the current commands and copies it to a given string
 * @author Daniel Gómez
 * 
 * @param command command struct
 * @param destination string to which the information is copied. Requires the user to assing this string a proper length
 * @param state state of the game
 * @param getAll Wheter it gets all of them or not
 * @return Status
 */
Status command_get_list(Command *command, char *destination, GameState state, bool getAll);

/**
 * @brief Checks if two commands codes are equal
 * 
 * @param cmd1 command struct 1
 * @param cmd2 command struct 2
 * @return int 
 */
int command_code_isEqual(void *cmd1, void *cmd2);

#endif