#version 400

layout (location = 0) in vec2 vertex_position;

uniform mat4 projection;

void main()
{
    gl_PointSize = 1.0;
    gl_Position = projection * vec4(vertex_position.xy, -10.0, 1.0);
}