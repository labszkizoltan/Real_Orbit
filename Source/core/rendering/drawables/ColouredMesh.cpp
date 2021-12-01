#include "ColouredMesh.h"

#include <glad/glad.h>
#include <fstream>

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

ColouredMesh::ColouredMesh(const std::vector<float>& vertexAndColorData, const std::vector<uint32_t>& indexData)
	: m_VertexArray(), m_VertexBuffer((float*)&vertexAndColorData[0], vertexAndColorData.size() * sizeof(float)), m_IndexBuffer((uint32_t*)&indexData[0], indexData.size())
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_Layout);
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

/*
ColouredMesh::ColouredMesh(const std::string& filename)
{
	std::ifstream myfile(filename.c_str());
	int v_count = 0, i_count = 0;

	myfile >> v_count;
	myfile >> i_count;

	// one vertex consist of 3 spatial coordiantes, and 3 rgb colour components, thats why there is a multiplier of 6
	std::vector<float> vertices(v_count*6);
	std::vector<uint32_t> indices(i_count);

	for (int i = 0; i < (v_count*6); i++)
		myfile >> vertices[i];

	for (int i = 0; i < i_count; i++)
		myfile >> indices[i];

	myfile.close();
}
*/

ColouredMesh::~ColouredMesh()
{
}

void ColouredMesh::Draw()
{
	m_VertexArray.Bind();
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
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
