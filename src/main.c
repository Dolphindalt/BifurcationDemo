#include <string.h>
#include <stdio.h>
#include <util.h>
#include <graph.h>
#include <cglm/cglm.h>

void input(int *running);

static GLuint get_shader_program();
float exp_equation(const float np1, const float r);

int main(int argc, char *argv[])
{
    int running = 1;
    window_gl_t windowgl = window_init();

    GLuint shader_program;

    shader_program = get_shader_program();

    graph_t exp_graph = generate_vertex_buffer(exp_equation);
    printf("Done loading graph\n");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_PROGRAM_POINT_SIZE);

    mat4 projection;
    glm_ortho(2.5, 4.0, 0.0, 1.0, 0.1, 100.0, projection);

    Uint64 now = 0;
    Uint64 last_time = SDL_GetPerformanceCounter();
    double delta = 0;
    const double ns = 1000000000.0 / 60.0;
    while(running)
    {
        now = SDL_GetPerformanceCounter();
        delta += (now - last_time) / ns;
        while(delta >= 1.0)
        {
            input(&running);
            delta--;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);

        glUseProgram(shader_program);

        glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, projection[0]);

        glBegin(GL_POINTS);
        render_graph(exp_graph);
        glEnd();

        glUseProgram(0);

        SDL_GL_MakeCurrent(windowgl.window, windowgl.context);
        SDL_GL_SwapWindow(windowgl.window);
    }

    free(exp_graph.points);
    SDL_GL_DeleteContext(windowgl.context);
    SDL_DestroyWindow(windowgl.window);
    SDL_Quit();
    return 0;
}

float exp_equation(const float np1, const float r)
{
    return r * np1 * (1 - np1);
}

void input(int *running)
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_QUIT:
                case SDLK_ESCAPE: *running = 0; break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym)
                {
                    case SDLK_ESCAPE: *running = 0; break;
                    default: break;
                }
            default: break;
        }
    }
}

GLuint get_shader_program()
{
    const char *vertex_shader = "src/vertex.glsl";
    const char *fragment_shader = "src/fragment.glsl";
    return shader_init(vertex_shader, fragment_shader);
}