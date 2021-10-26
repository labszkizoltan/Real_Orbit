
#ifndef TEXTURED_MESH_H
#define TEXTURED_MESH_H

#include <core/rendering/drawables/Mesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/VertexArray.h>
#include <utils/Vector_3D.h>

#include <SFML/Graphics/Texture.hpp>

class TexturedMesh : public Mesh
{
public:
	TexturedMesh();
	TexturedMesh(const std::vector<float>& vertexAndTexCoordData, const std::vector<uint32_t>& indexData);
	//	TexturedMesh(const std::string& filename);
	~TexturedMesh();

	virtual void Draw() override;
	virtual MeshType GetMeshType() override;
	virtual BufferLayout GetBufferLayout() override;

	static MeshType GetStaticMeshType();

private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	uint32_t m_Texture;
//	sf::Texture m_Texture;

private:
	static BufferLayout s_Layout;

};

#endif // TEXTURED_MESH_H
