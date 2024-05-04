#pragma once
#include "repch.h"
#include "Core.h"
#include "Window.h"

namespace RealEngine {

	class RE_API Application
	{
		public:
			Application();
			virtual ~Application();
			void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
	Application* CreateApplication();

}

  