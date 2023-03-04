#pragma once
#include "UIComponent.h"

class InspectorPanel : public UIComponent
{
public:
    void Init() override;
    void Begin() override;
    void Update(Timestep ts) override;
    void End() override;


private:
    bool m_lockedScale = true;
};

