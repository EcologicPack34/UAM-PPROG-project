#include "texture2D.h"

#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


struct _Texture2D{
    unsigned int texture;
    int width, height, nrChannels;

    bool loaded;
};

Texture2D tex2D_new(){
    Texture2D tex = NULL;

    stbi_set_flip_vertically_on_load(true);

    tex = calloc(1, sizeof(struct _Texture2D));
    if(!tex) return NULL;

    return tex;
}

void tex2D_destroy(void *texture){
    if(texture){
        if(((Texture2D)texture)->loaded){
            glDeleteTextures(1, &(((Texture2D)texture)->texture));
        }
        free(texture);
    }
}

bool tex2D_load_image(Texture2D tex, const char *imagePath, GLenum glWrapMode, GLenum glFilterMip, GLenum glFilter){
    unsigned char *texData;
    GLenum format;
    
    if(!tex || !imagePath || tex->loaded) return false;

    glGenTextures(1,&(tex->texture));
    glBindTexture(GL_TEXTURE_2D, tex->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapMode);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilterMip);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);

    texData = stbi_load(imagePath, &(tex->width), &(tex->height), &(tex->nrChannels), 0);
    if (texData)
    {
        switch (tex->nrChannels)
        {
            case 1:
                format = GL_RED;
                break;
            case 2:
                format = GL_RG;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                fprintf(stderr, "Unsuported texture channels on %s\n", imagePath);
                glDeleteTextures(1, &(tex->texture));
                stbi_image_free(texData);
                return false;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, tex->width, tex->height, 0, format, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        fprintf(stderr, "Failed to load texture: %s\n", imagePath);
        glDeleteTextures(1, &(tex->texture));
        return false;
    }
    stbi_image_free(texData);

    glBindTexture(GL_TEXTURE_2D, 0);

    tex->loaded = true;
    return true;
}

unsigned int tex2D_get_texture(Texture2D texture){
    if(!texture) return 0;
    return texture->texture;
}

void tex2D_bind(Texture2D tex, GLenum textureUnit){
    if(!tex || !(tex->loaded)){
        return;
    }

    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, tex->texture);
}

bool tex2D_loaded(Texture2D tex){
    if(!tex) return false;
    return tex->loaded;
}