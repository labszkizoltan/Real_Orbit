
#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include "Scene.h"
#include <yaml-cpp/yaml.h>

struct OGLBufferData
{
	std::vector<float> vertex_data;
	std::vector<uint32_t> index_data;
};

class SceneSerializer
{
public:

	SceneSerializer();
	SceneSerializer(std::shared_ptr<Scene> scene);
	~SceneSerializer();

	void Serialize(const std::string& output_file);
	void DeSerialize(const std::string& input_file);

private:
	void FillMeshLibrary(const YAML::Node& data);
	OGLBufferData ParseVertexFile(const std::string& filename);

private:
	std::shared_ptr<Scene> m_Scene = nullptr;

};

#endif // SCENE_SERIALIZER_H
