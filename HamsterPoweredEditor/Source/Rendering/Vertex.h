#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
    glm::vec3 Position = {0, 0, 0};
    glm::vec3 Colour = {0, 0, 0};
    glm::vec2 TexCoord = {0, 0};
    glm::vec3 Normal = {0, 0, 0};
    int TexIndex = 0;
};
