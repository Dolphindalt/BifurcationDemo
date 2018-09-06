#ifndef GRAPH_H
#define GRAPH_H

#include <GL/glew.h>

typedef struct point {
    GLfloat x;
    GLfloat y;
} point_t;

typedef struct graph {
    point_t *points;
    size_t size;
} graph_t;

void render_graph(graph_t graph);
graph_t generate_vertex_buffer(float(*equation_interface)(const float np1, const float r));

#endif