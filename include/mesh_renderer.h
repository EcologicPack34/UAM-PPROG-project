/**
 * @file mesh_renderer.h
 * @author your name (you@domain.com)
 * @brief Module aimed to unify mesh, shaders and textures all together
 * @version 0.1
 * @date 2025-05-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "mesh.h"
#include "shader.h"
#include "texture2D.h"

#include <cglm/cglm.h>

typedef struct _MeshRenderer* MeshRenderer;

MeshRenderer meshr_new();

/**
 * @brief Frees memory for mesh renderer, mesh, shader and textures
 * @author Daniel Gómez
 * 
 * @param meshr 
 */
void meshr_destroy(void *meshr);

bool meshr_set_mesh(MeshRenderer meshr , Mesh mesh);

bool meshr_set_shader(MeshRenderer meshr, Shader shader);

bool meshr_add_tex2D(MeshRenderer meshr, Texture2D tex);

Shader meshr_get_shader(MeshRenderer meshr);

Texture2D meshr_get_tex2D_at(MeshRenderer meshr, unsigned int index);

void meshr_draw(MeshRenderer meshr);

bool meshr_set_position(MeshRenderer meshr, vec3 pos);

bool meshr_set_rotation(MeshRenderer meshr, vec3 rot);

bool meshr_set_scale(MeshRenderer meshr, vec3 scale);

#endif