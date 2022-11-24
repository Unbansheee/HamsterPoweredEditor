#pragma once
#include "DynamicMeshActor.h"

class ShinyMesh : public DynamicMeshActor
{
public:
    ShinyMesh();
    void Begin() override;
    void Draw() override;
};
