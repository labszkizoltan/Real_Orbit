
#include <core/pch/pch.h>
#include <core/scene/Components.h>
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
//	entity.AddComponent<TransformComponent>();
	TagComponent& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? "UnnamedEntity" : name;

	return entity;
}

void Scene::OnUpdate(Timestep ts)
{
}

void Scene::OnViewportResize(uint32_t width, uint32_t height)
{
}

CameraComponent& Scene::GetCamera()
{
	return m_Camera;
}

LightComponent& Scene::GetLight()
{
	return m_Light;
}









