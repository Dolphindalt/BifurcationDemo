#version 400

layout (location = 0) in vec2 vertex_position;

uniform mat4 projection;

out vec4 frag_color;

void main()
{
    gl_PointSize = 1.0;
    frag_color = gl_Color;
    gl_Position = projection * vec4(vertex_position.xy, -10.0, 1.0);
}