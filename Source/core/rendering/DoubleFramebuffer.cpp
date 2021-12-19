
#include "DoubleFramebuffer.h"
#include "Renderer.h" // needed for the GLCall opengl macro

#include "glad/glad.h"

static const uint32_t s_MaxFrameBufferSize = 8192;

DoubleFramebuffer::DoubleFramebuffer(const FrameBufferSpecification& spec)
	: m_Specification(spec)
{
	Invalidate();
}

DoubleFramebuffer::~DoubleFramebuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteTextures(1, &m_ColorAttachment->m_RendererID);
	glDeleteTextures(1, &m_BrightColorAttachment->m_RendererID);
	glDeleteTextures(1, &m_DepthAttachment->m_RendererID);
}

void DoubleFramebuffer::Invalidate()
{
	glGetIntegerv(GL_VIEWPORT, m_ViewPortBefore);

	// delete current content
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment->m_RendererID);
		glDeleteTextures(1, &m_BrightColorAttachment->m_RendererID);
		glDeleteTextures(1, &m_DepthAttachment->m_RendererID);
	}

	// create the FBO
	GLCall(glCreateFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

	// create the color attachment
	TextureSpecifications colorSpec;
	colorSpec.Width = m_Specification.Width;
	colorSpec.Height = m_Specification.Height;
	colorSpec.InternalFormat = GL_RGBA8;
	colorSpec.DataFormat = GL_RGBA;
	m_ColorAttachment = std::make_shared<Texture>(colorSpec);

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment->m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment->m_RendererID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment->m_RendererID, 0));

	// create the bright color attachment
	m_BrightColorAttachment = std::make_shared<Texture>(colorSpec);

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_BrightColorAttachment->m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_BrightColorAttachment->m_RendererID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_BrightColorAttachment->m_RendererID, 0));

	// create the depth attachment
	TextureSpecifications depthSpec;
	depthSpec.Width = m_Specification.Width;
	depthSpec.Height = m_Specification.Height;
	depthSpec.InternalFormat = GL_DEPTH24_STENCIL8;
	depthSpec.DataFormat = GL_DEPTH24_STENCIL8; // i think this is not used
	m_DepthAttachment = std::make_shared<Texture>(depthSpec);

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment->m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthAttachment->m_RendererID));
	GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, depthSpec.InternalFormat, m_Specification.Width, m_Specification.Height));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment->m_RendererID, 0));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // it's important to unbind the frambuffer, because if a random framebuffer remains boud, it can interfere with the rendering
}

void DoubleFramebuffer::Bind()
{
	glGetIntegerv(GL_VIEWPORT, m_ViewPortBefore);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	static const unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void DoubleFramebuffer::Unbind()
{
	UnbindAll();
	glViewport(m_ViewPortBefore[0], m_ViewPortBefore[1], m_ViewPortBefore[2], m_ViewPortBefore[3]);
}

void DoubleFramebuffer::UnbindAll()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<Texture> DoubleFramebuffer::GetColorAttachment()
{
	return m_ColorAttachment;
}

std::shared_ptr<Texture> DoubleFramebuffer::GetBrightColorAttachment()
{
	return m_BrightColorAttachment;
}

std::shared_ptr<Texture> DoubleFramebuffer::GetDepthAttachment()
{
	return m_DepthAttachment;
}
