#pragma once
#include <glm/vec3.hpp>


struct Hit
{
    glm::vec3 Location = {0,0,0};
    glm::vec3 Normal = {0,0,0};
    mutable float Distance = 999999999.f;
    bool HitSomething = false;
    
    explicit operator bool() const { return HitSomething; }
};

class Raycast
{
public:
    Raycast(const glm::vec3& origin, const glm::vec3& direction, Hit& hit, float maxDistance = 1000.0f);

    
};
