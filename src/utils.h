/**
 * @file utils.h
 * @author Daniel Gómez
 * @brief 
 * @version 0.1
 * @date 2025-02-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef UTILS_H
#define UTILS_H

/* Util para hacer un graphics engine nuevo
#define console_clear() printf("\033[H\033[J")
#define console_gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
*/

typedef struct _Random Random;

/**
 * @brief Gets a random double
 * 
 * @param minInclusive 
 * @param maxInclusive 
 * @return double 
 */
double utils_get_random_double(double minInclusive, double maxInclusive);

/**
 * @brief Gets a random long
 * 
 * @param minInclusive 
 * @param maxExclusive 
 * @return long 
 */
long utils_get_random_long(long minInclusive, long maxExclusive);

/**
 * @brief Generic function to sort arrays
 * 
 * @param array pointer to the array
 * @param length length of the array
 * @param comparison function to compare elements of the array, must have to void* inputs
 * @return int 
 */
bool utils_sort_array(void *array, long length, int (*comparison)(void *, void *));

#endif

