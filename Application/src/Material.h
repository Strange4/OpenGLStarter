#pragma once
#include "pch.h"

struct Material
{
    glm::vec3 ambient;
    float ambient_strength;

    glm::vec3 diffuse;
    float diffuse_strength;

    glm::vec3 specular;
    float specular_strength;
    int shininess;
};
