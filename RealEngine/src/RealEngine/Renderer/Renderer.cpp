#include "repch.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace RealEngine {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const OrthographicCamera& Camera)
	{
		m_SceneData->ViewProjectionMatrix = Camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertedArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertedArray->Bind();
		RenderCommand::DrawIndex(vertedArray);
	}
}