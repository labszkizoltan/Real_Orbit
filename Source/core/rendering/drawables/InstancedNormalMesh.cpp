
#include "InstancedNormalMesh.h"

#include <glad/glad.h>
#include <vendor/stb_image/stb_image.h>
#include <core/scene/Components.h>

BufferLayout InstancedNormalMesh::s_VertexLayout = {
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aSurfaceNormal"},
		{ShaderDataType::Float2, "aTexCoord"}
};

InstancedNormalMesh::InstancedNormalMesh()
{
}

InstancedNormalMesh::InstancedNormalMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, std::shared_ptr<Texture> texture)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexData[0], vertexData.size() * sizeof(float)),
	m_InstanceBuffer(),
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

InstancedNormalMesh::InstancedNormalMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, const std::string& texturePath)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexData[0], vertexData.size() * sizeof(float)),
	m_InstanceBuffer(),
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


void InstancedNormalMesh::DrawInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);

}

InstancedMeshType InstancedNormalMesh::GetMeshType()
{
    return InstancedMeshType::INSTANCED_NORMAL_MESH;
}

BufferLayout InstancedNormalMesh::GetVertexLayout()
{
	return s_VertexLayout;
}

BufferLayout InstancedNormalMesh::GetInstanceLayout()
{
	return m_InstanceBuffer.GetLayout();
}