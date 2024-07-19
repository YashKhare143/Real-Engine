#include "repch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGLVertexArray.h"

namespace RealEngine {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();

		}
		RE_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}