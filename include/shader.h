/**
 * @file shader.h
 * @author your name (you@domain.com)
 * @brief Small module aimed to simplify shader loading and compilation
 * @version 0.1
 * @date 2025-05-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include <cglm/cglm.h>

typedef struct _Shader{
    unsigned int ID;
}* Shader;

Shader shader_new(const char *vertexPath, const char *fragmentPath);

void shader_destroy(void *shader);

void shader_use(Shader shader);

void shader_set_Bool(Shader shader, const char *name, bool value);

void shader_set_Int(Shader shader, const char *name, int value);

void shader_set_Float(Shader shader, const char *name, float value);

void shader_set_Mat4(Shader shader, const char *name, mat4 value);

#endif