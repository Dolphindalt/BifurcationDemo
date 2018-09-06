#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>
#include <SDL2/SDL.h>

typedef struct window_gl {
    SDL_Window *window;
    SDL_GLContext context;
} window_gl_t;

window_gl_t window_init();

GLuint shader_init(const char *vertex_code, const char *fragment_code);

#endif