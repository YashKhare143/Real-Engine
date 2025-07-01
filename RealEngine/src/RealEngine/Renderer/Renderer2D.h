#pragma once
#include "RenderCommand.h"
#include "Texture.h"
#include "OrthographicCamera.h"
namespace RealEngine {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);	
		static void EndScene();
		static void Flush();

		static void DrawQuard(const glm::vec2& pos,  const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuard(const glm::vec3& pos,  const glm::vec2& scale, const glm::vec4& color);

		static void DrawQuard(const glm::vec2& pos,  const glm::vec2& scale, const  Ref<Texture2D>& texture, const glm::vec4& tint);
		static void DrawQuard(const glm::vec3& pos,  const glm::vec2& scale, const  Ref<Texture2D>& texture, const glm::vec4& tint);

		static void DrawRotatedQuard(const glm::vec2& pos, const float rotation,  const glm::vec2& scale, const glm::vec4& color);
		static void DrawRotatedQuard(const glm::vec3& pos, const float rotation,  const glm::vec2& scale, const glm::vec4& color);
		static void DrawRotatedQuard(const glm::vec2& pos, const float rotation,  const glm::vec2& scale, const  Ref<Texture2D>& texture, const glm::vec4& tint);
		static void DrawRotatedQuard(const glm::vec3& pos, const float rotation,  const glm::vec2& scale, const  Ref<Texture2D>& texture, const glm::vec4& tint);
		
	};

}

