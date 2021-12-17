
#ifndef FRAMEBUFFER_DISPLAY_H
#define FRAMEBUFFER_DISPLAY_H

#include <core/rendering/VertexArray.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/Texture.h>
#include <core/rendering/Shader.h>

class FramebufferDisplay
{
public:
	FramebufferDisplay();
	~FramebufferDisplay();

	void SetTexture(std::shared_ptr<Texture> texture);
	void SetTextureSlot(int slot);
	int GetTextureSlot();
	void Draw();


private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	std::shared_ptr<Texture> m_Texture = nullptr;
	std::shared_ptr<Shader> m_Shader = nullptr;

};








#endif // FRAMEBUFFER_DISPLAY_H
