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

Vector2 vector2_new(float x, float y);

Vector2 vector2_add(Vector2 v1, Vector2 v2);

void vector2_add_to(Vector2 *v1, Vector2 v2);

Vector2 vector2_mult(Vector2 v, float value);

Vector2 vector2_mult_to(Vector2 *v1, float value);

#endif