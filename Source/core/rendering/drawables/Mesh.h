
#ifndef MESH_H
#define MESH_H

#include <core/rendering/VertexArray.h>
#include <core/rendering/Buffer.h>
#include <utils/Vector_3D.h>

class Mesh
{
public:
	Mesh();
	Mesh(const std::vector<Vec3D>& vertexAndColorData, const std::vector<uint32_t>& indexData);
//	Mesh(const std::string& filename);
	~Mesh();

	void Draw();

private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;

};



#endif // MESH_H
