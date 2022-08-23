#include "Stats.h"

#include <iostream>

#include "Rendering/Renderer.h"

void Stats::Init()
{
	name = "Statistics";
	canBeClosed = true;
}

void Stats::Update(Timestep ts)
{
	ImGui::Text("Run Time: %.2i : %.2i", (int)glm::floor(ts.GetTimeAsMinutes()), (int)glm::floor(ts.GetTimeAsSeconds() - glm::floor(ts.GetTimeAsMinutes()) * 60));
	ImGui::Text("Frame Time: %.2f ms", ts.GetMilliseconds());
	ImGui::Text("FPS: %.1f", 1.f / ts.GetSeconds());
	ImGui::Text("Draw Calls: %d", Renderer::drawCalls);
}
