
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>

struct TextureSpecifications
{
	uint32_t Width, Height;
	GLenum InternalFormat, DataFormat;
};

class Texture
{
public:
	Texture() = default;
	Texture(uint32_t width, uint32_t height);
	Texture(TextureSpecifications spec);
	Texture(const std::string path);
	~Texture();

	uint32_t GetWidth() const { return m_Specification.Width; }
	uint32_t GetHeight() const { return m_Specification.Height; }
	uint32_t GetRendererID() const { return m_RendererID; };

	void SetSlot(uint32_t slot) { m_Slot = slot; }
	void SetData(void* data, uint32_t size);

	void Bind() const;

	bool operator==(const Texture& other) const { return m_RendererID == ((Texture&)other).m_RendererID; };

private:
	TextureSpecifications m_Specification;
	uint32_t m_Slot = 0;
	uint32_t m_RendererID = 0;


	friend class Framebuffer;

};

#endif // TEXTURE_H
