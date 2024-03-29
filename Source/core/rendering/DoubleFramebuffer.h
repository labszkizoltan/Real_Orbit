
#ifndef DOUBLE_FRAMEBUFFER_H
#define DOUBLE_FRAMEBUFFER_H

#include "Framebuffer.h"

#include <stdint.h> // withoud including this, the uint32_t type is not recognized
#include <memory>
#include "Texture.h"
/*
struct FrameBufferSpecification
{
	uint32_t Width, Height;
	uint32_t Samples = 1;

	bool SwapChainTarget = false; // this means sg like this (unbind all framebuffers and render to the screen): glBindFramebuffer(0);
};
*/

class DoubleFramebuffer
{
public:
	DoubleFramebuffer(const FrameBufferSpecification& spec);
	~DoubleFramebuffer();

	void Invalidate();

	void Bind();
	void Unbind();

	// unbind all framebuffers or binding the default frame buffer
	static void UnbindAll();

	// get the textures:
	std::shared_ptr<Texture> GetColorAttachment();
	std::shared_ptr<Texture> GetBrightColorAttachment();
	std::shared_ptr<Texture> GetDepthAttachment();

	//	void Resize(uint32_t width, uint32_t height); // probably not going to deal with this now

	uint32_t GetTextureID() const { return m_ColorAttachment->GetRendererID(); }

	const FrameBufferSpecification& GetSpecification() const { return m_Specification; }

private:
	GLint m_ViewPortBefore[4];
	uint32_t m_RendererID = 0;
	FrameBufferSpecification m_Specification;
	std::shared_ptr<Texture> m_ColorAttachment;
	std::shared_ptr<Texture> m_BrightColorAttachment;
	std::shared_ptr<Texture> m_DepthAttachment;

};




#endif // DOUBLE_FRAMEBUFFER_H
