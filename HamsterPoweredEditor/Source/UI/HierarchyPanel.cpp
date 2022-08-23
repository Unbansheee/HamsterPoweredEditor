#include "HierarchyPanel.h"

#include <iostream>

#include "Core/App.h"
#include "ImGuiLayer.h"
#include "imgui_internal.h"

void HierarchyPanel::Init()
{
    UIComponent::Init();
    name = "Hierarchy";
}

void HierarchyPanel::Begin()
{
    UIComponent::Begin();
}

void HierarchyPanel::RenderTree(Actor* actor, int& index)
{
    index++;
    ImGuiTreeNodeFlags showArrow = actor->GetChildren().size() > 0 ? 0 : ImGuiTreeNodeFlags_Leaf;
    ImGuiTreeNodeFlags selected = actor == m_Parent->m_SelectedActor ? ImGuiTreeNodeFlags_Selected : 0;
    ImGuiTreeNodeFlags flags = showArrow | selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
    bool open = ImGui::TreeNodeEx((actor->GetName() + "##" + std::to_string(index)).c_str(), flags);
    
    //Handle click events before recursion
    if (ImGui::IsItemClicked())
    {
        m_Parent->m_SelectedActor = actor;
    }
    if (ImGui::IsItemClicked(1))
    {
        m_Parent->m_SelectedActor = actor;
        ImGui::OpenPopup("ContextMenu");
    }

    //Drag and drop logic
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("ACTOR", &actor, sizeof(Actor*));
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ACTOR");
        if (payload)
        {
            Actor* draggedActor = *(Actor**)payload->Data;
            //make sure item is not being dragged onto itself
            if (draggedActor != actor && actor->GetParent() != draggedActor)
            {
                if (draggedActor->GetParent() == actor)
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
                        draggedActor->DetachFromParent();
                    }
                }
                else
                {
                    draggedActor->SetParent(actor);
                }
            }
            //set active selection
            m_Parent->m_SelectedActor = actor;
        }
        ImGui::EndDragDropTarget();
    }


    //Context menu for selected item
    if (actor == m_Parent->m_SelectedActor)
    {
        if (ImGui::BeginPopup("ContextMenu"))
        {
            if (ImGui::MenuItem("Delete"))
            {
                actor->Destroy();
                m_Parent->m_SelectedActor = nullptr;
            }
            ImGui::EndPopup();
        }
    }
    
    //Recursive call for children
    if (open)
    {
        for (auto child : actor->GetChildren())
        {
            RenderTree(child, index);
        }
        ImGui::TreePop();
    }

    
    
}

void HierarchyPanel::Update(Timestep ts)
{
    UIComponent::Update(ts);


    int index = 0;

    RenderTree(App::Instance().m_Camera, index);

    ImGui::Separator();
    
    for (auto& i : App::Instance().m_currentScene->GetActors())
    {
        if (!i->GetParent())
        {
            RenderTree(i, index);
        }
    }

    //Clear selection when nothing is clicked
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
    {
        m_Parent->m_SelectedActor = nullptr;
    }

    
    if (ImGui::IsMouseReleased(0) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && !ImGui::IsDragDropPayloadBeingAccepted())
    {
        auto payload = ImGui::GetDragDropPayload();
        if (payload)
        {
            Actor* draggedActor = *(Actor**)payload->Data;
            if (draggedActor)
            {
                draggedActor->DetachFromParent();
            }
        }
    }
}


void HierarchyPanel::End()
{
    UIComponent::End();
    
}
