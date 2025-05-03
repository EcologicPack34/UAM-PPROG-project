#include "mesh.h"

#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#define VERTEX_SIZE 3U
#define UV_SIZE 2U
#define COLOR_SIZE 3U

#define INIT_VERTEX_SIZE (VERTEX_SIZE * 3) //Must be divisible by 3
#define INIT_COLOR_SIZE (COLOR_SIZE * 3)
#define INIT_UV_SIZE (UV_SIZE * 3)
#define INIT_INDEX_SIZE 3U
#define GROWTH_FACTOR 2U

#if INIT_VERTEX_SIZE / 3 * 3 != INIT_VERTEX_SIZE
    #error "INIT_VERTEX_SIZE must be divisible by 3"
#endif

struct _Mesh{
    float *vertices;
    unsigned int verticesSize;
    unsigned int verticesNum;
    
    unsigned int *indices;
    unsigned int indicesSize;
    unsigned int indicesNum;
    
    float *colors;
    unsigned int colorsSize;
    unsigned int colorsNum;
    
    float *uvs;
    unsigned int uvSize;
    unsigned int uvNum;


    /*buffers*/
    bool bufferGen;
    bool loadedBuffers;
    unsigned int VBO, cVBO, uvVBO, VAO, EBO;
};

Mesh mesh_create(){
    Mesh mesh = NULL;

    mesh = (Mesh)calloc(1, sizeof(struct _Mesh));
    if(!mesh)return NULL;

    mesh->vertices = (float *)calloc(INIT_VERTEX_SIZE, sizeof(float));
    if(!(mesh->vertices)){
        free(mesh);
        return NULL;
    }
    mesh->verticesSize = INIT_VERTEX_SIZE;

    mesh->indices = (unsigned int *)calloc(INIT_INDEX_SIZE, sizeof(unsigned int));
    if(!(mesh->indices)){
        free(mesh->vertices);
        free(mesh);
        return NULL;
    }
    mesh->indicesSize = INIT_INDEX_SIZE;

    mesh->colors = (float *)calloc(INIT_COLOR_SIZE, sizeof(float));
    if(!(mesh->colors)){
        free(mesh->vertices);
        free(mesh->indices);
        free(mesh);
        return NULL;
    }
    mesh->colorsSize = INIT_COLOR_SIZE;

    mesh->uvs = (float *)calloc(INIT_UV_SIZE, sizeof(float));
    if(!(mesh->uvs)){
        free(mesh->vertices);
        free(mesh->indices);
        free(mesh->colors);
        free(mesh);
        return NULL;
    }
    mesh->uvSize = INIT_UV_SIZE;

    return mesh;
}

void mesh_destroy(void * mesh){
    if(mesh){
        glDeleteBuffers(1, &(((Mesh)mesh)->VBO));
        glDeleteBuffers(1, &(((Mesh)mesh)->VAO));
        glDeleteBuffers(1, &(((Mesh)mesh)->EBO));

        free(((Mesh)mesh)->vertices);
        free(((Mesh)mesh)->indices);
        free(mesh);
    }
}

float *mesh_get_vertices(Mesh mesh){
    if(!mesh){
        return NULL;
    }
    return mesh->vertices;
}

unsigned int *mesh_get_indices(Mesh mesh){
    if(!mesh){
        return NULL;
    }
    return mesh->indices;
}

unsigned int mesh_vertex_size(Mesh mesh){
    if(!mesh) return 0;
    return mesh->verticesNum;
}

unsigned int mesh_indices_size(Mesh mesh){
    if(!mesh) return 0;
    return mesh->indicesNum;
}

bool mesh_add_vertex(Mesh mesh, float x, float y, float z){
    float *auxp = NULL;
    unsigned int num;
    
    if(!mesh) return false;

    if(mesh->verticesNum * VERTEX_SIZE == mesh->verticesSize){
        auxp = (float *)realloc(mesh->vertices, mesh->verticesSize * GROWTH_FACTOR * sizeof(float));
        if(!auxp) return false;

        mesh->vertices = auxp;
        mesh->verticesSize *= GROWTH_FACTOR;
    }

    num = (mesh->verticesNum * VERTEX_SIZE);
    mesh->vertices[num] = x;
    mesh->vertices[num + 1] = y;
    mesh->vertices[num + 2] = z;

    (mesh->verticesNum)++;
    return true;
}

bool mesh_add_color(Mesh mesh, float r, float g, float b){
    float *auxp = NULL;
    unsigned int num;
    
    if(!mesh) return false;

    if(mesh->colorsNum * VERTEX_SIZE == mesh->colorsSize){
        auxp = (float *)realloc(mesh->colors, mesh->colorsSize * GROWTH_FACTOR * sizeof(float));
        if(!auxp) return false;

        mesh->colors = auxp;
        mesh->colorsSize *= GROWTH_FACTOR;
    }

    num = (mesh->colorsNum * COLOR_SIZE);
    mesh->colors[num] = r;
    mesh->colors[num + 1] = g;
    mesh->colors[num + 2] = b;

    (mesh->colorsNum)++;
    return true;
}
bool mesh_add_uv(Mesh mesh, float s, float t){
    float *auxp = NULL;
    unsigned int num;
    
    if(!mesh) return false;

    if(mesh->uvNum * UV_SIZE == mesh->uvSize){
        auxp = (float *)realloc(mesh->uvs, mesh->uvSize * GROWTH_FACTOR * sizeof(float));
        if(!auxp) return false;

        mesh->uvs = auxp;
        mesh->uvSize *= GROWTH_FACTOR;
    }

    num = (mesh->uvNum * UV_SIZE);
    mesh->uvs[num] = s;
    mesh->uvs[num + 1] = t;

    (mesh->uvNum)++;
    return true;
}

bool mesh_add_index(Mesh mesh, unsigned int i){
    unsigned int *auxp = NULL;

    if(!mesh) return false;

    if(mesh->indicesNum == mesh->indicesSize){
        auxp = (unsigned int *)realloc(mesh->indices, mesh->indicesSize * GROWTH_FACTOR * sizeof(unsigned int));
        if(!auxp) return false;

        mesh->indices = auxp;
        mesh->indicesSize *= GROWTH_FACTOR;
    }

    mesh->indices[(mesh->indicesNum)++] = i;
    return true;
}

void mesh_generate_buffers(Mesh mesh){
    if(!mesh) return;

    if(mesh->bufferGen) return;

    glGenVertexArrays(1, &(mesh->VAO));

    glGenBuffers(1, &(mesh->VBO));
    glGenBuffers(1, &(mesh->cVBO));
    glGenBuffers(1, &(mesh->uvVBO));

    glGenBuffers(1, &(mesh->EBO));

    mesh->bufferGen = true;
}

void mesh_update_buffers(Mesh mesh){
    
    if(!mesh || !(mesh->bufferGen)) return;

    if(mesh->verticesNum != mesh->colorsNum && mesh->verticesNum != mesh->uvNum) return;

    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, (mesh->verticesNum * VERTEX_SIZE * sizeof(float)), mesh->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->cVBO);
    glBufferData(GL_ARRAY_BUFFER, (mesh->colorsNum * VERTEX_SIZE * sizeof(float)), mesh->colors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->uvVBO);
    glBufferData(GL_ARRAY_BUFFER, (mesh->uvNum * UV_SIZE * sizeof(float)), mesh->uvs, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (mesh->indicesNum * sizeof(unsigned int)), mesh->indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mesh->loadedBuffers = true;
}

unsigned int mesh_get_VBO(Mesh mesh){
    if(!mesh) return 0;
    return mesh->VBO;
}

unsigned int mesh_get_color_VBO(Mesh mesh){
    if(!mesh) return 0;
    return mesh->cVBO;
}

unsigned int mesh_get_uv_VBO(Mesh mesh){
    if(!mesh) return 0;
    return mesh->uvVBO;
}

unsigned int mesh_get_VAO(Mesh mesh){
    if(!mesh) return 0;
    return mesh->VAO;
}

unsigned int mesh_get_EBO(Mesh mesh){
    if(!mesh) return 0;
    return mesh->EBO;
}

bool mesh_loaded_buffers(Mesh mesh){
    if(!mesh || !(mesh->bufferGen)) return false;
    return mesh->loadedBuffers;
}

void mesh_draw(Mesh mesh){
    unsigned int iSize;

    if(!mesh) return;

    if(!mesh_loaded_buffers(mesh)) return;

    iSize = mesh_indices_size(mesh);

    /*Loads VAO*/
    glBindVertexArray(mesh_get_VAO(mesh));
    
    /*Loads vertex data*/
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    /*Loads color data*/
    glBindBuffer(GL_ARRAY_BUFFER, mesh->cVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->uvVBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(2);

    /*Loads inices data*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    //add mesh shader use in future
    glDrawElements(GL_TRIANGLES, iSize, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
