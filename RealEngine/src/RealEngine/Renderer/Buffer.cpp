#include "repch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGl/OpenGLBuffer.h"

namespace RealEngine {
	void VertexBuffer::SetLayour(const BufferLayout& layout)
	{
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(size);

		}
		RE_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(vertices, size);

		}
		RE_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indices, count);

		}
		RE_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

}