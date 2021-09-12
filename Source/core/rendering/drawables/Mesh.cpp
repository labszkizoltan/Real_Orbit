#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh()
{
}

Mesh::Mesh(const std::vector<Vec3D>& vertexAndColorData, const std::vector<uint32_t>& indexData)
	: m_VertexArray(), m_VertexBuffer((float*)&vertexAndColorData[0], vertexAndColorData.size() * sizeof(Vec3D)), m_IndexBuffer((uint32_t*)&indexData[0], indexData.size())
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aColor"}
		});
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

//Mesh::Mesh(const std::string& filename)
//{
//}

Mesh::~Mesh()
{
}

void Mesh::Draw()
{
	m_VertexArray.Bind();
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.m_Count, GL_UNSIGNED_INT, nullptr);
}
