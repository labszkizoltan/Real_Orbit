
#ifndef COLOURED_MESH_H
#define COLOURED_MESH_H

#include <core/rendering/drawables/Mesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/VertexArray.h>
#include <utils/Vector_3D.h>

class ColouredMesh : public Mesh
{
public:
	ColouredMesh();
	ColouredMesh(const std::vector<Vec3D>& vertexAndColorData, const std::vector<uint32_t>& indexData);
	//	ColouredMesh(const std::string& filename);
	~ColouredMesh();

	virtual void Draw() override;
	virtual MeshType GetMeshType() override;
	virtual BufferLayout GetBufferLayout() override;

private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;

private:
	static BufferLayout s_Layout;

};






#endif // COLOURED_MESH_H
