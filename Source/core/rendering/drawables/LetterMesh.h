
#ifndef LETTER_MESH_H
#define LETTER_MESH_H

#include <core/rendering/drawables/Mesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/InstanceBuffer.h>
#include <core/rendering/VertexArray.h>
#include <utils/Vector_3D.h>


class LetterMesh : public Mesh
{
public:
	LetterMesh();
	LetterMesh(const std::vector<Vec3D>& vertexAndColorData, const std::vector<uint32_t>& indexData);
	LetterMesh(const std::vector<float>& vertexAndColorData, const std::vector<uint32_t>& indexData);

	~LetterMesh();

	virtual void Draw() override;
	virtual void SetInstances(const std::vector<TransformComponent>& transforms) override;
	virtual void SetColourInstances(const std::vector<ColourComponent>& colours) override;
	virtual void DrawInstances(const std::vector<TransformComponent>& transforms) override;
	virtual int GetColourInstances() override;
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












#endif // LETTER_MESH_H
