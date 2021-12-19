
#include "ImageProcessor.h"

#include <vector>

#include <core/rendering/VertexArray.h>
// #include <core/rendering/Texture.h>


//create the vertex layout
BufferLayout ImageProcessor::s_VertexLayout = {
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float2, "aTexCoord"}
};

// create the vertex/index buffers
const std::vector<float> imgProc_vertices = {
	-1.0f,	-1.0f,	0.0f,	0.0f,	0.0f,
	 1.0f,	-1.0f,	0.0f,	1.0f,	0.0f,
	-1.0f,	 1.0f,	0.0f,	0.0f,	1.0f,
	 1.0f,	 1.0f,	0.0f,	1.0f,	1.0f
};

const std::vector<uint32_t> imgProc_indices = {
	0,	1,	2,
	1,	3,	2
};

// create the sahders
const char* imgProc_vertexSource =
"#version 460 core\n"

"layout(location = 0) in vec3 aPos; \n"
"layout(location = 1) in vec2 aTexCoord; \n"
"out vec2 texCoord; \n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"	texCoord = aTexCoord;\n"
"}\0";

const char* imgProc_blur_fragmentSource =
"#version 460 core\n"
"layout(location = 0) out vec4 FragColor; \n"
//"out vec4 FragColor; \n"
"in vec2 texCoord; \n"
"uniform sampler2D u_Textures[32]; \n"
"uniform float textureSlot; \n"
//"uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216); \n"
//"uniform float weight[11] = float[](1.0, 0.71653131, 0.51341712, 0.36787944, 0.26359714, 0.18887560, 0.13533528, 0.09697197, 0.06948345, 0.04978707, 0.03567399); \n" // unnormalized
"uniform float weight[11] = float[](0.29090466, 0.20844230, 0.14935543, 0.10701784, 0.07668164, 0.05494479, 0.03936966, 0.02820960, 0.02021306, 0.01448329, 0.01037773); \n" // sum = 1
//"uniform float weight[11] = float[](0.072726165, 0.052110574, 0.037338858, 0.026754461, 0.019170409, 0.013736198, 0.009842416, 0.007052399, 0.005053265, 0.003620823, 0.002594432); \n" // sum = 1/4

"void main()\n"
"{\n"
"	vec2 tex_offset = 1.0 / textureSize(u_Textures[int(textureSlot)], 0); \n"  // gets size of single texel
"	vec3 result = texture(u_Textures[int(textureSlot)], texCoord).rgb * weight[0]; \n"  // current fragment's contribution
"	for (int i = 1; i < 11; ++i)\n"
"	{\n"
"		result += texture(u_Textures[int(textureSlot)], texCoord + vec2(tex_offset.x * 2*i, 0.0)).rgb * weight[i];\n"
"		result += texture(u_Textures[int(textureSlot)], texCoord - vec2(tex_offset.x * 2*i, 0.0)).rgb * weight[i];\n"
"		result += texture(u_Textures[int(textureSlot)], texCoord + vec2(0.0, tex_offset.y * 2*i)).rgb * weight[i];\n"
"		result += texture(u_Textures[int(textureSlot)], texCoord - vec2(0.0, tex_offset.y * 2*i)).rgb * weight[i];\n"
"	}\n"
//"	FragColor = texture(u_Textures[int(textureSlot)], texCoord)/2; \n"
//"	FragColor = vec4(1.0,0,0,0); \n"
"	FragColor = vec4(result, 1.0); \n"
"}\0";


ImageProcessor::ImageProcessor()
	: m_VertexArray(),
	m_VertexBuffer((float*)&imgProc_vertices[0], imgProc_vertices.size() * sizeof(float)),
	m_IndexBuffer((uint32_t*)&imgProc_indices[0], imgProc_indices.size()),
	m_BlurShader(std::make_unique<Shader>(std::string(imgProc_vertexSource), std::string(imgProc_blur_fragmentSource)))
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();

//	m_BlurShader = std::make_unique<Shader>(std::string(imgProc_vertexSource), std::string(imgProc_blur_fragmentSource));

	SetInputSlot(0.0f);
	int samplers[32];
	for (uint32_t i = 0; i < 32; i++) { samplers[i] = i; }
	// shader already bound by the SetInputSlot call
	m_BlurShader->UploadUniformIntArray("u_Textures", samplers, 32);
}

/*
int ImageProcessor::Init()
{
	s_BlurShader->Bind();

	int samplers[32];
	for (uint32_t i = 0; i < 32; i++) { samplers[i] = i; }
	// shader already bound by the SetDisplayedSlot call
	s_BlurShader->UploadUniformIntArray("u_Textures", samplers, 32);

	SetInputSlot(0.0f);

	return 1;
}
*/

void ImageProcessor::Blur(int inputSlot, std::shared_ptr<Framebuffer> outputFramebuffer)
{
	outputFramebuffer->Bind();
//	m_BlurShader->Bind();
	SetInputSlot(inputSlot);

	m_VertexArray.Bind();
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
	m_VertexArray.UnBind();

	outputFramebuffer->Unbind();
}

void ImageProcessor::SetInputSlot(int slot)
{
	m_BlurShader->Bind();
	m_BlurShader->UploadUniformFloat("textureSlot", (float)slot);
}
