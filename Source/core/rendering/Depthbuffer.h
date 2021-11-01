
#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

#include <stdint.h> // withoud including this, the uint32_t type is not recognized
#include "Texture.h"

class Depthbuffer
{
public:
	Depthbuffer(const TextureSpecifications& spec);
	~Depthbuffer();

	void Invalidate();

	void Bind();
	void Unbind();

	//	void Resize(uint32_t width, uint32_t height); // probably not going to deal with this now

	uint32_t GetTextureID() const { return m_DepthAttachment; }

	const TextureSpecifications& GetSpecification() const { return m_Specification; }

private:
	uint32_t m_RendererID = 0;
	uint32_t m_DepthAttachment = 0;
	TextureSpecifications m_Specification;


friend class Texture;

};




#endif // DEPTHBUFFER_H
