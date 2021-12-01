
#include "TexturedMesh.h"

#include <glad/glad.h>
#include <vendor/stb_image/stb_image.h>

BufferLayout TexturedMesh::s_Layout = {
        {ShaderDataType::Float3, "aPos"},
        {ShaderDataType::Float2, "aTexCoord"}
};

TexturedMesh::TexturedMesh()
{
}

TexturedMesh::TexturedMesh(const std::vector<float>& vertexAndTexCoordData, const std::vector<uint32_t>& indexData, std::shared_ptr<Texture> texture)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexAndTexCoordData[0], vertexAndTexCoordData.size() * sizeof(float)),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size()),
	m_Texture(texture)
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_Layout);
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

TexturedMesh::TexturedMesh(const std::vector<float>& vertexAndTexCoordData, const std::vector<uint32_t>& indexData, const std::string& texturePath)
    : m_VertexArray(),
	m_VertexBuffer((float*)&vertexAndTexCoordData[0], vertexAndTexCoordData.size() * sizeof(float)),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size()),
	m_Texture(new Texture(texturePath))
{
    m_VertexArray.Bind();
    m_VertexBuffer.SetLayout(s_Layout);
    m_IndexBuffer.Bind();
    m_VertexArray.UnBind();
}

TexturedMesh::~TexturedMesh()
{
}

void TexturedMesh::Draw()
{
    m_VertexArray.Bind();
	m_Texture->Bind();

    glDrawElements(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

MeshType TexturedMesh::GetMeshType()
{
    return GetStaticMeshType();
}

BufferLayout TexturedMesh::GetBufferLayout()
{
    return s_Layout;
}

MeshType TexturedMesh::GetStaticMeshType()
{
    return MeshType::TEXTURED_MESH;
}
