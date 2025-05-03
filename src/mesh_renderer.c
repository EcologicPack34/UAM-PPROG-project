#include "mesh_renderer.h"

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#define TEXTURES_INIT_NUMBER 1U
#define MESHR_GROWTH_FACTOR 2U

struct _MeshRenderer{
    Mesh mesh;
    Shader shader;

    Texture2D *textures;
    unsigned int textureNum;
    unsigned int textureSize;

    vec3 position;
    vec3 rotation;
    vec3 scale;

    mat4 modelMat;
};

/*-------PRIVATE------*/
void _meshr_update_modelMat(MeshRenderer meshr){
    mat4 S, Rx, Ry, Rz, R, T, temp;
    
    if(!meshr) return;
    glm_translate_make(T, meshr->position);
    glm_scale_make(S, meshr->scale);

    glm_rotate_make(Rx, glm_rad(meshr->rotation[0]), (vec3){1.0,0.0,0.0});
    glm_rotate_make(Ry, glm_rad(meshr->rotation[1]), (vec3){0.0,1.0,0.0});
    glm_rotate_make(Rz, glm_rad(meshr->rotation[2]), (vec3){0.0,0.0,1.0});
    
    glm_mat4_mul(Ry, Rx, R);
    glm_mat4_mul(Rz, R, R);

    glm_mat4_mul(R,S, temp);
    glm_mat4_mul(T, temp, meshr->modelMat);
}


/*------PUBLIC-------*/
MeshRenderer meshr_new(){
    MeshRenderer meshr = NULL;

    meshr = calloc(1, sizeof(struct _MeshRenderer));
    if(!meshr) return NULL;

    meshr->textures = (Texture2D *)calloc(TEXTURES_INIT_NUMBER, sizeof(Texture2D));
    if(!(meshr->textures)){
        free(meshr);
        return NULL;
    }

    meshr->textureSize = TEXTURES_INIT_NUMBER;
    glm_mat4_identity(meshr->modelMat);
    glm_vec3_zero(meshr->position);
    glm_vec3_zero(meshr->rotation);
    glm_vec3_one(meshr->scale);
    return meshr;
}

void meshr_destroy(void *meshr){
    MeshRenderer mesh;

    if(meshr){
        mesh = (MeshRenderer)meshr;
        mesh_destroy(mesh->mesh);
        shader_destroy(mesh->shader);
        for (size_t i = 0; i < mesh->textureNum; i++)
        {
            tex2D_destroy(mesh->textures[i]);
        }
        free(mesh->textures);
        free(meshr);
    }

}

bool meshr_set_mesh(MeshRenderer meshr , Mesh mesh){
    if(!meshr) return false;
    meshr->mesh = mesh;
    return true;
}

bool meshr_set_shader(MeshRenderer meshr, Shader shader){
    if(!meshr) return false;
    meshr->shader = shader;
    return true;
}

bool meshr_add_tex2D(MeshRenderer meshr, Texture2D tex){
    Texture2D *auxp;
    int maxTexUnits;
    
    if(!meshr) return false;

    if(meshr->textureNum == meshr->textureSize){

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTexUnits);

        if((unsigned int)maxTexUnits == meshr->textureNum){
            fprintf(stderr, "Reached max texture units");
            return false;
        }

        auxp = (Texture2D *)realloc(meshr->textures, meshr->textureSize * MESHR_GROWTH_FACTOR * sizeof(Texture2D));
        if(!auxp) return false;

        meshr->textures = auxp;
    }

    meshr->textures[(meshr->textureNum)++] = tex;
    return true;
}

Shader meshr_get_shader(MeshRenderer meshr){
    if(!meshr) return NULL;
    return meshr->shader;
}

Texture2D meshr_get_tex2D_at(MeshRenderer meshr, unsigned int index){
    if(!meshr) return NULL;
    if(index > meshr->textureNum) return NULL;
    return meshr->textures[index];
}

void meshr_draw(MeshRenderer meshr){
    char uniformName[64];

    if(!meshr) return;
    if(!(meshr->mesh) || !meshr->shader) return;

    if(!mesh_loaded_buffers(meshr->mesh)) return;

    shader_use(meshr->shader);

    shader_set_Mat4(meshr->shader, "model", meshr->modelMat);

    for (size_t i = 0; i < meshr->textureNum; i++)
    {   
        if(!tex2D_loaded(meshr->textures[i])) continue;

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, tex2D_get_texture(meshr->textures[i]));
        
        snprintf(uniformName, sizeof(uniformName), "texture%ld",i);
        shader_set_Int(meshr->shader, uniformName, i);
    }
    
    mesh_draw(meshr->mesh);

    for (size_t i = 0; i < meshr->textureNum; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

bool meshr_set_position(MeshRenderer meshr, vec3 pos){
    if(!meshr) return false;
    glm_vec3_copy(pos, meshr->position);

    _meshr_update_modelMat(meshr);
    return true;
}

bool meshr_set_rotation(MeshRenderer meshr, vec3 rot){
    if(!meshr) return false;
    glm_vec3_copy(rot, meshr->rotation);

    _meshr_update_modelMat(meshr);
    return true;
}

bool meshr_set_scale(MeshRenderer meshr, vec3 scale){
    if(!meshr) return false;
    glm_vec3_copy(scale, meshr->scale);

    _meshr_update_modelMat(meshr);
    return true;
}
