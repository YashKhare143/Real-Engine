#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"	

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

	m_CameraController.OnUpdate(ts);

	RealEngine::RenderCommand::SetClearColor({ 0.203, 0.176, 0.2 , 1 });
	RealEngine::RenderCommand::Clear();

	RealEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

		RealEngine::Renderer2D::DrawQuard({0.0f,0.0f},	30.0f, {1.0f,1.0f}, m_HeartTexture, m_HeartTextureTint);
		RealEngine::Renderer2D::DrawQuard({1.5f,0.0f},	10.0f, {0.8f,1.2f}, SquareColor2);
		RealEngine::Renderer2D::DrawQuard({0.0f,0.0f, -0.1f},	0.0f, {10.0f,10.0f}, m_DotTexture, m_DotTextureTint);

	RealEngine::Renderer2D::EndScene();


}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

		ImGui::ColorEdit4("Dot Tint Color", glm::value_ptr(m_DotTextureTint));
		ImGui::ColorEdit4("Heart Tint Color", glm::value_ptr(m_HeartTextureTint));
		ImGui::ColorEdit4("Square 2 Color", glm::value_ptr(SquareColor2));

	ImGui::End();

}

void Sandbox2D::OnEvent(RealEngine::Event& event)
{
	m_CameraController.OnEvent(event);
}
