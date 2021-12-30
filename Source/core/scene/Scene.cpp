
#include <core/pch/pch.h>
#include <core/scene/CoreComponents.h>
#include "Scene.h"
#include "Entity.h"

#include <glm/glm.hpp>

Scene::Scene()
{
}

Scene::~Scene()
{
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity entity = { m_Registry.create(), this };
////	entity.AddComponent<TransformComponent>();
//	TagComponent& tag = entity.AddComponent<TagComponent>();
//	tag.Tag = name.empty() ? "UnnamedEntity" : name;

	return entity;
//	return Entity();
}

void Scene::OnUpdate(Timestep ts)
{
}


TransformComponent& Scene::GetCamera()
{
	return m_Camera;
}

TransformComponent& Scene::GetLight()
{
	return m_Light;
}

MeshLibrary& Scene::GetMeshLibrary()
{
	return m_MeshLibrary;
}









