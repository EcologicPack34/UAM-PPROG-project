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

typedef struct{
    float x, y;
}Vector2;


void vector2_add(Vector2 *v1, Vector2 v2);

void vector2_mult(Vector2 *v, float value);

int vector2_dot(Vector2 v1, Vector2 v2);

int vector2_isEqual(void *v1, void *v2);

#endif