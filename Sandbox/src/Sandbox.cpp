#include <RealEngine.h>

class ExampleLayer : public RealEngine::Layer
{
public:
	ExampleLayer() : Layer("Example") 
	{
	}
	void OnUpdate() override
	{
		//RE_INFO("ExampleLayer::Update");
		//if (RealEngine::Input::IsKeyPressed(RE_KEY_SPACE))
		//	RE_TRACE("Space Button Pressed");
	}
	void OnEvent(RealEngine::Event& event) override
	{
		//RE_TRACE("{0}",event);
		if (event.GetEventType() == RealEngine::EventType::KeyPressed) {
			RealEngine::KeyPressedEvent& e = (RealEngine::KeyPressedEvent&)event;
			RE_TRACE("{0}", (char)e.GetKeyCode());
		}

	}
};

class Sandbox : public RealEngine::Application
{
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new RealEngine::ImGuiLayer());
	}

	~Sandbox() {

	}

};

RealEngine::Application* RealEngine::CreateApplication() {
	return new Sandbox();
}

	