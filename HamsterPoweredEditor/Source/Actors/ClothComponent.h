#pragma once
#include "PerlinNoise.hpp"
#include "ProceduralMeshComponent.h"
#include "Core/Raycast.h"
#include "ClothComponent.generated.h"

struct ClothParticle;
struct ClothConstraint;

SERIALIZEDCLASS(Category = "3D")

class ClothComponent : public ProceduralMeshComponent
{

    GENERATED()
    
public:
    
    ClothComponent(GameObject* owner);

    void Begin() override;
    void SerializeCustom(nlohmann::json& j) override;
    void FixedUpdate(double ts) override;
    void Update(Timestep ts) override;
    void Render(const glm::mat4& transform) override;
    void OnInspectorGUI() override;
    void DeserializeCustom(nlohmann::json& j) override;

protected:
    // Reset cloth particles, constraints and mesh to defaults
    void ResetCloth();
    // Generate particles and constraints
    void GenerateCloth(int width, int height);

    // Make the forces go wee woo
    void VerletIntegration(float deltaTime);

    // Fix all the mistakes the forces made >:(
    void ResolveCollisions(float ts);

    // Prevent particles from being a not cloth
    void SatisfyConstraints(int iterations, float ts);

    // Fills in the inputs with vertices and indices derived from particles
    void GetVertices(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

    // Shortcut to update the mesh
    void UpdateMesh();

    SERIALIZEDVAR int width = 10; // Number of particles in the x direction
    SERIALIZEDVAR int height = 10; // Number of particles in the y direction
    SERIALIZEDVAR float restLength = 1.f; // Distance between particles
    SERIALIZEDVAR float stiffness = 0.1f; // How much the constraints resist being stretched
    SERIALIZEDVAR float damping = 0.1f; // Slow down over time
    SERIALIZEDVAR int iterations = 4; // Number of times to satisfy constraints
    //float physicsTime = 200.f; // DEPRECATED: How long to run physics for
    SERIALIZEDVAR bool collisionEnabled = true; // Whether to resolve collisions against sphere
    SERIALIZEDVAR float windTurbulenceStrength = 10.f; // How much the wind turbulence affects the cloth
    SERIALIZEDVAR float windFrequency = 0.5f; // How often the wind turbulence changes
    siv::PerlinNoise perlin; // Perlin noise generator for wind turbulence

    //Forces
    SERIALIZEDVAR glm::vec3 m_gravity = { 0, -9.81f, 0 };
    SERIALIZEDVAR glm::vec3 m_wind = { 0, 0, 0 };
    
    std::vector<ClothParticle> m_particles;
    std::vector<ClothConstraint> m_constraints;
    std::vector<Vertex> m_verts;
    std::vector<uint32_t> m_indices;
    bool m_mouseDown;
    Hit m_mouseHit;
    ClothParticle* m_mouseDownVert;


};

#undef GENERATED