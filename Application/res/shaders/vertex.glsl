#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;

uniform mat4 u_transform;

// out from the vertex shader
out vec2 v_tex_coord;

void main()
{
    gl_Position = u_transform * position;
    v_tex_coord = tex_coord;
}