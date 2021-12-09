
#include "ColouredMesh.h"

#include <glad/glad.h>
#include <core/rendering/Renderer.h> // needed for the GLCall macro
// #include <core/rendering/Buffer.h>
#include <core/scene/Components.h> // needed for TransformComponent definition

BufferLayout ColouredMesh::s_VertexLayout = {
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aColor"}
};

ColouredMesh::ColouredMesh()
{
}

ColouredMesh::ColouredMesh(const std::vector<Vec3D>& vertexAndColorData, const std::vector<uint32_t>& indexData)
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

ColouredMesh::ColouredMesh(const std::vector<float>& vertexAndColorData, const std::vector<uint32_t>& indexData)
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

ColouredMesh::~ColouredMesh()
{
}

void ColouredMesh::Draw()
{
	m_VertexArray.Bind();

	GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, 0, m_InstanceBuffer.GetElementCount()));

	m_VertexArray.UnBind();
}

void ColouredMesh::SetInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
}

void ColouredMesh::DrawInstances(const std::vector<TransformComponent>& transforms)
{
	m_VertexArray.Bind();
	m_InstanceBuffer.SetData(transforms);

	GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, 0, transforms.size()));

	m_VertexArray.UnBind();
}

MeshType ColouredMesh::GetMeshType()
{
	return GetStaticMeshType();
}

BufferLayout ColouredMesh::GetVertexLayout()
{
	return s_VertexLayout;
}

BufferLayout ColouredMesh::GetInstanceLayout()
{
	return m_InstanceBuffer.GetLayout();
}


MeshType ColouredMesh::GetStaticMeshType()
{
	return MeshType::COLOURED_MESH;
}
