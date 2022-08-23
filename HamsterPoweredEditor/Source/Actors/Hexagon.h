#include "Actor.h"
#include "Core/App.h"
#include "Rendering/GLVertexArray.h"
#include "Rendering/GLBuffers.h"
#include "ResourceManagement/Shader.h"
#include "ResourceManagement/Texture.h"

class Hexagon : public Actor
{
public:
    Hexagon();
    ~Hexagon();

    void Draw() override;
    void Update(Timestep ts) override;
    void Begin() override;
    void OnDestroy() override;

    void SetTexture(const std::string& path);
    void SetTexture(const std::string& path, const std::string& path2);
    void OnInspectorGUI() override;

    float vertices[8 * 7] = {
        //Positions             //Vertex Colurs     //Texture Coords
        0.0f,  0.0f, 0.0f,      0.f, 0.f, 1.f,      0.5f, 0.5f,  // center
        -0.25f, 0.5f, 0.0f,      0.f, 1.f, 1.f,      0.25f, 1.0f, // LTOP
         0.25f, 0.5f, 0.0f,      1.f, 1.f, 0.f,      0.75f, 1.0f, // RTOP
         0.5f, 0.0f, 0.0f,      1.f, 0.f, 1.f,      1.0f, 0.5f, // R
         0.25f,-0.5f, 0.0f,      1.f, 0.f, 1.f,      0.75f, 0.0f, // RBOT
        -0.25f,-0.5f, 0.0f,      0.f, 0.f, 1.f,      0.25f, 0.0f, // LBOT
        -0.5f, 0.0f, 0.0f,      0.f, 0.f, 1.f,      0.0f, 0.5f, // L
     };
    
    unsigned int indices[8] = {
        0, 1, 6, 5, 4, 3, 2, 1
    };  

    uint32_t m_DrawMode = GL_TRIANGLE_FAN;
    std::shared_ptr<GLVertexArray> va;
    std::shared_ptr<GLIndexBuffer> ib;
    std::shared_ptr<GLVertexBuffer> vb;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Texture> texture2;

    std::vector<glm::vec3> m_InstancePositions = {{-1, 0, 0}, {-2, 1, 0}};
    
};
