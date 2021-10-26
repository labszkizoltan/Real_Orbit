
#ifndef SKYBOX_H
#define SKYBOX_H

#include <core/rendering/drawables/Mesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/VertexArray.h>
#include <utils/Vector_3D.h>

class Skybox : public Mesh
{
public:
	Skybox();
	Skybox(const std::vector<Vec3D>& vertexAndColorData, const std::vector<uint32_t>& indexData);
	//	ColouredMesh(const std::string& filename);
	~Skybox();

	virtual void Draw() override;
	virtual MeshType GetMeshType() override;
	virtual BufferLayout GetBufferLayout() override;

	static MeshType GetStaticMeshType();

private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;

private:
	static BufferLayout s_Layout;

};

#endif // SKYBOX_H
