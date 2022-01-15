#include "MarkerMesh.h"

#include <glad/glad.h>
#include <core/rendering/Renderer.h> // needed for the GLCall macro
#include <core/scene/CoreComponents.h>

BufferLayout MarkerMesh::s_VertexLayout = {
		{ShaderDataType::Float3, "aPos"}
};

std::vector<Vec3D> MarkerMesh::s_DefaultMarkerVertices = { {0,0,0}, {-0.05,-0.03,0}, {0.05,-0.03,0} };
// std::vector<Vec3D> MarkerMesh::s_DefaultMarkerVertices = { {0.0f, -0.005f, 0.0f}, {-0.05f, -0.035f, 0.0f}, {0.05f, -0.035f, 0.0f} };
std::vector<uint32_t> MarkerMesh::s_DefaultMarkerIndices = { 0,2,1 };

MarkerMesh::MarkerMesh()
	: m_VertexArray(),
	m_VertexBuffer((float*)&s_DefaultMarkerVertices[0], s_DefaultMarkerVertices.size() * sizeof(Vec3D)),
	m_InstanceBuffer(0, 3),
	m_ColourInstanceBuffer(0, 8), // other buffers already use the first 7 vertex attribute binding points
	m_IndexBuffer((uint32_t*)&s_DefaultMarkerIndices[0], s_DefaultMarkerIndices.size())
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_InstanceBuffer.SetLayout();
	m_ColourInstanceBuffer.SetLayout();
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

MarkerMesh::MarkerMesh(const std::vector<Vec3D>& vertexData, const std::vector<uint32_t>& indexData)
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

MarkerMesh::MarkerMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData)
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

MarkerMesh::~MarkerMesh()
{
}

void MarkerMesh::Draw()
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

void MarkerMesh::SetInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
}

void MarkerMesh::SetColourInstances(const std::vector<ColourComponent>& colours)
{
	m_ColourInstanceBuffer.SetData(colours);
}

void MarkerMesh::DrawInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
	Draw();
}

int MarkerMesh::GetColourInstances()
{
	return m_ColourBufferIndex;
}

MeshType MarkerMesh::GetMeshType()
{
	return GetStaticMeshType();
}

BufferLayout MarkerMesh::GetVertexLayout()
{
	return s_VertexLayout;
}

BufferLayout MarkerMesh::GetInstanceLayout()
{
	return m_InstanceBuffer.GetLayout();
}

MeshType MarkerMesh::GetStaticMeshType()
{
	return MeshType::MARKER_MESH;
}

void MarkerMesh::SetColourBufferIndex(int idx)
{
	m_ColourBufferIndex = idx;
}
