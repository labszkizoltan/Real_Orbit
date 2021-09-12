#include "ShaderLibrary.h"

ShaderLibrary::~ShaderLibrary()
{
	// when the std::vector is destroyed, it should call the destructor on each of its element
	// so I think it is fine to have this empty
}

void ShaderLibrary::AddShader(Shader* shader)
{
	m_Shaders.push_back(shader);
}

void ShaderLibrary::AddShader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	m_Shaders.push_back(new Shader(vertexSrc, fragmentSrc));
}

void ShaderLibrary::SetAspectRatio(float aspectRatio)
{
	for (int i = 0; i < m_Shaders.size(); i++)
	{
		m_Shaders[i]->UploadUniformFloat("rotation_angle", aspectRatio);
	}
}
