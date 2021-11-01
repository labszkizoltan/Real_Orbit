
#ifndef TEXTURED_MESH_H
#define TEXTURED_MESH_H

#include <core/rendering/drawables/Mesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/VertexArray.h>
#include <core/rendering/Texture.h>
#include <core/rendering/Depthbuffer.h>
#include <utils/Vector_3D.h>

#include <SFML/Graphics/Texture.hpp>

class TexturedMesh : public Mesh
{
public:
	TexturedMesh();
	TexturedMesh(const std::vector<float>& vertexAndTexCoordData, const std::vector<uint32_t>& indexData, std::shared_ptr<Texture> texture); // use existing texture instead of creating one from a file
	TexturedMesh(const std::vector<float>& vertexAndTexCoordData, const std::vector<uint32_t>& indexData, const std::string& texturePath);
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
	std::shared_ptr<Texture> m_Texture;
//	uint32_t m_Texture;

private:
	static BufferLayout s_Layout;

};

#endif // TEXTURED_MESH_H
