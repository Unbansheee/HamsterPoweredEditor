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
}

void GameObject::Update(Timestep ts)
{
    for(auto& component : components)
    {
        component->Update(ts);
    }
}

void GameObject::SetupComponents()
{
    AddComponent<NameComponent>();
    AddComponent<TransformComponent>();
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

void GameObject::Serialize(nlohmann::json& j)
{
    j["Components"] = nlohmann::json::array();
    for (auto& component : components)
    {
        nlohmann::json componentJson;
        component->Serialize(componentJson);
        component->SerializeCustom(componentJson);
        j["Components"].push_back(componentJson);
    }
}

void GameObject::Deserialize(nlohmann::json& j)
{
    for (auto& componentjson : j["Components"])
    {
        if (componentjson == nullptr)
        {
            continue;
        }
        
        std::string type = componentjson["Type"];
        
        Component* component;
        

        
        if (type == "NameComponent")
        {
            component = GetComponent<NameComponent>();
            if (!component)
            {
                component = AddComponent<NameComponent>();
            }
        }
        else if (type == "TransformComponent")
        {
            component = GetComponent<TransformComponent>();
            if (!component)
            {
                component = AddComponent<TransformComponent>();
            }
        }
        else if (type == "MeshComponent")
        {
            component = GetComponent<MeshComponent>();
            if (!component)
            {
                component = AddComponent<MeshComponent>();
            }
        }
        else if (type == "PointLightComponent")
        {
            component = GetComponent<PointLightComponent>();
            if (!component)
            {
                component = AddComponent<PointLightComponent>();
            }
        }
        else if (type == "ProceduralMeshComponent")
        {
            component = GetComponent<ProceduralMeshComponent>();
            if (!component)
            {
                component = AddComponent<ProceduralMeshComponent>();
            }
        }
        else
        {
            continue;
        }
        component->Deserialize(componentjson);
        component->DeserializeCustom(componentjson);
        
    }
}


