#pragma once
#include "Actor.h"
#include "DynamicMeshActor.h"
#include "PerlinNoise.hpp"
#include "Core/Raycast.h"
#include "Rendering/DynamicMesh.h"

class MeshActor;

struct ClothParticle
{
    glm::vec3 Position = {0, 0, 0};
    glm::vec3 OldPosition = {0, 0, 0};
    glm::vec3 Velocity = { 0, 0, 0 };
    glm::vec3 Acceleration = {0, 0, 0};
    glm::vec3 Force = { 0, 0, 0 };
    float Mass = 1.f;

    bool bFixed;
};

struct ClothConstraint
{
    ClothConstraint(ClothParticle* p1, ClothParticle* p2, float restLength)
        : P1(p1), P2(p2), RestLength(restLength) {}

    ClothParticle* P1;
    ClothParticle* P2;
    float RestLength = 1.f;
    
};

class ClothActor : public DynamicMeshActor
{
public:
    void FixedUpdate(double ts) override;
    void Update(Timestep ts) override;
    void Draw() override;
    void Begin() override;
    void OnDestroy() override;
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;

protected:
    void OnInspectorGUI() override;

    void ResetCloth();
    
    void GenerateCloth(int width, int height);
    void VerletIntegration(float deltaTime);
    void ResolveCollisions(float ts);
    void SatisfyConstraints(int iterations, float ts);
    void GetVertices(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
    void UpdateMesh();

    int width = 10;
    int height = 10;
    float restLength = 1.f;
    float stiffness = 0.1f;
    float damping = 0.1f;
    int iterations = 4;
    float physicsTime = 200.f;
    bool collisionEnabled = true;
    float windTurbulenceStrength = 10.f;
    float windFrequency = 0.5f;
    siv::PerlinNoise perlin;
    
    
    //DynamicMesh m_mesh;
    StaticMesh m_collisionMesh;
    glm::vec3 m_collisionMeshCurrentPosition = { 0, 0, 0 };
    float m_collisionMeshScale = 1.f;
    glm::vec3 m_collisionMeshOrigin = { 0, 0, 0 };
    float m_collisionMeshSpeed = 1.f;
    glm::vec3 m_collisionTravelDistance = {0, 0, 1};

    glm::vec3 m_gravity = { 0, -9.81f, 0 };
    glm::vec3 m_wind = { 0, 0, 0 };
    
    std::vector<ClothParticle> m_particles;
    std::vector<ClothConstraint> m_constraints;
    std::vector<Vertex> m_verts;
    std::vector<uint32_t> m_indices;
    bool m_mouseDown;
    Hit m_mouseHit;
    ClothParticle* m_mouseDownVert;

public:
    ~ClothActor() override;
    
};
