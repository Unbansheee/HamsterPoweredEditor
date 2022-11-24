#include "InspectorPanel.h"

#include "Actors/Actor.h"
#include "ImGuiLayer.h"
#include "Actors/GameObject.h"
#include "Actors/MeshComponent.h"
#include "Actors/PointLightComponent.h"

void InspectorPanel::DrawTransformEdit(Actor* actor)
{
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        glm::vec3 pos = actor->GetPosition();
        if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
        {
            actor->SetPosition(pos);
        }

        glm::vec3 rot = actor->GetRotation();
        if(ImGui::DragFloat3("Rotation", &rot.x))
        {
            actor->SetRotation(rot);
        }
        
        glm::vec3 scale = actor->GetScale();
        if (m_lockedScale)
        {
            if (ImGui::DragFloat("Scale", &scale.x, 0.1f))
            {
                float deltaScale = scale.x - actor->GetScale().x;
                if (scale.x == 0.f)
                {
                    scale.x = 0.000001f * glm::sign(deltaScale);
                }
                
                float yscale = scale.x * actor->GetScale().y / actor->GetScale().x;
                float zscale = scale.x * actor->GetScale().z / actor->GetScale().x;
                actor->SetScale(scale.x, yscale, zscale);
            }
        }
        else
        {
            if(ImGui::DragFloat3("Scale", &scale.x, 0.1f))
            {
                float deltaScaleX = scale.x - actor->GetScale().x;
                if (scale.x == 0.f)
                {
                    scale.x = 0.000001f * glm::sign(deltaScaleX);
                }

                float deltaScaleY = scale.y - actor->GetScale().y;
                if (scale.y == 0.f)
                {
                    scale.y = 0.000001f * glm::sign(deltaScaleY);
                }

                float deltaScaleZ = scale.z - actor->GetScale().z;
                if (scale.z == 0.f)
                {
                    scale.z = 0.000001f * glm::sign(deltaScaleZ);
                }
                
                
                
                actor->SetScale(scale);
            }
        }
        ImGui::Checkbox("Lock Scale", &m_lockedScale);
        ImGui::Separator();
    }
}

void InspectorPanel::Init()
{
    UIComponent::Init();
    name = "Inspector";
}

void InspectorPanel::Begin()
{
    UIComponent::Begin();
}

void InspectorPanel::Update(Timestep ts)
{
    UIComponent::Update(ts);

    /*
    if (m_Parent->m_SelectedActor)
    {
        Actor* actor = m_Parent->m_SelectedActor;

        std::string name = actor->GetName();
        if(ImGui::InputText("Name", &name))
        {
            actor->SetName(name);
        }
        
        DrawTransformEdit(actor);

        actor->OnInspectorGUI();
    }
    */

    auto selected = m_Parent->m_SelectedGameObject;
    if (selected)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.2f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.3f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.1f, 0.0f, 1.0f));
        
        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponent");
        }

        ImGui::PopStyleColor(3);

        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Mesh"))
            {
                selected->AddComponent<MeshComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Point Light"))
            {
                selected->AddComponent<PointLightComponent>();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        
        selected->OnInspectorGUI();
    }
}

void InspectorPanel::End()
{
    UIComponent::End();
}


