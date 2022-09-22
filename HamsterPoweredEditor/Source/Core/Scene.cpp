#include "Scene.h"

#include <iostream>

#include "Timer.h"
#include "Actors/AnimatedQuad.h"
#include "Actors/Hexagon.h"
#include "Actors/Quad.h"
#include "Rendering/TextLabel.h"

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

    
    auto floor = SpawnActor<Quad>();
    floor->SetName("Floor");
    floor->SetPosition(0, -1.5);
    floor->SetScale(20);
    floor->SetTexture("Resources/Textures/Brick.jpg");
    floor->SetRotation(90, 0, 0);
    
    auto teetho = SpawnActor<Quad>();
    teetho->SetName("Teetho");
    teetho->SetPosition(1, 0);
    teetho->SetUpdateCallback([=]()
    {
        glm::vec3 pos = teetho->GetPosition();
        float pos1 = 0.5f;
        float pos2 = 0.f;

        //lerp between the two positions
        float lerp = (float)glm::mix(pos1, pos2, (float)glm::sin(App::Instance().timestep.GetTimeAsSeconds()));
        teetho->SetPosition(pos.x, lerp, pos.z);
    });

    auto gear = SpawnActor<Quad>();
    gear->SetName("Gear");
    gear->SetPosition(0, -1);
    gear->SetTexture("Resources/Textures/Gear.png");
    gear->SetUpdateCallback([=]()
    {
        gear->AddRotationOffset(0, 0, -100 * App::Instance().timestep.GetSeconds());
    });


    auto fire = SpawnActor<AnimatedQuad>();
    fire->SetName("Animated Fire");
    fire->SetPosition(-1, -0.1);
    fire->SetSpriteSheet("Resources/Textures/Fire.png", 60, 6, 10);
    fire->SetFPS(30);


    auto fire2 = SpawnActor<AnimatedQuad>();
    fire2->SetName("Animated Fire 2");
    fire2->SetPosition(1, -0.1);
    fire2->SetSpriteSheet("Resources/Textures/Fire2.png", 60, 6, 10)->SetFilteringMode(Texture::FilteringMode::NEAREST);
    fire2->SetFPS(30);
    
    auto hex = SpawnActor<Hexagon>();
    hex->SetName("Hexagon");
    hex->SetPosition(1, -1);
    hex->SetTexture("Resources/Textures/Brick.jpg");

    auto text = SpawnActor<TextLabel>();
    text->SetName("Text");


    
    
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
