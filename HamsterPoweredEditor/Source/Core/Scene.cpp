#include "Scene.h"

#include <fstream>
#include <iostream>

#include "Timer.h"

#include "json.hpp"
#include "Actors/NameComponent.h"
#include "Actors/TransformComponent.h"


class RimLitActor;

Scene::Scene()
{
    //Temporary editor camera. This should be moved to Scene or UI Layer later
    m_editorCamera.SetupComponents();
    m_editorCamera.Begin();
    m_cameraController = m_editorCamera.AddComponent<CameraController>();
    m_editorCamera.GetTransform()->SetWorldPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_editorCamera.GetComponent<NameComponent>()->SetName("Editor Camera");
    m_cameraController->SetZoom(2.5f);
    m_cameraController->SetCameraType(CameraController::CameraType::PERSPECTIVE);

    m_gameObjects.reserve(1024);



    Renderer::SetClearColor(m_sceneColour);
}

Scene::~Scene()
{


    //delete m_editorCamera;
    
}

void Scene::Update(Timestep ts)
{
    m_fixedUpdateAccumulator += ts.GetSeconds();
    while (m_fixedUpdateAccumulator >= m_fixedUpdateInterval)
    {

        

        
        m_fixedUpdateAccumulator -= m_fixedUpdateInterval;
        
        for (GameObject& gameobject : m_gameObjects)
        {
            gameobject.FixedUpdate(m_fixedUpdateInterval);
        }
    }
    
    m_editorCamera.Update(ts);

    for (GameObject& gameObject : m_gameObjects)
    {
        gameObject.Update(ts);
    }
    
    
}

void Scene::Render()
{

    for (GameObject& gameObject : m_gameObjects)
    {
        gameObject.Render();
    }

}

void Scene::Begin()
{

    for (GameObject& gameObject : m_gameObjects)
    {
        gameObject.Begin();
    }

    m_initialized = true;
    
}



void Scene::DestroyObject(GameObject* object)
{
    m_objectsToDestroy.push_back(object);
}

void Scene::SetColour(glm::vec4 colour)
{
    m_sceneColour = colour;
    Renderer::SetClearColor(colour);
}

GameObject& Scene::SpawnObject()
{
    auto& object = m_gameObjects.emplace_back();
    object.SetupComponents();
    object.GetComponent<NameComponent>()->SetName("New Object");
    object.m_Scene = this;
    if (m_initialized)
    {
        object.Begin();
    }
    return object;
}

void Scene::SerializeScene(const std::string& filepath)
{
    
    nlohmann::json j;
    nlohmann::json cam;
    m_editorCamera.Serialize(cam);
    
    j["EditorCamera"] = cam;

    j["SceneColour"] = m_sceneColour;
    j["AmbientIntensity"] = Renderer::m_AmbientLightStrength;
    j["RenderMode"] = Renderer::GetRenderMode();
    

    j["GameObjects"] = nlohmann::json::array();
    for (GameObject& gameObject : m_gameObjects)
    {
        nlohmann::json gameObjectJson;
        gameObject.Serialize(gameObjectJson);
        j["GameObjects"].push_back(gameObjectJson);
    }

    std::ofstream file(filepath);
    file << j.dump(4);
    
    
}

void Scene::DeserializeScene(const std::string& filepath)
{
    nlohmann::json j;
    std::ifstream file(filepath);
    file >> j;

    if (j.contains("EditorCamera"))
    {
        m_editorCamera.Deserialize(j["EditorCamera"]);
    }

    if (j.contains("SceneColour"))
    {
        m_sceneColour = j["SceneColour"];
        Renderer::SetClearColor(m_sceneColour);
    }

    if (j.contains("RenderMode"))
    {
        Renderer::SetRenderMode(j["RenderMode"]);
    }

    if (j.contains("AmbientIntensity"))
    {
        Renderer::m_AmbientLightStrength = j["AmbientIntensity"];
    }
    
    

    for (auto gameObjectJson : j["GameObjects"])
    {
        GameObject& object = SpawnObject();
        auto test = object.components;
        object.Deserialize(gameObjectJson);
    }
    
    
    
}



void Scene::SetParentChild(const HP::UUID& parentID, const HP::UUID& childID)
{
    m_parentChildQueue.push({ parentID, childID });
}


void Scene::DeferredDestroy()
{

    for (int i = 0; i < m_objectsToDestroy.size(); i++)
    {
        auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), *m_objectsToDestroy[i]);
        m_objectsToDestroy[i]->RemoveFromParent();
        m_gameObjects.erase(it);
    }
    m_objectsToDestroy.clear();
}
