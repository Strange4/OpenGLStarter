#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

// out from the vertex shader
out vec2 v_tex_coord;
out vec3 v_normal;
out vec3 v_frag_pos;

void main()
{
    v_frag_pos = vec3(u_model * vec4(a_position, 1.0));
    gl_Position = u_projection * u_view * vec4(v_frag_pos, 1.0);
    v_tex_coord = a_tex_coord;
    v_normal = a_normal;
}