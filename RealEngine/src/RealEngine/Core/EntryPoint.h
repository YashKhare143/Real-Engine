#pragma once
#include "RealEngine/Core/Core.h"
#include "repch.h"

#ifdef RE_PLATFORM_WINDOWS

	extern RealEngine::Application* RealEngine::CreateApplication();

	int main(int argc, char** argv) {

		RealEngine::Log::Init();

		RE_PROFILE_BEGIN_SESSION("Startup", "RealEnineProfile-Startup.json");
			auto app = RealEngine::CreateApplication();
		RE_PROFILE_END_SESSION();

		RE_PROFILE_BEGIN_SESSION("Runtime", "RealEnineProfile-Runtime.json");
			app->Run();
		RE_PROFILE_END_SESSION();

		RE_PROFILE_BEGIN_SESSION("Shutdown", "RealEnineProfile-Shutdown.json");
			delete app;
		RE_PROFILE_END_SESSION();
	}

#endif 
