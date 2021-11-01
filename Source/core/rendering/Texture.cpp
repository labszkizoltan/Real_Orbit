
#include "Texture.h"
#include <iostream>
#include <vendor/stb_image/stb_image.h>

Texture::Texture(uint32_t width, uint32_t height)
{
	m_Specification.Width = width;
	m_Specification.Height = height;
	m_Specification.InternalFormat = GL_RGBA8;
	m_Specification.DataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, m_Specification.InternalFormat, m_Specification.Width, m_Specification.Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(TextureSpecifications spec)
	: m_Specification(spec)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, m_Specification.InternalFormat, m_Specification.Width, m_Specification.Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


Texture::Texture(const std::string path)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = nullptr;
	{
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	}
	if (!data) { std::cout << "Failed to load image: " << path; }

	m_Specification.Height = height;
	m_Specification.Width = width;

	GLenum internalFormat = 0, dataFormat = 0;
	if (channels == 4)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels == 3)
	{
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	m_Specification.InternalFormat = internalFormat;
	m_Specification.DataFormat = dataFormat;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // flags: GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::SetData(void* data, uint32_t size)
{
	uint32_t bytesPerPixel = m_Specification.DataFormat == GL_RGBA ? 4 : 3;
	if (size != m_Specification.Width * m_Specification.Height * bytesPerPixel) { std::cout << "Texture::SetData() : size is incompatible with data dimensions!\n"; }
	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specification.Width, m_Specification.Height, m_Specification.DataFormat, GL_UNSIGNED_BYTE, data);
}

void Texture::Bind() const
{
	glBindTextureUnit(m_Slot, m_RendererID);
}


