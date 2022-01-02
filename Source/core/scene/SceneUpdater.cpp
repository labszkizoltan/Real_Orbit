
#include "SceneUpdater.h"
#include <core/scene/Entity.h>
#include <utils/OctTree.h>

SceneUpdater::SceneUpdater()
{
	// Load a sound to play
	if (!m_ExplosionSoundBuffer.loadFromFile("assets/audio/Explosion_big.wav"))
		LOG_ERROR("SceneUpdater() - Audio not found: assets/audio/Explosion_big.wav");
	m_ExplosionSound.setBuffer(m_ExplosionSoundBuffer);
	m_ExplosionSound.setVolume(60.0f);

}

SceneUpdater::SceneUpdater(std::shared_ptr<Scene> scene)
	: m_Scene(scene)
{
}

SceneUpdater::~SceneUpdater()
{
}

void SceneUpdater::UpdateScene(Timestep ts)
{
	m_Scene->m_MeshLibrary.Clear();

	auto timed_entities = m_Scene->m_Registry.view<TimerComponent>();
	for (auto entity : timed_entities)
	{
		TimerComponent& ttl = timed_entities.get<TimerComponent>(entity);
		ttl.timeToLive -= ts;
		if (ttl.timeToLive < 0.0f)
			m_Scene->m_Registry.destroy(entity);
	}

	auto HP_entities = m_Scene->m_Registry.view<HitPointComponent>();
	for (auto entity : HP_entities)
	{
		HitPointComponent& hp = HP_entities.get<HitPointComponent>(entity);
		if (hp.HP < 0.0f)
		{
			TransformComponent& trf = m_Scene->m_Registry.get<TransformComponent>(entity);
			DynamicPropertiesComponent& dyn = m_Scene->m_Registry.get<DynamicPropertiesComponent>(entity);
			SpawnExplosion(trf, dyn);
			m_Scene->m_Registry.destroy(entity);
			m_ExplosionSound.play();
		}
	}

	auto explosions = m_Scene->m_Registry.view<ExplosionComponent>();
	for (auto explosion : explosions)
	{
		TransformComponent& trf = m_Scene->m_Registry.get<TransformComponent>(explosion);
		trf.scale += ts / 2000.0f;
	}

	auto missiles = m_Scene->m_Registry.view<TargetComponent, TransformComponent, DynamicPropertiesComponent>();
	for (auto missile : missiles)
	{
		TargetComponent& target = missiles.get<TargetComponent>(missile);
		if (m_Scene->m_Registry.valid(target.targetEntity))
		{
			TransformComponent& missileTrf = missiles.get<TransformComponent>(missile);
			DynamicPropertiesComponent& missileVelocity = missiles.get<DynamicPropertiesComponent>(missile);
			TransformComponent& targetLoc = m_Scene->m_Registry.get<TransformComponent>(target.targetEntity);
			DynamicPropertiesComponent& targetVelocity = m_Scene->m_Registry.get<DynamicPropertiesComponent>(target.targetEntity);

			Vec3D dx = targetLoc.location - missileTrf.location;
			// delete missille on hit (by setting its timer to 0, so it will be removed automatically)
			if (dx.length() < (targetLoc.scale + missileTrf.scale))
			{
				TimerComponent& ttl = timed_entities.get<TimerComponent>(missile);
				ttl = 0.0f;
				HitPointComponent& targetHP = m_Scene->m_Registry.get<HitPointComponent>(target.targetEntity);
				targetHP.HP -= 1.0f;
			}
			Vec3D dv = targetVelocity.velocity - missileVelocity.velocity;
			float dt = sqrt(dx.lengthSquare() / dv.lengthSquare());
//			float dt = dx.lengthSquare() / (dx*dv);
//			dx += dt * dv;
//			dx += dt * dv/2;
			dx += dt * dv * 0.9f;
			float accel = 0.0001f;

			missileVelocity.velocity += accel * ts * dx / dx.length();
		}
		else
		{
			// this always returns false according the documentation: registry.valid(entt::tombstone);
			target.targetEntity = entt::tombstone;
		}
	}

	auto grav_masses = m_Scene->m_Registry.view<TransformComponent, GravitationalMassComponent>();
	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, MeshIndexComponent>();
	for (auto entity : view)
	{
		TransformComponent& entity_trf = view.get<TransformComponent>(entity);
		if (entity_trf.location.lengthSquare() > 1000000)
		{
			m_Scene->m_Registry.destroy(entity);
			continue;
		}
		DynamicPropertiesComponent& dyn_prop = view.get<DynamicPropertiesComponent>(entity);

		entity_trf.location += ts * dyn_prop.velocity;
		entity_trf.orientation = Rotation(ts*dyn_prop.angular_velocity.length(), dyn_prop.angular_velocity) * entity_trf.orientation;

		// this is not that great, should be separated cus meshes wo dyn props wont be drawn (they dont get into the mesh library)
		MeshIndexComponent& idx = view.get<MeshIndexComponent>(entity);
		m_Scene->m_MeshLibrary.m_MeshTransforms[idx.idx].push_back(entity_trf);

		Vec3D acceleration = {0,0,0};
		for (auto grav_entity : grav_masses)
		{
			if (entity != grav_entity)
			{
				GravitationalMassComponent& grav_mass = grav_masses.get<GravitationalMassComponent>(grav_entity);
				TransformComponent& grav_trf = grav_masses.get<TransformComponent>(grav_entity);
				Vec3D dr = grav_trf.location - entity_trf.location;
				acceleration += grav_mass.gravitational_mass * dr / pow(dr.length(), 3); // a gravitatonal constant scalar could be added here
			}
		}

		dyn_prop.velocity += ts * acceleration;
	}

	/*
	Box3D box; box.center = Vec3D(0, 0, 0); box.radius = Vec3D(100, 100, 100);
	OctTree<Vec3D> octtree(box);

	auto grav_masses = m_Scene->m_Registry.view<TransformComponent, GravitationalMassComponent>();
	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent>();
	for (auto entity = view.begin(); entity != view.end(); entity++)
	{
		TransformComponent& entity_trf = view.get<TransformComponent>(*entity);
		DynamicPropertiesComponent& dyn_prop = view.get<DynamicPropertiesComponent>(*entity);

		octtree.Insert(entity_trf.location, entity_trf.location);

		entity_trf.location += ts * dyn_prop.velocity;
		entity_trf.orientation = Rotation(ts * dyn_prop.angular_velocity.length(), dyn_prop.angular_velocity) * entity_trf.orientation;

		Vec3D acceleration = { 0,0,0 };
		for (auto grav_entity : grav_masses)
		{
			if (*entity != grav_entity)
			{
				GravitationalMassComponent& grav_mass = grav_masses.get<GravitationalMassComponent>(grav_entity);
				TransformComponent& grav_trf = grav_masses.get<TransformComponent>(grav_entity);
				Vec3D dr = grav_trf.location - entity_trf.location;
				acceleration += grav_mass.gravitational_mass * dr / pow(dr.length(), 3); // a gravitatonal constant scalar could be added here
			}
		}

		dyn_prop.velocity += ts * acceleration;

		// this technically works, but the framerate drops noticeably even in release mode, with just 4 entities
//		auto entity_next = entity;
//		entity_next++;
//		for (entity_next; entity_next != view.end(); entity_next++)
//		{
//			TransformComponent& next_trf = view.get<TransformComponent>(*entity_next);
//			DynamicPropertiesComponent& next_dyn_prop = view.get<DynamicPropertiesComponent>(*entity_next);
//
//			// this is the line that causes the frame rate drop
//			std::cout << next_trf.location - entity_trf.location << "\n";
//		}
	}
	*/


}

void SceneUpdater::SpawnExplosion(TransformComponent trf, DynamicPropertiesComponent dyn)
{
	static int explosionIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Explosion"];

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(trf);
	newEntity.AddComponent<MeshIndexComponent>(explosionIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dyn);
	newEntity.AddComponent<TimerComponent>(TimerComponent(3000.0f));
	newEntity.AddComponent<ExplosionComponent>(ExplosionComponent());

	
}
