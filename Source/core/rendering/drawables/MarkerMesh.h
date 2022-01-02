
/*
* 
#ifndef MARKER_MESH_H
#define MARKER_MESH_H

#include <core/rendering/drawables/Mesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/InstanceBuffer.h>
#include <core/rendering/ColourInstanceBuffer.h>
#include <core/rendering/VertexArray.h>
#include <utils/Vector_3D.h>

class MarkerMesh : public Mesh
{
public:
	MarkerMesh();
	MarkerMesh(const std::vector<Vec3D>& vertexData, const std::vector<uint32_t>& indexData);
	MarkerMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData);

	~MarkerMesh();

	virtual void Draw() override;
	virtual void SetInstances(const std::vector<TransformComponent>& transforms) override;
	void SetColourInstances(const std::vector<ColourComponent>& colours);
	virtual void DrawInstances(const std::vector<TransformComponent>& transforms) override;
	virtual MeshType GetMeshType() override;
	virtual BufferLayout GetVertexLayout() override;
	virtual BufferLayout GetInstanceLayout() override;

	static MeshType GetStaticMeshType();

private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	InstanceBuffer m_InstanceBuffer;
	ColourInstanceBuffer m_ColourInstanceBuffer;
	IndexBuffer m_IndexBuffer;

private:
	static BufferLayout s_VertexLayout;


};




#endif // MARKER_MESH_H

*/

