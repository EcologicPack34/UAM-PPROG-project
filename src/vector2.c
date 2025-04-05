/**
 * @file vector2.c
 * @author Daniel Gómez
 * @brief Vector internal funcitonality
 * @version 0.1
 * @date 2025-04-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "vector2.h"

void vector2_add(Vector2 *v1, Vector2 v2){
    if(!v1) return;
    v1->x += v2.x;
    v1->y += v2.y;
}


void vector2_mult(Vector2 *v, float value){
    if(!v) return;
    v->x *= value;
    v->y *= value;
}

int vector2_dot(Vector2 v1, Vector2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}

int vector2_isEqual(void *v1, void *v2){
    Vector2 *vt1;
    Vector2 *vt2;
    
    if(!v1 || !v2) return -2;

    vt1 = (Vector2 *)v1;
    vt2 = (Vector2 *)v2;

    if(vt1->x == vt2->x && vt1->y == vt2->y) return 0;
    return 1;
}

void vector2_copy(Vector2 *v1, Vector2 *v2){
    if(!v1 || !v2) return;

    v1->x = v2->x;
    v1->y = v2->y;
}
