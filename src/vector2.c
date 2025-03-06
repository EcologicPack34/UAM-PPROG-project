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
