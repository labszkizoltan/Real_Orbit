
#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

#include <stdint.h> // withoud including this, the uint32_t type is not recognized
//#include "Framebuffer.h"
#include "DoubleFramebuffer.h"
#include "Texture.h"

class Depthbuffer
{
public:
	Depthbuffer(const FrameBufferSpecification& spec);
	~Depthbuffer();

	void Invalidate();

	void Bind();
	void Unbind();

	// unbind all framebuffers or binding the default frame buffer
	static void UnbindAll();

	//	void Resize(uint32_t width, uint32_t height);

	// get the textures:
	std::shared_ptr<Texture> GetDepthAttachment();

	uint32_t GetTextureID() const { return m_DepthAttachment->GetRendererID(); }

	const FrameBufferSpecification& GetSpecification() const { return m_Specification; }

private:
	GLint m_ViewPortBefore[4];
	uint32_t m_RendererID = 0;
	FrameBufferSpecification m_Specification;
	std::shared_ptr<Texture> m_DepthAttachment;


friend class Texture;

};




#endif // DEPTHBUFFER_H
