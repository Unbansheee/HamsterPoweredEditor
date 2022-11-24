#pragma once
#include <glm/vec3.hpp>

struct PointLightData
{
    glm::vec3 position;
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;
    float radius = 20.0f;
};

struct DirectionalLightData
{
    glm::vec3 direction;
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;
};