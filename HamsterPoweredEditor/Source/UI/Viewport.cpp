#include "Viewport.h"

#include <iostream>

#include "Core/App.h"
#include "imgui_internal.h"

void Viewport::Init()
{
	name = "Viewport";
	canBeClosed = true;

}

void Viewport::Update(Timestep ts)
{
	CameraController* cam = App::Instance().m_currentScene->m_editorCamera;
	ImGui::PopStyleVar(1);
	ImGui::Image((void*)Renderer::m_FrameBuffer->GetColorAttachmentRendererID(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	//check if resized
	if (ImGui::GetWindowSize().x != lastSize.x || ImGui::GetWindowSize().y != lastSize.y)
	{
		float width = ImGui::GetContentRegionMax().x;
		float height = ImGui::GetContentRegionMax().y;
		Renderer::Resize(width, height);
		App::Instance().m_currentScene->m_editorCamera->Resize(width, height);
	}
	lastSize = ImGui::GetWindowSize();

	if (ImGui::IsMouseReleased(1))
	{
		mouseCaptured = false;
		App::Instance().window->SetCursorMode(GLFW_CURSOR_NORMAL);
	}
	
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsMouseClicked(1))
		{
			mouseCaptured = true;

		}

		if (cam->GetCameraType() == CameraController::CameraType::ORTHO) cam->SetZoom(cam->GetZoom() - (ImGui::GetIO().MouseWheel/4.f) * cam->GetZoom()/2.f);
		
	}
	if (mouseCaptured)
	{
		if (ImGui::IsMouseClicked(1)) return;
		//cam->AddPositionOffset(-ImGui::GetMouseDragDelta(1).x * 0.0028f * cam->GetZoom(), ImGui::GetMouseDragDelta(1).y * 0.0028f * cam->GetZoom(), 0);
		cam->HandleMouseMovement(-ImGui::GetMouseDragDelta(1).x, ImGui::GetMouseDragDelta(1).y);
		App::Instance().window->SetCursorMode(GLFW_CURSOR_DISABLED);
		
		if (cam->GetCameraType() == CameraController::CameraType::ORTHO) cam->SetZoom(cam->GetZoom() - (ImGui::GetIO().MouseWheel/4.f) * cam->GetZoom()/2.f);
		else cam->SetPerspSpeed(cam->GetPerspSpeed() + (ImGui::GetIO().MouseWheel/4.f) * cam->GetPerspSpeed()/2.f);

		if (ImGui::IsKeyDown(ImGuiKey_W))
		{
			cam->Move({1, 0, 0});
		}
		if (ImGui::IsKeyDown(ImGuiKey_S))
		{
			cam->Move({ -1, 0, 0 });
		}
		if (ImGui::IsKeyDown(ImGuiKey_A))
		{
			cam->Move({ 0, -1, 0 });
		}
		if (ImGui::IsKeyDown(ImGuiKey_D))
		{
			cam->Move({ 0, 1, 0 });
		}
		if (ImGui::IsKeyDown(ImGuiKey_Q))
		{
			cam->Move({ 0, 0, 1 });
		}
		if (ImGui::IsKeyDown(ImGuiKey_E))
		{
			cam->Move({ 0, 0, -1 });
		}
	}
	
}

void Viewport::Begin()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 2));
	UIComponent::Begin();
}

void Viewport::End()
{
	UIComponent::End();
}
