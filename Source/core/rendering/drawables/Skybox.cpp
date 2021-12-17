
#include "Skybox.h"
#include <core/scene/Components.h> // needed for TransformComponent definition
#include <core/GlobalConstants.h>

BufferLayout Skybox::s_Layout = {
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float2, "aTexCoord"},
		{ShaderDataType::Float, "aTexID"}
};

Skybox::Skybox()
{
}

Skybox::Skybox(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, std::vector<std::shared_ptr<Texture>> textures)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexData[0], vertexData.size() * sizeof(float)),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size()),
	m_Textures(textures)
{
	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_Layout);
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}

Skybox::Skybox(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, const std::vector<std::string>& textureFilenames)
	: m_VertexArray(),
	m_VertexBuffer((float*)&vertexData[0], vertexData.size() * sizeof(float)),
	m_IndexBuffer((uint32_t*)&indexData[0], indexData.size())
{
	m_Textures.resize(6);
	for (int i = 0; i < 6; i++)
	{
		m_Textures[i] = std::make_shared<Texture>(textureFilenames[i]);
		m_Textures[i]->SetSlot(g_SkyboxTextureSlots[i]); // slot 0 is for other bodies, slot 1 is for the shadow map
	}

	m_VertexArray.Bind();
	m_VertexBuffer.SetLayout(s_Layout);
	m_IndexBuffer.Bind();
	m_VertexArray.UnBind();
}


Skybox::~Skybox()
{
}

void Skybox::Draw()
{
	glDisable(GL_DEPTH_TEST);

	m_VertexArray.Bind();
	for (int i = 0; i < m_Textures.size(); i++)
		m_Textures[i]->Bind();

	static const unsigned int attachment = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &attachment);
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);

	m_VertexArray.UnBind();

	glEnable(GL_DEPTH_TEST);
}

void Skybox::SetInstances(const std::vector<TransformComponent>& transforms)
{
}

void Skybox::DrawInstances(const std::vector<TransformComponent>& transforms)
{
	/*
	glDisable(GL_DEPTH_TEST);

	m_VertexArray.Bind();
	for(int i=0; i< m_Textures.size(); i++)
		m_Textures[i]->Bind();

	glDrawElements(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);

	glEnable(GL_DEPTH_TEST);
	*/
}

MeshType Skybox::GetMeshType()
{
	return GetStaticMeshType();
}

BufferLayout Skybox::GetVertexLayout()
{
	return s_Layout;
}

BufferLayout Skybox::GetInstanceLayout()
{
	return BufferLayout();
}

MeshType Skybox::GetStaticMeshType()
{
	return MeshType::SKYBOX;
}


//---------------------------------//
//----- some helper functions -----//
//---------------------------------//

std::vector<float> Skybox::CreateSkyboxVertexData(int resolution)
{
	std::vector<float> result;
	int face_count = 6; // a cube has 6 faces
	int face_vertex_count = (resolution + 1) * (resolution + 1);
	int vertex_size = 3+2+1; // how many floats are in a vertex
	int delta = face_vertex_count * vertex_size; // offset between the starting index of faces
	// each vertex contains 6 floats, as described in s_Layout:
	// 3 - 3D vertex coordinates, 2 - 2D texture coordinates, 1 - texture id
	result.resize(face_count*face_vertex_count*vertex_size);

	for (int i = 0; i < resolution+1; i++)
	{
		for (int j = 0; j < resolution+1; j++)
		{
			// front face
			float* res_front = &result[vertex_size*((resolution + 1) * i + j) + 0*delta];
			res_front[0] = -0.5f + (float)j/(float)resolution;
			res_front[1] = -0.5f + (float)i/(float)resolution;
			res_front[2] = 0.5f;
			res_front[3] = (float)j / (float)resolution;
			res_front[4] = (float)i / (float)resolution;
			res_front[5] = 0.0f;

			// back face
			float* res_back = &result[vertex_size * ((resolution + 1) * i + j) + 1*delta];
			res_back[0] =  0.5f - (float)j / (float)resolution;
			res_back[1] = -0.5f + (float)i / (float)resolution;
			res_back[2] = -0.5f;
			res_back[3] = (float)j / (float)resolution;
			res_back[4] = (float)i / (float)resolution;
			res_back[5] = 1.0f;

			// left face
			float* res_left = &result[vertex_size * ((resolution + 1) * i + j) + 2*delta];
			res_left[0] = -0.5f;
			res_left[1] = -0.5f + (float)i / (float)resolution;
			res_left[2] = -0.5f + (float)j / (float)resolution;
			res_left[3] = (float)j / (float)resolution;
			res_left[4] = (float)i / (float)resolution;
			res_left[5] = 2.0f;

			// right face
			float* res_right = &result[vertex_size * ((resolution + 1) * i + j) + 3*delta];
			res_right[0] =  0.5f;
			res_right[1] = -0.5f + (float)i / (float)resolution;
			res_right[2] =  0.5f - (float)j / (float)resolution;
			res_right[3] = (float)j / (float)resolution;
			res_right[4] = (float)i / (float)resolution;
			res_right[5] = 3.0f;

			// up face
			float* res_up = &result[vertex_size * ((resolution + 1) * i + j) + 4*delta];
			res_up[0] = -0.5f + (float)j / (float)resolution;
			res_up[1] =  0.5f;
			res_up[2] =  0.5f - (float)i / (float)resolution;
			res_up[3] = (float)j / (float)resolution;
			res_up[4] = (float)i / (float)resolution;
			res_up[5] = 4.0f;

			// down face
			float* res_down = &result[vertex_size * ((resolution + 1) * i + j) + 5*delta];
			res_down[0] = -0.5f + (float)j / (float)resolution;
			res_down[1] = -0.5f;
			res_down[2] = -0.5f + (float)i / (float)resolution;
			res_down[3] = (float)j / (float)resolution;
			res_down[4] = (float)i / (float)resolution;
			res_down[5] = 5.0f;

		}
	}

	return result;
}

std::vector<uint32_t> Skybox::CreateSkyboxIndexData(int resolution)
{
	std::vector<uint32_t> result;

	int face_count = 6; // a cube has 6 faces
	int face_quad_count = resolution * resolution;
	int face_triangle_count = face_quad_count * 2;
	int quad_index_count = 6; // one quad consists of 2 triangles, and each of those has 3 indices
	int face_index_count = face_triangle_count * 3;
	result.resize(face_count * face_index_count);

	for (int i = 0; i < face_count; i++)
	{
		int face_offset = face_index_count * i;
		for (int j = 0; j < resolution; j++)
		{
			for (int k = 0; k < resolution; k++)
			{
				int quad_offset = (j * resolution + k) * quad_index_count;
				uint32_t *res = &result[face_offset+quad_offset];
				res[0] = 0+k+j*(resolution+1)+i*(resolution+ 1)*(resolution+1);
				res[1] = 1+k+j*(resolution+1)+i*(resolution+ 1)*(resolution+1);
				res[2] = (resolution+1)+k+j*(resolution+1)+i*(resolution+ 1)*(resolution+1);
				res[3] = 1+k+j*(resolution+1)+i*(resolution+ 1)*(resolution+1);
				res[4] = (resolution+2)+k+j*(resolution+1)+i*(resolution+ 1)*(resolution+1);
				res[5] = (resolution+1)+k+j*(resolution+1)+i*(resolution+ 1)*(resolution+1);
			}
		}
	}

	return result;
}
