#include "NameComponent.h"

#include "imgui.h"

#include "imgui_stdlib.h"

void NameComponent::OnInspectorGUI()
{
    Component::OnInspectorGUI();
    ImGui::InputText("Name", &m_name);
}
