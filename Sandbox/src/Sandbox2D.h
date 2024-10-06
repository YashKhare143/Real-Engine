#pragma once
#include <RealEngine.h>

class Sandbox2D : public RealEngine::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(RealEngine::Timestep ts) override;
	virtual void  OnImGuiRender() override;
	void OnEvent(RealEngine::Event& event) override;
private:

	RealEngine::OrthographicCameraController m_CameraController;

	RealEngine::Ref<RealEngine::VertexArray> m_SquareVA;
	RealEngine::Ref<RealEngine::Shader> m_FlatColorShader;

	RealEngine::Ref<RealEngine::Texture2D> m_HeartTexture;
	RealEngine::Ref<RealEngine::Texture2D> m_DotTexture;

	RealEngine::Shaderlibrary m_Shaderlibrary;

	glm::vec4 m_DotTextureTint = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 m_HeartTextureTint = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 SquareColor2 = { 1.0f, 1.0f, 1.0f, 1.0f };


	struct ProfileResult {
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult>	m_ProfileResults;

};

