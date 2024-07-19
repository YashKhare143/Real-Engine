#include <RealEngine.h>
#include "imgui/imgui.h"
 
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

	virtual void  OnImGuiRender() override {
		//auto m_ImGuiLayer = new RealEngine::ImGuiLayer();

		//ImGui::Begin("Test");
		//ImGui::Text("Hello World");
		//ImGui::End();
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
	}

	~Sandbox() {

	}

};

RealEngine::Application* RealEngine::CreateApplication() {
	return new Sandbox();
}

	