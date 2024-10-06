#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"	
#include <iostream>
#include <chrono>




template <typename Fn>
class Timer {
public:
	Timer(const char* name, Fn&& func) : m_Name(name), m_func(func), m_Stopped(false) {
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}
	void Stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;

		m_func({m_Name, duration});
	}
	~Timer() {
		if (!m_Stopped)
			Stop();
	}
private:
	const char* m_Name;
	Fn m_func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;

};
#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult); });





Sandbox2D::Sandbox2D() : Layer("Example"), m_CameraController(1280.0f / 720.0f)
{
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
	m_DotTexture = RealEngine::Texture2D::Create("assets/textures/Dot.png");
	m_HeartTexture = RealEngine::Texture2D::Create("assets/textures/Heart.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(RealEngine::Timestep ts)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	{
		PROFILE_SCOPE("Renderer Prep");
		RealEngine::RenderCommand::SetClearColor({ 0.203, 0.176, 0.2 , 1 });
		RealEngine::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		RealEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

			RealEngine::Renderer2D::DrawQuard({0.0f,0.0f},	30.0f, {1.0f,1.0f}, m_HeartTexture, m_HeartTextureTint);
			RealEngine::Renderer2D::DrawQuard({1.5f,0.0f},	10.0f, {0.8f,1.2f}, SquareColor2);
			RealEngine::Renderer2D::DrawQuard({0.0f,0.0f, -0.1f},	0.0f, {10.0f,10.0f}, m_DotTexture, m_DotTextureTint);

		RealEngine::Renderer2D::EndScene();

	}

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

		ImGui::ColorEdit4("Dot Tint Color", glm::value_ptr(m_DotTextureTint));
		ImGui::ColorEdit4("Heart Tint Color", glm::value_ptr(m_HeartTextureTint));
		ImGui::ColorEdit4("Square 2 Color", glm::value_ptr(SquareColor2));

		for (auto& result : m_ProfileResults) {
			char label[50];
			strcpy(label, "%.3fms");
			strcat(label, result.Name);
			ImGui::Text(label, result.Time);

		}
		m_ProfileResults.clear();

	ImGui::End();

}

void Sandbox2D::OnEvent(RealEngine::Event& event)
{
	m_CameraController.OnEvent(event);
}
