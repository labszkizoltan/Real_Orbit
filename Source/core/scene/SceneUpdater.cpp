
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
			dyn.velocity *= 0.5f;
			SpawnExplosion(trf, dyn);
			m_Scene->m_Registry.destroy(entity);
			m_ExplosionSound.play();
		}
	}

	auto explosions = m_Scene->m_Registry.view<ExplosionComponent>();
	for (auto explosion : explosions)
	{
		TransformComponent& trf = m_Scene->m_Registry.get<TransformComponent>(explosion);
		trf.scale += ts / 200.0f;
		ColourComponent& col = m_Scene->m_Registry.get<ColourComponent>(explosion);
		col.a = 1.0f / (trf.scale * trf.scale); // * trf.scale);
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
			Vec3D dv = targetVelocity.velocity - missileVelocity.velocity;

			// check collision:
			float lambda = dv.lengthSquare() < 0.00001f ? 0 : -(dx * dv) / (ts*dv.lengthSquare());
			float distance = (dx + dv * ts * std::min(1.0f, std::max(lambda, 0.0f))).length();

			// delete missille on hit (by setting its timer to 0, so it will be removed automatically)
//			if (dx.length() < (targetLoc.scale + missileTrf.scale))
			if (distance < (targetLoc.scale + missileTrf.scale))
			{
				TimerComponent& ttl = timed_entities.get<TimerComponent>(missile);
				ttl = 0.0f;
				HitPointComponent& targetHP = m_Scene->m_Registry.get<HitPointComponent>(target.targetEntity);
				targetHP.HP -= 1.0f;
			}
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

	auto asteroids = m_Scene->m_Registry.view<TransformComponent, AsteroidComponent>();
	for (auto asteroid : asteroids)
	{
		TransformComponent& asteroidTrf = asteroids.get<TransformComponent>(asteroid);
		if (asteroidTrf.location.length() < 2.3f + asteroidTrf.scale) // 2.3 is the radius of the earth
		{
			SpawnExplosion(asteroidTrf, DynamicPropertiesComponent());
			m_Scene->m_Registry.destroy(asteroid);
			m_ExplosionSound.play();
		}
	}

	// update wo collision
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
		entity_trf.orientation = Rotation(ts * dyn_prop.angular_velocity.length(), dyn_prop.angular_velocity) * entity_trf.orientation;

		// this is not that great, should be separated cus meshes wo dyn props wont be drawn (they dont get into the mesh library)
		MeshIndexComponent& idx = view.get<MeshIndexComponent>(entity);
		m_Scene->m_MeshLibrary.m_MeshTransforms[idx.idx].push_back(entity_trf);

		if (m_Scene->m_Registry.all_of<ColourComponent>(entity))
		{
			std::shared_ptr<Mesh> tmp_mesh = m_Scene->m_MeshLibrary.m_Meshes[idx.idx];
			int colBufIdx = tmp_mesh->GetColourInstances();
			if (colBufIdx != -1)
			{
				ColourComponent& colour = m_Scene->m_Registry.get<ColourComponent>(entity);
				m_Scene->m_MeshLibrary.m_ColourBuffers[colBufIdx].push_back(colour);
			}
		}

		if (!m_Scene->m_Registry.all_of<ExplosionComponent>(entity))
		{
			Vec3D acceleration = { 0,0,0 };
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
	}

	// for testing purposes, try to add markers to the asteroids
	static int markerIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["DefaultMarker"];
	static std::shared_ptr<Mesh> marker_mesh = m_Scene->m_MeshLibrary.m_Meshes[markerIdx];
	static int markerColBufIdx = marker_mesh->GetColourInstances();
	static const ColourComponent marker_colour = ColourComponent(0,0,1,1);
	for (auto asteroid : asteroids)
	{
		TransformComponent& asteroidTrf = asteroids.get<TransformComponent>(asteroid);
		m_Scene->m_MeshLibrary.m_MeshTransforms[markerIdx].push_back(asteroidTrf);
		m_Scene->m_MeshLibrary.m_ColourBuffers[markerColBufIdx].push_back(marker_colour);
	}

	/*
	auto grav_masses = m_Scene->m_Registry.view<TransformComponent, GravitationalMassComponent>();
	auto view = m_Scene->m_Registry.group<TransformComponent, DynamicPropertiesComponent, MeshIndexComponent>();
	int n = view.size();
	m_Distances.resize(n);
	int i = 0, j = 0;

	//for (auto entity : view)
	for (auto entity = view.begin(); entity != view.end(); entity++)
	{
		// check location
		TransformComponent& entity_trf = view.get<TransformComponent>(*entity);
		if (entity_trf.location.lengthSquare() > 1000000)
		{
			m_Scene->m_Registry.destroy(*entity);
			continue;
		}

		// update with collisions
		DynamicPropertiesComponent& dyn_prop = view.get<DynamicPropertiesComponent>(*entity);

		auto entity_next = entity;
		entity_next++;
		float min_distance = 5000.0f; // entities further from the center by 1000 are destroyed anyway
		entt::entity closest_entity = entt::tombstone;
		j = i + 1;

		for (entity_next; entity_next != view.end(); entity_next++)
		{
			TransformComponent& next_trf = view.get<TransformComponent>(*entity_next);
			DynamicPropertiesComponent& next_dyn_prop = view.get<DynamicPropertiesComponent>(*entity_next);

			Vec3D dr = entity_trf.location - next_trf.location;
			Vec3D dv = dyn_prop.velocity - next_dyn_prop.velocity;
			float lambda = dv.lengthSquare() < 0.00001 ? 0 : -(dr * dv) / dv.lengthSquare();
			float distance = (dr + dv * ts * lambda).length();
			closest_entity = distance < min_distance ? *entity_next : closest_entity;
			min_distance = distance < min_distance ? distance : min_distance;
//			m_Distances[i * n + j] = (dr + dv * ts * lambda).length();
//			m_Distances[j * n + i] = lambda; // store the lambda values in the unused half of the distance matrix
			j++;
		}

		if (closest_entity != entt::tombstone)
		{
			TransformComponent& closest_trf = view.get<TransformComponent>(closest_entity);
			if (min_distance < (entity_trf.scale + closest_trf.scale))
			{
				DynamicPropertiesComponent& closest_dyn_prop = view.get<DynamicPropertiesComponent>(closest_entity);
				Vec3D v_eff = (dyn_prop.inertial_mass * dyn_prop.velocity + closest_dyn_prop.inertial_mass * closest_dyn_prop.velocity) / (dyn_prop.inertial_mass + closest_dyn_prop.inertial_mass);
				Vec3D tempV = dyn_prop.velocity - v_eff;
				dyn_prop.velocity = v_eff + (closest_dyn_prop.inertial_mass / dyn_prop.inertial_mass) * (closest_dyn_prop.velocity - v_eff);
				closest_dyn_prop.velocity = v_eff + (dyn_prop.inertial_mass / closest_dyn_prop.inertial_mass) * tempV;
			}
		}

		entity_trf.location += ts * dyn_prop.velocity;
		entity_trf.orientation = Rotation(ts * dyn_prop.angular_velocity.length(), dyn_prop.angular_velocity) * entity_trf.orientation;

		// this is not that great, should be separated cus meshes wo dyn props wont be drawn (they dont get into the mesh library)
		MeshIndexComponent& idx = view.get<MeshIndexComponent>(*entity);
		m_Scene->m_MeshLibrary.m_MeshTransforms[idx.idx].push_back(entity_trf);


		// update velocities
		Vec3D acceleration = {0,0,0};
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

		i++;
	}
	*/
}

void SceneUpdater::FillDistanceMatrix()
{

}

void SceneUpdater::SpawnExplosion(TransformComponent trf, DynamicPropertiesComponent dyn)
{
	static int explosionIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Explosion"];

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(trf);
	newEntity.AddComponent<MeshIndexComponent>(explosionIdx);
//	newEntity.AddComponent<ColourComponent>(ColourComponent(0.7f, 0.3f, 0.3f, 0.8f)); // usual orange explosion
//	newEntity.AddComponent<ColourComponent>(ColourComponent(0.3f, 0.8f, 0.5f, 0.8f));
//	newEntity.AddComponent<ColourComponent>(ColourComponent(float(rand() % 1000) / 1000.0f, float(rand() % 1000) / 1000.0f, float(rand() % 1000) / 1000.0f, 0.8f));
	newEntity.AddComponent<ColourComponent>(ColourComponent(0.8, 0.1f + float(rand() % 1000) / 5000.0f, 0.1f + float(rand() % 1000) / 5000.0f, 0.8f));
//	newEntity.AddComponent<ColourComponent>(ColourComponent());
	newEntity.AddComponent<DynamicPropertiesComponent>(dyn);
	newEntity.AddComponent<TimerComponent>(TimerComponent(5000.0f));
	newEntity.AddComponent<ExplosionComponent>(ExplosionComponent());

	
}
