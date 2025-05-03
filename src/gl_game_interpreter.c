#include "gl_game_interpreter.h"

#include "mesh_renderer.h"
#include "mesh.h"
#include "shader.h"
#include "texture2D.h"

/*Temp vertex shader*/
const char *vertexShaderSource = "./shaders/vertex.glsl";

const char *framentSource = "./shaders/fragment.glsl";

float quadVerts[] = {
    // X     Y     Z
    -0.5f, -0.5f, 0.0f,  // Bottom-left  (0)
     0.5f, -0.5f, 0.0f,  // Bottom-right (1)
     0.5f,  0.5f, 0.0f,  // Top-right    (2)
    -0.5f,  0.5f, 0.0f   // Top-left     (3)
};
unsigned int quadIndices[] = {
    0, 1, 2,   // First triangle
    2, 3, 0    // Second triangle
};
float quadUVs[] = {
    // U     V
    0.0f, 0.0f,  // Bottom-left
    1.0f, 0.0f,  // Bottom-right
    1.0f, 1.0f,  // Top-right
    0.0f, 1.0f   // Top-left
};

struct _GLInterpreter{
    MeshRenderer space;
    MeshRenderer link;
    MeshRenderer oDoor;
    MeshRenderer pDoor;
    MeshRenderer bDoor;
};

/*-----PRIVATE-----*/
void _inter_gen_quad(Mesh mesh);
void _inter_draw_space(Space *space, Direction dir, GLInterpreter *gl, mat4 view, mat4 projection);

/*----PUBLIC----- */

GLInterpreter *gl_inter_create(){
    GLInterpreter *gl;

    gl = (GLInterpreter *)calloc(1, sizeof(GLInterpreter));
    if(!gl) return NULL;

    gl->space = meshr_new();
    if(!(gl->space)){
        free(gl);
        return NULL;
    }

    gl->link = meshr_new();
    if(!(gl->link)){
        meshr_destroy(gl->space);
        free(gl);
        return NULL;
    }

    gl->oDoor = meshr_new();
    if(!(gl->oDoor)){
        meshr_destroy(gl->space);
        meshr_destroy(gl->link);
        free(gl);
        return NULL;
    }

    gl->pDoor = meshr_new();
    if(!(gl->pDoor)){
        meshr_destroy(gl->space);
        meshr_destroy(gl->link);
        meshr_destroy(gl->oDoor);
        free(gl);
        return NULL;
    }

    gl->bDoor = meshr_new();
    if(!(gl->pDoor)){
        meshr_destroy(gl->space);
        meshr_destroy(gl->link);
        meshr_destroy(gl->oDoor);
        meshr_destroy(gl->pDoor);
        free(gl);
        return NULL;
    }

    return gl;
}

void gl_inter_destroy(void *gl){
    GLInterpreter *gli;

    if(gl){
        gli = (GLInterpreter *)gl;
        meshr_destroy(gli->space);
        meshr_destroy(gli->bDoor);
        meshr_destroy(gli->link);
        meshr_destroy(gli->oDoor);
        meshr_destroy(gli->pDoor);
        free(gl);
    }
}

Status gl_inter_load_space_tex(GLInterpreter *inter, const char *path){
    Mesh mesh;
    Texture2D tex;
    Shader shader;

    if(!inter) return ERROR;

    mesh = mesh_create();
    if(!mesh) return ERROR;

    tex = tex2D_new();
    if(!tex){
        mesh_destroy(mesh);
        return ERROR;
    }

    shader = shader_new(vertexShaderSource, framentSource);
    if(!shader){
        mesh_destroy(mesh);
        tex2D_destroy(tex);
        return ERROR;
    }

    meshr_set_mesh(inter->space, mesh);
    meshr_set_shader(inter->space, shader);
    meshr_add_tex2D(inter->space, tex);

    _inter_gen_quad(mesh);

    return (tex2D_load_image(tex, path, GL_CLAMP_TO_BORDER, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST)) ? OK : ERROR;
}

Status gl_inter_load_link_tex(GLInterpreter *inter, const char *path){
    Mesh mesh;
    Texture2D tex;
    Shader shader;

    if(!inter) return ERROR;

    mesh = mesh_create();
    if(!mesh) return ERROR;

    tex = tex2D_new();
    if(!tex){
        mesh_destroy(mesh);
        return ERROR;
    }

    shader = shader_new(vertexShaderSource, framentSource);
    if(!shader){
        mesh_destroy(mesh);
        tex2D_destroy(tex);
        return ERROR;
    }

    meshr_set_mesh(inter->link, mesh);
    meshr_set_shader(inter->link, shader);
    meshr_add_tex2D(inter->link, tex);

    _inter_gen_quad(mesh);

    return (tex2D_load_image(tex, path, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST)) ? OK : ERROR;
}

Status gl_inter_load_oDoor_tex(GLInterpreter *inter, const char *path){
    Mesh mesh;
    Texture2D tex;
    Shader shader;

    if(!inter) return ERROR;

    mesh = mesh_create();
    if(!mesh) return ERROR;

    tex = tex2D_new();
    if(!tex){
        mesh_destroy(mesh);
        return ERROR;
    }

    shader = shader_new(vertexShaderSource, framentSource);
    if(!shader){
        mesh_destroy(mesh);
        tex2D_destroy(tex);
        return ERROR;
    }

    meshr_set_mesh(inter->oDoor, mesh);
    meshr_set_shader(inter->oDoor, shader);
    meshr_add_tex2D(inter->oDoor, tex);

    _inter_gen_quad(mesh);

    return (tex2D_load_image(tex, path, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST)) ? OK : ERROR;
}

Status gl_inter_load_pDoor_tex(GLInterpreter *inter, const char *path){
    Mesh mesh;
    Texture2D tex;
    Shader shader;

    if(!inter) return ERROR;

    mesh = mesh_create();
    if(!mesh) return ERROR;

    tex = tex2D_new();
    if(!tex){
        mesh_destroy(mesh);
        return ERROR;
    }

    shader = shader_new(vertexShaderSource, framentSource);
    if(!shader){
        mesh_destroy(mesh);
        tex2D_destroy(tex);
        return ERROR;
    }

    meshr_set_mesh(inter->pDoor, mesh);
    meshr_set_shader(inter->pDoor, shader);
    meshr_add_tex2D(inter->pDoor, tex);

    _inter_gen_quad(mesh);

    return (tex2D_load_image(tex, path, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST)) ? OK : ERROR;
}

Status gl_inter_load_bDoor_tex(GLInterpreter *inter, const char *path){
    Mesh mesh;
    Texture2D tex;
    Shader shader;

    if(!inter) return ERROR;

    mesh = mesh_create();
    if(!mesh) return ERROR;

    tex = tex2D_new();
    if(!tex){
        mesh_destroy(mesh);
        return ERROR;
    }

    shader = shader_new(vertexShaderSource, framentSource);
    if(!shader){
        mesh_destroy(mesh);
        tex2D_destroy(tex);
        return ERROR;
    }

    meshr_set_mesh(inter->bDoor, mesh);
    meshr_set_shader(inter->bDoor, shader);
    meshr_add_tex2D(inter->bDoor, tex);

    _inter_gen_quad(mesh);

    return (tex2D_load_image(tex, path, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST)) ? OK : ERROR;
}

void gl_inter_update_graphics(GLInterpreter *gl, Game *game, float ratio){
    Space *currentSpace, *directionSpace;
    
    mat4 view = GLM_MAT4_IDENTITY_INIT;//in a future will be inside camera
    mat4 projection = GLM_MAT4_IDENTITY_INIT;//in a futurewill be inside camera

    if(!gl || !game) return;


    glm_perspective(glm_rad(45.0f), ratio, 0.1f, 100.0f, projection);
    glm_translate(view, (vec3){0,0,-3});

    currentSpace = game_get_space(game, game_get_player_location(game));
    if(!currentSpace) return;

    shader_use(meshr_get_shader(gl->space));
    shader_set_Mat4(meshr_get_shader(gl->space), "view", view);
    shader_set_Mat4(meshr_get_shader(gl->space), "projection", projection);

    shader_use(meshr_get_shader(gl->link));
    shader_set_Mat4(meshr_get_shader(gl->link), "view", view);
    shader_set_Mat4(meshr_get_shader(gl->link), "projection", projection);

    /*meshr_set_position(gl->space, (vec3){0,0,-5});

    shader_use(meshr_get_shader(gl->space));
    shader_set_Mat4(meshr_get_shader(gl->space), "view", view);
    shader_set_Mat4(meshr_get_shader(gl->space), "projection", projection);
    meshr_draw(gl->space);*/

    _inter_draw_space(currentSpace, NO_DIR, gl, view, projection);

    directionSpace = space_get_neighbour(currentSpace, NW);
    _inter_draw_space(directionSpace, NW, gl, view, projection);
  
    directionSpace = space_get_neighbour(currentSpace, N);
    _inter_draw_space(directionSpace, N, gl, view, projection);
  
    directionSpace = space_get_neighbour(currentSpace, NE);
    _inter_draw_space(directionSpace, NE, gl, view, projection);
  
    directionSpace = space_get_neighbour(currentSpace, W);
    _inter_draw_space(directionSpace, W, gl, view, projection);
  
    directionSpace = space_get_neighbour(currentSpace, E);
    _inter_draw_space(directionSpace, E, gl, view, projection);

    directionSpace = space_get_neighbour(currentSpace, SW);
    _inter_draw_space(directionSpace, SW, gl, view, projection);
    directionSpace = space_get_neighbour(currentSpace, S);
    _inter_draw_space(directionSpace, S, gl, view, projection);
  
    directionSpace = space_get_neighbour(currentSpace, SE);
    _inter_draw_space(directionSpace, SE, gl, view, projection);

}
void _inter_draw_space(Space *space, Direction dir, GLInterpreter *gl, mat4 view, mat4 projection){
    float pos[] = {
        0,1,//N
        1,1,//NE
        1,0,//E
        1,-1,//SE
        0,-1,//S
        -1,-1,//SW
        -1,0,//W
        -1,1,//NW
        0,0//NO DIR
    };
    float *currentPos;

    float linkOffset = .48;

    Link *link = NULL;

    if(!space || !gl) return;

    currentPos = pos + ((dir - N)*2);

    meshr_set_position(gl->space, (vec3){currentPos[0],currentPos[1],-2});
    meshr_set_scale(gl->space, (vec3){1.01,1.01,1.01});

    meshr_draw(gl->space);

    link = space_get_north(space);
    if(link){
        meshr_set_position(gl->link, (vec3){currentPos[0], currentPos[1] + linkOffset , -2});
        meshr_set_scale(gl->link, (vec3){.2,.2,.2});
        meshr_set_rotation(gl->link, (vec3){0,0,0});
        meshr_draw(gl->link);
    }
    link = space_get_south(space);
    if(link){
        meshr_set_position(gl->link, (vec3){currentPos[0], currentPos[1] - linkOffset , -2});
        meshr_set_scale(gl->link, (vec3){.2,.2,.2});
        meshr_set_rotation(gl->link, (vec3){0,0,0});
        meshr_draw(gl->link);
    }
    link = space_get_east(space);
    if(link){
        meshr_set_position(gl->link, (vec3){currentPos[0] + linkOffset, currentPos[1] , -2});
        meshr_set_scale(gl->link, (vec3){.2,.2,.2});
        meshr_set_rotation(gl->link, (vec3){0,0,90});
        meshr_draw(gl->link);
    }
    link = space_get_west(space);
    if(link){
        meshr_set_position(gl->link, (vec3){currentPos[0] - linkOffset, currentPos[1], -2});
        meshr_set_scale(gl->link, (vec3){.2,.2,.2});
        meshr_set_rotation(gl->link, (vec3){0,0,-90});
        meshr_draw(gl->link);
    }

}

void _inter_gen_quad(Mesh mesh){
    if(!mesh)return;

    for (size_t i = 0; i < sizeof(quadVerts)/sizeof(quadVerts[0]); i+= 3)
    {
        mesh_add_vertex(mesh, quadVerts[i], quadVerts[i + 1], quadVerts[i + 2]);
        mesh_add_color(mesh, 1, 1, 1);
    }
    for (size_t i = 0; i < sizeof(quadUVs)/sizeof(quadUVs[0]); i+= 2)
    {
        mesh_add_uv(mesh, quadUVs[i], quadUVs[i + 1]);
    }
    for (size_t i = 0; i < sizeof(quadIndices)/sizeof(quadIndices[0]); i++)
    {
        mesh_add_index(mesh, quadIndices[i]);
    }
    mesh_generate_buffers(mesh);
    mesh_update_buffers(mesh);

}
