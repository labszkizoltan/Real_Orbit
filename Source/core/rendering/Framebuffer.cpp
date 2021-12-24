
#include "Framebuffer.h"
#include "Renderer.h" // needed for the GLCall opengl macro

#include "glad/glad.h"

static const uint32_t s_MaxFrameBufferSize = 8192;

static const GLenum s_FBAttachments[32] = {
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7,
	GL_COLOR_ATTACHMENT8,
	GL_COLOR_ATTACHMENT9,
	GL_COLOR_ATTACHMENT10,
	GL_COLOR_ATTACHMENT11,
	GL_COLOR_ATTACHMENT12,
	GL_COLOR_ATTACHMENT13,
	GL_COLOR_ATTACHMENT14,
	GL_COLOR_ATTACHMENT15,
	GL_COLOR_ATTACHMENT16,
	GL_COLOR_ATTACHMENT17,
	GL_COLOR_ATTACHMENT18,
	GL_COLOR_ATTACHMENT19,
	GL_COLOR_ATTACHMENT20,
	GL_COLOR_ATTACHMENT21,
	GL_COLOR_ATTACHMENT22,
	GL_COLOR_ATTACHMENT23,
	GL_COLOR_ATTACHMENT24,
	GL_COLOR_ATTACHMENT25,
	GL_COLOR_ATTACHMENT26,
	GL_COLOR_ATTACHMENT27,
	GL_COLOR_ATTACHMENT28,
	GL_COLOR_ATTACHMENT29,
	GL_COLOR_ATTACHMENT30,
	GL_COLOR_ATTACHMENT31
};


Framebuffer::Framebuffer(const FrameBufferSpecification& spec)
	: m_Specification(spec)
{
	Invalidate();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
	for (int i = 0; i < m_ColorAttachments.size(); i++)
		glDeleteTextures(1, &m_ColorAttachments[i]->m_RendererID);
	m_ColorAttachments.clear();
	glDeleteTextures(1, &m_DepthAttachment->m_RendererID);
}

void Framebuffer::Invalidate()
{
	glGetIntegerv(GL_VIEWPORT, m_ViewPortBefore);

	// delete current content
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		for(int i=0; i< m_ColorAttachments.size(); i++)
			glDeleteTextures(1, &m_ColorAttachments[i]->m_RendererID);
		m_ColorAttachments.clear();
		glDeleteTextures(1, &m_DepthAttachment->m_RendererID);
	}

	// create the FBO
	GLCall(glCreateFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

	// create the color attachment
	/*
	TextureSpecifications colorSpec;
	colorSpec.Width = m_Specification.Width;
	colorSpec.Height = m_Specification.Height;
	colorSpec.InternalFormat = GL_RGBA8;
	colorSpec.DataFormat = GL_RGBA;
	m_ColorAttachment = std::make_shared<Texture>(colorSpec);

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment->m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment->m_RendererID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); // GL_LINEAR or GL_NEAREST
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment->m_RendererID, 0));
	*/


	// create the color attachments
	TextureSpecifications colorSpec;
	colorSpec.Width = m_Specification.Width;
	colorSpec.Height = m_Specification.Height;
	colorSpec.InternalFormat = GL_RGBA8;
	colorSpec.DataFormat = GL_RGBA;
	m_ColorAttachments = std::vector<std::shared_ptr<Texture>>(m_Specification.ColourAttachmentCount);

	for (int i = 0; i < m_Specification.ColourAttachmentCount; i++)
	{
//		m_ColorAttachments.push_back(std::make_shared<Texture>(colorSpec));
		m_ColorAttachments[i] = (std::make_shared<Texture>(colorSpec));
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachments[i]->m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i]->m_RendererID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); // GL_LINEAR or GL_NEAREST
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, m_ColorAttachments[i]->m_RendererID, 0));
	}

	// create the depth attachment
	if (m_Specification.HasDepthBuffer)
	{
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
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // it's important to unbind the frambuffer, because if a random framebuffer remains boud, it can interfere with the rendering
}

void Framebuffer::Bind()
{
	glGetIntegerv(GL_VIEWPORT, m_ViewPortBefore);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

//	static const GLenum attachment = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(m_Specification.ColourAttachmentCount, &s_FBAttachments[0]);

	glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void Framebuffer::Unbind()
{
	UnbindAll();
	glViewport(m_ViewPortBefore[0], m_ViewPortBefore[1], m_ViewPortBefore[2], m_ViewPortBefore[3]);
}

void Framebuffer::UnbindAll()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<Texture> Framebuffer::GetColorAttachment(uint32_t idx)
{
	return m_ColorAttachments[idx];
}

std::shared_ptr<Texture> Framebuffer::GetDepthAttachment()
{
	return m_DepthAttachment;
}
