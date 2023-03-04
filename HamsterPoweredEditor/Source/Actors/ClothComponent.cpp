#include "ClothComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "Core/Input.h"

struct ClothParticle
{
    glm::vec3 Position = {0, 0, 0};
    glm::vec3 OldPosition = {0, 0, 0};
    glm::vec3 Velocity = { 0, 0, 0 };
    glm::vec3 Acceleration = {0, 0, 0};
    glm::vec3 Force = { 0, 0, 0 };
    float Mass = 1.f;

    bool bFixed;
    bool bCollisionParticle;
};

struct ClothConstraint
{
    ClothConstraint(ClothParticle* p1, ClothParticle* p2, float restLength)
        : P1(p1), P2(p2), RestLength(restLength) {}

    ClothParticle* P1;
    ClothParticle* P2;
    float RestLength = 1.f;
    
};

ClothComponent::ClothComponent(GameObject* owner) : ProceduralMeshComponent(owner)
{

}

void ClothComponent::Begin()
{

    ProceduralMeshComponent::Begin();
    GenerateCloth(width, height);
}

void ClothComponent::SerializeCustom(nlohmann::json& j)
{
    ProceduralMeshComponent::SerializeCustom(j);
}

void ClothComponent::FixedUpdate(double ts)
{

    ProceduralMeshComponent::FixedUpdate(ts);
    VerletIntegration((float)ts);
    SatisfyConstraints(iterations, (float)ts);
    
}

void ClothComponent::Update(Timestep ts)
{
    ProceduralMeshComponent::Update(ts);
    //If mouse is clicked, raycast to find the nearest vertice and attach it to the mouse
    if (Input::WasMouseButtonPressed(Mouse::Left))
    {
        /*
        auto hit = Input::RaycastMouse();
        if (hit.Actor == this)
        {
            m_mouseDown = true;
            m_mouseHit = hit;

            // Find closest point
            float minDist = 1000000.f;
            for (auto& p : m_particles)
            {
                float dist = glm::distance(p.Position, m_mouseHit.Location);
                if (dist < minDist)
                {
                    minDist = dist;
                    m_mouseDownVert = &p;
                }
            }
            
        }
        */
    }
    

    // Let go of the dragged vertice if mouse is released (if there is one)
    if (Input::WasMouseButtonReleased(Mouse::Left))
    {
        m_mouseDown = false;
        if (m_mouseDownVert)
        {
            m_mouseDownVert->bFixed = false;
            m_mouseDownVert = nullptr;
        }
    }


    // If we currently have a vertex selected and are holding mouse button, make it follow the mouse in projected screenspace
    if (m_mouseDown)
    {
        if (m_mouseDownVert)
        {
            // translate vert perpendicular to the hit normal
            glm::vec3 v0 = Renderer::ScreenToWorldPos(Input::GetMousePositionInViewport(), 0.0f);
            glm::vec3 v1 = Renderer::ScreenToWorldPos(Input::GetMousePositionInViewport(), 1.0f);

            glm::vec3 dir  = (v1 - v0); 
            dir = glm::normalize(dir);

            //Project from screen plane into worldspace based on initial distance from vertex to camera
            glm::vec3 mouseposprojected = v0 + (dir * m_mouseHit.Distance);
            m_mouseDownVert->bFixed = true;
            m_mouseDownVert->Position = mouseposprojected;
            m_mouseDownVert->OldPosition = mouseposprojected;
            
            
        }
    }
    UpdateMesh();
}

void ClothComponent::Render(const glm::mat4& transform)
{
    ProceduralMeshComponent::Render(transform);
}

void ClothComponent::OnInspectorGUI()
{
    //ProceduralMeshComponent::OnInspectorGUI();

    if (ImGui::Button("Reset Cloth"))
    {
        GenerateCloth(width, height);
    }
    
    if(ImGui::CollapsingHeader("Cloth Attributes"))
    {
        
        
        if (ImGui::DragInt("Width", &width, 1, 1, 100))
        {
            GenerateCloth(width, height);   
        }
        if (ImGui::DragInt("Height", &height, 1, 1, 100))
        {
            GenerateCloth(width, height);
        }
        if (ImGui::DragFloat("Rest Distance", &restLength, 0.1f, 0.1f, 10.0f))
        {

        }
    }
    
    if (ImGui::CollapsingHeader("Solver"))
    {
        if (ImGui::DragInt("Iterations", & iterations, 1, 1, 16))
        {}

        if (ImGui::DragFloat3("Wind", &m_wind.x, 0.1f, -10.0f, 10.0f))
        {}

        if (ImGui::DragFloat("Turbulence", &windTurbulenceStrength, 0.1f, 0.0f, 50.0f))
        {}

        if (ImGui::DragFloat("Wind Frequency", &windFrequency, 0.1f, 0.0f, 50.0f))
        {}

        if (ImGui::DragFloat3("Gravity", &m_gravity.x, 0.1f, -10.0f, 10.0f))
        {}

        if (ImGui::DragFloat("Stiffness", &stiffness, 0.01f, 0.0f, 1.0f))
        {}

        if (ImGui::DragFloat("Damping", &damping, 0.01f, 0.0f, 1.0f))
        {}
    }
    
}

void ClothComponent::DeserializeCustom(nlohmann::json& j)
{

}

void ClothComponent::ResetCloth()
{
}

void ClothComponent::GenerateCloth(int width, int height)
{
    m_particles.clear();
    m_constraints.clear();
    m_Mesh.Clear();

    this->width = width;
    this->height = height;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    //Create particles
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y * width + x;
            ClothParticle& p = m_particles.emplace_back();
            p.Position = glm::vec3(x, y, 0);
            p.OldPosition = p.Position;
            p.Velocity = glm::vec3(0, 0, 0);

            //Top left, top right and top middle particles are fixed
            p.bFixed = (y == height - 1 && (x == 0 || x == width - 1 || x == width / 2));

            //All edge particles and every second particle is used to test collision
            if (y % 2 == 0 || x % 2 == 0 || y == height - 1 || y == 0 || x == 0 || x == width - 1)
            {
                p.bCollisionParticle = true;
            }
        }
    }

    //Create constraints
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y  * width + x;
            if (x < width - 1)
            {
                m_constraints.emplace_back(&m_particles[index], &m_particles[index + 1], restLength);
            }
            if (y < height - 1)
            {
                m_constraints.emplace_back(&m_particles[index], &m_particles[index + width], restLength);
            }
            if (x < width - 1 && y < height - 1)
            {
                m_constraints.emplace_back(ClothConstraint(&m_particles[index], &m_particles[index + width + 1], restLength * (float)sqrt(2)));
                m_constraints.emplace_back(ClothConstraint(&m_particles[index + 1], &m_particles[index + width], restLength * (float)sqrt(2)));
            }
        }
    }


    
    //Derive mesh from particles, then initialise the cloth mesh
    GetVertices(vertices, indices);
    m_Mesh.Create(vertices, indices);
}

void ClothComponent::VerletIntegration(float deltaTime)
{
    auto transform = Owner->GetComponent<TransformComponent>()->GetWorldTransform();
    //convert gravity from world space to local space
    glm::vec3 gravity = glm::vec3(glm::inverse(transform) * glm::vec4(m_gravity, 0));

    //convert wind from world space to local space
    glm::vec3 wind = glm::vec3(glm::inverse(transform) * glm::vec4(m_wind, 0));

    // apply some turbulent wind using perlin noise
    //wind.x += (perlin.normalizedOctave1D(glfwGetTime() * windFrequency + p.Position.x, 3, 0.5)) * windTurbulenceStrength;
    // wind.y += (perlin.normalizedOctave1D(glfwGetTime() * windFrequency + p.Position.y, 3, 0.5)) * windTurbulenceStrength;
    // wind.z += (perlin.normalizedOctave1D(glfwGetTime() * windFrequency + p.Position.z, 3, 0.5)) * windTurbulenceStrength;
    // perlin.noise




    for (auto& p : m_particles)
    {
        if (!p.bFixed)
        {
            glm::vec3 FinalForce = { 0,0,0 };
            //Apply Gravity
            FinalForce += gravity;
            //Apply Wind
            FinalForce += wind;
            //Apply Drag
            FinalForce += -damping * p.Velocity;

            //Verlet Integration
            glm::vec3 temp = p.Position;
            p.Position += (p.Position - p.OldPosition) + (FinalForce * deltaTime * deltaTime);
            p.OldPosition = temp;
            p.Velocity = (p.Position - p.OldPosition) / deltaTime;
            
        }
    }
}

void ClothComponent::ResolveCollisions(float ts)
{
}

void ClothComponent::SatisfyConstraints(int iterations, float ts)
{
    //Can run multiple iterations to make the cloth more rigid & stable
    for (int i = 0; i < iterations; i++)
    {
        for (auto& c : m_constraints)
        {
            ClothParticle* p1 = c.P1;
            ClothParticle* p2 = c.P2;

            //Get the current distance between the two particles
            glm::vec3 delta = p1->Position - p2->Position;
            const float distance = (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);



            
            if (distance > 0)
            {
                //Correct particle positions
                const float distsqrt = sqrt(distance);
                const float difference = (c.RestLength - distsqrt) / distsqrt;
                glm::vec3 translate = delta * difference * 0.5f * stiffness * ts;
                
                if (!p1->bFixed)
                {
                    p1->Position += translate;
                }
                if (!p2->bFixed)
                {
                    p2->Position -= translate;
                }
            }
        }
    }
}

void ClothComponent::GetVertices(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
    if (m_particles.size() == 0)
        return;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            //Front Faces
            ClothParticle& p = m_particles[y * width + x];

            // Create vertex
            Vertex& v = vertices.emplace_back();
            v.Position = p.Position;
            v.TexCoord = glm::vec2(x / (float)width, y / (float)height);
            v.Colour = glm::vec3(1.f, 1.f, 1.f);
            v.Normal = {0, 0, 1};

            // Create indices
            if (x < width - 1 && y < height - 1)
            {
                indices.push_back(y * width + x);
                indices.push_back(y * width + x + 1);
                indices.push_back((y + 1) * width + x);

                indices.push_back(y * width + x + 1);
                indices.push_back((y + 1) * width + x + 1);
                indices.push_back((y + 1) * width + x);
            }
            
        }
    }

    //Generate Back faces for shading. These get slapped on to the back because I thought about this too late and did not want to rework it
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            //Back faces
            ClothParticle& p = m_particles[y * width + x];

            // Create vertex
            Vertex& v = vertices.emplace_back();
            v.Position = p.Position;
            v.TexCoord = glm::vec2(x / (float)width, y / (float)height);
            v.Colour = glm::vec3(1.f, 1.f, 1.f);
            v.Normal = { 0, 0, -1 };

            // Create indices
            if (x < width - 1 && y < height - 1)
            {
                indices.push_back(width * height + y * width + x);
                indices.push_back(width * height + (y + 1) * width + x);
                indices.push_back(width * height + y * width + x + 1);

                indices.push_back(width * height + y * width + x + 1);
                indices.push_back(width * height + (y + 1) * width + x);
                indices.push_back(width * height + (y + 1) * width + x + 1);
            }
        }
    }
    
    

    // Calculate triangle normals using cross product
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        uint32_t i0 = indices[i];
        uint32_t i1 = indices[i + 1];
        uint32_t i2 = indices[i + 2];

        glm::vec3 v1 = vertices[i1].Position - vertices[i0].Position;
        glm::vec3 v2 = vertices[i2].Position - vertices[i0].Position;

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        vertices[i0].Normal += normal;
        vertices[i1].Normal += normal;
        vertices[i2].Normal += normal;
    }
}

void ClothComponent::UpdateMesh()
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    GetVertices(vertices, indices);
    m_Mesh.Update(vertices, indices);
}
