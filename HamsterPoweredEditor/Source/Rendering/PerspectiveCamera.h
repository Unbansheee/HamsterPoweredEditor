#pragma once
#include "Renderer.h"
#include "View.h"

class PerspectiveCamera : public View
{
public:
    PerspectiveCamera(float _fov = 45.f, float _aspect = (float)Renderer::AspectRatio(), float _near = 0.1f, float _far = 100.f);
    void SetProjection(float _fov, float _aspect, float _near, float _far);

    void RecalculateViewMatrix() override;
private:
    glm::vec3 m_direction;
    float fov = 45.0f;
    float aspect = 1.f;
    float near = 0.1f;
    float far = 100.0f;
};
