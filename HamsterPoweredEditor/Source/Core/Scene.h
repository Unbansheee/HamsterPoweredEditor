#pragma once
#include <list>
#include <memory>
#include <queue>

#include "Timer.h"
#include "Timestep.h"
#include <vector>

#include "Actors/CameraController.h"
#include "Actors/GameObject.h"

#include "Physics.h"
#include "UUID.h"

class Quad;
class Scene
{
public:
    Scene();
    ~Scene();
    void Update(Timestep ts);
    void Render();
    void Begin();
    void DestroyObject(GameObject* object);
    void SetColour(glm::vec4 colour);
    CameraController* GetCameraController() { return m_cameraController; }
    const std::string& GetPath() const { return m_filepath; }
    
    std::vector<GameObject>& GetGameObjects() { return m_gameObjects; }

    GameObject& SpawnObject();
    


    void SerializeScene(const std::string& filepath);
    void DeserializeScene(const std::string& filepath);

    void SetParentChild(const HP::UUID& parentID, const HP::UUID& childID);
    
private:
    friend class App;
    friend class ImGuiLayer;
    friend class UIComponent;
    friend class HierarchyPanel;
    friend class Viewport;
    void DeferredDestroy();
    
    std::vector<GameObject*> m_objectsToDestroy;
    std::string m_name;
    std::string m_filepath;
    GameObject m_editorCamera;
    CameraController* m_cameraController;
    glm::vec4 m_sceneColour = { 0.1f, 0.1f, 0.1f, 1.0f };
    std::queue<std::pair<HP::UUID, HP::UUID>> m_parentChildQueue;
    double m_fixedUpdateAccumulator = 0.0;
    double m_fixedUpdateInterval = 1.0 / 120.0;
    
    
    std::vector<GameObject> m_gameObjects;
    
    
    std::shared_ptr<Quad> quad1;
    std::shared_ptr<Quad> quad2;

    bool m_initialized = false;
};
