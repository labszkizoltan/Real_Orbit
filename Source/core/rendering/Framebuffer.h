
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h> // withoud including this, the uint32_t type is not recognized
#include <vector>
#include <memory>
#include "Texture.h"


struct FrameBufferSpecification
{
	uint32_t Width, Height;
	uint32_t Samples = 1;
	bool HasDepthBuffer = true;
	uint32_t ColourAttachmentCount = 1;

	bool SwapChainTarget = false; // this means sg like this (unbind all framebuffers and render to the screen): glBindFramebuffer(0);
};

class Framebuffer
{
public:
	Framebuffer(const FrameBufferSpecification& spec);
	~Framebuffer();

	void Invalidate();

	void Bind();
	void Unbind();

	// unbind all framebuffers or binding the default frame buffer
	static void UnbindAll();

	// get the textures:
	std::shared_ptr<Texture> GetColorAttachment(uint32_t idx = 0);
	std::shared_ptr<Texture> GetDepthAttachment();

	//	void Resize(uint32_t width, uint32_t height); // probably not going to deal with this now

	uint32_t GetTextureID(uint32_t idx) const { return m_ColorAttachments[idx]->GetRendererID(); }

	const FrameBufferSpecification& GetSpecification() const { return m_Specification; }

private:
	GLint m_ViewPortBefore[4];
	uint32_t m_RendererID = 0;
	FrameBufferSpecification m_Specification;
	std::vector<std::shared_ptr<Texture>> m_ColorAttachments;
	std::shared_ptr<Texture> m_DepthAttachment;

};


#endif // FRAMEBUFFER_H
