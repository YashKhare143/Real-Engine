#include "repch.h"
#include "stb_image.h"
#include "OpenGLTexture.h"

namespace RealEngine {
	OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height) : m_Width(width), m_Height(height)
	{
		RE_PROFILE_FUNC();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
	{
		RE_PROFILE_FUNC();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;

		{
			RE_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		RE_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		GLenum InternalFormat = 0, DataFormat = 0;
		switch (channels)
		{
			case 3:	InternalFormat = GL_RGB8;
					DataFormat = GL_RGB;

			case 4: InternalFormat = GL_RGBA8;
					DataFormat = GL_RGBA;
		}

		RE_CORE_ASSERT(InternalFormat & DataFormat, "Format not supported!");
		m_InternalFormat = InternalFormat;
		m_DataFormat = DataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data); 
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		RE_PROFILE_FUNC();

		glDeleteTextures(1, &m_RendererID);
	}
	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		RE_PROFILE_FUNC();

		uint32_t BytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
		RE_CORE_ASSERT(size == m_Width * m_Height * BytesPerPixel, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		RE_PROFILE_FUNC();

		glBindTextureUnit(slot, m_RendererID);
	}
}