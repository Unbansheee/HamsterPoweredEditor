#pragma once
#include "Component.h"
#include "Rendering/OrthographicCamera.h"
#include "CameraController.generated.h"

SERIALIZEDCLASS(Category = "Rendering")
//Perspective camera rotation is currently broken
class CameraController : public Component
{
    GENERATED()

    
public:
    enum class CameraType
    {
        ORTHO,
        PERSPECTIVE
    };
    
public:
    CameraController(CameraType type, float aspectRatio);
    CameraController(GameObject* owner);
    void SetCameraType(CameraType type);
    void HandleMouseMovement(float x, float y);
    void Update(Timestep ts) override;
    void Begin() override;
    float GetZoom() const { return m_CameraType == CameraType::ORTHO ? m_OrthoZoom : m_Zoom; }
    void SetZoom(float zoom);
    void Move(glm::vec3 offset);
    CameraType GetCameraType() const { return m_CameraType; }
    float GetPerspSpeed() const { return m_PerspMoveSpeed; }
    void SetPerspSpeed(float speed) { m_PerspMoveSpeed = glm::clamp(speed, 0.1f, 20.f); }

    void Resize(float width, float height);
    View* GetCamera() { return m_Camera; }
    const View* GetCamera() const { return m_Camera; }

protected:
    void OnInspectorGUI() override;
public:

    void DeserializeCustom(nlohmann::json& j) override;
    void SerializeCustom(nlohmann::json& j) override;
    void FixedUpdate(double ts) override;

private:
    SERIALIZEDVAR float yaw = 0.0f;
    SERIALIZEDVAR float pitch = 0.0f;
    SERIALIZEDVAR CameraType m_CameraType;
    SERIALIZEDVAR float dt = 0;
    SERIALIZEDVAR float m_Zoom = 75.f;
    SERIALIZEDVAR float m_OrthoZoom = 2.5f;
    float m_AspectRatio;
    SERIALIZEDVAR float m_PerspMoveSpeed = 1.f;
    View* m_Camera;
};

#undef GENERATED