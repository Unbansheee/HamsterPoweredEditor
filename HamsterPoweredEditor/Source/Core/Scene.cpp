#include "Scene.h"

#include <fstream>
#include <iostream>

#include "Timer.h"
#include "Actors/AnimatedQuad.h"
#include "Actors/Hexagon.h"

#include "Actors/Actor.h"
#include "Actors/Quad.h"
#include "Rendering/TextLabel.h"
#include "Actors/Mesh.h"
#include "json.hpp"

Scene::Scene()
{
    //Temporary editor camera. This should be moved to Scene or UI Layer later
    m_editorCamera = new CameraController(CameraController::CameraType::ORTHO, (float)Renderer::GetViewportSize().x / (float)Renderer::GetViewportSize().x);
    m_editorCamera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_editorCamera->SetName("Editor Camera");
    m_editorCamera->SetZoom(2.5f);

    Renderer::SetClearColor(m_sceneColour);
}

Scene::~Scene()
{
    for (auto& actor : m_actors)
    {
        delete actor;
    }

    delete m_editorCamera;
    
}

void Scene::Update(Timestep ts)
{
    m_editorCamera->Update(ts);
    for (Actor* actor : m_actors)
    {
        actor->Update(ts);
    }
}

void Scene::Render()
{
    for (Actor* actor : m_actors)
    {
        actor->Draw();
    }
}

void Scene::Begin()
{
    for (Actor* actor : m_actors)
    {
        actor->Begin();
    }


    
    
}

void Scene::DestroyActor(Actor* actor)
{
    m_actorsToDestroy.push_back(actor);
}

void Scene::SetColour(glm::vec4 colour)
{
    m_sceneColour = colour;
    Renderer::SetClearColor(colour);
}

void Scene::SerializeScene(const std::string& filepath)
{
    
    nlohmann::json j;
    j["EditorCamera"] = m_editorCamera->Serialize();

    j["SceneColour"] = m_sceneColour;
    
    j["Actors"] = nlohmann::json::array();
    for (Actor* actor : m_actors)
    {
        nlohmann::json actorJson = actor->Serialize();
        j["Actors"].push_back(actorJson);
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
        m_editorCamera->Deserialize(j["EditorCamera"]);
    }

    if (j.contains("SceneColour"))
    {
        m_sceneColour = j["SceneColour"];
        Renderer::SetClearColor(m_sceneColour);
    }
    
    //iterate through all the actors in the json file
     for (auto& actorJson : j["Actors"])
    {
        std::string actorType = actorJson["ActorType"];
        Actor* actor = nullptr;
        if (actorType == "Actor")
        {
            actor = SpawnActor<Actor>();
        }
        else if (actorType == "Quad")
        {
            actor = SpawnActor<Quad>();
        }
        else if (actorType == "AnimatedQuad")
        {
            actor = SpawnActor<AnimatedQuad>();
        }
        else if (actorType == "Hexagon")
        {
            actor = SpawnActor<Hexagon>();
        }
        else if (actorType == "Mesh")
        {
            actor = SpawnActor<Mesh>();
        }
        else if (actorType == "TextLabel")
        {
            actor = SpawnActor<TextLabel>();
        }
        else
        {
            std::cout << "Unknown actor type: {0}" << actorType << std::endl;
            continue;
        }
        actor->Deserialize(actorJson);
    }
    
}

void Scene::DeferredDestroy()
{
    for (int i = 0; i < m_actorsToDestroy.size(); i++)
    {
        auto it = std::find(m_actors.begin(), m_actors.end(), m_actorsToDestroy[i]);
        m_actorsToDestroy[i]->DetachFromParent();
        delete *it;
        m_actors.erase(it);
        
    }
    m_actorsToDestroy.clear();
}
