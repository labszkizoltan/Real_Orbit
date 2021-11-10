#include "Depthbuffer.h"

#include "Renderer.h" // included for opengl error checks

#include "glad/glad.h"

static const uint32_t s_MaxDepthbufferSize = 8192;

Depthbuffer::Depthbuffer(const FrameBufferSpecification& spec)
	: m_Specification(spec)
{
	Invalidate();
}

Depthbuffer::~Depthbuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteTextures(1, &m_DepthAttachment->m_RendererID);
	// GLCall(glDeleteFramebuffers(1, &m_RendererID));
	// GLCall(glDeleteTextures(1, &m_DepthAttachment->m_RendererID));
}

void Depthbuffer::Invalidate()
{
	// delete current content
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_DepthAttachment->m_RendererID);
	}

	// create the FBO
	glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	// create the depth attachment
	TextureSpecifications depthSpec;
	depthSpec.Width = m_Specification.Width;
	depthSpec.Height = m_Specification.Height;
	depthSpec.InternalFormat = GL_DEPTH_COMPONENT32F; // GL_DEPTH24_STENCIL8;
//	depthSpec.DataFormat = GL_DEPTH24_STENCIL8; // i think this is not used
	m_DepthAttachment = std::shared_ptr<Texture>(new Texture(depthSpec));


	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment->m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment->m_RendererID);
	glTexStorage2D(GL_TEXTURE_2D, 1, depthSpec.InternalFormat, m_Specification.Width, m_Specification.Height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment->m_RendererID, 0);
	
	// we explicitly tell OpenGL we're not going to render any color data
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // it's important to unbind the frambuffer, because if a random framebuffer remains boud, it can interfere with the rendering
}

void Depthbuffer::Bind()
{
	glGetIntegerv(GL_VIEWPORT, m_ViewPortBefore);

	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glViewport(0, 0, m_Specification.Width, m_Specification.Height); // before adding this line, the rotation center wasnt in the middle of the imgui window
}

void Depthbuffer::Unbind()
{
	UnbindAll();
	glViewport(m_ViewPortBefore[0], m_ViewPortBefore[1], m_ViewPortBefore[2], m_ViewPortBefore[3]);
}

void Depthbuffer::UnbindAll()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<Texture> Depthbuffer::GetDepthAttachment()
{
	return m_DepthAttachment;
}
