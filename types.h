/**
 * @brief It defines common types for the whole project
 *
 * @file types.h
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef TYPES_H
#define TYPES_H

#define WORD_SIZE 500 /*!< Maximum word size */
#define NO_ID -1 /*!< ERROR ID value */
#define UNDEFINED_ID 0 /*!< Value of a non assigned ID, usefull to not confuse errors with intended behaviour*/

typedef long Id; /*!< Id typedef */

typedef enum { FALSE, TRUE } Bool; /*!< Bool variable that admits TRUE or FALSE */

typedef enum { ERROR, OK } Status; /*!< Status enum that admits ERROR or OK */

typedef enum { N, S, E, W } Direction; /*!< Directions of the map: North, South, East, West */

#endif
