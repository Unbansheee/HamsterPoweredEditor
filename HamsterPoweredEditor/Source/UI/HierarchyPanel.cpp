#include "HierarchyPanel.h"

#include <iostream>

#include "Core/App.h"
#include "ImGuiLayer.h"
#include "imgui_internal.h"
#include "Actors/NameComponent.h"
#include "Actors/TransformComponent.h"
#include "Core/Scene.h"

void HierarchyPanel::Init()
{
    UIComponent::Init();
    name = "Hierarchy";
}

void HierarchyPanel::Begin()
{
    UIComponent::Begin();
}


void HierarchyPanel::RenderTree(GameObject& gameObject, int& index)
{
    index++;
    ImGuiTreeNodeFlags showArrow = gameObject.GetComponent<TransformComponent>()->GetChildren().size() > 0 ? 0 : ImGuiTreeNodeFlags_Leaf;
    ImGuiTreeNodeFlags selected = &gameObject == m_Parent->m_SelectedGameObject ? ImGuiTreeNodeFlags_Selected : 0;
    ImGuiTreeNodeFlags flags = showArrow | selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
    bool open = ImGui::TreeNodeEx((gameObject.GetComponent<NameComponent>()->GetName() + "##" + std::to_string(index)).c_str(), flags);
    
    //Handle click events before recursion
    if (ImGui::IsItemClicked())
    {
        m_Parent->m_SelectedGameObject = &gameObject;
    }
    if (ImGui::IsItemClicked(1))
    {
        m_Parent->m_SelectedGameObject = &gameObject;
        ImGui::OpenPopup("ContextMenu");
    }

    //Drag and drop logic
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GAMEOBJECT", &gameObject, sizeof(GameObject*));
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
        {
            GameObject* draggedActor = *static_cast<GameObject**>(payload->Data);
            //make sure item is not being dragged onto itself
            if (draggedActor != &gameObject && gameObject.GetParent() != draggedActor)
            {
                if (draggedActor->GetParent() == &gameObject)
                {
                    //Check for grandparent
                    if (draggedActor->GetParent()->GetParent())
                    {
                        //Attach to grandparent
                        draggedActor->SetParent(draggedActor->GetParent()->GetParent());
                    }
                    else
                    {
                        //If there is no grandparent, become a root node
                        draggedActor->RemoveFromParent();
                    }
                }
                else
                {
                    draggedActor->SetParent(&gameObject);
                }
            }
            //set active selection
            m_Parent->m_SelectedGameObject = &gameObject;
        }
        ImGui::EndDragDropTarget();
    }


    //Context menu for selected item
    if (&gameObject == m_Parent->m_SelectedGameObject)
    {
        if (ImGui::BeginPopup("ContextMenu"))
        {
            if (ImGui::MenuItem("Delete"))
            {
                gameObject.Destroy();
                m_Parent->m_SelectedGameObject = nullptr;
            }
            ImGui::EndPopup();
        }
    }
    
    //Recursive call for children
    if (open)
    {
        for (auto child : gameObject.GetComponent<TransformComponent>()->GetChildren())
        {
            RenderTree(*child->GetOwner(), index);
        }
        ImGui::TreePop();
    }
    
}

void HierarchyPanel::Update(Timestep ts)
{
    UIComponent::Update(ts);


    int index = 0;

    RenderTree(App::Instance().m_currentScene->m_editorCamera, index);



    ImGui::Separator();
    
    if (ImGui::Button("Add Object"))
    {
        //Spawn context menu
        //ImGui::OpenPopup("AddActor");
        App::Instance().m_currentScene->SpawnObject();
    }
    
    for (auto& i : App::Instance().m_currentScene->GetGameObjects())
    {
        
        if (!i.GetParent())
        {
            RenderTree(i, index);
        }
    }

    //Clear selection when nothing is clicked
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
    {
        m_Parent->m_SelectedGameObject = nullptr;
    }
    


    
    if (ImGui::IsMouseReleased(0) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && !ImGui::IsDragDropPayloadBeingAccepted())
    {

    }
    
}


void HierarchyPanel::End()
{
    UIComponent::End();
    
}
