
#include "BrightColouredMesh.h"

#include <glad/glad.h>
#include <core/rendering/Renderer.h> // needed for the GLCall macro
// #include <core/rendering/Buffer.h>
#include <core/scene/CoreComponents.h> // needed for TransformComponent definition

BufferLayout BrightColouredMesh::s_VertexLayout = {
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aColor"}
};

BrightColouredMesh::BrightColouredMesh()
{
}

BrightColouredMesh::BrightColouredMesh(const std::vector<Vec3D>& vertexAndColorData, const std::vector<uint32_t>& indexData)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexAndColorData[0], vertexAndColorData.size() * sizeof(Vec3D)),
	m_InstanceBuffer(0, 3),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size())
{
	// s_VertexLayout already uses 0/1 slots in the layout, so the instance buffer elements should continue from there
//	m_InstanceBuffer.SetAttribStartIdx(s_VertexLayout.m_Elements.size());

	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_InstanceBuffer.SetLayout();
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

BrightColouredMesh::BrightColouredMesh(const std::vector<float>& vertexAndColorData, const std::vector<uint32_t>& indexData)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexAndColorData[0], vertexAndColorData.size() * sizeof(float)),
	m_InstanceBuffer(0, 3),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size())
{
	// s_VertexLayout already uses 0/1 slots in the layout, so the instance buffer elements should continue from there
//	m_InstanceBuffer.SetAttribStartIdx(s_VertexLayout.m_Elements.size());

	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_InstanceBuffer.SetLayout();
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

BrightColouredMesh::~BrightColouredMesh()
{
}

void BrightColouredMesh::Draw()
{
	m_VertexArray.Bind();

//	static const unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
//	glDrawBuffers(2, attachments);
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, 0, m_InstanceBuffer.GetElementCount()));
//	glLineWidth(10.0f);
//	glDrawArraysInstanced(GL_LINES, 0, m_IndexBuffer.GetCount(), m_InstanceBuffer.GetElementCount()); // woow cool this works :D

	m_VertexArray.UnBind();
}

void BrightColouredMesh::SetInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
}

void BrightColouredMesh::SetColourInstances(const std::vector<ColourComponent>& colours)
{
}

void BrightColouredMesh::DrawInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
	Draw();
}

int BrightColouredMesh::GetColourInstances()
{
	return -1;
}

MeshType BrightColouredMesh::GetMeshType()
{
	return GetStaticMeshType();
}

BufferLayout BrightColouredMesh::GetVertexLayout()
{
	return s_VertexLayout;
}

BufferLayout BrightColouredMesh::GetInstanceLayout()
{
	return m_InstanceBuffer.GetLayout();
}


MeshType BrightColouredMesh::GetStaticMeshType()
{
	return MeshType::BRIGHT_COLOURED_MESH;
}
