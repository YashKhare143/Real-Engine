#include "repch.h"
#include "Application.h"
#include "stdio.h"
#include "RealEngine/Log.h"

#include <glad/glad.h>

namespace RealEngine {

#define BIND_EVENT_FN(f) std::bind(&Application::f, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		RE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		unsigned int id;
		glGenVertexArrays(1, &id);
	};
	Application::~Application() {};
	
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
		
	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		RE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}

	};

	void Application::Run() {
		
		while (m_Running)
		{
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_Window->WindowOnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{

		m_Running = false;
		return true;

	}

}