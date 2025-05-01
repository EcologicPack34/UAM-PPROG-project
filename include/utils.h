/**
 * @file utils.h
 * @author Daniel Gómez
 * @brief 
 *      Module that implements various utilities that can be used in a general use case.
 * 
 *      NOT-IMPLEMENTED
 * 
 * @version 0.1
 * @date 2025-02-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef UTILS_H
#define UTILS_H

#include "types.h"

/* Util para hacer un graphics engine nuevo
#define console_clear() printf("\033[H\033[J")
#define console_gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
*/

/**
 * @brief Removes a newline escape sequence from a string if found
 * @author Maksym Polyak
 * 
 * @param str to evaluate
 * @return Status 
 */
Status string_remove_newline_escape_sequence_on_end(char *str);

/**
 * @brief Removes an end of line escape sequence from a string if found
 * and adds a newline escape sequence and a end of string
 * @author Maksym Polyak
 * 
 * @param str to evaluate
 * @return Status 
 */
Status string_remove_endofline_escape_sequence_on_end_to_newline(char *str);

#endif

