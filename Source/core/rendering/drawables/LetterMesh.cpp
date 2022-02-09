
#include "LetterMesh.h"

#include <glad/glad.h>
#include <core/rendering/Renderer.h> // needed for the GLCall macro
// #include <core/rendering/Buffer.h>
#include <core/scene/CoreComponents.h> // needed for TransformComponent definition

BufferLayout LetterMesh::s_VertexLayout = {
		{ShaderDataType::Float3, "aPos"}
};

std::vector<Vec3D> letterMeshDefaultVertexData = {
	Vec3D(0,0,0),
	Vec3D(1,0,0),
	Vec3D(0,1,0),
	Vec3D(1,1,0)
};

std::vector<uint32_t> letterMeshDefaultIndexData = {
	0,1,2,
	1,3,2
};

LetterMesh::LetterMesh()
	: m_VertexArray(),
	m_VertexBuffer((float*)&letterMeshDefaultVertexData[0], letterMeshDefaultVertexData.size() * sizeof(Vec3D)),
	m_InstanceBuffer(0, 3),
	m_IndexBuffer((uint32_t*)&letterMeshDefaultIndexData[0], letterMeshDefaultIndexData.size())
{
	// s_VertexLayout already uses 0/1 slots in the layout, so the instance buffer elements should continue from there
//	m_InstanceBuffer.SetAttribStartIdx(s_VertexLayout.m_Elements.size());

	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_InstanceBuffer.SetLayout();
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

LetterMesh::LetterMesh(const std::vector<Vec3D>& vertexAndColorData, const std::vector<uint32_t>& indexData)
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

LetterMesh::LetterMesh(const std::vector<float>& vertexAndColorData, const std::vector<uint32_t>& indexData)
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

LetterMesh::~LetterMesh()
{
}

void LetterMesh::Draw()
{
	glDisable(GL_DEPTH_TEST);

	m_VertexArray.Bind();

	//	static const unsigned int attachment = GL_COLOR_ATTACHMENT0;
	//	glDrawBuffers(1, &attachment);
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, 0, m_InstanceBuffer.GetElementCount()));

	m_VertexArray.UnBind();

	glEnable(GL_DEPTH_TEST);
}

void LetterMesh::SetInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
}

void LetterMesh::SetColourInstances(const std::vector<ColourComponent>& colours)
{
}

void LetterMesh::DrawInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
	Draw();
}

int LetterMesh::GetColourInstances()
{
	return -1;
}

MeshType LetterMesh::GetMeshType()
{
	return GetStaticMeshType();
}

BufferLayout LetterMesh::GetVertexLayout()
{
	return s_VertexLayout;
}

BufferLayout LetterMesh::GetInstanceLayout()
{
	return m_InstanceBuffer.GetLayout();
}


MeshType LetterMesh::GetStaticMeshType()
{
	return MeshType::LETTER_MESH;
}








