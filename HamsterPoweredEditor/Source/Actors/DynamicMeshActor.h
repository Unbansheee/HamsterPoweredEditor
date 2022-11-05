#pragma once
#include "Actor.h"
#include "Rendering/DynamicMesh.h"

class DynamicMeshActor : public Actor
{
public:
    void Update(Timestep ts) override;
    void Draw() override;
    void Begin() override;
    void OnDestroy() override;
    
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;

protected:
    void OnInspectorGUI() override;

public:
    ~DynamicMeshActor() override;


    DynamicMesh m_mesh;
};
