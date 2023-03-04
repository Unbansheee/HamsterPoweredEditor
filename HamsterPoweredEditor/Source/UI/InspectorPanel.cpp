#include "InspectorPanel.h"

#include "ImGuiLayer.h"
#include "Actors/GameObject.h"
#include "Actors/ComponentSpawns.generated.h"
#include "Actors/MeshComponent.h"
#include "Actors/NameComponent.h"


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
 

    {
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
                GENERATED_COMPONENTADDSUI()
                
                ImGui::EndPopup();
            }
        
            if (selected) selected->OnInspectorGUI();
        }
    }
}


void InspectorPanel::End()
{
    UIComponent::End();
}


