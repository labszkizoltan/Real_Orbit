#include "AlphaMesh.h"

#include <glad/glad.h>
#include <core/rendering/Renderer.h> // needed for the GLCall macro
#include <core/scene/CoreComponents.h>

BufferLayout AlphaMesh::s_VertexLayout = {
		{ShaderDataType::Float3, "aPos"}
};

AlphaMesh::AlphaMesh()
{
}

AlphaMesh::AlphaMesh(const std::vector<Vec3D>& vertexData, const std::vector<uint32_t>& indexData)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexData[0], vertexData.size() * sizeof(Vec3D)),
	m_InstanceBuffer(0, 3),
	m_ColourInstanceBuffer(0, 8), // other buffers already use the first 7 vertex attribute binding points
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size())
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_InstanceBuffer.SetLayout();
	m_ColourInstanceBuffer.SetLayout();
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

AlphaMesh::AlphaMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexData[0], vertexData.size() * sizeof(float)),
	m_InstanceBuffer(0, 3),
	m_ColourInstanceBuffer(0, 8),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size())
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_InstanceBuffer.SetLayout();
	m_ColourInstanceBuffer.SetLayout();
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

AlphaMesh::~AlphaMesh()
{
}

void AlphaMesh::Draw()
{
	// turn off writing to the depth buffer
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);

	m_VertexArray.Bind();
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, 0, m_InstanceBuffer.GetElementCount()));
	m_VertexArray.UnBind();

	// turn back on again
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void AlphaMesh::SetInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
}

void AlphaMesh::SetColourInstances(const std::vector<ColourComponent>& colours)
{
	m_ColourInstanceBuffer.SetData(colours);
}

// this is not good, the colour instances are not set
void AlphaMesh::DrawInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
	Draw();
}

bool AlphaMesh::HasColourInstances()
{
	return true;
}

MeshType AlphaMesh::GetMeshType()
{
	return GetStaticMeshType();
}

BufferLayout AlphaMesh::GetVertexLayout()
{
	return s_VertexLayout;
}

BufferLayout AlphaMesh::GetInstanceLayout()
{
	return m_InstanceBuffer.GetLayout();
}

MeshType AlphaMesh::GetStaticMeshType()
{
	return MeshType::ALPHA_MESH;
}
