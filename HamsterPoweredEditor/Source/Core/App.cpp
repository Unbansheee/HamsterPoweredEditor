#include "App.h"

#include <iostream>
#include <ostream>

#include "UI/ImGuiLayer.h"

#include "imgui.h"
#include "ResourceManagement/Shader.h"
#include "ResourceManagement/Texture.h"
#include "Window.h"
#include "Actors/CameraController.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

float CurrentTime;
Shader* shader = nullptr;
Texture* texture = nullptr;

void App::Begin()
{
	gladLoadGL();
	
	window = new Window("Hamster Powered Editor", 1280, 720);
	window->PollEvents();

	
	
	m_Camera = new CameraController(CameraController::CameraType::ORTHO, (float)window->GetWidth() / (float)window->GetHeight());
	m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	m_Camera->SetName("Editor Camera");
	
	
	Renderer::Init();
	Renderer::SetClearColor(glm::vec4(0.09f, 0.09f, 0.12f, 1.0f));
	
	EditorLayer = new ImGuiLayer(window);
	EditorLayer->Begin();
	
	LoadScene<Scene>();
}


void App::Update()
{
	CurrentTime = (float)glfwGetTime();
	float time = (float)glfwGetTime();
	timestep.Update(time - m_LastFrameTime);
	m_LastFrameTime = time;

	EditorLayer->BeginFrame();
	
	//UPDATE SCENE
	m_Camera->Update(timestep);
	if (m_currentScene)
	{
		m_currentScene->Update(timestep);

		//RENDER SCENE
		Renderer::BeginScene(*m_Camera->GetCamera());
		m_currentScene->Render();
		Renderer::EndScene();

		m_currentScene->DeferredDestroy();
	}
	
	//UPDATE/RENDER EDITOR
	if (EditorLayer)
	{
		EditorLayer->Update(timestep);
	}

	EditorLayer->EndFrame();
	
	window->SwapBuffers();
	window->PollEvents();

	ImGui::ResetMouseDragDelta(1);
}


void App::Quit()
{
	window->Quit();

}

float App::GetTime()
{
	return CurrentTime;
}
