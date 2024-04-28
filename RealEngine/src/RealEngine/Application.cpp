#include "Application.h"
#include "stdio.h"

#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Log.h"

namespace RealEngine {
	Application::Application() {};
	Application::~Application() {};

	void Application::Run() {
		printf("Welcom To Real Engine!");
		WindowResizeEvent e(1280, 720);
		//RE_TRACE(e);
		while (true);
	}
}