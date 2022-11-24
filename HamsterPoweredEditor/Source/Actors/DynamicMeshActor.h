﻿#pragma once
#include "Actor.h"
#include "Rendering/DynamicMesh.h"
#include <string>

class DynamicMeshActor : public Actor
{
public:
    DynamicMeshActor();
    void Update(Timestep ts) override;
    void Draw() override;
    void Begin() override;
    void OnDestroy() override;

    DynamicMesh& GetMesh() { return m_mesh; }
    
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;

protected:
    void OnInspectorGUI() override;
    std::string texturePath;
public:
    ~DynamicMeshActor() override;
    std::string meshPath;


    DynamicMesh m_mesh;
    
    bool TraceEnabled = false;
};
