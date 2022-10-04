#pragma once
#include <list>
#include <memory>

#include "Timer.h"
#include "Timestep.h"
#include <vector>

#include "Actors/CameraController.h"

class Actor;
class Quad;
class Scene
{
public:
    Scene();
    ~Scene();
    void Update(Timestep ts);
    void Render();
    void Begin();
    void DestroyActor(Actor* actor);
    void SetColour(glm::vec4 colour);
    const std::string& GetPath() const { return m_filepath; }
    
    const std::vector<Actor*>& GetActors() const { return m_actors; }
    
    template<typename T, typename... Args>
    T* SpawnActor(Args... args)
    {
        T* actor = new T(args...);
        actor->m_scene = this;
        m_actors.emplace_back(actor);
        return actor;
    }

    void SerializeScene(const std::string& filepath);
    void DeserializeScene(const std::string& filepath);
    
private:
    friend class App;
    friend class ImGuiLayer;
    friend class UIComponent;
    friend class HierarchyPanel;
    friend class Viewport;
    void DeferredDestroy();
    
    std::vector<Actor*> m_actors;
    std::vector<Actor*> m_actorsToDestroy;
    std::string m_name;
    std::string m_filepath;
    CameraController* m_editorCamera;
    glm::vec4 m_sceneColour = { 0.1f, 0.1f, 0.1f, 1.0f };
    
    std::shared_ptr<Quad> quad1;
    std::shared_ptr<Quad> quad2;
};
