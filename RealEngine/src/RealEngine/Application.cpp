#include "repch.h"
#include "Application.h"
#include "stdio.h"

#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Log.h"

namespace RealEngine {
	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
	};
	Application::~Application() {};

	void Application::Run() {
		
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}