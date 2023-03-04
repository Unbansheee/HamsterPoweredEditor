#pragma once
#include "UIComponent.h"
#include "Actors/GameObject.h"

class HierarchyPanel : public UIComponent
{
public:
    void Init() override;
    void Begin() override;
    void RenderTree(GameObject& gameObject, int& index);
    void Update(Timestep ts) override;
    void End() override;

private:
    
};
