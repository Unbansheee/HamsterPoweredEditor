#pragma once
#include "Actor.h"
#include "Rendering/OrthographicCamera.h"

class CameraController : public Actor
{
public:
    enum class CameraType
    {
        ORTHO,
        PERSPECTIVE
    };
    
public:
    CameraController(CameraType type, float aspectRatio);

    void SetCameraType(CameraType type);
    void HandleMouseMovement(float x, float y);
    void Update(Timestep ts) override;
    void Begin() override;
    void OnDestroy() override;
    float GetZoom() const { return m_Zoom; }
    void SetZoom(float zoom);
    void Move(glm::vec3 offset);

    void Resize(float width, float height);
    View* GetCamera() { return m_Camera; }
    const View* GetCamera() const { return m_Camera; }

protected:
    void OnInspectorGUI() override;
private:
    CameraType m_CameraType;
    float dt = 0;
    float m_Zoom = 1.f;
    float m_AspectRatio;
    View* m_Camera;
};
