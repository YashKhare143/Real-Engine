#include "repch.h"
#include "Application.h"
#include "RealEngine/Core/Log.h"

#include "Input.h"
#include "RealEngine/Renderer/Renderer.h"
#include <GLFW/glfw3.h>


namespace RealEngine {

#define BIND_EVENT_FN(f) std::bind(&Application::f, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	

	Application::Application() {
		RE_PROFILE_FUNC();

		RE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		

	};
	Application::~Application() {
		RE_PROFILE_FUNC();
		Renderer::Shutdown();
	};
	
	void Application::PushLayer(Layer* layer)
	{
		RE_PROFILE_FUNC();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		RE_PROFILE_FUNC();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
		
	void Application::OnEvent(Event& e) {
		RE_PROFILE_FUNC();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		//RE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}

	};

	void Application::Run() {
		RE_PROFILE_FUNC();
		while (m_Running)
		{
			RE_PROFILE_SCOPE("RunLoop");
			float time = (float)glfwGetTime(); // (todo): Make it platform specific
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				{
					RE_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack) {
						layer->OnUpdate(timestep);
					}
				}

				m_ImGuiLayer->Begin();
				{
					RE_PROFILE_SCOPE("LayerStack OnImGuiRender");
					for (Layer* layer : m_LayerStack) {
						layer->OnImGuiRender();
					}
				}
				m_ImGuiLayer->End();

			}
			m_Window->WindowOnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{

		m_Running = false;
		return true;

	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		RE_PROFILE_FUNC();

		RE_ERROR("({0}): {1}", e.GetWidth(), e.GetHeight());
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}