
#ifndef SKYBOX_H
#define SKYBOX_H

#include <core/rendering/drawables/Mesh.h>
#include <core/rendering/Buffer.h>
#include <core/rendering/VertexArray.h>
#include <core/rendering/Texture.h>

class Skybox : public Mesh
{
public:
	Skybox();
	Skybox(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, std::vector<std::shared_ptr<Texture>> textures);
	Skybox(const std::vector<float>& vertexData, const std::vector<uint32_t>& indexData, const std::vector<std::string>& textureFilenames);
	~Skybox();


	virtual void Draw() override;
	virtual void SetInstances(const std::vector<TransformComponent>& transforms) override;
	virtual void SetColourInstances(const std::vector<ColourComponent>& colours) override;
	virtual void DrawInstances(const std::vector<TransformComponent>& transforms) override;
	virtual int GetColourInstances() override;
	virtual MeshType GetMeshType() override;
	virtual BufferLayout GetVertexLayout() override;
	virtual BufferLayout GetInstanceLayout() override;

	static MeshType GetStaticMeshType();

	//---------------------------------//
	//----- some helper functions -----//
	//---------------------------------//

	static std::vector<float> CreateSkyboxVertexData(int resolution);
	static std::vector<uint32_t> CreateSkyboxIndexData(int resolution);


private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;

	std::vector<std::shared_ptr<Texture>> m_Textures;

private:
	static BufferLayout s_Layout;

};


#endif // SKYBOX_H
