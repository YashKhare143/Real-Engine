#pragma once
#include "Core.h"
#include "Window.h"
#include "RealEngine/LayerStack.h"
#include "RealEngine/Events/Event.h"
#include "RealEngine/Events/ApplicationEvent.h"	

namespace RealEngine {

	class RE_API Application
	{
		public:
			Application();
			virtual ~Application();
			void Run();
			void OnEvent(Event& e);

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);

			inline Window& GetWindow() { return *m_Window; }
			inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};
	Application* CreateApplication();

}

  