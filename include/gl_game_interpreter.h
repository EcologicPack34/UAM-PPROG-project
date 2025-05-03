/**
 * @file gl_game_interpreter.h
 * @author Daniel Gómez
 * @brief 
 * @version 0.1
 * @date 2025-05-03
 * 
 * @copyright GNU Public License
 */

#ifndef GL_GAME_INTER
#define GL_GAME_INTER

#include "game.h"
#include "types.h"

typedef struct _GLInterpreter GLInterpreter;

GLInterpreter *gl_inter_create();

Status gl_inter_load_space_tex(GLInterpreter *inter, const char *path);

Status gl_inter_load_link_tex(GLInterpreter *inter, const char *path);

Status gl_inter_load_oDoor_tex(GLInterpreter *inter, const char *path);

Status gl_inter_load_pDoor_tex(GLInterpreter *inter, const char *path);

Status gl_inter_load_bDoor_tex(GLInterpreter *inter, const char *path);

void gl_inter_destroy(void *gl);

void gl_inter_update_graphics(GLInterpreter *gl, Game *game, float ratio);

#endif