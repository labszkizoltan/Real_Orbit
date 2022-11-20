
#ifndef ROUGH_MESH_H
#define ROUGH_MESH_H

#include <core/rendering/drawables/Mesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/InstanceBuffer.h>
#include <core/rendering/VertexArray.h>
#include <core/rendering/Texture.h>
#include <utils/Vector_3D.h>

class RoughMesh : public Mesh
{
public:
	RoughMesh();
	RoughMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normalMap);
	RoughMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, const std::string& texturePath, const std::string& normalMapPath);
	~RoughMesh();

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
	std::shared_ptr<Texture> m_Texture;
	std::shared_ptr<Texture> m_NormalMap;

private:
	static BufferLayout s_VertexLayout;

};












#endif // ROUGH_MESH_H
