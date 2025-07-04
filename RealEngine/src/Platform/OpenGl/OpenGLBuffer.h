#pragma once
#include "RealEngine/Renderer/Buffer.h"

namespace RealEngine {

	class	OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;


		virtual void SetData(const void* data, uint32_t size) override;
		virtual inline const BufferLayout& GetLayout() const override { return m_Layout; };
		virtual inline void SetLayour(const BufferLayout& layout) override { m_Layout = layout; };

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;

	};

	class	OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const { return m_Count; };
	private:

		uint32_t m_RendererID;
		uint32_t m_Count;

	};

}
