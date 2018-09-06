#include <graph.h>
#include <stdio.h>

#include <stdlib.h>

void render_graph(graph_t graph)
{
    for(int i = 0; i < graph.size; i++)
    {
        glVertex2f(graph.points[i].x, graph.points[i].y);
    }
}

graph_t generate_vertex_buffer(float(*equation_interface)(const float np1, const float r))
{
    point_t *graph = malloc(sizeof(point_t) * 604000);
    float np1 = 0.1;
    int i, j = 0;
    for(float minr = 2.5; minr < 4.0; minr += 0.001)
    {
        np1 = 0.1;
        for(i = 0; i < 100; i++)
            np1 = equation_interface(np1, minr);
        for(i = 0; i < 400; i++)
        {
            np1 = equation_interface(np1, minr);
            graph[j++] = (point_t) { minr, np1 };
        }
    }

    return (graph_t) { graph, 604000 };
}