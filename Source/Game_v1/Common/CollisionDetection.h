
#ifndef COLLSION_DETECTION_H
#define COLLSION_DETECTION_H

#include <core/scene/Scene.h>
#include <core/scene/Entity.h>
#include <utils/Matrix_3D.h>

#include <core/scene/CoreComponents.h>
#include <Game_v1/Components/GameComponents.h>

class CollisionDetector
{
public:
	CollisionDetector() = default;
	CollisionDetector(Scene* scene);
	~CollisionDetector() = default;

	void SetScene(Scene* scene);

	Vec3D CheckRoughCollision(entt::entity entity_1, entt::entity entity_2, Timestep ts);

	static Vec3D CheckRoughCollision(
		const TransformComponent& loc1,
		const TransformComponent& loc2,
		const DynamicPropertiesComponent& vel1,
		const DynamicPropertiesComponent& vel2,
		Timestep ts);


private:
	Scene* m_Scene = nullptr;

};



#endif // COLLSION_DETECTION_H
