#include <util.h>

#include <assert.h>
#include <stdlib.h>

#define MAJOR_VERSION 3
#define MINOR_VERSION 1

static void shader_compile(const char *src, GLuint *id);
static char *file_to_string(const char *file_name);

window_gl_t window_init()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_DisplayMode resolution;
    if(SDL_GetDesktopDisplayMode(0, &resolution) != 0)
    {
        SDL_Log("Desktop resolution failed: %s \n", SDL_GetError());
    }

    window = SDL_CreateWindow(
        "Bifurication", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        resolution.w,
        resolution.h,
        SDL_WINDOW_OPENGL
    );

    assert(window != NULL);

    SDL_GLContext glcon = SDL_GL_CreateContext(window);
    assert(glcon);

    GLenum err = glewInit();
    assert(err == GLEW_OK);

    glViewport(0, 0, resolution.w, resolution.h);

    SDL_GL_SetSwapInterval(1);

    return (window_gl_t) { window, glcon };
}

char *file_to_string(const char *file_name)
{
    char *buffer = 0;
    unsigned long len;
    FILE *fp = fopen(file_name, "r");
    assert(fp);

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = (char *)malloc(len + 1);
    assert(buffer);

    fread(buffer, 1, len, fp);
    fclose(fp);
    buffer[len] = '\0';

    return buffer;
}

GLuint shader_init(const char *vertex_path, const char *fragment_path)
{
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    char *vertex_code = file_to_string(vertex_path);
    char * fragment_code = file_to_string(fragment_path);

    shader_compile(vertex_code, &vertex_shader_id);
    shader_compile(fragment_code, &fragment_shader_id);

    GLuint shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vertex_shader_id);
    glAttachShader(shader_program_id, fragment_shader_id);
    glLinkProgram(shader_program_id);

    GLint result = GL_FALSE;
    int error_length;
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(shader_program_id, GL_INFO_LOG_LENGTH, &error_length);
    if(error_length > 0 && result == GL_FALSE)
    {
        char msg[error_length + 1];
        glGetProgramInfoLog(shader_program_id, error_length, NULL, &msg[0]);
        printf("%s\n", msg);
        exit(1);
    }

    glDetachShader(shader_program_id, vertex_shader_id);
    glDetachShader(shader_program_id, fragment_shader_id);
    glDeleteShader(fragment_shader_id);
    glDeleteShader(vertex_shader_id);

    free(vertex_code);
    free(fragment_code);

    assert(shader_program_id > 0);

    return shader_program_id;
}

void shader_compile(const char *src, GLuint *id)
{
    GLint result = GL_FALSE;
    int error_length;
    glShaderSource(*id, 1, &src, NULL);
    glCompileShader(*id);
    glGetShaderiv(*id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(*id, GL_INFO_LOG_LENGTH, &error_length);
    if(error_length > 0 && result == GL_FALSE)
    {
        char msg[error_length + 1];
        glGetShaderInfoLog(*id, error_length, NULL, &msg[0]);
        printf("%s\n", msg);
        exit(1);
    }
}