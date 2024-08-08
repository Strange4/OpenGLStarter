#pragma once
#include <array>
#include <glm/glm.hpp>

#include "VertexBufferLayout.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec2 tex_coordinates;

    static VertexBufferLayout getLayout() {
        return VertexBufferLayout({
            { GL_FLOAT, 3, GL_FALSE },
            { GL_FLOAT, 2, GL_FALSE },
        });
    };
};
