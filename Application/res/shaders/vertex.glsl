#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_color;
layout(location = 3) in vec2 a_tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normal_mat;

// out from the vertex shader
out vec2 v_tex_coord;
out vec3 v_normal;
out vec3 v_frag_pos;
out vec3 v_color;

void main()
{
    // the fragment position is in model space
    v_frag_pos = vec3(u_model * vec4(a_position, 1.0));
    gl_Position = u_projection * u_view * vec4(v_frag_pos, 1.0);
    v_tex_coord = a_tex_coord;
    // TODO: handle transforming the normal matrix in the CPU instead of the GPU
    v_normal = u_normal_mat * a_normal;
    v_color = a_color;
}