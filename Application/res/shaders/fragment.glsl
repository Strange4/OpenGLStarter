#version 330 core

layout(location = 0) out vec4 color;

// in from the vertex shader
in vec2 v_tex_coord;

uniform vec4 u_color;
uniform sampler2D u_texture_slot;

void main()
{
    vec4 texColor = texture(u_texture_slot, v_tex_coord);
    color = texColor * u_color;
}