#pragma once



#ifdef RE_PLATFORM_WINDOWS

	extern RealEngine::Application* RealEngine::CreateApplication();

	int main(int argc, char** argv) {

		RealEngine::Log::Init();

		RE_CORE_WARN("Initialize!");
		int a = 5;
		RE_ERROR("Hello! Var={0}", a);

		auto app = RealEngine::CreateApplication();
		app->Run();
		delete app;
	}

#endif 
