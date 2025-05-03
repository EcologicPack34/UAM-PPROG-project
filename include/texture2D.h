/**
 * @file texture2D.h
 * @author your name (you@domain.com)
 * @brief Small module aimed to simplify opengl texture loading
 *  REQUIRES "stb_image.h" IN THE PROJECT
 * @version 0.1
 * @date 2025-05-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef TEXTURE_2D
#define TEXTURE_2D

#include <glad/glad.h>
#include <stdbool.h>

typedef struct _Texture2D* Texture2D;

Texture2D tex2D_new();

void tex2D_destroy(void *texture);

bool tex2D_load_image(Texture2D tex, const char *imagePath, GLenum glWrapMode, GLenum glFilterMip, GLenum glFilter);

unsigned int tex2D_get_texture(Texture2D texture);

void tex2D_bind(Texture2D tex, GLenum textureUnit);

bool tex2D_loaded(Texture2D tex);

#endif