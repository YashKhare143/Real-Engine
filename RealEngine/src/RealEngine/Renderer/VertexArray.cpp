#include "repch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGLVertexArray.h"

namespace RealEngine {
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();

		}
		RE_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}