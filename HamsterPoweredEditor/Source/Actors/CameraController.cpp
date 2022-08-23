#include "CameraController.h"

#include "imgui.h"
#include "Rendering/PerspectiveCamera.h"


CameraController::CameraController(CameraType type, float aspectRatio) : m_CameraType(type), m_AspectRatio(aspectRatio)
    
{
    if (type == CameraType::ORTHO)
    {
        m_Camera = new OrthographicCamera(-aspectRatio * m_Zoom, aspectRatio * m_Zoom, -m_Zoom, -m_Zoom);
    }
    else
    {
        m_Camera = new PerspectiveCamera(45.f, aspectRatio, 0.1f, 100.0f);
    }
    
}

void CameraController::SetCameraType(CameraType type)
{
    delete m_Camera;
    m_CameraType = type;
    if (type == CameraType::ORTHO)
    {
        m_Camera = new OrthographicCamera(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, -m_Zoom);
        SetRotation(0.f, 0.f, 0.f);
        SetZoom(GetZoom());
    }
    else
    {
        m_Camera = new PerspectiveCamera(45.f, -m_AspectRatio, 0.1f, 100.0f);
        SetRotation(0.f, 180.f, 0.f);
    }
    
}

void CameraController::HandleMouseMovement(float x, float y)
{
    if (m_CameraType == CameraType::ORTHO)
    {
        SetPosition(GetPosition() + glm::vec3(x * 0.0028f * m_Zoom, y * 0.0028f * m_Zoom, 0.f));
    }
    else
    {
        SetRotation(GetRotation().x, GetRotation().y + x, GetRotation().z + y);
        
    }
}

void CameraController::Update(Timestep ts)
{
    dt =  ts.GetSeconds();
    m_Camera->SetPosition(GetPosition());
    m_Camera->SetRotation(GetRotation());
    

}

void CameraController::Begin()
{
}

void CameraController::OnDestroy()
{
}

void CameraController::SetZoom(float zoom)
{
    m_Zoom = glm::clamp(zoom, 0.1f, 100.f);
    if (m_CameraType == CameraType::ORTHO)
    {
        static_cast<OrthographicCamera*>(m_Camera)->SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, m_Zoom, -m_Zoom);
    }
}

void CameraController::Move(glm::vec3 direction)
{
    if (m_CameraType == CameraType::ORTHO)
    {
        return;
    }
    else
    {
        glm::vec3 forwardMotion = m_Camera->GetForwardVector() * dt * direction.x;
        glm::vec3 rightMotion = m_Camera->GetRightVector() * dt * direction.y;
        glm::vec3 upMotion = m_Camera->GetUpVector() * dt * direction.z;
        SetPosition(GetPosition() + forwardMotion + rightMotion + upMotion);
    }
    
}

void CameraController::Resize(float width, float height)
{
    m_AspectRatio = width / height;
    if (m_CameraType == CameraType::ORTHO)
    {
        static_cast<OrthographicCamera*>(m_Camera)->SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, m_Zoom, -m_Zoom);
    }
}

void CameraController::OnInspectorGUI()
{
    if (ImGui::CollapsingHeader("Camera"))
    {
        if (ImGui::RadioButton("Perspective", m_CameraType == CameraType::PERSPECTIVE))
        {
            SetCameraType(CameraType::PERSPECTIVE);
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Orthographic", m_CameraType == CameraType::ORTHO))
        {
            SetCameraType(CameraType::ORTHO);
        }
        if (ImGui::DragFloat("Zoom", &m_Zoom))
        {
            SetZoom(m_Zoom);
        }

        ImGui::Separator();
    }



    
}


