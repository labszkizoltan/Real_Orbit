
#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <core/rendering/Framebuffer.h>
#include <core/rendering/VertexArray.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/Shader.h>




class ImageProcessor
{
public:
	ImageProcessor();
	~ImageProcessor() = default;

//	static int Init();

	void Blur(int inputSlot, std::shared_ptr<Framebuffer> outputFramebuffer);
	void SetMipMapLevel(int level);

private:
	void SetInputSlot(int slot);

private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	std::unique_ptr<Shader> m_BlurShader;

	static BufferLayout s_VertexLayout;
//	static std::unique_ptr<Shader> s_BlurShader;

	

};

#endif // IMAGE_PROCESSOR_H
