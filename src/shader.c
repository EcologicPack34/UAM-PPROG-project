#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


/*----PRIVATE FUNCTIONS----*/
char *_shader_load_source(const char *filename){
    FILE *file = NULL;
    long length;

    char *fileContent = NULL;
    size_t readSize;

    if(!filename) return NULL;

    file = fopen(filename, "rb");
    if(!file){
        return NULL;
    }

    /*Go to end of file to know its length*/
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    rewind(file);/*rewind the file to read it*/

    /*reserves memory for file content*/
    fileContent = (char *)malloc((length + 1)*sizeof(char));
    if(!fileContent){
        fclose(file);
        return NULL;
    }

    /*reads file and null terminates the content*/
    readSize = fread(fileContent, sizeof(char), length, file);
    fileContent[readSize] = 0;

    fclose(file);
    return fileContent;
}


/*----PUBLIC FUNCTIONS-----*/

Shader shader_new(const char *vertexPath, const char *fragmentPath){
    Shader shader = NULL;

    const char* vshader;
    const char* fshader;

    unsigned int vertex, fragment;
    int compileSucces;
    char infoLog[512];

    if(!vertexPath || !fragmentPath) return NULL;

    shader = calloc(1, sizeof(struct _Shader));
    if(!shader) return NULL;

    /*Load vertex shader code*/
    vshader = _shader_load_source(vertexPath);
    if(!vshader){
        free(shader);
        fprintf(stderr, "Failed loading vertex shader: %s\n", vertexPath);
        return NULL;
    }
    /*Load fragment shader code*/
    fshader = _shader_load_source(fragmentPath);
    if(!fshader){
        free(shader);
        free((char *)vshader);
        fprintf(stderr, "Failed loading fragment shader: %s\n", fragmentPath);
        return NULL;
    }

    /*Compile vertex shader*/
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vshader, NULL);
    glCompileShader(vertex);

    
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &compileSucces);
    if(!compileSucces){
        free((char *)fshader);
        
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        free(shader);
        fprintf(stderr, "Error compiling vertex shader: \n%s", infoLog);
        return NULL;
    }
    free((char *)vshader);
    
    /*Compile fragment shader*/
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fshader, NULL);
    glCompileShader(fragment);
    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &compileSucces);
    if(!compileSucces){
        free((char *)fshader);
        glDeleteShader(vertex);
        
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        free(shader);
        fprintf(stderr, "Error compiling vertex shader: \n%s", infoLog);
        return NULL;
    }
    
    free((char *)fshader);
    
    /*Link shader program*/
    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vertex);
    glAttachShader(shader->ID, fragment);
    glLinkProgram(shader->ID);
    
    glGetProgramiv(shader->ID, GL_LINK_STATUS, &compileSucces);
    if(!compileSucces){
        free((char *)vshader);
        free((char *)fshader);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        
        glGetProgramInfoLog(shader->ID, 512, NULL, infoLog);
        free(shader);
        fprintf(stderr, "Error linking shader program: \n%s", infoLog);
        return NULL;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return shader;
}

void shader_destroy(void *shader){
    if(shader){
        glDeleteProgram(((Shader)shader)->ID);
        free(shader);
    }
}

void shader_use(Shader shader){
    if(!shader) return;
    glUseProgram(shader->ID);
}

void shader_set_Bool(Shader shader, const char *name, bool value){
    int location;
    
    if(!shader) return;

    location = glGetUniformLocation(shader->ID, name);
    if(location == -1){
        fprintf(stderr, "Warning: uniform %s not found in shader\n", name);
        return;
    }

    glUniform1i(location, (int)value); 
}

void shader_set_Int(Shader shader, const char *name, int value){
    int location;
    
    if(!shader) return;

    location = glGetUniformLocation(shader->ID, name);
    if(location == -1){
        fprintf(stderr, "Warning: uniform %s not found in shader\n", name);
        return;
    }

    glUniform1i(location, value); 
}

void shader_set_Float(Shader shader, const char *name, float value){
    int location;
    
    if(!shader) return;

    location = glGetUniformLocation(shader->ID, name);
    if(location == -1){
        fprintf(stderr, "Warning: uniform %s not found in shader\n", name);
        return;
    }

    glUniform1f(location, value); 
}

void shader_set_Mat4(Shader shader, const char *name, mat4 value){
    int location;
    
    if(!shader) return;

    location = glGetUniformLocation(shader->ID, name);
    if(location == -1){
        fprintf(stderr, "Warning: uniform %s not found in shader\n", name);
        return;
    }

    glUniformMatrix4fv(location, 1, GL_FALSE, (float *)value);
}