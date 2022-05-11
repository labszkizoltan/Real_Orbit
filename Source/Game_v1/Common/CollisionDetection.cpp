
#include "CollisionDetection.h"
// #include <Game_v1/Components/GameComponents.h>

#include <limits>


CollisionDetector::CollisionDetector(Scene* scene) : m_Scene(scene) {}

void CollisionDetector::SetScene(Scene* scene)
{
	m_Scene = scene;
}

Vec3D CollisionDetector::CheckRoughCollision(entt::entity entity_1, entt::entity entity_2, Timestep ts)
{
	TransformComponent& loc1 = m_Scene->m_Registry.get<TransformComponent>(entity_1);
	TransformComponent& loc2 = m_Scene->m_Registry.get<TransformComponent>(entity_2);
	DynamicPropertiesComponent& vel1 = m_Scene->m_Registry.get<DynamicPropertiesComponent>(entity_1);
	DynamicPropertiesComponent& vel2 = m_Scene->m_Registry.get<DynamicPropertiesComponent>(entity_2);


	Vec3D dx = loc2.location - loc1.location;
	Vec3D dv = vel2.velocity - vel1.velocity;

	// check collision:
	float lambda = dv.lengthSquare() < 0.00001f ? 0 : -(dx * dv) / (ts * dv.lengthSquare());
	Vec3D hit_location = dx + dv * ts * std::min(1.0f, std::max(lambda, 0.0f));
	float distance = hit_location.length();


	if (distance < (loc1.scale + loc2.scale))
		return loc1.location + hit_location - dx;
	else
		return Vec3D(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

	/*
	std::numeric_limits<float>::max();
	std::numeric_limits<float>::min();
	std::numeric_limits<float>::infinity();
	*/

}

Vec3D CollisionDetector::CheckRoughCollision(
	const TransformComponent& loc1,
	const TransformComponent& loc2,
	const DynamicPropertiesComponent& vel1,
	const DynamicPropertiesComponent& vel2,
	Timestep ts
)
{
	Vec3D dx = loc2.location - loc1.location;
	Vec3D dv = vel2.velocity - vel1.velocity;

	// check collision:
	float lambda = dv.lengthSquare() < 0.00001f ? 0 : -(dx * dv) / (ts * dv.lengthSquare());
	Vec3D hit_location = dx + dv * ts * std::min(1.0f, std::max(lambda, 0.0f));
	float distance = hit_location.length();

	if (distance < (loc1.scale + loc2.scale))
		return loc1.location + hit_location - dx;
	else
		return Vec3D(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
}














/*



auto missiles = m_Scene->m_Registry.view<TargetComponent, TransformComponent, DynamicPropertiesComponent>();
for (auto missile : missiles)
{
	TargetComponent& target = missiles.get<TargetComponent>(missile);
	if (m_Scene->m_Registry.valid(target.targetEntity))
	{
		TransformComponent& missileTrf = missiles.get<TransformComponent>(missile);
		DynamicPropertiesComponent& missileVelocity = missiles.get<DynamicPropertiesComponent>(missile);

		static std::vector<entt::entity> missille_vicinity;
		Box3D box; box.center = missileTrf.location; box.radius = Vec3D(20, 20, 20);
		missille_vicinity.clear();
		m_CollidersOctTree->GetActiveTree()->QueryRange(box, missille_vicinity, 0);

		Vec3D avoidanceVector = Vec3D();

		for (int i = 0; i < missille_vicinity.size(); i++)
		{
			// check validity
			if (m_Scene->m_Registry.valid(missille_vicinity[i]))
			{
				TransformComponent& targetLoc = m_Scene->m_Registry.get<TransformComponent>(missille_vicinity[i]);
				DynamicPropertiesComponent& targetVelocity = m_Scene->m_Registry.get<DynamicPropertiesComponent>(missille_vicinity[i]);

				Vec3D dx = targetLoc.location - missileTrf.location;
				Vec3D dv = targetVelocity.velocity - missileVelocity.velocity;

				// avoidanceVector += dx / dx.lengthSquare();
				avoidanceVector += dx / dx.length() / (dx.length() - targetLoc.scale);

				// check collision:
				float lambda = dv.lengthSquare() < 0.00001f ? 0 : -(dx * dv) / (ts * dv.lengthSquare());
				Vec3D hit_location = dx + dv * ts * std::min(1.0f, std::max(lambda, 0.0f));
				float distance = hit_location.length();

				// delete bullet on hit (by setting its timer to 0, so it will be removed automatically)
				if (distance < (targetLoc.scale + missileTrf.scale))
				{
					TimerComponent& ttl = timed_entities.get<TimerComponent>(missile);
					ttl = 0.0f;
					HitPointComponent& targetHP = m_Scene->m_Registry.get<HitPointComponent>(missille_vicinity[i]);
					targetHP.HP -= 10.0f;

					missileTrf.scale /= 8;

					for (int k = 0; k < 15; k++)
					{
						m_EntityManager.SpawnDebris(missileTrf.location + hit_location - dx, targetVelocity.velocity, 0.05, m_BulletSpawnChance);
						m_EntityManager.SpawnDebris(missileTrf.location + hit_location - dx, missileVelocity.velocity / 4, 0.1, m_BulletSpawnChance);
						m_EntityManager.SpawnDebris(missileTrf.location + hit_location - dx, missileVelocity.velocity / 4, 0.1, m_BulletSpawnChance);
					}
				}
			}

		}
		TransformComponent& targetLoc = m_Scene->m_Registry.get<TransformComponent>(target.targetEntity);
		DynamicPropertiesComponent& targetVelocity = m_Scene->m_Registry.get<DynamicPropertiesComponent>(target.targetEntity);

		Vec3D dx = targetLoc.location - missileTrf.location;
		Vec3D dv = targetVelocity.velocity - missileVelocity.velocity;

		float dt = sqrt(dx.lengthSquare() / dv.lengthSquare());
		dx += dt * dv * 0.9f;
		float accel = 0.0001f;

		// This is the place where I can add repulsive forces, so the missilles avoid nearby objects and focus on reaching the target!!!
		// missileVelocity.velocity += accel * ts * dx / dx.length();
		missileVelocity.velocity += accel * ts * (dx + 2 * avoidanceVector) / (dx + 2 * avoidanceVector).length();
	}
}


*/
