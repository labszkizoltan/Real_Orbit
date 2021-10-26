
#include "TexturedMesh.h"

#include <glad/glad.h>
//#include <vendor/SFML/include/SFML/Graphics/Image.hpp>
#include <vendor/stb_image/stb_image.h>

BufferLayout TexturedMesh::s_Layout = {
        {ShaderDataType::Float3, "aPos"},
        {ShaderDataType::Float2, "aTexCoord"}
};

TexturedMesh::TexturedMesh()
{
}

TexturedMesh::TexturedMesh(const std::vector<float>& vertexAndTexCoordData, const std::vector<uint32_t>& indexData)
    : m_VertexArray(), m_VertexBuffer((float*)&vertexAndTexCoordData[0], vertexAndTexCoordData.size() * sizeof(float)), m_IndexBuffer((uint32_t*)&indexData[0], indexData.size())
{
//    sf::Image img;
//    img.loadFromFile("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/saucer_texture.png");
//    if (!m_Texture.loadFromFile("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/saucer_texture.png"))
//    {
//        std::cout << "loading texture failed\n";
//    }


	// Create the texture
	{
		std::string textureFilepath("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/saucer_texture.png");
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(textureFilepath.c_str(), &width, &height, &channels, 0);
		}
		if (!data) { std::cout << "Failed to load image: " << textureFilepath; }

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture);
		glTextureStorage2D(m_Texture, 1, internalFormat, width, height);
		glTextureParameteri(m_Texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_Texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // flags: GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT
		glTextureParameteri(m_Texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureSubImage2D(m_Texture, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
		glBindTextureUnit(0, m_Texture); // texture units 0-5 are used by the skybox

		stbi_image_free(data);
	}


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
//    sf::Texture::bind(&m_Texture);
	glBindTextureUnit(0, m_Texture);
    glDrawElements(GL_TRIANGLES, m_IndexBuffer.m_Count, GL_UNSIGNED_INT, nullptr);
    // unbind the texture
    // sf::Texture::bind(NULL);
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
