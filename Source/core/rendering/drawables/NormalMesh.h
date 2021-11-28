
#ifndef NORMAL_MESH_H
#define NORMAL_MESH_H



#include <core/rendering/drawables/Mesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/VertexArray.h>
#include <core/rendering/Texture.h>
#include <utils/Vector_3D.h>

class NormalMesh : public Mesh
{
public:
	NormalMesh();
	NormalMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, std::shared_ptr<Texture> texture);
	NormalMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, const std::string& texturePath);
	~NormalMesh();

	virtual void Draw() override;
//	virtual void DrawInstances(std::vector<TransformComponent> transforms) override;
	virtual MeshType GetMeshType() override;
	virtual BufferLayout GetBufferLayout() override;

	static MeshType GetStaticMeshType();

private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	std::shared_ptr<Texture> m_Texture;

private:
	static BufferLayout s_Layout;

};












#endif // NORMAL_MESH_H
