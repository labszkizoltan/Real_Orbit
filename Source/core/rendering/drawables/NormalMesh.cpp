
#include "NormalMesh.h"

#include <glad/glad.h>
#include <vendor/stb_image/stb_image.h>

BufferLayout NormalMesh::s_Layout = {
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aSurfaceNormal"},
		{ShaderDataType::Float2, "aTexCoord"}
};

NormalMesh::NormalMesh()
{
}

NormalMesh::NormalMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, std::shared_ptr<Texture> texture)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexData[0], vertexData.size() * sizeof(float)),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size()),
	m_Texture(texture)
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_Layout);
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

NormalMesh::NormalMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, const std::string& texturePath)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexData[0], vertexData.size() * sizeof(float)),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size()),
	m_Texture(new Texture(texturePath))
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_Layout);
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

NormalMesh::~NormalMesh()
{
}

void NormalMesh::Draw()
{
	m_VertexArray.Bind();
	m_Texture->Bind();

	glDrawElements(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

MeshType NormalMesh::GetMeshType()
{
	return GetStaticMeshType();
}

BufferLayout NormalMesh::GetBufferLayout()
{
	return s_Layout;
}

MeshType NormalMesh::GetStaticMeshType()
{
	return MeshType::NORMAL_MESH;
}
