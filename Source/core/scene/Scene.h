
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
	void OnViewportResize(uint32_t width, uint32_t height);

	CameraComponent& GetCamera();
	LightComponent& GetLight();

private:
	entt::registry m_Registry;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	CameraComponent m_Camera;
	LightComponent m_Light;
	MeshLibrary m_MeshLibrary;
	MeshLibrary_old m_MeshLibrary_old;

	friend class Entity;
	friend class SceneSerializer;
	friend class SceneUpdater;
	friend class SceneRenderer;
};


#endif // SCENE_H
