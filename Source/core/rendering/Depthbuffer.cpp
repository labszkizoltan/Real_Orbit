#include "Depthbuffer.h"

#include "glad/glad.h"

static const uint32_t s_MaxDepthbufferSize = 8192;

Depthbuffer::Depthbuffer(const TextureSpecifications& spec)
	: m_Specification(spec)
{
	Invalidate();
}

Depthbuffer::~Depthbuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteTextures(1, &m_DepthAttachment);
}

void Depthbuffer::Invalidate()
{
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
//		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

//	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
//	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
	
	// we explicitly tell OpenGL we're not going to render any color data
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // it's important to unbind the frambuffer, because if a random framebuffer remains boud, it can interfere with the rendering
}

void Depthbuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glViewport(0, 0, m_Specification.Width, m_Specification.Height); // before adding this line, the rotation center wasnt in the middle of the imgui window
}

void Depthbuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
