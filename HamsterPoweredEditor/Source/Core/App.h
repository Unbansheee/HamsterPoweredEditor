#pragma once
#include <memory>
#include <vector>

#include "Scene.h"
#include "Rendering/OrthographicCamera.h"
#include "Actors/Quad.h"
#include "Rendering/Renderer.h"
#include "Window.h"
#include "Timestep.h"
#include "Rendering/GLFrameBuffer.h"
#include "Actors/CameraController.h"

struct GLFWwindow;

class ImGuiLayer;
class App
{
public:
	void Update();
	void Begin();
	void Quit();
	float GetTime();

	template<typename T>
	void LoadScene()
		{
			if (m_currentScene != nullptr)
			{
				delete m_currentScene;
				m_currentScene = nullptr;
			}
			m_currentScene = new T();
			m_currentScene->Begin();
		}
	
private:
	App(){}

	float CurrentTime;

	
	friend class Quad;
	friend class HierarchyPanel;
public:
	CameraController* m_Camera;
	static App& Instance()
	{
		static App instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}
	
	App(App const&) = delete;
	void operator=(App const&) = delete;

	ImGuiLayer* EditorLayer = nullptr;
	Window* window = nullptr;
	Timestep timestep;
	float m_LastFrameTime = 0.0f;

	Scene* m_currentScene;
	std::shared_ptr<GLFrameBuffer> m_FrameBuffer;

	
	
};

