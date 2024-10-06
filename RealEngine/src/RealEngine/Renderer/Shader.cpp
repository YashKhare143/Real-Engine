#include "repch.h"
#include "Shader.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "Renderer.h"

namespace RealEngine {
	Ref<Shader> Shader::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(path);

		}
		RE_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);

		}
		RE_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	void Shaderlibrary::Add(const std::string& name, Ref<Shader>& shader)
	{

		RE_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void Shaderlibrary::Add(Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}


	Ref<Shader> Shaderlibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);

		return shader;
	}

	Ref<Shader> Shaderlibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);

		return shader;
	}

	Ref<Shader> Shaderlibrary::Get(const std::string& name)
	{
		RE_CORE_ASSERT(Exists(name), "Shader not found!");

		return m_Shaders[name];
	}

	bool Shaderlibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}