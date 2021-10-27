
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>

class Texture
{
public:
	Texture() = default;
	Texture(uint32_t width, uint32_t height);
	Texture(const std::string path);
	~Texture();

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	uint32_t GetRendererID() const { return m_RendererID; };

	void SetSlot(uint32_t slot) { m_Slot = slot; }
	void SetData(void* data, uint32_t size);

	void Bind() const;

	bool operator==(const Texture& other) const { return m_RendererID == ((Texture&)other).m_RendererID; };

private:
	uint32_t m_Width;
	uint32_t m_Height;
	uint32_t m_Slot = 0;
	uint32_t m_RendererID = 0;
	GLenum m_InternalFormat, m_DataFormat;

};

#endif // TEXTURE_H
