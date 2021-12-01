
#ifndef INSTANCED_NORMAL_MESH_H
#define INSTANCED_NORMAL_MESH_H

#include <core/rendering/drawables/InstancedMesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/InstanceBuffer.h>
#include <core/rendering/VertexArray.h>
#include <core/rendering/Texture.h>
#include <utils/Vector_3D.h>

class InstancedNormalMesh : public InstancedMesh
{
public:
	InstancedNormalMesh();
	InstancedNormalMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, std::shared_ptr<Texture> texture);
	InstancedNormalMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, const std::string& texturePath);
	~InstancedNormalMesh() {};

	void DrawInstances(const std::vector<TransformComponent>& transforms) override;
	InstancedMeshType GetMeshType() override;
	BufferLayout GetVertexLayout() override;
	BufferLayout GetInstanceLayout() override;

private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	InstanceBuffer m_InstanceBuffer;
	IndexBuffer m_IndexBuffer;
	std::shared_ptr<Texture> m_Texture;
//	int m_InstanceCount = 0; // this is now contained in the InstanceBuffer

private:
	static BufferLayout s_VertexLayout;
//	static BufferLayout s_InstanceLayout; // this is also contained in the InstanceBuffer
};



#endif // INSTANCED_NORMAL_MESH_H
