/**
 * @file debug_printing.h
 * @author Daniel Gómez Rodríguez
 * @brief
 * @version 1.  1
 * @date 2025-01-30
 *
 * @copyright GNU Public License
 *
 */
#ifndef DEBUG_PRINTING_H
#define DEBUG_PRINTING_H

#define MAX_FILE_PATH_LENGTH 32

/**
 * @brief LOG_LEVEL
 *
 * Data type for the different log types
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
 *
 */
typedef struct _Debug Debug;

/**
 * @brief Creates and Initializes a Debug
 *
 * @param debugFilePath
 * @return Debug*
 */
Debug *debug_create(char *debugFilePath, short setAsGlobal);

/**
 * @brief Frees all the memory and closes file
 *
 * @param info
 */
void debug_destroy(Debug *info);

/**
 * @brief Prints a message with format into the log file
 *
 * @param info
 * @param level
 * @param formatedString
 * @param ...
 * @return int
 */
int debug_log(LOG_LEVEL level, char *formatedString, ...);

/**
 * @brief Returns the path of the log file
 *
 * @param info
 * @return char*
 */
char *debug_getPath(Debug *info);

#endif