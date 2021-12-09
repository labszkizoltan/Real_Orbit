
#include "NormalMesh.h"

#include <glad/glad.h>
#include <vendor/stb_image/stb_image.h>
#include <core/rendering/Renderer.h> // needed for the GLCall macro
// #include <core/rendering/Buffer.h>
#include <core/scene/Components.h> // needed for TransformComponent definition

BufferLayout NormalMesh::s_VertexLayout = {
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
	m_InstanceBuffer(0, 3),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size()),
	m_Texture(texture)
{
	// s_VertexLayout already uses 0/1/2 slots in the layout, so the instance buffer elements should continue from there
	m_InstanceBuffer.SetAttribStartIdx(s_VertexLayout.m_Elements.size());

	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_InstanceBuffer.SetLayout();
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

NormalMesh::NormalMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, const std::string& texturePath)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexData[0], vertexData.size() * sizeof(float)),
	m_InstanceBuffer(0, 3),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size()),
	m_Texture(new Texture(texturePath))
{
	// s_VertexLayout already uses 0/1/2 slots in the layout, so the instance buffer elements should continue from there
	m_InstanceBuffer.SetAttribStartIdx(s_VertexLayout.m_Elements.size());

	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_InstanceBuffer.SetLayout();
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

	GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, 0, m_InstanceBuffer.GetElementCount()));

	m_VertexArray.UnBind();
}

void NormalMesh::SetInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
}

void NormalMesh::DrawInstances(const std::vector<TransformComponent>& transforms)
{
	m_VertexArray.Bind();
	m_Texture->Bind();
	m_InstanceBuffer.SetData(transforms);

	GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, 0, transforms.size()));

	m_VertexArray.UnBind();
}

MeshType NormalMesh::GetMeshType()
{
	return GetStaticMeshType();
}

BufferLayout NormalMesh::GetVertexLayout()
{
	return s_VertexLayout;
}

BufferLayout NormalMesh::GetInstanceLayout()
{
	return m_InstanceBuffer.GetLayout();
}


MeshType NormalMesh::GetStaticMeshType()
{
	return MeshType::NORMAL_MESH;
}
