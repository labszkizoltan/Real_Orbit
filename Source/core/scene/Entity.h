
#ifndef ENTITY_H
#define ENTITY_H

#include <core/scene/Scene.h>
#include <core/Core.h>
#include <entt/entt.hpp>

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, Scene* scene);
	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args);

	template<typename T>
	T& GetComponent();

	template<typename T>
	bool HasComponent();

	template<typename T>
	void RemoveComponent();

	operator bool() const { return m_EntityHandle != entt::null; }
	operator uint32_t() const { return (uint32_t)m_EntityHandle; }

	bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
	bool operator!=(const Entity& other) const { return !(*this == other); }

private:
	entt::entity m_EntityHandle{ entt::null };
	Scene* m_Scene = nullptr; // total size is 4(uint32_t) + 8(any pointer on 64 bit architecture) = 12 bytes
};


template<typename T, typename... Args>
T& Entity::AddComponent(Args&&... args)
{
	if (HasComponent<T>())
		LOG_CORE_WARN("Entity::AddComponent() warning: Entity already has component!");
	return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
}

template<typename T>
T& Entity::GetComponent()
{
	if(!HasComponent<T>())
		LOG_CORE_WARN("Entity::GetComponent() warning: Entity does not have component!");
	return m_Scene->m_Registry.get<T>(m_EntityHandle);
}

template<typename T>
bool Entity::HasComponent()
{
	return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
}

template<typename T>
void Entity::RemoveComponent()
{
	LOG_CORE_WARN("Entity::RemoveComponent() warning: Entity does not have component!");
	m_Scene->m_Registry.remove<T>(m_EntityHandle);
}






#endif // ENTITY_H
