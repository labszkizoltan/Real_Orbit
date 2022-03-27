
// class responsible for Entity creation and destruction but not spatial selection or updating

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <core/scene/Scene.h>
#include <core/scene/Entity.h>
#include <core/scene/CoreComponents.h>
#include <Game_v1/Components/GameComponents.h>

// #include <Game_v1/Common/DualOctTree.h>


class EntityManager
{
public:
	EntityManager() = default;
	EntityManager(Scene* scene);
	~EntityManager() = default;

	void SetScene(Scene* scene);

	void EmitMesh(int meshIdx, TransformComponent transform);
	void SpawnAsteroid(Vec3D center, Vec3D velocity, float spread);
	void SpawnAsteroidCloud();
	void SpawnDebris(Vec3D center, Vec3D velocity, float spread, float bulletChance);
	void ShootBullett(TransformComponent transform, Vec3D velocity, bool anti_missille = false);
	void LaunchMissile(int meshIdx, TransformComponent transform, DynamicPropertiesComponent hostVelocity, entt::entity target);
	void RemoveMesh(int meshIdx);
	void SpawnExplosion(TransformComponent trf, DynamicPropertiesComponent dyn);
	void CreateStars();

	//void BuildOctTree(); // -> this may need to move to the EntityManager as well, not 100% sure

private:
	Scene* m_Scene = nullptr;

};







#endif // ENTITY_MANAGER_H
