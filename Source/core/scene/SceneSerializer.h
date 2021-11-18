
#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include "Scene.h"

class SceneSerializer
{
public:

	SceneSerializer();
	SceneSerializer(std::shared_ptr<Scene> scene);
	~SceneSerializer();

	void Serialize(const std::string& output_file);
	void DeSerialize(const std::string& input_file);

private:
	std::shared_ptr<Scene> m_Scene = nullptr;

};

#endif // SCENE_SERIALIZER_H
