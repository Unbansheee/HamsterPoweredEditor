#include "PerspectiveCamera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include "Rendering/Renderer.h"
PerspectiveCamera::PerspectiveCamera(float _fov, float _aspect, float _near, float _far) : fov(_fov), aspect(_aspect), near(_near), far(_far)
{
    m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Rotation, glm::vec3(0.0f, 1.0f, 0.0f));

    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; 
}

void PerspectiveCamera::SetProjection(float _fov, float _aspect, float _near, float _far)
{
    m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)Renderer::AspectRatio(), 0.1f, 100.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; 
}

void PerspectiveCamera::RecalculateViewMatrix()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + GetForwardVector(), -GetUpVector());
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; 
}

