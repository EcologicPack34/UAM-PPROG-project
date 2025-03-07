/**
 * @file vector2.h
 * @author Daniel Gómez
 * @brief 
 * @version 0.1
 * @date 2025-03-06
 * 
 * @copyright GNU Public License
 */

#ifndef VECTOR2_H
#define VECTOR2_H

/**
 * @brief Data structure to store 2 dimensional values
 */
typedef struct{
    float x, y;
}Vector2;

/**
 * @brief Adds v2 to v1 coordenate by coordenate
 * @author Daniel Gómez
 * 
 * @param v1 
 * @param v2 
 */
void vector2_add(Vector2 *v1, Vector2 v2);

/**
 * @brief Multiplies each component of v by value
 * @author Daniel Gómez
 * 
 * @param v 
 * @param value 
 */
void vector2_mult(Vector2 *v, float value);

/**
 * @brief Calculates the dot product between 2 vectors
 * @author Daniel Gómez
 * 
 * @param v1 
 * @param v2 
 * @return int 
 */
int vector2_dot(Vector2 v1, Vector2 v2);

/**
 * @brief Compares 2 vectores
 * @author Daniel Gómez
 * 
 * @param v1 
 * @param v2 
 * @return 0 if equal, -1 if not, -2 if error
 */
int vector2_isEqual(void *v1, void *v2);

/**
 * @brief Copies v2 into v1. Both vectors must have been intialized before
 * @author Daniel Gómez
 *   
 * @param v1 
 * @param v2 
 */
void vector2_copy(Vector2 *v1, Vector2 *v2);

#endif