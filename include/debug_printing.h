/**
 * @file debug_printing.h
 * @author Daniel Gómez Rodríguez
 * @brief Creates a log to make debug easily
 * @version 1.1
 * @date 2025-01-30
 *
 * @copyright GNU Public License
 *
 */
#ifndef DEBUG_PRINTING_H
#define DEBUG_PRINTING_H

#include <stdbool.h>

#define MAX_FILE_PATH_LENGTH 32 /*!< Max length of the path with the file*/

/**
 * @brief LOG_LEVEL - Data type for the different log types
 */
typedef enum _LOG_LEVEL
{
    PRINT = 10,
    LOG_WARNING = 20,
    LOG_ERROR = 30,
    DEBUG = 40
} LOG_LEVEL;

/**
 * @brief Data type containing information like the log file path
 */
typedef struct _Debug Debug;

/**
 * @brief Creates and Initializes a Debug
 * @author Daniel Gómez
 *
 * @param debugFilePath path to the debug file
 * @param setAsGlobal bool true if is set as global or false if not set as global
 * @return Debug*
 */
Debug *debug_create(char *debugFilePath, bool setAsGlobal);

/**
 * @brief Frees all the memory and closes file
 * @author Daniel Gómez
 *
 * @param info debug struct
 */
void debug_destroy(Debug *info);

/**
 * @brief Prints a message with format into the log file
 * @author Daniel Gómez
 *
 * @param level log level enum for message type
 * @param formatedString string with the information to print
 * @param ...
 * @return int
 */
int debug_log(LOG_LEVEL level, char *formatedString, ...);

/**
 * @brief Returns the path of the log file
 * @author Daniel Gómez
 *
 * @param info debug struct
 * @return char*
 */
char *debug_getPath(Debug *info);

/**
 * @brief Closes the global debug file without the need to destroy the debug
 *  USE ONLY BEFORE ABORTING PROGRAM, OTHER WISE IT WILL BREAK DEBUG LOGS
 */
void debug_force_global_fclose();

#endif