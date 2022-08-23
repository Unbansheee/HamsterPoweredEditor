#include "Scene.h"

#include <iostream>

#include "Actors/Hexagon.h"
#include "Actors/Quad.h"

void Scene::Update(Timestep ts)
{
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
    
    auto teetho = SpawnActor<Quad>();
    teetho->SetName("Teetho");
    teetho->SetPosition(1, 0);

    auto gear = SpawnActor<Quad>();
    gear->SetName("Gear");
    gear->SetPosition(0, 0);
    gear->SetTexture("Resources/Textures/Gear.png");

    auto hex = SpawnActor<Hexagon>();
    hex->SetName("Hexagon");
    hex->SetPosition(1, -1);
    hex->SetTexture("Resources/Textures/Brick.jpg");
    
    
}

void Scene::DestroyActor(Actor* actor)
{
    m_actorsToDestroy.push_back(actor);
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
