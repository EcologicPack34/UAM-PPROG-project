/**
 * @file mesh.h
 * @author your name (you@domain.com)
 * @brief Module aimed to store and render generic shapes
 * @version 0.1
 * @date 2025-05-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef MESH_H
#define MESH_H

#include <stdbool.h>
 
/**
 * @brief Struct that contains mesh information. Note is already a pointer
 */
typedef struct _Mesh* Mesh;

Mesh mesh_create();

void mesh_destroy(void *mesh);

float *mesh_get_vertices(Mesh mesh);

unsigned int *mesh_get_indices(Mesh mesh);

unsigned int mesh_vertex_size(Mesh mesh);

unsigned int mesh_indices_size(Mesh mesh);

bool mesh_add_vertex(Mesh mesh, float x, float y, float z);

bool mesh_add_index(Mesh mesh, unsigned int i);

bool mesh_add_color(Mesh mesh, float r, float g, float b);

bool mesh_add_uv(Mesh mesh, float s, float t);

void mesh_generate_buffers(Mesh mesh);

void mesh_update_buffers(Mesh mesh);

void mesh_draw(Mesh mesh);

unsigned int mesh_get_VBO(Mesh mesh);

unsigned int mesh_get_color_VBO(Mesh mesh);

unsigned int mesh_get_uv_VBO(Mesh mesh);

unsigned int mesh_get_VAO(Mesh mesh);

unsigned int mesh_get_EBO(Mesh mesh);

bool mesh_loaded_buffers(Mesh mesh);

#endif