#include "GameObject.h"

#include <iostream>
#include <vcruntime_typeinfo.h>

#include "MeshComponent.h"
#include "NameComponent.h"
#include "PointLightComponent.h"
#include "ProceduralMeshComponent.h"
#include "TransformComponent.h"

GameObject::GameObject()
{
    //ctor
    AddComponent<NameComponent>();
    AddComponent<TransformComponent>();
    AddComponent<MeshComponent>("Resources/Meshes/Sphere.obj");
    
    
}

void GameObject::Update(Timestep ts)
{
    for(auto& component : components)
    {
        component->Update(ts);
    }
}

void GameObject::Render()
{
    for(auto& component : components)
    {
        component->Render(GetComponent<TransformComponent>()->GetWorldTransform());
    }
}

void GameObject::OnInspectorGUI()
{
    for(auto& component : components)
    {
        
        std::string name = typeid(*component).name();
        name.erase(0, 6);

        if (name == "NameComponent")
        {
            std::string name = GetComponent<NameComponent>()->GetName();
            if (ImGui::InputText("Name", &name))
            {
                GetComponent<NameComponent>()->SetName(name);
            }
            continue;
        }
        
        if (ImGui::CollapsingHeader(name.c_str()))
        {
            component->OnInspectorGUI();
            ImGui::Separator();
        }
    }
}

void GameObject::Destroy()
{
}

GameObject* GameObject::GetParent()
{
    auto transform = GetComponent<TransformComponent>();
    if (transform->GetParent())
    {
        return transform->GetParent()->GetOwner();
    }
    return nullptr;
    
}

void GameObject::SetParent(GameObject* parent)
{
    auto transform = GetComponent<TransformComponent>();
    transform->SetParent(parent->GetComponent<TransformComponent>());
}

void GameObject::AddChild(GameObject* child)
{
    auto transform = GetComponent<TransformComponent>();
    transform->AddChild(child->GetComponent<TransformComponent>());
}

void GameObject::RemoveChild(GameObject* child)
{
    auto transform = GetComponent<TransformComponent>();
    transform->RemoveChild(child->GetComponent<TransformComponent>());
}

void GameObject::RemoveAllChildren()
{
    auto transform = GetComponent<TransformComponent>();
    transform->RemoveAllChildren();
}

void GameObject::RemoveFromParent()
{
    auto transform = GetComponent<TransformComponent>();
    transform->RemoveFromParent();
}


