#include "repch.h"	
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace RealEngine {

	static GLenum ShaderDataTypeToGLBaseType(ShaderDataType type) {

		switch (type)
		{

		case RealEngine::ShaderDataType::None:		return 0;
		case RealEngine::ShaderDataType::Float:		return GL_FLOAT;
		case RealEngine::ShaderDataType::Float2:	return GL_FLOAT;
		case RealEngine::ShaderDataType::Float3:	return GL_FLOAT;
		case RealEngine::ShaderDataType::Float4:	return GL_FLOAT;
		case RealEngine::ShaderDataType::Mat3:		return GL_FLOAT;
		case RealEngine::ShaderDataType::Mat4:		return GL_FLOAT;
		case RealEngine::ShaderDataType::Int:		return GL_INT;
		case RealEngine::ShaderDataType::Int2:		return GL_INT;
		case RealEngine::ShaderDataType::Int3:		return GL_INT;
		case RealEngine::ShaderDataType::Int4:		return GL_INT;
		case RealEngine::ShaderDataType::Bool:		return GL_BOOL;

		}
		RE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	};


	OpenGLVertexArray::OpenGLVertexArray() 
	{
		
		glCreateVertexArrays(1, &m_RendererID);
	}	
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{

		RE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToGLBaseType(element.Type), element.Normalize ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);

	}
	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}