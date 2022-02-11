
#ifndef LETTER_MESH_H
#define LETTER_MESH_H

#include <core/rendering/drawables/Mesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/InstanceBuffer.h>
#include <core/rendering/VertexArray.h>
#include <core/rendering/Texture.h>
#include <utils/Vector_3D.h>


class LetterMesh : public Mesh
{
public:
	LetterMesh();
//	LetterMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, std::shared_ptr<Texture> texture);
//	LetterMesh(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, const std::string& texturePath);
	LetterMesh(std::shared_ptr<Texture> texture);
	LetterMesh(const std::string& texturePath);

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
	std::shared_ptr<Texture> m_Texture;

private:
	static BufferLayout s_VertexLayout;

};












#endif // LETTER_MESH_H
