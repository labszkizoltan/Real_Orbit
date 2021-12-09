
#ifndef SCENE_H
#define SCENE_H

#include <core/Timestep.h>
#include <entt/entt.hpp>
#include <core/rendering/drawables/MeshLibrary.h>
#include <core/scene/Components.h>
//#include <entt/entity/registry.hpp>

class Entity;

class Scene
{
public:
	Scene();
	~Scene();

	Entity CreateEntity(const std::string& name = std::string());

	void OnUpdate(Timestep ts);

	TransformComponent& GetCamera();
	TransformComponent& GetLight();

private:
	entt::registry m_Registry; // do I even need this?
	TransformComponent m_Camera;
	TransformComponent m_Light;
	std::shared_ptr<Mesh> m_Skybox = nullptr;

	MeshLibrary m_MeshLibrary;

	friend class Entity;
	friend class SceneSerializer;
	friend class SceneUpdater;
	friend class SceneRenderer;
};


#endif // SCENE_H
