#include "repch.h"
#include "Renderer2D.h"
#include "RealEngine/Renderer/VertexArray.h"
#include "RealEngine/Renderer/Shader.h"
#include <glm/gtc/matrix_transform.hpp>
//#include "Platform/OpenGl/OpenGLShader.h"

namespace RealEngine {
	struct Renderer2DStorage {
		Ref<VertexArray> QuadVertexArray;	
		Ref<Shader> TextureShader;
		Shaderlibrary m_Shaderlibrary;

		Ref<Texture2D> m_WhiteTexture;
	};
	static Renderer2DStorage* s_Data;
	void Renderer2D::Init()
	{
		RE_PROFILE_FUNC();

		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float SquareVertices[5 * 4] = {						// Defining Vertex Data

			////// vertex position ////// 		
				-0.5f,-0.5f, 0.0f,	0.0f, 0.0f,
				 0.5f,-0.5f, 0.0f,	1.0f, 0.0f,
				 0.5f, 0.5f, 0.0f,	1.0f, 1.0f,
				-0.5f, 0.5f, 0.0f,	0.0f, 1.0f
		////////////////////////////////////////// 
		};

		Ref<VertexBuffer> m_SquareVB;
		m_SquareVB = (VertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));

		BufferLayout SquareVBlayout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};

		m_SquareVB->SetLayour(SquareVBlayout);
		s_Data->QuadVertexArray->AddVertexBuffer(m_SquareVB);


		uint32_t SquareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> m_SquareIB;
		m_SquareIB = IndexBuffer::Create(SquareIndices, sizeof(SquareIndices) / sizeof(uint32_t));  //Creating Index Buffer

		s_Data->QuadVertexArray->SetIndexBuffer(m_SquareIB);

		s_Data->m_WhiteTexture = Texture2D::Create(1, 1);

		static uint32_t WhiteTextureData = 0xffffffff;
		s_Data->m_WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32_t));
		////Creating Square Texture Shader

		s_Data->TextureShader = s_Data->m_Shaderlibrary.Load("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);

		///////////////////////
	}

	void Renderer2D::Shutdown()
	{
		RE_PROFILE_FUNC();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		RE_PROFILE_FUNC();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		RE_PROFILE_FUNC();

	}

	void Renderer2D::DrawQuard(const glm::vec2& pos, float rotation, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawQuard( {pos.x, pos.y, 0.0f}, rotation, scale, color);
	}

	void Renderer2D::DrawQuard(const glm::vec3& pos, float rotation, const glm::vec2& scale,const glm::vec4& color)
	{
		RE_PROFILE_FUNC();

		//s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->m_WhiteTexture->Bind();

		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Data->QuadVertexArray);

	}

	void Renderer2D::DrawQuard(const glm::vec2& pos, float rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{

		DrawQuard({ pos.x, pos.y, 0.0f }, rotation, scale, texture, tint);
	}

	void Renderer2D::DrawQuard(const glm::vec3& pos, float rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{

		RE_PROFILE_FUNC();

		s_Data->TextureShader->SetFloat4("u_Color", tint);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);

		texture->Bind();
		s_Data->TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndex(s_Data->QuadVertexArray);
	}

}