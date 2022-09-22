#include "App.h"

#include <iostream>
#include <ostream>

#include "Font.h"
#include "UI/ImGuiLayer.h"

#include "imgui.h"
#include "Timer.h"
#include "ResourceManagement/Shader.h"
#include "ResourceManagement/Texture.h"
#include "Window.h"
#include "Actors/CameraController.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



void App::Begin()
{

	//Entry point of App. Initialises GLAD
	gladLoadGL();

	//Create a window context for the app
	window = new Window("Hamster Powered Editor", 1280, 720);
	window->PollEvents();
	
	//Initialise fonts
	Font::LoadFont("Resources/Fonts/Roboto-Regular.ttf", 100.f);
	Font::LoadFont("Resources/Fonts/Roboto-Bold.ttf", 100.f);
	Font::LoadFont("Resources/Fonts/Roboto-Italic.ttf", 100.f);
	Font::LoadFont("Resources/Fonts/ARIAL.TTF", 100.f);
	Font::LoadFont("Resources/Fonts/VERDANA.TTF", 100.f);
	Font::LoadFont("Resources/Fonts/Gresta.ttf", 100.f);
	Font::LoadFont("Resources/Fonts/South Australia.ttf", 100.f);
	
	//Temporary editor camera. This should be moved to Scene or UI Layer later
	m_Camera = new CameraController(CameraController::CameraType::ORTHO, (float)window->GetWidth() / (float)window->GetHeight());
	m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	m_Camera->SetName("Editor Camera");
	m_Camera->SetZoom(2.5f);
	
	
	Renderer::Init();
	Renderer::SetClearColor(glm::vec4(0.09f, 0.09f, 0.12f, 1.0f));

	//Initialise editor. At a later point this can be skipped and the window should show the framebuffer.
	//Currently the framebuffer is only shown in the Viewport widget, so this must be left on for now.
	
	EditorLayer = new ImGuiLayer(window);
	EditorLayer->Begin();
	
	LoadScene<Scene>();
}


void App::Update()
{
	//Update timestep
	CurrentTime = (float)glfwGetTime();
	float time = (float)glfwGetTime();
	timestep.Update(time - m_LastFrameTime);
	m_LastFrameTime = time;

	if (EditorLayer) EditorLayer->BeginFrame();
	
	//UPDATE SCENE

	if (m_currentScene)
	{
		m_Camera->Update(timestep);
		m_currentScene->Update(timestep);

		//RENDER SCENE
		Renderer::BeginScene(*m_Camera->GetCamera());
		m_currentScene->Render();
		Renderer::EndScene();

		//Destroy queued items as not to break references in the update loop
		m_currentScene->DeferredDestroy();
	}
	
	//UPDATE/RENDER EDITOR
	if (EditorLayer)
	{
		EditorLayer->Update(timestep);
		EditorLayer->EndFrame();
	}
	

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
