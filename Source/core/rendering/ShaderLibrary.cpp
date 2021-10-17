#include "ShaderLibrary.h"

ShaderLibrary::~ShaderLibrary()
{
	// when the std::vector is destroyed, it should call the destructor on each of its element
	// so I think it is fine to have this empty
	// -> update, now I dont think it's fine, so I explicitely call the destructors, but as I read about the topic,
	// this is bad : https://stackoverflow.com/questions/14187006/is-calling-destructor-manually-always-a-sign-of-bad-design
	// -> update, I changed all raw Shader pointers to shared pointers, and now the destructor is called automatically
	// so there is no need for the function body below

//	for (int i = 0; i < m_Shaders.size(); i++)
//	{
//		m_Shaders[i]->~Shader();
//	}
}

void ShaderLibrary::AddShader(std::shared_ptr<Shader> shader)
{
	m_Shaders.push_back(shader);
}

void ShaderLibrary::AddShader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
//	m_Shaders.push_back(std::shared_ptr<Shader>(new Shader(vertexSrc, fragmentSrc)));
	m_Shaders.push_back(std::make_shared<Shader>(vertexSrc, fragmentSrc));
}

void ShaderLibrary::SetAspectRatio(float aspectRatio)
{
	for (int i = 0; i < m_Shaders.size(); i++)
	{
		m_Shaders[i]->Bind();
		m_Shaders[i]->UploadUniformFloat("aspect_ratio", aspectRatio);
	}
	m_LastBoundShader = m_Shaders[m_Shaders.size() - 1];
}

void ShaderLibrary::SetCamera(TransformComponent camera_transform)
{
	for (int i = 0; i < m_Shaders.size(); i++)
	{
		m_Shaders[i]->Bind();
		m_Shaders[i]->UploadUniformFloat3("camera_location", camera_transform.location.Glm());
		m_Shaders[i]->UploadUniformMat3("camera_orientation", camera_transform.orientation.Glm());
	}
	m_LastBoundShader = m_Shaders[m_Shaders.size()-1];
}

void ShaderLibrary::SetZoomLevel(float zoom_level)
{
	for (int i = 0; i < m_Shaders.size(); i++)
	{
		m_Shaders[i]->Bind();
		m_Shaders[i]->UploadUniformFloat("zoom_level", zoom_level);
	}
	m_LastBoundShader = m_Shaders[m_Shaders.size() - 1];
}

std::shared_ptr<Shader> ShaderLibrary::BindShader(MeshType meshType)
{
	switch (meshType)
	{
	case MeshType::COLOURED_MESH:	m_Shaders[0]->Bind(); m_LastBoundShader = m_Shaders[0]; return m_LastBoundShader;
	}

	LOG_CORE_INFO("MeshType not recognized, cannot bind shader");
	m_LastBoundShader = nullptr;
	return nullptr;
}

std::shared_ptr<Shader> ShaderLibrary::GetLastBoundShader()
{
	return m_LastBoundShader;
}
