#include "FramebufferDisplay.h"


BufferLayout FramebufferDisplay::s_VertexLayout = {
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float2, "aTexCoord"}
};

const std::vector<float> fbDisplay_vertices = {
	-1.0f,	-1.0f,	0.0f,	0.0f,	0.0f,
	 1.0f,	-1.0f,	0.0f,	1.0f,	0.0f,
	-1.0f,	 1.0f,	0.0f,	0.0f,	1.0f,
	 1.0f,	 1.0f,	0.0f,	1.0f,	1.0f
};

const std::vector<uint32_t> fbDisplay_indices = {
	0,	1,	2,
	1,	3,	2
};


const char* fbDisplay_vertexSource =
"#version 460 core\n"

"layout(location = 0) in vec3 aPos; \n"
"layout(location = 1) in vec2 aTexCoord; \n"
"out vec2 texCoord; \n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"	texCoord = aTexCoord;\n"
"}\0";

const char* fbDisplay_fragmentSource =
"#version 460 core\n"
//"layout(location = 0) out vec4 FragColor; \n"
"out vec4 FragColor; \n"
"in vec2 texCoord; \n"
"uniform sampler2D u_Textures[32]; \n"
"uniform float textureSlot; \n"
"void main()\n"
"{\n"
"	FragColor = texture(u_Textures[int(textureSlot)], texCoord); \n"
"}\0";

FramebufferDisplay::FramebufferDisplay()
    : m_VertexArray(),
	m_VertexBuffer((float*)&fbDisplay_vertices[0], fbDisplay_vertices.size() * sizeof(float)),
	m_IndexBuffer((uint32_t*)&fbDisplay_indices[0], fbDisplay_indices.size()),
	m_Texture(nullptr),
	m_Shader(std::make_shared<Shader>(std::string(fbDisplay_vertexSource), std::string(fbDisplay_fragmentSource)))
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();

	
	SetDisplayedSlot(0.0f);
	int samplers[32];
	for (uint32_t i = 0; i < 32; i++) { samplers[i] = i; }
	// shader already bound by the SetDisplayedSlot call
	m_Shader->UploadUniformIntArray("u_Textures", samplers, 32);
}

FramebufferDisplay::~FramebufferDisplay()
{
}

void FramebufferDisplay::SetTexture(std::shared_ptr<Texture> texture)
{
	m_Texture = texture;
	SetDisplayedSlot(m_Texture->GetSlot());
}

void FramebufferDisplay::SetDisplayedSlot(int slot)
{
	m_Shader->Bind();
	m_Shader->UploadUniformFloat("textureSlot", (float)slot);
}

int FramebufferDisplay::GetTextureSlot()
{
    return m_Texture->GetSlot();
}

void FramebufferDisplay::Draw()
{
	m_Shader->Bind();
	m_VertexArray.Bind();
	m_Texture->Bind();

	glDrawElements(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);

	m_VertexArray.UnBind();
}
