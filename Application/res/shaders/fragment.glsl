#version 330 core
struct Material
{
    vec3 ambient;
    float ambient_strength;

    vec3 diffuse;
    
    vec3 specular;
    float specular_strength;
    int shininess;
};

layout(location = 0) out vec4 color;

// in from the vertex shader
in vec2 v_tex_coord;
in vec3 v_normal;
in vec3 v_frag_pos;
in vec3 v_color;

uniform vec3 u_light_color;
uniform vec3 u_light_pos;
uniform vec3 u_camera_pos;
uniform Material u_material;


void main()
{

    // ambient
    // float ambient_strength = 0.2;
    vec3 ambient = u_material.ambient_strength * u_material.ambient * u_light_color;

    // diffuse
    vec3 normal = normalize(v_normal);
    vec3 light_direction = normalize(u_light_pos - v_frag_pos);
    float diffuse_strength = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = (diffuse_strength * u_material.diffuse) * u_light_color;

    // specular
    // float specular_strength = 0.5;
    // int shininess = 50;
    vec3 reflection_direction = reflect(-light_direction, normal);
    vec3 view_direction = normalize(u_camera_pos - v_frag_pos);
    float specular_value = pow(max(dot(view_direction, reflection_direction), 0.0), u_material.shininess);
    vec3 specular = (specular_value * u_material.specular * u_material.specular_strength) * u_light_color;
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}