#include "repch.h"
#include "Renderer2D.h"
#include "RealEngine/Renderer/VertexArray.h"
#include "RealEngine/Renderer/Shader.h"
#include <glm/gtc/matrix_transform.hpp>
//#include "Platform/OpenGl/OpenGLShader.h"

namespace RealEngine {


	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCord;
		float TexIndex;
	};

	struct Renderer2DData {
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // RenderCaps

		Ref<VertexArray> QuadVertexArray;	
		Ref<VertexBuffer> QuadVertexBuffer;	
		Ref<Shader> TextureShader;
		Shaderlibrary m_Shaderlibrary;

		Ref<Texture2D> m_WhiteTexture;

		uint32_t QuadIndexCount = 0 ;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;
	};

	static Renderer2DData s_Data;


	void Renderer2D::Init()
	{
		RE_PROFILE_FUNC();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = (VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex)));
		s_Data.QuadVertexBuffer->SetLayour({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"}
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices]; 

		
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);  //Creating Index Buffer
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.m_WhiteTexture = Texture2D::Create(1, 1);
		static uint32_t WhiteTextureData = 0xffffffff;
		s_Data.m_WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32_t));

		////Creating Square Texture Shader

		s_Data.TextureShader = s_Data.m_Shaderlibrary.Load("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_Texture", 0);

		///////////////////////

		// Set index 0 texture slots to white texture
		s_Data.TextureSlots[0] = s_Data.m_WhiteTexture;

	}

	void Renderer2D::Shutdown()
	{
		RE_PROFILE_FUNC();

	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		RE_PROFILE_FUNC();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;	

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		RE_PROFILE_FUNC();
		
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		//Bind all the textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndex(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuard(const glm::vec2& pos,  const glm::vec2& scale, const glm::vec4& color)
	{
		DrawQuard( {pos.x, pos.y, 0.0f},  scale, color);
	}

	void Renderer2D::DrawQuard(const glm::vec3& pos,  const glm::vec2& scale,const glm::vec4& color)
	{
		RE_PROFILE_FUNC();


		s_Data.QuadVertexBufferPtr->Position = pos;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCord = {0.0f, 0.0f};
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { pos.x + scale.x, pos.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;	
		s_Data.QuadVertexBufferPtr->TexCord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { pos.x + scale.x, pos.y + scale.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { pos.x, pos.y + scale.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
		//s_Data.TextureShader->Bind();

		/* s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.m_WhiteTexture->Bind();

		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) 
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Data.QuadVertexArray);
		*/
	}

	void Renderer2D::DrawQuard(const glm::vec2& pos,  const glm::vec2& scale, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{

		DrawQuard({ pos.x, pos.y, 0.0f },  scale, texture, tint);
	}

	void Renderer2D::DrawQuard(const glm::vec3& pos,  const glm::vec2& scale, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{

		RE_PROFILE_FUNC();

		constexpr glm::vec4 color = { 1.0f,1.0f ,1.0f ,1.0f };
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {

				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		}

		s_Data.QuadVertexBufferPtr->Position = pos;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { pos.x + scale.x, pos.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { pos.x + scale.x, pos.y + scale.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { pos.x, pos.y + scale.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		/*s_Data.TextureShader->SetFloat4("u_Color", tint);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);

		texture->Bind();
		s_Data.TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) 
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Data.QuadVertexArray);*/
	}

	void Renderer2D::DrawRotatedQuard(const glm::vec2& pos, const float rotation, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawRotatedQuard({ pos.x, pos.y, 0.0f },rotation, scale, color);
	}
	void Renderer2D::DrawRotatedQuard(const glm::vec3& pos, const float rotation, const glm::vec2& scale, const glm::vec4& color)
	{
		RE_PROFILE_FUNC();

		//s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.m_WhiteTexture->Bind();

		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Data.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuard(const glm::vec2& pos, const float rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{
		DrawRotatedQuard({ pos.x, pos.y, 0.0f }, rotation, scale, texture, tint);
	}
	void Renderer2D::DrawRotatedQuard(const glm::vec3& pos, const float rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{
		RE_PROFILE_FUNC();

		s_Data.TextureShader->SetFloat4("u_Color", tint);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);

		texture->Bind();
		s_Data.TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), rotation,glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Data.QuadVertexArray);

	}

}