/**
 * @file graphic_description.h
 * @author Daniel Gómez
 * @brief ADT for generalized graphic description
 * @version 0.1
 * @date 2025-04-28
 * 
 * @copyright GNU Public License
 */

#ifndef GDESC_H
#define GDESC_H

#include "types.h"
#include <stdio.h>

typedef enum {NO_DESC = 0, SPACE_DESC, ENTITY_DESC} GDescType;

/**
 * @brief Datatype containing the information of a graphic description 
 */
typedef struct _GDesc GDesc;

/**
 * @brief Creates a graphic description
 * @author Daniel Gómez
 * 
 * @param height 
 * @param width 
 * @return GDesc* 
 */
GDesc *gdesc_create(Id id, int height, int width, GDescType type);

/**
 * @brief Destroys and frees memory for a graphic description
 * @author Daniel Gómez
 * 
 * @param gdesc 
 */
void gdesc_destroy(void *gdesc);

/*-------SETTERS---------*/

/**
 * @brief Sets a line of the gdesc to a specified string by making a copy of it
 * @author Daniel Gómez
 * 
 * @param gdesc 
 * @param line 
 * @param content 
 * @return Status 
 */
Status gdesc_set_line(GDesc *gdesc, int line, char *content);

/*------GETTERS------*/

/**
 * @brief Gets the id of a graphic description
 * @author Daniel Gómez
 * 
 * @param gdesc 
 * @return Id 
 */
Id gdesc_get_id(GDesc *gdesc);

/**
 * @brief Gets a line of the gdesc
 * @author Daniel Gómez
 * 
 * @param gdesc 
 * @param line 
 * @return pointer to the line
 */
char *gdesc_get_line(GDesc *gdesc, int line);

/**
 * @brief Gets the type of graphic description
 * @author Daniel Gómez
 * 
 * @param gdesc 
 * @return GDescType 
 */
GDescType gdesc_get_type(GDesc *gdesc);

/*-----OTHERS-----*/

/**
 * @brief Compares two graphic descriptions by their ID
 * @author Daniel Gómez
 *  
 * @param e1 
 * @param e2 
 * @return negative if e1<e2, 0 if equal, positive if e1>e2
 */
int gdesc_cmp(void *e1, void *e2);

/**
 * @brief Saves a gdesc on a file
 * @author Maksym Polyak
 * 
 * @param gdesc graphic description
 * @param fOUT file stream output
 * @return int with num of char printed or -1 if error
 */
int gdesc_save_on_file(GDesc *gdesc, FILE *fOUT);

/**
 * @brief Creates a gdesc from a file stream
 * @author Maksym Polyak
 * 
 * @param fIN file stream input
 * @return GDesc* or NULL if error
 */
GDesc *gdesc_create_from_file(FILE *fIN);

#endif