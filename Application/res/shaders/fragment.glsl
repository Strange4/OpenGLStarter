#version 330 core

layout(location = 0) out vec4 color;

// in from the vertex shader
in vec2 v_tex_coord;
in vec3 v_normal;
in vec3 v_frag_pos;

uniform vec4 u_color;
uniform sampler2D u_texture0;

void main()
{
    color = mix(u_color, texture(u_texture0, v_tex_coord), 0.5);
}