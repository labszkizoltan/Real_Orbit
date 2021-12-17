
#ifndef BRIGHT_COLOURED_MESH_H
#define BRIGHT_COLOURED_MESH_H

#include <core/rendering/drawables/Mesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/InstanceBuffer.h>
#include <core/rendering/VertexArray.h>
#include <utils/Vector_3D.h>

class BrightColouredMesh : public Mesh
{
public:
	BrightColouredMesh();
	BrightColouredMesh(const std::vector<Vec3D>& vertexAndColorData, const std::vector<uint32_t>& indexData);
	BrightColouredMesh(const std::vector<float>& vertexAndColorData, const std::vector<uint32_t>& indexData);

	~BrightColouredMesh();

	virtual void Draw() override;
	virtual void SetInstances(const std::vector<TransformComponent>& transforms) override;
	virtual void DrawInstances(const std::vector<TransformComponent>& transforms) override;
	virtual MeshType GetMeshType() override;
	virtual BufferLayout GetVertexLayout() override;
	virtual BufferLayout GetInstanceLayout() override;

	static MeshType GetStaticMeshType();

private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	InstanceBuffer m_InstanceBuffer;
	IndexBuffer m_IndexBuffer;

private:
	static BufferLayout s_VertexLayout;

};






#endif // BRIGHT_COLOURED_MESH_H