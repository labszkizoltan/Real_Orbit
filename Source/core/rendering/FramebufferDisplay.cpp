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


const char* fbDisplay_combined_fragmentSource =
"#version 460 core\n"
//"layout(location = 0) out vec4 FragColor; \n"
"out vec4 FragColor; \n"
"in vec2 texCoord; \n"
"uniform sampler2D u_Textures[32]; \n"
"uniform float textureSlot_1; \n"
"uniform float textureSlot_2; \n"
"void main()\n"
"{\n"
"	vec4 col_1 = texture(u_Textures[int(textureSlot_1)], texCoord); \n"
"	vec4 col_2 = texture(u_Textures[int(textureSlot_2)], texCoord); \n"
//"	vec4 col_1 = texture(u_Textures[8], texCoord); \n"
//"	vec4 col_2 = texture(u_Textures[9], texCoord); \n"
"	FragColor = col_1 + col_2; \n"
"}\0";


FramebufferDisplay::FramebufferDisplay()
    : m_VertexArray(),
	m_VertexBuffer((float*)&fbDisplay_vertices[0], fbDisplay_vertices.size() * sizeof(float)),
	m_IndexBuffer((uint32_t*)&fbDisplay_indices[0], fbDisplay_indices.size()),
	m_Texture(nullptr),
	m_Shader(std::make_shared<Shader>(std::string(fbDisplay_vertexSource), std::string(fbDisplay_fragmentSource))),
	m_CombinedShader(std::make_shared<Shader>(std::string(fbDisplay_vertexSource), std::string(fbDisplay_combined_fragmentSource)))
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
	m_CombinedShader->Bind();
	m_CombinedShader->UploadUniformIntArray("u_Textures", samplers, 32);
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
//	m_Texture->Bind();

	glDrawElements(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);

	m_VertexArray.UnBind();
}

void FramebufferDisplay::DrawCombined(int slot_1, int slot_2)
{
	m_CombinedShader->Bind();
	m_CombinedShader->UploadUniformFloat("textureSlot_1", (float)slot_1);
	m_CombinedShader->UploadUniformFloat("textureSlot_2", (float)slot_2);

	m_VertexArray.Bind();
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
	m_VertexArray.UnBind();
}

/*
void FramebufferDisplay::DrawCombined(std::shared_ptr<Texture> tex_1, std::shared_ptr<Texture> tex_2)
{
	m_VertexArray.Bind();

	m_CombinedShader->Bind();
	m_CombinedShader->UploadUniformFloat("textureSlot_1", (float)tex_1->GetSlot());
	m_CombinedShader->UploadUniformFloat("textureSlot_2", (float)tex_2->GetSlot());

	tex_1->Bind();
	tex_2->Bind();
	
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
	m_VertexArray.UnBind();
}
*/

