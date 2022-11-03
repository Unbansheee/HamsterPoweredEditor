#include "DynamicMeshActor.h"

void DynamicMeshActor::Update(Timestep ts)
{
    Actor::Update(ts);
}

void DynamicMeshActor::OnDestroy()
{
    Actor::OnDestroy();
}

nlohmann::json DynamicMeshActor::Serialize()
{
    return Actor::Serialize();
}

void DynamicMeshActor::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
}

void DynamicMeshActor::OnInspectorGUI()
{
    Actor::OnInspectorGUI();
}

DynamicMeshActor::~DynamicMeshActor()
{
}

void DynamicMeshActor::Draw()
{
    Actor::Draw();
    mesh.Draw(m_transform);
}

void DynamicMeshActor::Begin()
{
    Actor::Begin();

    
}
