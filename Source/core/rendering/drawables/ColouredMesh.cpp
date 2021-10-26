#include "ColouredMesh.h"

#include <glad/glad.h>

BufferLayout ColouredMesh::s_Layout = {
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aColor"}
};

ColouredMesh::ColouredMesh()
{
}

ColouredMesh::ColouredMesh(const std::vector<Vec3D>& vertexAndColorData, const std::vector<uint32_t>& indexData)
	: m_VertexArray(), m_VertexBuffer((float*)&vertexAndColorData[0], vertexAndColorData.size() * sizeof(Vec3D)), m_IndexBuffer((uint32_t*)&indexData[0], indexData.size())
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_Layout);
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

//ColouredMesh::ColouredMesh(const std::string& filename)
//{
//}

ColouredMesh::~ColouredMesh()
{
}

void ColouredMesh::Draw()
{
	m_VertexArray.Bind();
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.m_Count, GL_UNSIGNED_INT, nullptr);
}

MeshType ColouredMesh::GetMeshType()
{
	return GetStaticMeshType();
}

BufferLayout ColouredMesh::GetBufferLayout()
{
	return s_Layout;
}

MeshType ColouredMesh::GetStaticMeshType()
{
	return MeshType::COLOURED_MESH;
}
