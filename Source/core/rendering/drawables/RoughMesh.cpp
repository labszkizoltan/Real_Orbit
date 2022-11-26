
#include "RoughMesh.h"

#include <glad/glad.h>
#include <vendor/stb_image/stb_image.h>
#include <core/rendering/Renderer.h> // needed for the GLCall macro
#include <core/scene/CoreComponents.h> // needed for TransformComponent definition
#include <core/GlobalConstants.h>

BufferLayout RoughMesh::s_VertexLayout = {
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aSurfaceNormal"},
		{ShaderDataType::Float2, "aTexCoord"}
};

// BufferLayout RoughMesh::s_VertexLayout = {
// 		{ShaderDataType::Float3, "aPos"},
// 		{ShaderDataType::Float3, "aSurfaceNormal_x"},
// 		{ShaderDataType::Float3, "aSurfaceNormal_y"},
// 		{ShaderDataType::Float3, "aSurfaceNormal_z"},
// 		{ShaderDataType::Float2, "aTexCoord"}
// };

// BufferLayout RoughMesh::s_VertexLayout = {
// 		{ShaderDataType::Float3, "aPos"},
// 		{ShaderDataType::Mat3, "aSurfaceNormal"},
// 		{ShaderDataType::Float2, "aTexCoord"}
// };

RoughMesh::RoughMesh()
{
}

RoughMesh::RoughMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normalMap)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexData[0], vertexData.size() * sizeof(float)),
	m_InstanceBuffer(0, 3),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size()),
	m_Texture(texture),
	m_NormalMap(normalMap)
{
	// s_VertexLayout already uses 0/1/2 slots in the layout, so the instance buffer elements should continue from there
	m_InstanceBuffer.SetAttribStartIdx(s_VertexLayout.m_Elements.size());

	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_InstanceBuffer.SetLayout();
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
	m_Texture->CreateMipMap();
	m_NormalMap->CreateMipMap();
	m_NormalMap->SetSlot(g_MeshNormalMapSlot);
}

RoughMesh::RoughMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, const std::string& texturePath, const std::string& normalMapPath)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexData[0], vertexData.size() * sizeof(float)),
	m_InstanceBuffer(0, 3),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size()),
	m_Texture(new Texture(texturePath)),
	m_NormalMap(new Texture(normalMapPath))
{
	// s_VertexLayout already uses 0/1/2 slots in the layout, so the instance buffer elements should continue from there
//	m_InstanceBuffer.SetAttribStartIdx(s_VertexLayout.m_Elements.size());

	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_VertexLayout);
	m_InstanceBuffer.SetLayout();
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
	m_Texture->CreateMipMap();
	m_NormalMap->CreateMipMap();
	m_NormalMap->SetSlot(g_MeshNormalMapSlot);
}

RoughMesh::~RoughMesh()
{
}

void RoughMesh::Draw()
{
	m_VertexArray.Bind();
	m_Texture->Bind();
	m_NormalMap->Bind();

	//	static const unsigned int attachment = GL_COLOR_ATTACHMENT0;
	//	glDrawBuffers(1, &attachment);
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, 0, m_InstanceBuffer.GetElementCount()));
	//	glDrawArraysInstanced(GL_LINES, 0, m_IndexBuffer.GetCount(), m_InstanceBuffer.GetElementCount()); // woow cool this works :D

	m_VertexArray.UnBind();
}

void RoughMesh::SetInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
}

void RoughMesh::SetColourInstances(const std::vector<ColourComponent>& colours)
{
}

void RoughMesh::DrawInstances(const std::vector<TransformComponent>& transforms)
{
	m_InstanceBuffer.SetData(transforms);
	Draw();
}

int RoughMesh::GetColourInstances()
{
	return -1;
}

MeshType RoughMesh::GetMeshType()
{
	return GetStaticMeshType();
}

BufferLayout RoughMesh::GetVertexLayout()
{
	return s_VertexLayout;
}

BufferLayout RoughMesh::GetInstanceLayout()
{
	return m_InstanceBuffer.GetLayout();
}


MeshType RoughMesh::GetStaticMeshType()
{
	return MeshType::ROUGH_MESH;
}
