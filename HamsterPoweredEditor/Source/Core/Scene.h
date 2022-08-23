#pragma once
#include <list>
#include <memory>
#include "Timestep.h"
#include "Actors/Actor.h"
class Quad;
class Scene
{
public:
    void Update(Timestep ts);
    void Render();
    void Begin();
    void DestroyActor(Actor* actor);

    const std::vector<Actor*>& GetActors() const { return m_actors; }
    
    template<typename T, typename... Args>
    T* SpawnActor(Args... args)
    {
        T* actor = new T(args...);
        actor->m_scene = this;
        m_actors.emplace_back(actor);
        return actor;
    }
    
private:
    friend class App;
    void DeferredDestroy();
    
    std::vector<Actor*> m_actors;
    std::vector<Actor*> m_actorsToDestroy;
    std::string m_name;

    std::shared_ptr<Quad> quad1;
    std::shared_ptr<Quad> quad2;
};
